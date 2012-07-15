"running qzmq test"
\l qzmq.q
//  --------------------------------------------------------------------------
"running zclock test"
start:zclock.time[]; zclock.sleep[10]; if[(zclock.time[] - start)<10; '`fail]
zclock.log["log format is: YY-MM-DD hh:mm:ss followed by user entered text"]
"pass"
//  --------------------------------------------------------------------------
"running zctx test"
ctx:zctx.new[]; zctx.destroy[ctx];
ctx:zctx.new[]; zctx.set_iothreads[ctx; 1]; zctx.set_linger[ctx; 5]; zctx.destroy[ctx]
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
zsocket.connect[input; `inproc://zframe.test]
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
/if[max "454E44"<>zframe.strhex[frame]; '`fail]
if[max 0x454e44<>zframe.strdup[frame]; '`fail]
rc:zframe.send[frame; output; 0]; if[rc<>0; '`fail]
end:0b; while[not end; frame:zframe.recv[input]; zframe.print[frame; "recv:"]; if[end:zframe.streq[frame; "END"]; zframe.destroy[frame]]]
zctx.destroy[ctx]
"pass"
//  --------------------------------------------------------------------------
"running zmsg test"
ctx:zctx.new[]
output:zsocket.new[ctx; zmq.PAIR]
port:zsocket.bind[output; `inproc://zmsg.test]
input:zsocket.new[ctx; zmq.PAIR]
zsocket.connect[input; `inproc://zmsg.test]
msg:zmsg.new[]
frame:zframe.new["hello"]
zmsg.push[msg; frame]
if[1<>zmsg.size[msg]; '`fail]
if[5<>zmsg.content_size[msg]; '`fail]
zmsg.send[msg; output]
msg:zmsg.recv[input]
if[1<>zmsg.size[msg]; '`fail]
if[5<>zmsg.content_size[msg]; '`fail]
zmsg.destroy[msg]
msg:zmsg.new[]
i:0; do[10; zmsg.push[msg; zframe.new["Frame",string i]]; i+:1]
copy:zmsg.dup[msg]
zmsg.send[copy; output]
zmsg.send[msg; output]
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
rc:zfile.delete[`:zmsg.test]; if[rc<>0; '`fail]
zmsg.destroy[msg]
msg:zmsg.new[]
if[0<>zmsg.size[msg]; '`fail]
if[0<>zmsg.content_size[msg]; '`fail]
if[min "empty"<>@[zmsg.first; msg; ::]; '`fail]
if[min "empty"<>@[zmsg.next; msg; ::]; '`fail]
if[min "empty"<>@[zmsg.last; msg; ::]; '`fail]
if[min "empty"<>@[zmsg.pop; msg; ::]; '`fail]
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
zsocket.connect[reader; `$"tcp://",domain,":",string service]
s:zstr.send[writer; "HELLO"]; if[max[s<>"HELLO"]; '`fail]
s1:zstr.send[writer; "1"]; if[max[s1<>"1"]; '`fail]
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
zsockopt.set_hwm[socket; 1]
if[zsockopt.hwm[socket]<>1; '`fail]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_swap[socket; 1]
if[zsockopt.swap[socket]<>1; '`fail]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_affinity[socket; 1]
if[zsockopt.affinity[socket]<>1; '`fail]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_identity[socket; "test"]
if[max["test"<>zsockopt.identity socket]; '`fail]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_rate[socket; 1]
if[zsockopt.rate[socket]<>1; '`fail]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_recovery_ivl[socket; 1]
if[zsockopt.recovery_ivl[socket]<>1; '`fail]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_recovery_ivl_msec[socket; 1]
if[zsockopt.recovery_ivl_msec[socket]<>1; '`fail]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_mcast_loop[socket; 1]
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
zsocket.connect[input;`inproc://zstr.test]
i:0; do[10; zstr.send[output;"xxx",string i]; i+:1]
i:0; do[10; m:zstr.recv[input]; if[min m<>"xxx",string i; '`fail]; i+:1]
i:0; do[10; zstr.send[output;"☺⌃⌥⌘↩",string i]; i+:1]
i:0; do[10; m:zstr.recv[input]; if[min m<>"☺⌃⌥⌘↩",string i; '`fail]; i+:1]
m0:zstr.send0[output;1000000#"A"]
m:zstr.recv[input]; if[min m<>m0; '`fail]
zsocket.destroy[ctx; input]
zsocket.destroy[ctx; output]
zctx.destroy[ctx]
"pass"
//  --------------------------------------------------------------------------
"running zthread test"
/ detached thread
ctx:zctx.new[]
dthr:{ctx:zctx.new[]; push:zsocket.new[ctx; zmq.PUSH]; zclock.sleep[1000]; zctx.destroy[ctx]; :0N}
rc:zthread.new[`dthr; 0N]
if[rc<>0; '`fail]
zclock.sleep 100
/attached thread
pong:{[args;ctx;pipe] zsocket.new[ctx; zmq.PUSH]; zstr.recv[pipe]; zstr.send[pipe; "pong"]}
pipe:zthread.fork[ctx; `pong; 0N]
ping:zstr.send[pipe; "ping"]; if[max[ping<>"ping"]; '`fail]
result:zstr.recv[pipe]
if[max result<>"pong"; '`fail]
zctx.destroy[ctx]
"pass"
//  --------------------------------------------------------------------------
"qzmq test done"
\\
caternation of czmq API's self tests translated to q.
