import os
import sha
import zmq

ctx = zmq.Context(1)
sock = ctx.socket(zmq.REP)
sock.connect("tcp://127.0.0.1:5560")
while 1:
    data = sock.recv_string()
    print "python: received '%s' from client." % data
    print "python (pid=%s): sending back the received data: %s" % (os.getpid(), data)
    sock.send_string(data)
