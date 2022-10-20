import json
import os
import re
import sys
import shutil
import subprocess

def extract_instructions(data: str):
    lines = data.splitlines(False)
    in_instruction = False
    section = []
    for line in lines:
        if re.match(r"^\.\. _([^:]+):$", line):
            if section:
                if in_instruction:
                    yield section
                    in_instruction = False
                section.clear()
        if line.lower().endswith("' instruction"):
            if in_instruction:
                # Sometimes the format isn't consistent
                yield section
                section.clear()
            in_instruction = True
        section.append(line)

def main():
    pandoc = shutil.which("pandoc")
    if pandoc is None:
        print("Executable 'pandoc' not found in PATH, install from: https://pandoc.org/installing.html")
        sys.exit(1)

    with open("LangRef.rst", "r") as f:
        data = f.read()
    os.makedirs("out", exist_ok=True)
    documentation = {}
    for instruction in extract_instructions(data):
        title = None
        count = 0
        for line in instruction:
            if line.endswith("' Instruction"):
                title = line
            if "^^^^^" in line:
                count += 1
        assert count == 1
        assert title is not None
        match = re.match(r"'``([^ `]+)([^`]*)``' Instruction", title)
        id = match.group(1)
        anchor = id + match.group(2) + "-instruction"
        print(f"{title} => '{id}', #{anchor}")
        rst = f"out/{id}.rst"
        with open(rst, "w") as f:
            f.write("\n".join(instruction))
        html = f"out/{id}.html"
        command = f"\"{pandoc}\" --from rst --to html --no-highlight \"{rst}\" --output \"{html}\""
        subprocess.check_output(command)
        with open(html, "r") as f:
            doc = f.read()
            # Fix some broken links
            doc = doc.replace("CodeGenerator.html#sibcallopt", "CodeGenerator.html#sibling-call-optimization")
            doc = doc.replace("CodeGenerator.html#tailcallopt", "CodeGenerator.html#tail-call-optimization")
            # Replace relative links with absolute ones
            hrefs = re.findall(r"href=\"([^\"]+)\"", doc)
            unique = set()
            for href in hrefs:
                if href.startswith("http"):
                    continue
                if href not in unique:
                    unique.add(href)
                    doc = re.sub(f"\"{href}\"", f"\"https://releases.llvm.org/14.0.0/docs/{href}\"", doc)
            documentation[id] = doc
    with open("out/LLVM.json", "w") as f:
        json.dump(documentation, f, indent=2)

if __name__ == "__main__":
    main()