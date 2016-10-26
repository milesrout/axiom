#!/usr/bin/env python3

from pathlib import Path
from itertools import chain
import collections
import functools
import re

def accumulate(accum_type):
    def outer_wrapper(f):
        @functools.wraps(f)
        def inner_wrapper(*args, **kwds):
            return accum_type(iter(f(*args, **kwds)))
        return inner_wrapper
    return outer_wrapper

def dependencies(path, include_regex=re.compile('#include "(.*)"')):
    if isinstance(include_regex, str):
        include_regex = re.compile(include_regex)
    with path.open('r') as f:
        for line in f:
            match = include_regex.match(line)
            if match:
                yield match.groups()[0]

@accumulate(collections.OrderedDict)
def files(ext, obj_ext='.o'):
    for fname in Path.cwd().glob('**/*' + ext):
        source_file = fname.relative_to(Path.cwd())
        yield source_file, source_file.with_suffix(obj_ext)

def objfiles_variable(obj_files):
    return 'OBJFILES = {}'.format(' '.join(map(str, obj_files)))

def source_targets(files, target_template, *cmd_templates):
    for source_file, object_file in files.items():
        yield ''

        yield target_template.format(
            obj_file=object_file,
            source_file=source_file,
            deps=' '.join(dependencies(source_file)))
        for cmd in cmd_templates:
             yield cmd.format(
                source_file=source_file,
                obj_file=object_file)

@accumulate('\n'.join)
def get_makefile_text():
    source_files = files('.c')
    asm_files = files('.s')

    yield objfiles_variable(chain(source_files.values(), asm_files.values()))

    yield from source_targets(asm_files,
            '{obj_file}: {source_file} {deps}',
            '\t${{AS}} {source_file} -o {obj_file}')

    yield from source_targets(source_files,
            '{obj_file}: {source_file} {deps}',
            '\t${{CC}} -c {source_file} -o {obj_file} ${{CFLAGS}}')

    yield ''

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description='Generate dependency Makefile')
    parser.add_argument('output', nargs='?', default='Makefile.mk',
                        type=argparse.FileType('w'))

    args = parser.parse_args()
    args.output.write(get_makefile_text())
