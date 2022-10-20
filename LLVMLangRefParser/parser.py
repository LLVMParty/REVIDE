from asyncio import events
from stringprep import in_table_a1
import sys
import textwrap
import types
import os

import docutils.nodes
import docutils.parsers.rst
import docutils.utils
import docutils.frontend
import sphinx.writers.text
import sphinx.builders.text
import sphinx.util.osutil
from sphinx.application import Sphinx
from docutils.parsers.rst import roles
from docutils import nodes
from pygments.lexers.special import TextLexer
from sphinx.events import EventManager

from docutils.core import publish_string

roles.register_generic_role('ref', nodes.emphasis)
roles.register_generic_role('doc', nodes.emphasis)

# TODO: register this lexer somehow
# https://www.iamjonas.me/2013/03/custom-syntax-in-pygments.html
# https://stackoverflow.com/a/39540683/1806760
class NoneLexer(TextLexer):
    name = 'Text only'
    aliases = ['none']
    filenames = ['*.txt']
    mimetypes = ['text/plain']
    priority = 0.01

# Reference: https://stackoverflow.com/a/48719723/1806760
def parse_rst(text: str) -> docutils.nodes.document:
    parser = docutils.parsers.rst.Parser()
    components = (docutils.parsers.rst.Parser,)
    settings = docutils.frontend.OptionParser(components=components).get_default_values()
    document = docutils.utils.new_document("LangRef.rst", settings=settings)
    parser.parse(text, document)
    return document

class MyVisitor(docutils.nodes.NodeVisitor):

    def visit_reference(self, node: docutils.nodes.reference) -> None:
        """Called for "reference" nodes."""
        print(node)

    def unknown_visit(self, node: docutils.nodes.Node) -> None:
        """Called for all other node types."""
        pass

def get_section_title(node):
    if isinstance(node, docutils.nodes.section):
        title = node.children[0]
        assert isinstance(title, docutils.nodes.title)
        text = title.children[0]
        assert isinstance(text, docutils.nodes.Text)
        return text.astext()
    return None

def find_instruction_reference(document):
    assert len(document.children) == 1
    child = document.children[0]
    for node in child.children:
        if get_section_title(node) == "Instruction Reference":
            return node
    return None

def match_instruction_section(node):
    if isinstance(node, docutils.nodes.section):
        ids = node.attributes.get("ids", [])
        if len(ids) == 1:
            id = ids[0]
            return id.endswith("-instruction")
    return False
app = Sphinx(srcdir="", confdir=None, outdir="out", doctreedir="/", buildername=None)
index = 0
# Reference: https://stackoverflow.com/a/57120114/1806760
# NOTE: the existing example was totally not working
def to_text(node: docutils.nodes.Node):
    global index
    """
    app = types.SimpleNamespace(
        srcdir=None,
        confdir=None,
        outdir=None,
        doctreedir="/",
        config=types.SimpleNamespace(
            text_newlines="native",
            text_sectionchars="==",
            text_add_secnumbers=False,
            text_secnumber_suffix=".",
        ),
        events=None,
        tags=set(),
        registry=types.SimpleNamespace(
            create_translator=lambda self, something, new_builder: sphinx.writers.text.TextTranslator(
                document, new_builder
            )
        ),
    )
    """

    builder = sphinx.builders.text.TextBuilder(app)
    builder.secnumbers = {}

    document = docutils.utils.new_document(f"rst_{index}")
    index += 1
    copy = node.deepcopy()
    document.children.append(copy)
    document.setup_child(copy)

    translator = sphinx.writers.text.TextTranslator(document, builder)

    document.walkabout(translator)

    return translator.body
import re

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

def main_parser():
    with open("LangRef.rst", "r") as f:
        data = f.read()
    document = parse_rst(data)
    reference = find_instruction_reference(document)
    assert reference is not None
    instructions = reference.findall(match_instruction_section)
    for instruction in instructions:
        if isinstance(instruction, docutils.nodes.section):
            ids = instruction.attributes.get("ids", [])
            assert len(ids) == 1
            id = ids[0]
        with open(id + ".txt", "w") as f:
            f.write(to_text(instruction))

def main():
    with open("LangRef.rst", "r") as f:
        data = f.read()
    os.makedirs("out", exist_ok=True)
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
        match = re.match(r"'``([^ `]+)[^`]*``' Instruction", title)
        id = match.group(1)
        print(f"{title} => '{id}'")
        with open(f"out/{id}.rst", "w") as f:
            f.write("\n".join(instruction))

if __name__ == "__main__":
    main()