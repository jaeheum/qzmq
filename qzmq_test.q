"running qzmq test"
\l qzmq.q
//  --------------------------------------------------------------------------
"running zclock test"
start:zclock.time[]; zclock.sleep[10i]; if[(zclock.time[] - start)<10i; '`fail]
zclock.log["log format is: YY-MM-DD hh:mm:ss followed by user entered text"]
"pass"
//  --------------------------------------------------------------------------
"running zctx test"
ctx:zctx.new[]; zctx.destroy[ctx];
ctx:zctx.new[]; zctx.set_iothreads[ctx; 1i]; zctx.set_linger[ctx; 5i]; zctx.destroy[ctx]
"pass"
//  --------------------------------------------------------------------------
"running zfile test"
if[-1<>zfile.delete[`:nosuchfile]; '`fail]
if[0b<>zfile.exists[`:nosuchfile]; '`fail]
if[-1<>zfile.size[`:nosuchfile]; '`fail]
if[not zfile.size[`:qzmq_test.q]>0; '`fail]
if[1b<>zfile.exists[`:qzmq_test.q]; '`fail]
"pass"
//  --------------------------------------------------------------------------
"running zframe test"
ctx:zctx.new[]
output:zsocket.new[ctx; zmq.PAIR]
port:zsocket.bind[output; `inproc://zframe.test] / for inproc, port=0.
input:zsocket.new[ctx; zmq.PAIR]
port:zsocket.connect[input; `inproc://zframe.test]
frame:zframe.new["hello"]
rc:zframe.send[frame; output; zmq.ZFRAME_MORE]
do[5; frame:zframe.new["hello"];
    if[0<>rc:zframe.send[frame; output; zmq.ZFRAME_MORE]; '`fail]]
frame:zframe.new[1000#0x00] /"hello2"
do[5; if[0<>rc:zframe.send[frame; output; zmq.ZFRAME_MORE+zmq.ZFRAME_REUSE]; '`fail]]
copy:zframe.dup[frame]
if[not zframe.eq[frame; copy]; '`fail]
zframe.destroy[frame]
if[zframe.size[copy]<>count 1000#0x00; '`fail] / "hello2"
zframe.destroy[copy]
frame:zframe.new["NOT"]
zframe.reset[frame; "END"]
if[not 0x454e44~zframe.strdup[frame]; '`fail]
if[0<>rc:zframe.send[frame; output; 0i]; '`fail]
end:0b; while[not end; frame:zframe.recv[input]; zframe.print[frame; "recv:"]; if[end:zframe.streq[frame; "END"]; zframe.destroy[frame]]]
zctx.destroy[ctx]
"pass"
//  --------------------------------------------------------------------------
"running zloop test"
ctx:zctx.new[]
output:zsocket.new[ctx; zmq.PAIR]
outputport:zsocket.bind[output; `inproc://zloop.test]
input:zsocket.new[ctx; zmq.PAIR]
inputport:zsocket.connect[input; `inproc://zloop.test]
loop:zloop.new[]
zloop.set_verbose[loop; 0b]
timer_event:{[loop;item;arg] zstr.send[output; "PING"]; :0}
if[0<>rc:zloop.timer[loop; 10; 1; `timer_event; output]; '`fail]
socket_event:{[loop;item;arg] :-1}
if[0<>rc:zloop.poller[loop; (input;0;zmq.POLLIN;0h); `socket_event; "socket_event"]; '`fail]
if[-1<>rc:zloop.start[loop]; '`fail] / "Returns 0 if interrupted, -1 if cancelled by a handler."
zloop.destroy[loop]
zctx.destroy[ctx]
"pass"
//  --------------------------------------------------------------------------
"running zmsg test"
ctx:zctx.new[]
output:zsocket.new[ctx; zmq.PAIR]
port:zsocket.bind[output; `inproc://zmsg.test]
input:zsocket.new[ctx; zmq.PAIR]
port:zsocket.connect[input; `inproc://zmsg.test]
msg:zmsg.new[]
frame:zframe.new["hello"]
zmsg.push[msg; frame]
if[1<>zmsg.size[msg]; '`fail]
if[5<>zmsg.content_size[msg]; '`fail]
if[0<>zmsg.send[msg; output]; '`fail]
msg:zmsg.recv[input]
if[1<>zmsg.size[msg]; '`fail]
if[5<>zmsg.content_size[msg]; '`fail]
zmsg.destroy[msg]
msg:zmsg.new[]
i:0; do[10; zmsg.push[msg; zframe.new["Frame",string i]]; i+:1]
copy:zmsg.dup[msg]
if[0<>zmsg.send[copy; output]; '`fail]
if[0<>zmsg.send[msg; output]; '`fail]
copy:zmsg.recv[input]
if[10<>zmsg.size[copy]; '`fail]
if[60<>zmsg.content_size[copy]; '`fail]
zmsg.destroy[copy]
msg:zmsg.recv[input]
if[10<>zmsg.size[msg]; '`fail]
if[60<>zmsg.content_size[msg]; '`fail]
zmsg.dump[msg]
if[0<>zmsg.save[msg; `:zmsg.test]; '`fail]
if[10<>zmsg.size[msg:zmsg.load[zmsg.new[]; `:zmsg.test]]; '`fail]
if[60<>zmsg.content_size[msg]; '`fail]
if[0<>rc:zfile.delete[`:zmsg.test]; '`fail]
zmsg.destroy[msg]
msg:zmsg.new[]
if[0<>zmsg.size[msg]; '`fail]
if[0<>zmsg.content_size[msg]; '`fail]
if[not "empty" like @[zmsg.first; msg; ::]; '`fail]
if[not "empty" like @[zmsg.next; msg; ::]; '`fail]
if[not "empty" like @[zmsg.last; msg; ::]; '`fail]
if[not "empty" like @[zmsg.pop; msg; ::]; '`fail]
zmsg.destroy[msg]
zsocket.destroy[ctx; input]
zsocket.destroy[ctx; output]
zctx.destroy[ctx]
"pass"
//  --------------------------------------------------------------------------
"running zsocket test"
ctx:zctx.new[]
writer:zsocket.new[ctx; zmq.PUSH]
reader:zsocket.new[ctx; zmq.PULL]
service:5560
interf:"*"
domain:"localhost"
port:zsocket.bind[writer; `$"tcp://",interf,":",string service]
if[port<>service; '`fail]
port:zsocket.connect[reader; `$"tcp://",domain,":",string service]
if[0i<>rc:zstr.send[writer; "HELLO"]; '`fail]
if[0i<>rc:zstr.send[writer; "1"]; '`fail]
msg:zstr.recv[reader]
0N!msg
port:zsocket.bind[writer; `$"tcp://",interf,":*"]
0N!port
zsocket.destroy[ctx; writer]
zctx.destroy[ctx]
"pass"
//  --------------------------------------------------------------------------
"running zsockopt test"
ctx:zctx.new[]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_hwm[socket; 1i]
if[zsockopt.hwm[socket]<>1; '`fail]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_swap[socket; 1i]
if[zsockopt.swap[socket]<>1; '`fail]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_affinity[socket; 1i]
if[zsockopt.affinity[socket]<>1; '`fail]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_identity[socket; "test"]
if[not "test" like zsockopt.identity socket; '`fail]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_rate[socket; 1i]
if[zsockopt.rate[socket]<>1; '`fail]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_recovery_ivl[socket; 1i]
if[zsockopt.recovery_ivl[socket]<>1; '`fail]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_recovery_ivl_msec[socket; 1i]
if[zsockopt.recovery_ivl_msec[socket]<>1; '`fail]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_mcast_loop[socket; 1i]
if[zsockopt.mcast_loop[socket]<>1; '`fail]
zsocket.destroy[ctx; socket]
zctx.destroy[ctx]
"pass"
//  --------------------------------------------------------------------------
"running zstr test"
ctx:zctx.new[]
output:zsocket.new[ctx;zmq.PAIR]
input:zsocket.new[ctx;zmq.PAIR]
port:zsocket.bind[output;`inproc://zstr.test]
port:zsocket.connect[input;`inproc://zstr.test]
i:0; do[10; zstr.send[output;"xxx",string i]; i+:1]
i:0; do[10; m:zstr.recv[input]; if[not m like "xxx",string i; '`fail]; i+:1]
i:0; do[10; zstr.send[output;"☺⌃⌥⌘↩",string i]; i+:1]
i:0; do[10; m:zstr.recv[input]; if[not m like "☺⌃⌥⌘↩",string i; '`fail]; i+:1]
zsocket.destroy[ctx; input]
zsocket.destroy[ctx; output]
zctx.destroy[ctx]
"pass"
//  --------------------------------------------------------------------------
"running zthread test"
/ detached thread
ctx:zctx.new[]
dthr:{ctx:zctx.new[]; push:zsocket.new[ctx; zmq.PUSH]; zclock.sleep[1000i]; zctx.destroy[ctx]; :0N}
if[0i<>rc:zthread.new[`dthr; 0N]; '`fail]
zclock.sleep 100i
/attached thread
pong:{[args;ctx;pipe] zsocket.new[ctx; zmq.PUSH]; zstr.recv[pipe]; zstr.send[pipe; "pong"]}
pipe:zthread.fork[ctx; `pong; 0N]
if[0i<>ping:zstr.send[pipe; "ping"]; '`fail]
if[not "pong" like result:zstr.recv[pipe]; '`fail]
zctx.destroy[ctx]
"pass"
//  --------------------------------------------------------------------------
"qzmq test done"
\\
caternation of czmq API's self tests translated to q.
