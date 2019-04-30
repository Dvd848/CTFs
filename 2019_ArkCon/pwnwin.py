import sys
import signal
import socket
import logging 
#from subprocess import Popen, PIPE
from collections import defaultdict

#https://stackoverflow.com/questions/2352181/how-to-use-a-dot-to-access-members-of-dictionary
class dotdict(dict):
    """dot.notation access to dictionary attributes"""
    __getattr__ = dict.get
    __setattr__ = dict.__setitem__
    __delattr__ = dict.__delitem__

def connect(host, port):
    io = TelnetIO()
    io.connect(host, port)
    return io


class IO(object):
    LINE_ENDING = "\r\n"

    def __init__(self):
        #self.connection = None
        self.remainder = ""
        self.buffer_size = 1

    def send(self, s):
        raise NotImplementedError("Please implement send()")

    def recv(self, size):
        raise NotImplementedError("Please implement recv()")

    def sendline(self, s):
        self.send(s + self.LINE_ENDING)

    def recvuntil(self, pred):
        res = self.remainder
        while True:
            try:
                index = res.index(pred)
                border = index + len(pred)
                self.remainder = res[border:]
                res = res[:border]
                break
            except ValueError:
                pass
            buf = self.recv(self.buffer_size)
            res += buf
        return res

    def recvline(self):
        return self.recvuntil(self.LINE_ENDING)

class TelnetIO(IO):
    def __init__(self):
        IO.__init__(self)
        self.connection = None
        self.buffer_size = 4096

    def connect(self, host, port):
        self.connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connection.connect((host, port))

    def send(self, s):
        self.connection.send(s)
        
    def recv(self, size):
        return self.connection.recv(size)

try:
    from pexpect.popen_spawn import PopenSpawn
    
    def process(exe):
        io = ProcessIO()
        io.run(exe)
        return io

    class ProcessIO(IO):
        def __init__(self):
            IO.__init__(self)
            self.proc = None
            self.buffer_size = 1

        def __del__(self):
            if self.proc is not None:
                self.proc.kill(signal.CTRL_BREAK_EVENT)

        def run(self, exe):
            #self.proc = Popen([exe], stdout=PIPE, stdin=PIPE, bufsize=self.buffer_size, shell=True)
            self.proc = PopenSpawn(exe)
            

        def send(self, s):
            #self.proc.stdin.write(s)
            self.proc.write(s)
            
        def recv(self, size):
            #return self.proc.stdout.read(size)
            return self.proc.read(size)

        def recvuntil(self, pred):
            self.proc.expect(pred)
            return self.proc.before
except ImportError:
    def process(exe):
        raise ImportError("Warning: No pexpect module found, please install via 'pip install pexpect'")


log = logging
log.basicConfig(level=log.INFO, format='%(levelname)s: %(message)s',)


args = dotdict({arg:True for arg in sys.argv[1:]})
