import sys

with open(sys.argv[1], "rb") as gid, open(sys.argv[2], "rb") as log, open(sys.argv[3], "wb") as art:
    gid.read(16) # Read out GNU Header
    art.write(gid.read(20)) # Write 20 Byte Build ID
    art.write('\n'.encode()) # New Line
    art.write(log.read()) # Write Log Lines