#!/usr/bin/env python3
import json

templates = {
    
}

for i in range(1, 129):
    for line in open(f"{i}.tbl"):
        if not line.startswith('TableData'):
            continue
        _, data = line.split(":")
        print(i, 'data len:', len(data.strip()))
        templates[str(i)] = data.strip()

json.dump(templates, open("templates.json", "w"), indent=4)
