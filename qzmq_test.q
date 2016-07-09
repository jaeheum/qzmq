// qzmq - q bindings for CZMQ, the high-level C binding for 0MQ:
//   http://czmq.zeromq.org.
//
// Copyright (c) 2012-2016 Jaeheum Han <jay.han@gmail.com>
// This file is part of qzmq.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

show"running qzmq test"
\l qzmq.q
\l assert.q
show"running zframe test"
output:zsock.new[zmq`PAIR]
port:zsock.bind[output; `inproc://zframe.test] / for inproc, port=0.
input:zsock.new[zmq`PAIR]
port:zsock.connect[input; `inproc://zframe.test]
frame:zframe.new["hello"]
rc:zframe.send[frame; output; zmq`ZFRAME_MORE]
do[5; frame:zframe.new["hello"];
    ae[0; rc:zframe.send[frame; output; zmq`ZFRAME_MORE]]]
frame:zframe.new[1000#0x00]
do[5; ae[0; rc:zframe.send[frame; output; (zmq`ZFRAME_MORE)+zmq`ZFRAME_REUSE]]]
copy:zframe.dup[frame]
aa[zframe.eq[frame; copy]]
zframe.destroy[frame]
ae[zframe.size[copy]; count 1000#0x00]
zframe.destroy[copy]
frame:zframe.new["NOT"]
zframe.reset[frame; "END"]
ae[`END; zframe.strdup[frame]]
ae[0; rc:zframe.send[frame; output; 0i]]
end:0b; while[not end; frame:zframe.recv[input]; zframe.print[frame; `recv:];
    if[end:zframe.streq[frame; `END]; zframe.destroy[frame]]]
zsock.destroy[input]
zsock.destroy[output]
show"pass"
//  --------------------------------------------------------------------------
show"running zmsg test"
output:zsock.new[zmq`PAIR]
port:zsock.bind[output; `inproc://zmsg.test]
input::zsock.new[zmq`PAIR]
port:zsock.connect[input; `inproc://zmsg.test]
msg:zmsg.new[]
frame:zframe.new["hello"]
ae[0; zmsg.prepend[msg; frame]]
ae[1; zmsg.size[msg]]
ae[5; zmsg.content_size[msg]]
ae[0; zmsg.send[msg; output]]
msg:zmsg.recv[input]
ae[1; zmsg.size[msg]]
ae[5; zmsg.content_size[msg]]
zmsg.destroy[msg]
msg:zmsg.new[]
i:0; do[10; zmsg.prepend[msg; zframe.new["Frame",string i]]; i+:1]
copy:zmsg.dup[msg]
ae[0; zmsg.send[copy; output]]
ae[0; zmsg.send[msg; output]]
copy:zmsg.recv[input]
ae[10; zmsg.size[copy]]
ae[60; zmsg.content_size[copy]]
zmsg.destroy[copy]
msg:zmsg.recv[input]
ae[10; zmsg.size[msg]]
ae[60; zmsg.content_size[msg]]
zmsg.print[msg]
zmsg.destroy[msg]
msg:zmsg.new[]
ae[0; zmsg.size[msg]]
ae[0; zmsg.content_size[msg]]
ae["empty"; @[zmsg.first; msg; ::]]
ae["empty"; @[zmsg.next; msg; ::]]
ae["empty"; @[zmsg.last; msg; ::]]
ae["empty"; @[zmsg.pop; msg; ::]]
zmsg.destroy[msg]
zsock.destroy[input]
zsock.destroy[output]
show"pass"
//  --------------------------------------------------------------------------
show"running zsock test"
writer:zsock.new[zmq`PUSH]
reader:zsock.new[zmq`PULL]
service:5560
interf:"*"
domain:"localhost"
port:zsock.bind[writer; `$"tcp://",interf,":",string service]
ae[port; service]
port:zsock.connect[reader; `$"tcp://",domain,":",string service]
ae[0; rc:zstr.send[writer; `$"HELLO"]]
ae[0; rc:zstr.send[writer; `$"1"]]
msg:zstr.recv[reader]
0N!msg
port:zsock.bind[writer; `$"tcp://",interf,":*"]
0N!port
zsock.destroy[reader]
zsock.destroy[writer]
show"pass"
//  --------------------------------------------------------------------------
show"running zstr test"
output:zsock.new[zmq`PAIR]
input:zsock.new[zmq`PAIR]
port:zsock.bind[output;`inproc://zstr.test]
port:zsock.connect[input;`inproc://zstr.test]
i:0; do[10; zstr.send[output;`$"xxx",string i]; i+:1]
i:0; do[10; m:zstr.recv[input]; ae[m; `$"xxx",string i]; i+:1]
i:0; do[10; zstr.send[output;`$"☺⌃⌥⌘↩",string i]; i+:1]
i:0; do[10; m:zstr.recv[input]; ae[m;`$"☺⌃⌥⌘↩",string i]; i+:1]
zsock.destroy[input]
zsock.destroy[output]
show"pass"
//  --------------------------------------------------------------------------
"qzmq test done"

\\

caternation of some of czmq API's self tests translated to q.

