flags:.Q.def[`url`size`count!(`$"inproc://lat_test"; 30; 1000*1000)].Q.opt .z.x
\l assert.q
\l qzmq.q

worker:{[args; ctx; pipe]
    rep:zsocket.new[ctx; zmq`REP];
    port:zsocket.connect[rep; flags`url];
    do[flags`count; zstr.send[rep; zstr.recv[rep]]]}

ctx:zctx.new[]
req:zsocket.new[ctx; zmq`REQ]
port:zsocket.bind[req; flags`url]
pipe:zthread.fork[ctx; `worker; 0N]
msg:(flags`size)#"0"
starttime:zclock.time[]
do[flags`count; zstr.send[req; msg]; zstr.recv[req]]
elapsed:zclock.time[]-starttime
show"message size: ", (string flags`size), " [B]"
show"roundtrip count: ", (string flags`count)
show"average latency: ", (string (elapsed%(2*flags`count))*1000), " [us]"

zsocket.destroy[ctx; pipe]
zctx.destroy[ctx]
\\

\
Copyright (c) 2012-2013 Jaeheum Han
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
