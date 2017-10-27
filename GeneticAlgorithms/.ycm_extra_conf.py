import os
import ycm_core

flags = [
        '-Wall',
        '-Wpadded',
        '-Wextra',
        '-Werror',
        '-Wno-long-long',
        '-Wno-variadic-macros',
        '-fexceptions',
        '-ferror-limit=10000',
        '-DNDEBUG',
        '-std=c11',
        '-xc',
        '-isystem/usr/include',
        '-I/home/gopiandcode/Documents/C-Projects/GeneticAlgorithms/util',
        '-lm'
        ]

SOURCE_EXTENSIONS = ['.cpp', '.cxx', '.cc', '.c']

def FlagsForFile(filename, **kwargs):
    return {
            'flags': flags,
            'do_cache': True
    }
