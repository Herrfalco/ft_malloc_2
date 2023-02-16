#!/usr/bin/env  python3

"""Makefiler installation script"""

import os
import sys

def     install(home, rc_path):
    """Installation function"""
    new_rc = []
    with open(rc_path, 'r') as rc_file:
        new_rc = [line for line in rc_file if 'alias rusage' not in line]
        new_rc.append("alias rusage='~/Utils/rusage/rusage'\n")
    with open(rc_path, 'w') as rc_file:
        for line in new_rc:
            print(line, file=rc_file, end='')
    os.system(f'make -s re; mkdir -p {home}/Utils/rusage 2> /dev/null; \
            cp rusage {home}/Utils/rusage/rusage 2> /dev/null; make -s fclean')
    print("Rusage has been successfully installed (reload your shell).")

def     uninstall(home, rc_path):
    """Uninstallation function"""
    new_rc = None
    with open(rc_path, 'r') as rc_file:
        new_rc = [line for line in rc_file if 'alias rusage' not in line]
    with open(rc_path, 'w') as rc_file:
        for line in new_rc:
            print(line, file=rc_file, end='')
    os.system(f'rm -rf {home}/Utils/rusage 2> /dev/null')
    print("Rusage has been successfully uninstalled.")

if __name__ == '__main__':
    print("""Rusage Installer""")
    CHOICE = input("""  1 - Install\n  2 - Uninstall\n  > """)
    try:
        HOME = os.environ.get('HOME')
        RC_PATH = HOME + '/.' + os.environ.get('SHELL') \
                .split('/')[-1] + 'rc'
        if CHOICE == '1':
            install(HOME, RC_PATH)
        elif CHOICE == '2':
            uninstall(HOME, RC_PATH)
        else:
            raise IndexError
    except Exception:
        print("Error: Can't install Rusage", file=sys.stderr)
