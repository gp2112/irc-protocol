

f = open("/dev/virtualcom0", "ab")

p = 61

f.write(p.to_bytes(4, 'little'))

f.close()
