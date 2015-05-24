flags:.Q.def[`url`size`count!(`$"inproc://lat_test"; 30; 1000*1000)].Q.opt .z.x
\l qzmq.q
\l assert.q

U:flags`url
S:flags`size
N:flags`count

fnew:zframe.new
fsnd:zframe.send
frcv:zframe.recv
fdst:zframe.destroy
REUSE:zmq`ZFRAME_REUSE

show"message size: ", (string S), " [B]";
show"roundtrip count: ", (string N);

requester:{[pipe; args]
    zsock.signal[pipe; 0x0];
    req:zsock.new_req U;
    payload:S#0xff;
    f:fnew payload;
    starttime:.z.N;
    do[N; fsnd[f; req; REUSE]; fdst frcv req];
    elapsed:.z.N-starttime; / in ns.
    show"average latency: ", (string (elapsed%(2*N))%1000), " [us]";
    zsock.destroy req}
replier:{[pipe;args]
    zsock.signal[pipe; 0x0];
    rep:zsock.new_rep U;
    do[N; fsnd[frcv rep; rep; 0]];
    zsock.destroy rep}
reply:zactor.new[replier; 0N]
request:zactor.new[requester; 0N]
zactor.destroy reply
zactor.destroy request
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
