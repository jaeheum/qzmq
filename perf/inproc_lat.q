flags:.Q.def[`url`size`count!(`$"inproc://lat_test"; 30; 1000*1000)].Q.opt .z.x
\l qzmq.q
\l assert.q

snd:zstr.send
rcv:zstr.recv
N:flags`count
S:flags`size
/
replier:{[pipe;args]
    zsock.signal[pipe; 0x0];
    rep:zsock.new_rep[flags`url];
    do[N; snd[rep; rcv[rep]]];
    zsock.destroy[rep]}

m0:.Q.w[]
show "m0:.Q.w[] / before running with zstr."
show m0;
req:zsock.new_req[flags`url]
pipe:zactor.new[replier; 0N]
msg:`$(flags`size)#"0"
starttime:.z.N
do[N; snd[req; msg]; rcv[req]]
elapsed:.z.N-starttime / in ns.
show ".Q.w[]-m0 / after-before with zstr."
show .Q.w[]-m0
show ".Q.gc[]"
.Q.gc[]
show"message size: ", (string flags`size), " [B]"
show"roundtrip count: ", (string N)
show"average latency: ", (string (elapsed%(2*N))%1000), " [us]"
zsock.destroy[req]
zactor.destroy[pipe]
\
/ redo with zmsg, not zstr.
msnd:zmsg.send
mrcv:zmsg.recv
mdst:zmsg.destroy
mprp:zmsg.prepend
mnew:zmsg.new
fnew:zframe.new

replier:{[pipe;args]
    zsock.signal[pipe; 0x0];
    rep:zsock.new_rep[flags`url];
    do[N; msnd[mrcv[rep]; rep]];
    zsock.destroy[rep]}

m0:.Q.w[]
show "m0:.Q.w[] / before running with zmsg."
show m0;
req:zsock.new_req[flags`url]
pipe:zactor.new[replier; 0N]
msg:zmsg.new[]
frame:zframe.new[(flags`size)#"0"]
zmsg.prepend[msg; frame]
starttime:.z.N
do[N; mprp[m:mnew[]; f:fnew[S#"0"]]; msnd[m; req]; mdst mrcv[req]]
elapsed:.z.N-starttime / in ns.
show ".Q.w[]-m0 / after-before with zmsg."
show .Q.w[]-m0
show ".Q.gc[]"
.Q.gc[]
show"message size: ", (string flags`size), " [B]"
show"roundtrip count: ", (string N)
show"average latency: ", (string (elapsed%(2*N))%1000), " [us]"
zsock.destroy[req]
zactor.destroy[pipe]
\\



\
Copyright (c) 2012-2015 Jaeheum Han
Translation of inproc_lat.cpp to use qzmq/czmq APIs.
N.B. instead of spawning the replier thread by hand, we use an attached thread
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
