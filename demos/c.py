import os
import time
import zmq

pid = os.getpid()
ctx = zmq.Context(1)
sock = ctx.socket(zmq.REQ)
sock.connect("tcp://127.0.0.1:5556")
while 1:
    data = time.ctime()
    print "python (pid=%s): sending data: '%s'" % (pid, data)
    sock.send_string(data)
    print "python (pid=%s): received sha1 of data: %s" % (pid, sock.recv_string())
    time.sleep(1) # sleep 1 second (not millisecond)
