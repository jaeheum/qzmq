import os
import sha
import zmq

ctx = zmq.Context(1)
pull = ctx.socket(zmq.PULL)
source = pull.bind("tcp://127.0.0.1:5555")

sock = ctx.socket(zmq.REQ)
sock.connect("tcp://127.0.0.1:5556")
while 1:
    data = pull.recv_string()
    print "python: received '%s'" % data
    sock.send_string(data)
    digest = sha.new(data).hexdigest()
    print "python (pid=%s): received sha1 from perl or python: %s" % (os.getpid(), digest)
