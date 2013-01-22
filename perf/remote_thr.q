flags:.Q.def[`url`size`count!(`tcp://127.0.0.1:55555; 30; 1000*1000)].Q.opt .z.x

\l qzmq.q
ctx:zctx.new[]
pub:zsocket.new[ctx; zmq`PUB]
port:zsocket.connect[pub; flags`url]
msg:flags.size#"0"
starttime:zclock.time[]
do[flags`count; zstr.send[pub; msg]]
elapsed:zclock.time[]-starttime
throughput:flags.count%(elapsed%1000)
megabits:throughput*flags.size*8%(1000*1000)
zsocket.destroy[ctx; pub]
zctx.destroy[ctx]
\\

\
Copyright (c) 2012-2013 Jaeheum Han
Translation of remote_thr.cpp to use qzmq/czmq APIs.
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