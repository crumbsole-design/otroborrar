# .github/scripts/get_tasks.py
import subprocess
import json
import sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--slice-id", help="ID del slice")
args = parser.parse_args()

# Busca issues que contengan el ID del slice y tengan el formato {slice}_{orden}
cmd = f"gh issue list --state open --search '{args.slice_id}' --json number,title --limit 50"
result = subprocess.check_output(cmd, shell=True).decode("utf-8")
tasks = json.loads(result)

# Ordenar por título para asegurar la secuencia 01, 02, 03...
tasks.sort(key=lambda x: x['title'])

print(json.dumps(tasks, indent=2))