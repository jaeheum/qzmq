flags:.Q.def[`url`size`count!(`$"inproc://lat_test"; 30; 1000*1000)].Q.opt .z.x
\l qzmq.q
\l assert.q

snd:zstr.send
rcv:zstr.recv
N:flags`count

worker:{[pipe;args]
    zsock.signal[pipe; 0x0];
    rep:zsock.new[zmq`REP];
    port:zsock.connect[rep; flags`url];
    do[N; snd[rep; rcv[rep]]];
    zsock.destroy[rep]}

m0:.Q.w[]
show "m0:.Q.w[] / before running with zstr."
show m0;
req:zsock.new[zmq`REQ]
port:zsock.bind[req; flags`url]
pipe:zactor.new[worker; 0N]
msg:`$(flags`size)#"0"
starttime:.z.P / zclock.time[]
do[N; snd[req; msg]; rcv[req]]
show ".Q.w[]-m0 / after-before with zstr."
show .Q.w[]-m0
show ".Q.gc[]"
.Q.gc[]
elapsed:.z.P-starttime / in ns.
show"message size: ", (string flags`size), " [B]"
show"roundtrip count: ", (string N)
show"average latency: ", (string (elapsed%(2*N))%1000), " [us]"
zsock.destroy[req]
zactor.destroy[pipe]

/ redo with zmsg, not zstr.
mworker:{[pipe;args]
    zsock.signal[pipe; 0x0];
    rep:zsock.new[zmq`REP];
    port:zsock.connect[rep; flags`url];
    do[N; msnd[mrcv[rep]; rep]];
    zsock.destroy[rep]}

msnd:zmsg.send
mrcv:zmsg.recv
m0:.Q.w[]
show "m0:.Q.w[] / before running with zmsg."
show m0;
req:zsock.new[zmq`REQ]
port:zsock.bind[req; flags`url]
pipe:zactor.new[mworker; 0N]
msg:zmsg.new[]
frame:zframe.new[(flags`size)#"0"]
zmsg.prepend[msg; frame]
starttime:.z.P / zclock.time[]
do[N; msnd[msg; req]; mrcv[req]]
show ".Q.w[]-m0 / after-before with zmsg."
show .Q.w[]-m0
show ".Q.gc[]"
.Q.gc[]
elapsed:.z.P-starttime / in ns.
show"message size: ", (string flags`size), " [B]"
show"roundtrip count: ", (string N)
show"average latency: ", (string (elapsed%(2*N))%1000), " [us]"
zsock.destroy[req]
zactor.destroy[pipe]
\\



\
Copyright (c) 2012-2015 Jaeheum Han
Translation of inproc_lat.cpp to use qzmq/czmq APIs.
N.B. instead of spawning the worker thread by hand, we use an attached thread
with zthread_fork.

/*
    Copyright (c) 2007-2012 iMatix Corporation
    Copyright (c) 2009-2011 250bpm s.r.o.
    Copyright (c) 2007-2011 Other contributors as noted in the AUTHORS file

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
