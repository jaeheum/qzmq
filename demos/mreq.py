import os
import random
import string
import time
import zmq

ctx = zmq.Context(1)
req = ctx.socket(zmq.REQ)
req.connect("tcp://127.0.0.1:5559")
while 1:
    time.sleep(1.0)
    data = ''.join(random.sample('qwertyuiop', 10))
    req.send_string(data)
    print "python (pid=%s): received the original data: %s" % (os.getpid(), req.recv_string())
