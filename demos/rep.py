import hashlib
import os
import zmq

ctx = zmq.Context(1)
sock = ctx.socket(zmq.REP)
sock.bind("tcp://127.0.0.1:5556")
while 1:
    data = sock.recv_string()
    print "python: received '%s' from client." % data
    digest = hashlib.sha1(data).hexdigest()
    print "python (pid=%s): sending back sha1 of the received data: %s" % (os.getpid(), digest)
    sock.send_string(digest)
