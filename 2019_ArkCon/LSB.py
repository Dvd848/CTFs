import wave
import binascii

res = ''
f = wave.open('S3cr3tM3ss4g3.wav','rb')
for frame in f.readframes(f.getnframes()):
    res += str(ord(frame) & 0x1)

print binascii.unhexlify(format(int(res, 2), 'x'))