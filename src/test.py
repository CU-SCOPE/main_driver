#!/home/zach/anaconda/bin/python
import posix_ipc
import sys
import time
import os
import struct

sem_name = sys.argv[1]
print(sem_name)
semaphore = posix_ipc.Semaphore(sem_name)

if(len(sys.argv) > 2):
	pipeout = int(sys.argv[2])
	print(bytes(struct.pack("f", 3.14)))
	os.write(pipeout, bytes(struct.pack("f", 3.14)))

print("release semaphore to give control to main program")
time.sleep(2)
semaphore.release()
sys.exit()