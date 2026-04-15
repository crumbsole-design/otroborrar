import os
import sys
from openai import OpenAI
import re

# --- CONFIGURACIÓN ---
# Prioridad: llama-server local (RTX 2060) > GitHub Models (fallback CI)
LOCAL_ENDPOINT = "http://127.0.0.1:8080/v1"
LOCAL_MODEL = "qwen2.5-coder-32b-q4_k_m.gguf"
GITHUB_ENDPOINT = "https://models.github.ai/inference"
GITHUB_MODEL = "microsoft/Phi-4"

# Con modelo local de 32B y 131K ctx, el límite es mucho mayor
MAX_CHARS_LOCAL = 80000
MAX_CHARS_GITHUB = 20000

def _create_client():
    """Intenta llama-server local; si no responde, cae a GitHub Models."""
    import urllib.request
    try:
        req = urllib.request.urlopen(f"{LOCAL_ENDPOINT.rstrip('/v1')}/health", timeout=5)
        if req.status == 200:
            print(f"Usando modelo local: {LOCAL_MODEL} en {LOCAL_ENDPOINT}")
            return OpenAI(base_url=LOCAL_ENDPOINT, api_key="no-key"), LOCAL_MODEL, MAX_CHARS_LOCAL
    except Exception:
        pass

    token = os.environ.get("GH_MODELS_TOKEN")
    if not token:
        print("Error: llama-server no disponible y no se encontró GH_MODELS_TOKEN")
        sys.exit(1)
    print(f"Fallback a GitHub Models: {GITHUB_MODEL}")
    return OpenAI(base_url=GITHUB_ENDPOINT, api_key=token), GITHUB_MODEL, MAX_CHARS_GITHUB

client, MODEL_NAME, MAX_CHARS = _create_client()

def read_codebase(start_path="."):
    code_content = ""
    valid_extensions = {".ts", ".js", ".py", ".java", ".cs", ".php", ".go", ".rb"}
    
    # Filtro de carpetas mucho más estricto
    ignore_patterns = {
        'node_modules', '.git', 'dist', 'build', 'coverage', 'test', 'tests', 
        'spec', 'specs', '__pycache__', '.github', '.vscode', 'docs', 'venv', 'env'
    }

    print(f"Leyendo código desde: {start_path} (Límite: {MAX_CHARS} chars)...")
    
    file_count = 0
    for root, dirs, files in os.walk(start_path):
        # 1. Ignorar directorios si contienen cualquier patrón prohibido
        dirs[:] = [d for d in dirs if not any(p in d.lower() for p in ignore_patterns)]
        
        for file in files:
            file_lower = file.lower()
            # 2. Ignorar si el nombre del archivo contiene "test" o "spec"
            if "test" in file_lower or "spec" in file_lower:
                continue
            
            if os.path.splitext(file)[1] in valid_extensions:
                if len(code_content) > MAX_CHARS:
                    break
                
                file_path = os.path.join(root, file)
                try:
                    with open(file_path, "r", encoding="utf-8") as f:
                        content = f.read()
                        # Solo lógica real, saltamos archivos muy pequeños o muy grandes
                        if 100 < len(content) < 15000: 
                            code_content += f"\n--- FILE: {file_path} ---\n"
                            code_content += content
                            file_count += 1
                except: continue
    return code_content

def generate_diagram(code_context):
    print(f"Consultando a {MODEL_NAME} ({len(code_context)} caracteres)...")
    
    if len(code_context) < 50:
        print("❌ Error: No se encontró suficiente código fuente.")
        return None

    system_prompt = """
    ERES: Arquitecto de Software Senior.
    TAREA: Genera un diagrama Mermaid `classDiagram`.
    
    REGLAS CRÍTICAS:
    1. PROHIBIDO incluir clases de Test, Mocks o Debug (ej: TestXXX, DebugYYY).
    2. SOLO incluye clases de Lógica de Negocio, Modelos y Servicios.
    3. NO incluyas referencias tipo .
    4. SOLO devuelve el código Mermaid entre bloques ```mermaid.
    """

    try:
        response = client.chat.completions.create(
            messages=[
                {"role": "system", "content": system_prompt},
                {"role": "user", "content": code_context}
            ],
            model=MODEL_NAME,
            temperature=0.1,
            max_tokens=2000 
        )

        content = response.choices[0].message.content
        
        # Limpieza de markdown
        if "```mermaid" in content:
            content = content.split("```mermaid")[1].split("```")[0]
        elif "```" in content:
            content = content.split("```")[1].split("```")[0]
            
        # Eliminar referencias entre corchetes que algunos modelos generan
        content = re.sub(r'\[.*?\]', '', content)
        
        return content.strip() 
        
    except Exception as e:
        print(f"❌ Error CRÍTICO en la API: {e}")
        # Si falla Phi-4, intenta capturar el error para depurar
        sys.exit(1)

if __name__ == "__main__":
    # Lee desde la raíz (.)
    full_code = read_codebase(".")
    
    if full_code:
        mermaid_code = generate_diagram(full_code)
        if mermaid_code:
            output_path = "docs/architecture.mmd"
            os.makedirs(os.path.dirname(output_path), exist_ok=True)
            with open(output_path, "w", encoding="utf-8") as f:
                f.write(mermaid_code)
            print(f"🎉 ¡Éxito! Diagrama guardado en {output_path}")