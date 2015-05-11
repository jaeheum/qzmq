
Title: README

Date: 20150511

<A name="toc1-6" title="qzmq" />
# qzmq

Qzmq provides [Q][q] bindings for [CZMQ][czmq],
the high-level C binding for [ØMQ][zeromq].

Version 3.0.1 of qzmq corresponding to the same version of CZMQ.
Currently qzmq offers a subset of CZMQ APIs.
See the CZMQ Coverage below.

Qzmq is written and maintained by Jay Han (<jay.han@gmail.com>)

Qzmq is hosted at [github][qzmq] and
it uses the [issue tracker][issues] for all issues and comments.

<A name="toc2-21" title="Contents" />
## Contents

&emsp;<a href="#toc2-26">Features of qzmq</a>
&emsp;<a href="#toc2-36">CZMQ Coverage</a>
&emsp;<a href="#toc2-126">License</a>
&emsp;<a href="#toc2-135">Files of qzmq</a>
&emsp;<a href="#toc2-148">Building qzmq</a>
&emsp;<a href="#toc2-173">Issues</a>
&emsp;<a href="#toc2-177"></a>

<A name="toc2-26" title="Features of qzmq" />
## Features of qzmq
Qzmq lets Q users write

- concurrent code using multi-threads,
- distributed systems connecting with systems
    - written in other languages,
    - written with many software patterns,
using [CZMQ][czmq] and  [ØMQ][zeromq] developed by the [ØMQ][zeromq] community.

<A name="toc2-36" title="CZMQ Coverage" />
## CZMQ Coverage
Qzmq covers a subset of [CZMQ APIs][czmq-reference].
Some czmq APIs seem redundant viz-a-viz q:
zuuid or UUID support, zlist or generic container, etc.

[zsock(3) - working with ZeroMQ sockets (high-level)][zsock]

    zsock.new[type] //  Return a new socket with a type (zmq`XXX).
    zsock.destroy[socket] //  Destroy a socket returned from zsock.new[type].
    zsock.bind[socket; `endpoint] //  Bind a socket to an endpoint.
    zsock.connect[socket; `endpoint] //  Connect a socket to an endpoint.
    zsock.typestr[socket] //  Returns socket type name as a symbol.

[zstr(3) - sending and receiving strings][zstr]

    // DO NOT USE with byte vectors or serialized objects.
    zstr.recv[socket] //  Receive a symbol from a socket.
    zstr.send[socket; `symbol] //  Sends `symbol to a socket.

[zmsg(3) - working with multipart messages][zmsg]

    zmsg.new[] //  Create a new empty message object
    zmsg.destroy[] //  Destroy a message object and all frames it contains
    zmsg.recv[socket] //  Receive a zmsg from socket.
    zmsg.send[message; socket] //  Send message to destination socket, and destroy
                               //  the message after sending it successfully. 
    zmsg.size[message] //  Return size of message, i.e. number of frames (0 or more).
    zmsg.contentsize[message] //  Return the size of the content in a message.
    zmsg.prepend[message; frame] //  Push frame to the front of the message, i.e. before all other frames.
    zmsg.append[message; frame] //  Add frame to the end of the message, i.e. after all other frames.
    zmsg.pop[message] //  Remove first frame from message, if any. Returns frame, or 'empty.
    zmsg.first[message] //  Set cursor to first frame in message. Returns frame, or 'empty
    zmsg.next[message] //  Return the next frame, or 'empty.
    zmsg.last[message] //  Return the last frame, or 'empty.
    zmsg.save[message; `:file] //  Save message to an open file, return 0 if OK, else -1.
    zmsg.load[message; `:file] //  Load/append an open file into message.
    zmsg.dup[message] // Create copy of message, as new message object.
    zmsg.print[message] // Send message to STDOUT.
    zmsg.is[message] //  Probe the supplied object, and report if it looks like a zmsg_t.

[zframe(3) - working with single message frames][zframe]

    zframe.new[data] //  Create a new frame with  data byte vector.
    zframe.empty[] //  Create an empty (zero-sized) frame.
    zframe.destroy[frame] // Destroy a frame.
    zframe.recv[socket] // Receive frame from socket, or nothing.
    zframe.send[frame; socket; flags] // Send a frame to a socket, destroy frame after sending.
                                      // Return -1 on error, 0 on success.
    zframe.dup[frame] //  Create a new frame that duplicates an existing frame.
    zframe.strdup[frame] //  Return frame data as a new symbol.
    zframe.streq[frame; `symbol] //  Return 1b if frame body is equal to `symbol
    zframe.more[frame] //  Return frame MORE indicator (1 or 0).
    zframe.size[frame] //  Return number of bytes in frame data.
    zframe.eq[frame1; frame2] // Return 1b if two frames have identical size and data
    zframe.reset[frame; data] //  Set new contents for frame with data.
    zframe.print[frame; `prefix] //  Send message to STDOUT with `prefix (may be `)
    zframe.is[frame] //  Probe the supplied object, and report if it looks like a zframe_t.
    
[zctx(3) - working with ZeroMQ contexts][zctx]
    zctx.new[]
    zctx.destroy[ctx]
    
[zthread(3) - working with system threads][zthread]

    zthread.new[thread_fn; args]
    //  Create a detached thread. A detached thread operates autonomously
    //  and is used to simulate a separate process. It gets no ctx, and no
    //  pipe.

    zthread.fork[ctx; thread_fn]
    //  Create an attached thread. An attached thread gets a ctx and a PAIR
    //  pipe back to its parent. It must monitor its pipe, and exit if the
    //  pipe becomes unreadable. Do not destroy the ctx, the thread does this
    //  automatically when it ends.
    //  Pass arguments to thread_fn by means of sockets.

http://thread.gmane.org/gmane.network.zeromq.devel/22647/focus=22651
> The difference is for the CZMQ infrastructure; the first creates an
> "attached thread" (sharing the same context and with a pipe back to
> the parent), while the second creates a "detached thread" (shares
> nothing, so acts more like a separate process, and cannot talk over
> inproc to the parent).

http://article.gmane.org/gmane.network.zeromq.devel/19147/match=zthread_fork
> Parent thread creates two sockets, binds one, connects the other,
> creates a thread, and passes it one end of the pipe. The child thread
> can then talk to the parent over the pipe. We use PAIR sockets for
> that.

<A name="toc2-126" title="License" />
## License

    // Copyright (c) 2012, 2015 Jaeheum Han <jay.han@gmail.com>
    // This file is part of qzmq.
    // This Source Code Form is subject to the terms of the Mozilla Public
    // License, v. 2.0. If a copy of the MPL was not distributed with this
    // file, You can obtain one at http://mozilla.org/MPL/2.0/.

<A name="toc2-135" title="Files of qzmq" />
## Files of qzmq

* README.html -- from README.md
* README.md -- README.txt in Markdown syntax thanks to [Gitdown][gitdown].
* README.txt -- this file.
* kx/* -- `k.h` and `c.o` from kx.com for building qzmq. (see http://code.kx.com/wiki/TermsAndConditions)
* qzmq.c -- C bindings to be linked with CZMQ (with ØMQ) and Q (`k.h` and `c.o`).
* qzmq.q -- q code used to load the bindings.
* qzmq.so -- dynamic library to be loaded into q by `qzmq.q`.
* qzmq_test.q -- test code translated to q from CZMQ's self-test code.
* zthread_test.q -- zthread example usage.

<A name="toc2-148" title="Building qzmq" />
## Building qzmq
Prerequisites: kdb+ 3.0 or later, [ØMQ][zeromq] 4, [CZMQ][czmq] 3.
Consult your local sysadmin for installation prerequisites.

Current version 3.0.1 of qzmq has been built with 32-bit kdb+ on Ubuntu 14.04.
Instructions for builidng qzmq assumes kdb+ is instaled under `$HOME/q/` directory.

    # for Debian, Ubuntu, ... (kdb+ v3.0 "l32") with ZeroMQ & CZMQ in /usr/local/lib
    gcc -DKXVER=3 -m32 -shared -fPIC qzmq.c -o $HOME/q/l32/qzmq.so \
        -Wall -Wextra  -Wl,-rpath -Wl,/usr/local/lib \
        -I./kx/kdb+3.0 -I/usr/local/include/ \
        -L./kx/kdb+3.0/l32 -L/usr/local/lib -lzmq -lczmq
    cp qzmq.q assert.q $HOME/q/

For 64-bit kdb+, drop `-m32` gcc flag and
replace `l32` architecture path to appropriate one like `l64`.

`qzmq_test.q` contains qzmq translation of czmq's self-tests. It can be used as examples of each API.

ZeroMQ [Guide][zguide] has examples in many languages.
Translation of these examples to q is [under way][zguide-in-q]
(pulled from [here][zguide-in-q-original]).
See [Issue #5](https://github.com/jaeheum/qzmq/issues/5).

<A name="toc2-173" title="Issues" />
## Issues
See the [issue tracker][issues].
<A name="toc2-177" title="" />

---

`README.txt` is written in [Gitdown][gitdown].
    
        gitdown README.txt && markdown README.md > README.html

[qzmq]: https://github.com/jaeheum/qzmq
[zeromq]: http://www.zeromq.org
[czmq]: http://czmq.zeromq.org
[q]: http://kx.com
[czmq-reference]:http://api.zeromq.org/czmq3-0:czmq
[zsock]:http://api.zeromq.org/CZMQ3-0:zsock
[zstr]:http://api.zeromq.org/CZMQ3-0:zstr
[zmsg]:http://api.zeromq.org/CZMQ3-0:zmsg
[zframe]:http://api.zeromq.org/CZMQ3-0:zframe
[zctx]:http://api.zeromq.org/CZMQ3-0:zctx
[zthread]:http://api.zeromq.org/CZMQ3-0:zthread
[kdbdoc]: http://code.kx.com/wiki/Cookbook/ExtendingWithC
[issues]: https://github.com/jaeheum/qzmq/issues
[zguide]: http://zguide.zeromq.org
[zguide-in-q]:https://github.com/imatix/zguide/tree/master/examples/Q
[zguide-in-q-original]:https://github.com/jaeheum/zguide/tree/master/examples/Q
[gitdown]: https://github.com/imatix/gitdown
