# Contexto del Proyecto
Este es un proyecto de Python que utiliza herramientas de IA y GitHub Actions.

# Reglas de Entorno y Ejecución
- **Archivos de fixtures para tests**: Cualquier archivo temporal o de datos creado para ser usado en un test (p. ej. `tmp_*.txt`, archivos de entrada de prueba) debe crearse en `./tests/fixtures/`, **nunca en el directorio raíz `./` ni en `./test_features/`**.
- **Entorno Virtual**: Todo el código, scripts y tests deben ejecutarse dentro del entorno virtual local `.venv`.
- **Comandos en Terminal**: Si sugieres comandos de terminal (ej. para instalar librerías o lanzar scripts), prefiérelos con la activación del entorno. Asume entorno Linux/Mac por defecto: `source .venv/bin/activate && comando`. En Windows: `.venv\Scripts\Activate.ps1`.
- **Tests**: Usamos `pytest`. Cuando me pidas ejecutar tests como `test_find_by_fingerprints_found`, `test_find_by_fingerprints_partial` o `test_find_by_fingerprints_empty`, asegúrate de incluir la activación de `.venv` en el comando sugerido.
# Protocolo de Comunicación M2M (LLM-Optimized)
- **Zero-Fluff Policy**: Prohibido usar saludos, despedidas, introducciones o frases de transición ("Aquí tienes...", "Espero que esto ayude").
- **Densidad de Información**: Prioriza listas de atributos, tablas y bloques de código sobre prosa descriptiva.
- **Estructura Estricta**: Usa siempre encabezados Markdown como delimitadores de datos para que el receptor pueda "parsear" la información fácilmente.
- **Referencia Absoluta**: No uses pronombres ("este archivo", "esa función"). Usa siempre rutas completas (`src/feature/logic.py`) e identificadores únicos.
- **Contexto Autónomo**: Cada issue o mensaje debe contener el 100% de la información necesaria para ser ejecutado sin que el receptor tenga que buscar en turnos anteriores.
# Regla: GitHub CLI (gh) — Obligatorio en todos los agentes y workflows
- **MINI-SKILL (usar primero)**: `.github/skills/gh-common.md` — comandos más frecuentes (issues, PRs, git add/commit/push). Minimiza el consumo de tokens.
- **SKILL COMPLETO (fallback)**: `.github/skills/gh-cli.md` — referencia completa. Úsalo solo si el comando no aparece en `gh-common.md`.
- **PROHIBIDO**: No uses el MCP de GitHub (`github/*` tools) para ninguna operación. Está **desactivado**.
- **OBLIGATORIO**: Toda operación con GitHub (crear/listar/cerrar issues, PRs, ramas, comentarios, etiquetas, releases, etc.) se realiza exclusivamente mediante comandos `gh` ejecutados en terminal (`execute/runInTerminal`).
- **Equivalencias clave**:
  | MCP (prohibido)         | gh CLI (obligatorio)                                      |
  |-------------------------|-----------------------------------------------------------|
  | `github/issue_write`    | `gh issue create --title "..." --body "..." --label "..."` |
  | `github/issue_read`     | `gh issue view <number>` / `gh issue list`                |
  | `github/add_issue_comment` | `gh issue comment <number> --body "..."`               |
  | `github/list_issues`    | `gh issue list --state open`                              |
  | `github/create_pull_request` | `gh pr create --title "..." --body "..."`            |
  | `github/pull_request_read` | `gh pr view <number>`                                  |
  | `github/list_pull_requests` | `gh pr list`                                          |
  | `github/merge_pull_request` | `gh pr merge <number>`                                |
  | `github/sub_issue_write`| `gh issue create --title "..." --body "..." --label "..."` |