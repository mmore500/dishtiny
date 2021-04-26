import exdown
import os
import sys
import tempfile

def audited_execute(command):
    if not os.EX_OK == os.system(command):
        sys.exit(f"command {command} failed")

for snippet, __ in exdown.extract('README.md', syntax_filter='cpp'):
    with tempfile.TemporaryDirectory() as tmpdirname:

        source = f'{tmpdirname}/source.cpp'
        out = f'{tmpdirname}/a.out'

        with open(f'{source}', 'w') as file:
            file.write(snippet)

        audited_execute(
            f'mpicxx --std=c++2a -Iinclude/ -Ithird-party/ {source} -lpthread -o {out}'
        )
        audited_execute(f'script/uitexec -n 2 script/uitwrap {out}')
