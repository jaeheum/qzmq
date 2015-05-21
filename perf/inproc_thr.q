flags:.Q.def[`url`size`count!(`$"inproc://thr_test"; 30; 1000*1000)].Q.opt .z.x
\l assert.q
\l qzmq.q

snd:zstr.send
rcv:zstr.recv
N:flags`count
S:flags`size

show"simple zstr throughput with push/pull"
pusher:{[pipe; args]
    zsock.signal[pipe; 0x0];
    msg:`$S#"0";
    push:zsock.new_push[flags`url];
    ae[`ready; rcv[pipe]];
    do[N; snd[push; msg]];
    zsock.destroy[push]}

pull:zsock.new_pull[flags`url]
pipe:zactor.new[pusher; 0N]
ae[0; rc:zstr.send[pipe; `ready]]
show "message size: ", (string S), " [B]"
show "message count: ", (string N)
m:rcv[pull]
starttime:.z.P / in ns
do[N-1; rcv[pull]]
elapsed:.z.P-starttime
throughput:(N%elapsed)*(1000*1000*1000)
megabits:throughput*S*8%(1000*1000)
show "mean throughput: ",(string throughput)," [msg/s]"
show "mean throughput: ",(string megabits)," [Mb/s]"
zsock.destroy[pull]
zactor.destroy[pipe]

mrcv:zmsg.recv
msnd:zmsg.send
mdst:zmsg.destroy
mprp:zmsg.prepend
mnew:zmsg.new
fnew:zframe.new

show""
show"simple zmsg throughput with push/pull"
mpusher:{[pipe; args]
    zsock.signal[pipe; 0x0];
    push:zsock.new_push[flags`url];
    ae[`ready; rcv[pipe]];
    do[N; mprp[m:mnew[]; f:fnew[S#"0"]];
        msnd[m; push]];
    zsock.destroy[push]}

pull:zsock.new_pull[flags`url]
pipe:zactor.new[mpusher; 0N]
ae[0; rc:zstr.send[pipe; `ready]]
show "message size: ", (string S), " [B]"
show "message count: ", (string N)
mdst mrcv pull
starttime:.z.N / in ns
do[N-1; mdst[mrcv[pull]]]
elapsed:.z.N-starttime
throughput:(N%elapsed)*(1000*1000*1000) / 1/s
megabits:throughput*S*8%(1000*1000)
show "mean throughput: ",(string throughput)," [msg/s]"
show "mean throughput: ",(string megabits)," [Mb/s]"
zsock.destroy[pull]
zactor.destroy[pipe]

show""
show"zmsg throughput with push/pull with prepared zmsg's before pushing"
pmpusher:{[pipe; args]
    zsock.signal[pipe; 0x0];
    push:zsock.new_push[flags`url];
    ae[`ready; rcv[pipe]];
    m:(); f:(); do[N; m,:mnew[]; f,:fnew[S#"0"]];
    mprp ./:flip(m;f);
    msnd[;push] @/:m;
    zsock.destroy[push]}

pull:zsock.new_pull[flags`url]
pipe:zactor.new[pmpusher; 0N]
ae[0; rc:zstr.send[pipe; `ready]]
show "message size: ", (string S), " [B]"
show "message count: ", (string N)
mdst mrcv pull
starttime:.z.N / in ns
do[N-1; mdst[m:mrcv[pull]]]
elapsed:.z.N-starttime
throughput:(N%elapsed)*(1000*1000*1000) / 1/s
megabits:throughput*S*8%(1000*1000)
show "mean throughput: ",(string throughput)," [msg/s]"
show "mean throughput: ",(string megabits)," [Mb/s]"
zsock.destroy[pull]
zactor.destroy[pipe]

show""
show"simple zmsg throughput with pub/sub and null subscription"
publisher:{[pipe; args]
    zsock.signal[pipe; 0x0];
    ae[`ready; rcv[pipe]];
    pubs:zsock.new_pub[flags`url];
    do[N; mprp[m:mnew[]; f:fnew[S#"0"]];
        msnd[m; pubs]];
    ae[`done; rcv[pipe]];
    zsock.destroy[pubs]}

subs:zsock.new_sub[flags`url; `]
pipe:zactor.new[publisher; 0N]
ae[0; rc:zstr.send[pipe; `ready]]
show "message size: ", (string S), " [B]"
show "message count: ", (string N)
mdst mrcv subs
starttime:.z.N / in ns
do[N-1; mdst mrcv[subs]]
elapsed:.z.N-starttime
ae[0; rc:zstr.send[pipe; `done]]
throughput:(N%elapsed)*(1000*1000*1000) / 1/s
megabits:throughput*S*8%(1000*1000)
show "mean throughput: ",(string throughput)," [msg/s]"
show "mean throughput: ",(string megabits)," [Mb/s]"
zsock.destroy[subs]
zactor.destroy[pipe]

show""
show"simple zmsg throughput with pub/sub matching subscription"
publisher:{[pipe; args]
    zsock.signal[pipe; 0x0];
    ae[`ready; rcv[pipe]];
    pubs:zsock.new_pub[flags`url];
    do[N; mprp[m:mnew[]; f:fnew[S#"0"]];
        msnd[m; pubs]];
    ae[`done; rcv[pipe]];
    zsock.destroy[pubs]}

subs:zsock.new_sub[flags`url; `$S#"0"]
pipe:zactor.new[publisher; 0N]
ae[0; rc:zstr.send[pipe; `ready]]
show "message size: ", (string S), " [B]"
show "message count: ", (string N)
mdst mrcv subs
starttime:.z.N / in ns
do[N-1; mdst mrcv[subs]]
elapsed:.z.N-starttime
ae[0; rc:zstr.send[pipe; `done]]
throughput:(N%elapsed)*(1000*1000*1000) / 1/s
megabits:throughput*S*8%(1000*1000)
show "mean throughput: ",(string throughput)," [msg/s]"
show "mean throughput: ",(string megabits)," [Mb/s]"
zsock.destroy[subs]
zactor.destroy[pipe]
\\

\
Copyright (c) 2012-2015 Jaeheum Han
Translation of inproc_thr.cpp to use qzmq/czmq APIs.
N.B. instead of spawning the worker thread by hand,
we use an attached thread/actor using zactor.new.

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
