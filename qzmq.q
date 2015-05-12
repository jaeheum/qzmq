/ Set port to a random negative number for multithreaded input.
/ \p -0W
zmq:(`qzmq 2:(`zmq;1))`
zactor:(`qzmq 2:(`zactor;1))`
zctx:(`qzmq 2:(`zctx;1))`
zclock:(`qzmq 2:(`zclock;1))`
zfile:(`qzmq 2:(`zfile;1))`
zframe:(`qzmq 2:(`zframe;1))`
zmsg:(`qzmq 2:(`zmsg;1))`
zsock:(`qzmq 2:(`zsock;1))`
zstr:(`qzmq 2:(`zstr;1))`
zsys:(`qzmq 2:(`zsys;1))`
zthread:(`qzmq 2:(`zthread;1))`
libzmq:(`qzmq 2:(`libzmq;1))`

\
    qzmq - q bindings for CZMQ, the high-level C binding for 0MQ:
    http://czmq.zeromq.org.

    Copyright (c) 2012, 2015 Jaeheum Han <jay.han@gmail.com>

    qzmq license is same as CZMQ's or Mozilla Public License version 2 (MPL v2).

