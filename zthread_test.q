// qzmq - q bindings for CZMQ, the high-level C binding for 0MQ:
//   http://czmq.zeromq.org.
//
// Copyright (c) 2012-2015 Jaeheum Han <jay.han@gmail.com>
// This file is part of qzmq.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

\l qzmq.q
\l assert.q
show"running zctx test"
ctx:zctx.new[]; zctx.destroy[ctx];
ctx:zctx.new[];
zctx.set_iothreads[ctx; 1i]; zctx.set_linger[ctx; 5i];
zctx.destroy[ctx]
show"pass"

show"Attached threads testing"
show"Main and pong are connected with zmq`PAIR pipe."
show"Pong threads run until they receive `ping message from main."
ctx:zctx.new[]
pong:{[args;ctx;pipe]
    zsys.warning[`$"pong",(string args)," waiting for the ping"];
    while[not `ping~s:zstr.recv[pipe]; / zstr.send[pipe; s];
        do[1000*1000; md5 string args]]; // NOT .z.P (that limits parallelism)
    zstr.send[pipe; `pong]}
i:0
pipe0:zthread.fork[ctx; pong; i]; i+:1
pipe1:zthread.fork[ctx; pong; i]; i+:1
pipe2:zthread.fork[ctx; pong; i]; i+:1
pipe3:zthread.fork[ctx; pong; i]; i+:1
pipe4:zthread.fork[ctx; pong; i]; i+:1
pipe5:zthread.fork[ctx; pong; i]; i+:1
pipe6:zthread.fork[ctx; pong; i]; i+:1
pipe7:zthread.fork[ctx; pong; i]; i+:1

show""
show"Make a pong spin with ~1 thread"
do[10; zstr.send[pipe0;`xtra]]

zclock.sleep 15*500

show""
show"Make pongs spin more with ~4 threads"
do[10; zstr.send[pipe4;`xtra];
            zstr.send[pipe5;`xtra];
            zstr.send[pipe6;`xtra];
            zstr.send[pipe7;`xtra]];

zclock.sleep 15*500

show""
show"Make pongs spin more with ~8 threads"
do[10; zstr.send[pipe0;`xtra];
            zstr.send[pipe1;`xtra];
            zstr.send[pipe2;`xtra];
            zstr.send[pipe3;`xtra];
            zstr.send[pipe4;`xtra];
            zstr.send[pipe5;`xtra];
            zstr.send[pipe6;`xtra];
            zstr.send[pipe7;`xtra]];

zclock.sleep 20*500

show""
show"Send `ping and stop pongs"
rc:zstr.send[pipe0; `ping]
rc:zstr.send[pipe1; `ping]
rc:zstr.send[pipe2; `ping]
rc:zstr.send[pipe3; `ping]
rc:zstr.send[pipe4; `ping]
rc:zstr.send[pipe5; `ping]
rc:zstr.send[pipe6; `ping]
rc:zstr.send[pipe7; `ping]

zsys.warning[zstr.recv[pipe0]]
zsys.warning[zstr.recv[pipe1]]
zsys.warning[zstr.recv[pipe2]]
zsys.warning[zstr.recv[pipe3]]
zsys.warning[zstr.recv[pipe4]]
zsys.warning[zstr.recv[pipe5]]
zsys.warning[zstr.recv[pipe6]]
zsys.warning[zstr.recv[pipe7]]

zctx.destroy[ctx]

zclock.sleep 20*500

show""
pi:4*atan 1
show""
show"(sq;cos;exp)@\\:pi"
({x*x};cos;exp)@\:pi
show""
show"Three threads running sq, cos, exp of pi respectively."
sq:{show x*x}
rc:zthread.new[sq; pi]
rc:zthread.new[c:{show cos x}; pi]
rc:zthread.new[e:{show exp x}; pi]

zclock.sleep 1234

show"zn:zthread.new shortcut"
zn:zthread.new
rc:zn[sq;pi]
rc:zn[c;pi]
rc:zn[e;pi]

zclock.sleep 1234

show"Start a thread with a function argument (higher-order programming!)"
show"f:{[function] show string function pi} on detached threads."
f:{[function] show function pi}
rc:zthread.new[f; {x*x}]
rc:zthread.new[f; cos]
rc:zthread.new[f; exp]

zclock.sleep 1234

show""
show".z.c (# of cores) detached threads with busy loop"
busyloop:{[args]
    zsys.warning[args];
    while[1; md5 string args]}
do[.z.c; ae[0; rc:zthread.new[busyloop; `$(string .z.P)]]]
show"Sleeping for 20 seconds while .z.c busyloops are running."
show string .z.p
zclock.sleep 30*1000
show string .z.p
show"4*.z.c threads but lesser CPU per thread."
do[3*.z.c; ae[0; rc:zthread.new[busyloop; `$(string .z.P)]]]
show"Sleeping for 20 seconds after adding 3*.z.c more busyloops."
show string .z.p
zclock.sleep 20*1000
show string .z.p
show""
show"For the people who missed qplplot demo"
zclock.sleep 10*1000
\l /home/hjh/x/svn/contrib/jhan/qplplot/x21.q

\\


