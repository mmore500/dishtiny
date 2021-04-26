import exdown
import tempfile
import os

def audited_execute(command):
    assert os.EX_OK == os.system(command)

for snippet, __ in exdown.extract('README.md', syntax_filter='cpp'):
    with tempfile.TemporaryDirectory() as tmpdirname:

        source = f'{tmpdirname}/source.cpp'
        out = f'{tmpdirname}/a.out'

        with open(f'{source}', 'w') as file:
            file.write(snippet)

        audited_execute(
            f'g++ --std=c++17 -Iinclude/ -Ithird-party/ {source} -o {out}'
        )
        audited_execute(f'{out}')
