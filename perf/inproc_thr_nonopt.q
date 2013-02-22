/dict lookups cost throughput: 400 Mb/s vs 700 Mb/s on intel i7-3520M CPU @ 2.90GHz 

flags:.Q.def[`url`size`count!(`$"inproc://thr_test"; 30; 1000*1000)].Q.opt .z.x
\l assert.q
\l qzmq.q

/snd:zstr.send
/rcv:zstr.recv
/N:flags`count

worker:{[args; ctx; pipe]
    msg:(flags`size)#"0";
    pub:zsocket.new[ctx; zmq`PUB];
    port:zsocket.connect[pub; flags`url];
    ae["ready"; zstr.recv[pipe]];
    starttime:zclock.time[];
    do[flags`count; zstr.send[pub; msg]];
    elapsed:zclock.time[]-starttime;
    / show "send elapsed time: ",(string elapsed), " [ms]";
    throughput:(flags`count)%(elapsed%1000);
    megabits:throughput*(flags`size)*8%(1000*1000);
    show "send mean throughput: ",(string throughput)," [msg/s]";
    show "send mean throughput: ", (string megabits)," [Mb/s]"}

ctx:zctx.new[]
sub:zsocket.new[ctx; zmq`SUB]
zsockopt.set_subscribe[sub; `$""]
port:zsocket.bind[sub; flags`url]
pipe:zthread.fork[ctx; `worker; 0N]
ae[0i; zstr.send[pipe; "ready"]]
show "message size: ", (string flags`size), " [B]"
show "message count: ", (string flags`count)
starttime:zclock.time[]
do[flags`count; zstr.recv[sub]]
elapsed:zclock.time[]-starttime
throughput:(flags`count)%(elapsed%1000)
megabits:throughput*(flags`size)*8%(1000*1000)
show "mean throughput: ",(string throughput)," [msg/s]"
show "mean throughput: ", (string megabits)," [Mb/s]"
zsocket.destroy[ctx; sub]
zctx.destroy[ctx]
\\

\
Copyright (c) 2012-2013 Jaeheum Han
Translation of inproc_thr.cpp to use qzmq/czmq APIs.
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
