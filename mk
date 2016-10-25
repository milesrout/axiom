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

include_regex = re.compile('#include "(.*)"')

header = """
CC = i686-elf-gcc
AS = i686-elf-as
CFLAGS = -std=c89 -pedantic -ffreestanding -O2 -W -Wall
LDFLAGS = -ffreestanding -O2 -nostdlib

all: axiom.bin

clean:
\trm *.o *.bin

run: all
\tqemu-system-i386 -kernel axiom.bin -initrd axiom.bin

.PHONY: all clean run
"""

def dependencies(path):
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

def print_final_target(obj_files):
    obj_files_string = ' '.join(str(obj_file) for obj_file in obj_files)
    print('axiom.bin: linker.ld {obj_files}'.format(
        obj_files=obj_files_string))
    print('\t${{CC}} -T linker.ld -o axiom.bin ${{LDFLAGS}} {obj_files} -lgcc'.format(
        obj_files=obj_files_string))

def print_targets(files, target_template, *cmd_templates):
    for source_file, object_file in files.items():
        print()

        print(target_template.format(
            obj_file=object_file,
            source_file=source_file,
            deps=' '.join(dependencies(source_file))))
        for cmd in cmd_templates:
            print(cmd.format(
                source_file=source_file,
                obj_file=object_file))

def main():
    source_files = files('.c')
    asm_files = files('.s')

    print(header)

    print_final_target(chain(source_files.values(), asm_files.values()))

    print_targets(asm_files,
            '{obj_file}: {source_file} {deps}',
            '\t${{AS}} {source_file} -o {obj_file}')

    print_targets(source_files,
            '{obj_file}: {source_file} {deps}',
            '\t${{CC}} -c {source_file} -o {obj_file} ${{CFLAGS}}')

if __name__ == '__main__':
    main()
