---
name: gh-common
description: Comandos gh CLI y git más usados por los agentes VSA. Cubre el 95% de operaciones habituales. Si necesitas algo no listado aquí (releases, codespaces, rulesets, etc.), usa .github/skills/gh-cli.md
---

# gh CLI — Comandos Frecuentes (mini-skill)

> REGLA: Usa SIEMPRE estos comandos via `execute/runInTerminal`. NUNCA uses `github/*` MCP tools.
> Si el comando que necesitas no aparece aquí → lee `.github/skills/gh-cli.md`.

---

## ⚠️ OBLIGATORIO en Windows — Codificación UTF-8

Antes de cualquier comando `gh` que pase texto (títulos, cuerpos, comentarios), ejecuta:

```powershell
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
```

Sin esto, los caracteres especiales (tildes, ñ, etc.) se corrompen al enviarse a la API de GitHub.

---

## ⚠️ OBLIGATORIO — Patrón canónico para crear issues en Windows

**NUNCA** uses `--body "..."` ni `--body-file`. Ambos siguen teniendo problemas de encoding en Windows.  
**USA SIEMPRE `gh api -F body=@docs/file.md`** — lee el cuerpo directamente del fichero, sin pasar por la consola.

```powershell
# PATRÓN CANÓNICO — copiar y adaptar siempre
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8   # <-- fija encoding del título

$issueSlug = "nombre-sin-tildes"   # nombre corto del fichero, snake_case
$bodyContent = @"
cuerpo con tildes á é ó y múltiples líneas

## Sección
- item 1
"@
New-Item -ItemType Directory -Force -Path "docs" | Out-Null
[System.IO.File]::WriteAllText("docs\$issueSlug.md", $bodyContent, [System.Text.Encoding]::UTF8)

$REPO = (gh repo view --json nameWithOwner -q .nameWithOwner)
gh api "repos/$REPO/issues" --method POST `
  -F title="Título con tildes Ñ" `
  -F "labels[]=etiqueta" `
  -F body=@"docs\$issueSlug.md"
```

---

## Issues

```powershell
# Crear issue — patrón canónico (ver sección OBLIGATORIO arriba)
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
$REPO = (gh repo view --json nameWithOwner -q .nameWithOwner)
[System.IO.File]::WriteAllText("docs\issue-slug.md", $body, [System.Text.Encoding]::UTF8)
gh api "repos/$REPO/issues" --method POST -F title="TÍTULO" -F "labels[]=etiqueta" -F body=@"docs\issue-slug.md"

# Forma rápida (solo si el cuerpo NO tiene tildes ni caracteres especiales)
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
gh issue create --title "TITULO SIN TILDES" --body "cuerpo simple" --label "etiqueta"

# Listar issues abiertos
gh issue list --state open

# Listar issues por label
gh issue list --label "vsa-slice-pending"

# Ver issue
gh issue view 123

# Ver issue con comentarios
gh issue view 123 --comments

# Añadir comentario
gh issue comment 123 --body "COMENTARIO"

# Cerrar issue con comentario
gh issue close 123 --comment "Motivo de cierre"

# Reabrir issue
gh issue reopen 123

# Editar labels de un issue
gh issue edit 123 --add-label "etiqueta" --remove-label "otra-etiqueta"

# Editar título o body
gh issue edit 123 --title "Nuevo título"
gh issue edit 123 --body "Nuevo cuerpo"
```

---

## Pull Requests

```bash
# Crear PR (desde rama actual)
gh pr create --title "TÍTULO" --body "CUERPO" --base main

# Crear PR como draft
gh pr create --title "TÍTULO" --body "CUERPO" --draft

# Listar PRs abiertos
gh pr list --state open

# Ver PR
gh pr view 123

# Mergear PR (squash)
gh pr merge 123 --squash --delete-branch

# Mergear PR (merge commit)
gh pr merge 123 --merge --delete-branch
```

---

## Git

```bash
# Staging y commit
git add .
git add ruta/archivo.py
git commit -m "tipo(scope): descripción [skip ci]"

# Push
git push
git push origin nombre-rama

# Pull
git pull
git pull origin main

# Nueva rama y push
git checkout -b nombre-rama
git push -u origin nombre-rama

# Estado
git status
git log --oneline -5
```

---

## Combinados frecuentes (copy-paste ready)

```powershell
# Commit + push (workflow habitual tras implementar tarea)
git add .
git commit -m "feat(slice): implementar {nombre_tarea}"
git push

# ── Crear issue SLICE (patrón canónico Windows) ──
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
New-Item -ItemType Directory -Force -Path "docs" | Out-Null
$body = @"
## Alcance
Descripción del slice.

## Criterios de aceptación
- Criterio 1
"@
[System.IO.File]::WriteAllText("docs\slice-nombre.md", $body, [System.Text.Encoding]::UTF8)
$REPO = (gh repo view --json nameWithOwner -q .nameWithOwner)
gh api "repos/$REPO/issues" --method POST -F title="[SLICE] Nombre con tildes y ñ" -F "labels[]=vsa-slice-pending" -F body=@"docs\slice-nombre.md"
gh issue list --label "vsa-slice-pending" --state open --json number,title

# ── Crear sub-tarea de un slice ──
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
[System.IO.File]::WriteAllText("docs\tarea-nombre.md", $specContenido, [System.Text.Encoding]::UTF8)
$REPO = (gh repo view --json nameWithOwner -q .nameWithOwner)
gh api "repos/$REPO/issues" --method POST -F title="nombre_slice_01_nombre_tarea" -F "labels[]=vsa-task-pending" -F body=@"docs\tarea-nombre.md"

# ── Cerrar slice tras auditoría exitosa ──
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
gh issue comment 123 --body "Slice completado. Todas las tareas implementadas y auditadas."
gh issue close 123
```
