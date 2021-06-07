from queue import Queue, Empty
import subprocess
from threading import Thread
import os
import re

threads_count = 12
q = Queue()
dir_name = '/zfs/store5.hydra.local/projects/user-projects/e/er/exp/exp201904/data'
file_pattern = 'h7_ct_0.*.lmd'

fileNames = [f for f in os.listdir(dir_name) if os.path.isfile(os.path.join(dir_name, f)) and re.match(file_pattern, f)]
print(fileNames)
for fileName in fileNames:
  q.put(fileName)

def worker():
  while True:
    try:
        fileName = q.get_nowait()
    except Empty:
        return
    proc = subprocess.Popen(['./run.sh', fileName])
    proc.wait()

threads = [Thread(target=worker) for _ in range(threads_count) ]
for thread in threads:
    thread.start()
for thread in threads:
    thread.join()
