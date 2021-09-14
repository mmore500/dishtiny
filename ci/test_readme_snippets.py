import exdown
import os
import tempfile

def audited_execute(command):
    assert os.EX_OK == os.system(command)

for snippet, __ in exdown.extract('README.md', syntax_filter='cpp'):
    with tempfile.TemporaryDirectory() as tmpdirname:

        source = f'{tmpdirname}/native.cpp'
        out = f'{tmpdirname}/a.out'

        with open(f'{source}', 'w') as file:
            file.write(snippet)

        audited_execute(
            f'mpicxx --std=c++2a -Iinclude/ -Ithird-party/ {source} -lpthread -o {out}'
        )
        audited_execute(f'script/uitexec -n 2 script/uitwrap {out}')
