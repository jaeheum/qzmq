.set GIT=https://github.com/jaeheum/qzmq

# Cross-Language Distributed System With ZeroMQ and qzmq

This article outlines a few simple demonstrations of cross-language distributed systems. Please use the [issue tracker][issues] for all issues and comments. (Last edit date: &date())

## Contents

.toc 1

## Installation

See [installation.md][installation] for installing both ZeroMQ and language bindings.

## Running Clients/Servers In Different Languages

[diagram]
 lang:bash                             lang:q                                    lang:python
+----------+                          +--------------+                          +-------------+
| {io}     |                          | {io}         |                          | {io}        |
| zmq_push | 'psy oppa gangnam style' | req.q        |                          | rep.py      |
|(ZMQ_PUSH)|------------------------->|(ZMQ_PULL)    |                          |             |
|          |                          |              | 'psy oppa gangnam style' |             |
|          |                          |     (ZMQ_REQ)|------------------------->|             |  
|          |                          |              |                          | sha1        |
|          |                          |              | '7c4db1c7db942d98a6....' |             |
|          |                          |              |<-------------------------|(ZMQ_REP)    |
+----------+                          +--------------+                          +-------------+

                       Figure # - two pairs of client/server
[/diagram]

Let's set up two pairs of client/server. First pair made up with a q client (`req.q`)
and python server (`rep.py`). Launch these two in q and python in two separate terminals:

    $ q req.q -q # terminal 1, substitute with 'python req.py' if either q or qzmq is not installed.
    $ python rep.py # terminal 2

From terminal 3, start pushing text message data 

    $ source zmq_push.sh # write a message, hit return and hit `control-d` to terminate.
    $ zmq_push
    psy oppa gangnam style
    ^D
    $

This shell function acts as a client to q "server" `req.q`, that receives the message and requests SHA1 of the message from `rep.py` in Python:

    $ q req.q -q # or 'python req.py'
    12-08-09 23:25:29 q received: 'psy oppa gangnam style'
    12-08-09 23:25:29 q (pid=5553) received sha1 from perl or python: 7c4db1c7db942d98a6f885efd4de173acd3c789f
    ...
    $ python rep.py 
    python: received 'psy oppa gangnam style' from client.
    python (pid=5568): sending back sha1 of the received data: 7c4db1c7db942d98a6f885efd4de173acd3c789f
    ...

Perl can be used Instead of Python with `rep.pl`:

    $ perl rep.pl
    perl: received 'psy oppa gangnam style' from client.
    perl (pid=5646): sending back sha1 of the received data: 7c4db1c7db942d98a6f885efd4de173acd3c789f
    ...

Different ZeroMQ socket types (`ZMQ_PUSH`, `ZMQ_PULL`, `ZMQ_REQ`, `ZMQ_REP` in the Figure 1) are explained in [ZeroMQ Reference](http://api.zeromq.org/2-2:zmq-socket). 

## Multiple Clients Talking to a Single Server with Different Languages for the Clients and the Server

Launch a single server from a terminal:

    $ perl rep.pl

Launch many clients `c.py` in many terminals. `c.py` sends a date/time string every second to the server `rep.pl`:

    $ python c.py
    python (pid=7962): sending data: 'Fri Aug 10 00:03:40 2012'
    python (pid=7962): received sha1 of data: a41e87cbe2b1ee6765383de73f571f9d44087101
    python (pid=7962): sending data: 'Fri Aug 10 00:03:41 2012'
    python (pid=7962): received sha1 of data: 97bf5dfb1c76991dbb15054728826422c175e13e
    # ... in other terminals the different pid values of c.py will appear

## Multiple Clients/Multiple Servers with Load Balancing and Fault Tolerance

Multiple clients and a single server configuration has at least two problems: the server has a scalability limit and is a single point of failure (SPOF). Running multiple servers to share load is a solution to SPOF and scalability. However this solution needs more elaboration. If the clients need to maintain addresses of the servers, then the clients themselves need to handle load balancing (choosing one server out of many) and fault tolerance (as servers appear/disappear). It is obvious that clients need to coordinate among themselves to devise globally optimal dynamic configuration because otherwise poor utilization will be added to the problem of SPOF and scalability.

For a simple demonstration of multiple clients and multiple servers, we use

* clients that generate and send a random string every second to servers (`mreq.q`)
* servers that echo their input back to the clients (`mrep.pl` and `mrep.py`)
* a message queue or a request-reply broker that mediates the clients and the servers as shown in the [ZeroMQ Guide][broker].  (q version is [msgqueue.q](https://github.com/imatix/zguide/blob/master/examples/Q/msgqueue.q))

Start the message queue:

    $ q msgqueue.q -q # or 'python msgqueue.py' or 'perl msgqueue.pl'
    
Start the servers:

    $ python mrep.py
    $ perl mrep.pl
    $ perl mrep.pl
    ...

Start the clients:

    $ q mreq.q -q # 'python mreq.py' or ..
    $ q mreq.q -q # 'perl mreq.pl' (may need to install `cpan -f -i String::Random`)
    $ q mreq.q -q
    ...

Output from the servers will show multiple client pid values:

    perl: received 'turuyiuirw (from client pid=10922)' from client.
    perl (pid=10862): sending back the received data: turuyiuirw (from client pid=10922)
    perl: received 'ttyryyrqyp (from client pid=10920)' from client.
    perl (pid=10862): sending back the received data: ttyryyrqyp (from client pid=10920)
    perl: received 'toitiwtqer (from client pid=10909)' from client.
    perl (pid=10862): sending back the received data: toitiwtqer (from client pid=10909)
    ...

Output from a client pid=10909 shows the client received back what it had sent out, not the data other clients sent to servers:

    12-08-10 01:36:35 q (pid=10909) received the original data: yoqwpyuyyt (from client pid=10909)
    12-08-10 01:36:37 q (pid=10909) received the original data: pwiertptor (from client pid=10909)
    ... 

### Load Balancing and Fault Tolerance

Try the following:

* Kill (`Control-C`) a server and the clients do not notice. (fault tolerance)
* Kill all the servers and the clients "pause" or become idle.
* Start a server, it handles all requests from the clients that come out of their pause.
* Start more servers and they share the load together. (load balancing)

## Further Information

Cross-language distributed system is a large subject and we have already touched on a sensitive topic of "broker" that enables a multiple client to multiple server configuration at a cost of having a [man-in-the-middle][mitm]. For more discussions from ZeroMQ perspectives, start with:

* [ZeroMQ Guide][zguide]
* [Broker vs Brokerless][brokerless]

## License

Copyright (c) 2012 Jaeheum Han

* GNU Affero General Public License v.3.
* Non-AGPL TBD

Note that `msgqueue.py` and `msgqueue.pl` are copyright (c) 2010-2011 iMatix Corporation and Contributors. Their license is [ZeroMQ Guide examples license][zguide-license].

---

Cross-language distributed systems have nothing to do with [Psy's Gangnam Style][gangnam].

.- Reference
.-
[qzmq]: $(GIT)
[issues]: $(GIT)/issues
[zeromq]: http://www.zeromq.org
[czmq]: http://czmq.zeromq.org
[q]: http://kx.com
[zguide]: http://zguide.zeromq.org
[zguide-license]: https://github.com/imatix/zguide/blob/master/examples/LICENSE
[gitdown]: https://github.com/imatix/gitdown
[broker]: http://zguide.zeromq.org/page:all#A-Request-Reply-Broker
[brokerless]: http://www.zeromq.org/whitepapers:brokerless
[mitm]: http://en.wikipedia.org/wiki/MiTM
[perl]: http://www.zeromq.org/bindings:perl
[pyzmq]: http://www.zeromq.org/bindings:python
[languages]: http://www.zeromq.org/bindings:_start
[gangnam]: http://www.youtube.com/watch?v=9bZkp7q19f0&feature=related
[installation]: $(GIT)/blob/master/demos/installation.md

