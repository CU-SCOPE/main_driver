#!/home/zach/anaconda/bin/python
import posix_ipc
import sys
import time

sem_name = sys.argv[1]
print(sem_name)
semaphore = posix_ipc.Semaphore(sem_name)
print("release semaphore to give control to main program")
time.sleep(2)
semaphore.release()
sys.exit()