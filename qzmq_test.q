ae:{if[not x~y;'`fail]}
aa:{if[not x;'`fail]}

"running qzmq test"
\l qzmq.q
//  --------------------------------------------------------------------------
"running zclock test"
start:zclock.time[]; zclock.sleep[10j]; aa[zclock.time[]>=start+10j]
zclock.log["log format is: YY-MM-DD hh:mm:ss followed by user entered text"]
"pass"
//  --------------------------------------------------------------------------
"running zctx test"
ctx:zctx.new[]; zctx.destroy[ctx];
ctx:zctx.new[]; zctx.set_iothreads[ctx; 1i]; zctx.set_linger[ctx; 5i]; zctx.destroy[ctx]
"pass"
//  --------------------------------------------------------------------------
"running zfile test"
ae[-1i; zfile.delete[`:nosuchfile]]
ae[0i; zfile.exists[`:nosuchfile]]
ae[-1j; zfile.size[`:nosuchfile]]
aa[zfile.size[`:qzmq_test.q]>0]
ae[1i; zfile.exists`:qzmq_test.q]
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
    ae[0i; rc:zframe.send[frame; output; zmq.ZFRAME_MORE]]]
frame:zframe.new[1000#0x00] /"hello2"
do[5; ae[0i; rc:zframe.send[frame; output; zmq.ZFRAME_MORE+zmq.ZFRAME_REUSE]]]
copy:zframe.dup[frame]
aa[zframe.eq[frame; copy]]
zframe.destroy[frame]
ae[zframe.size[copy]; "j"$count 1000#0x00] / "hello2"
zframe.destroy[copy]
frame:zframe.new["NOT"]
zframe.reset[frame; "END"]
ae[0x454e44; zframe.strdup[frame]]
ae[0i; rc:zframe.send[frame; output; 0i]]
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
ae[0i; rc:zloop.timer[loop; 10; 1; `timer_event; output]]
socket_event:{[loop;item;arg] :-1}
ae[0i; rc:zloop.poller[loop; (input;0;zmq.POLLIN;0h); `socket_event; "socket_event"]]
ae[-1i; rc:zloop.start[loop]] / "Returns 0 if interrupted, -1 if cancelled by a handler."
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
ae[1j; zmsg.size[msg]]
ae[5j; zmsg.content_size[msg]]
ae[0i; zmsg.send[msg; output]]
msg:zmsg.recv[input]
ae[1j; zmsg.size[msg]]
ae[5j; zmsg.content_size[msg]]
zmsg.destroy[msg]
msg:zmsg.new[]
i:0; do[10; zmsg.push[msg; zframe.new["Frame",string i]]; i+:1]
copy:zmsg.dup[msg]
ae[0i; zmsg.send[copy; output]]
ae[0i; zmsg.send[msg; output]]
copy:zmsg.recv[input]
ae[10j; zmsg.size[copy]]
ae[60j; zmsg.content_size[copy]]
zmsg.destroy[copy]
msg:zmsg.recv[input]
ae[10j; zmsg.size[msg]]
ae[60j; zmsg.content_size[msg]]
zmsg.dump[msg]
ae[0i; zmsg.save[msg; `:zmsg.test]]
ae[10j; zmsg.size[msg:zmsg.load[zmsg.new[]; `:zmsg.test]]]
ae[60j; zmsg.content_size[msg]]
ae[0i; rc:zfile.delete[`:zmsg.test]]
zmsg.destroy[msg]
msg:zmsg.new[]
ae[0j; zmsg.size[msg]]
ae[0j; zmsg.content_size[msg]]
ae["empty"; @[zmsg.first; msg; ::]]
ae["empty"; @[zmsg.next; msg; ::]]
ae["empty"; @[zmsg.last; msg; ::]]
ae["empty"; @[zmsg.pop; msg; ::]]
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
ae[port; "i"$service]
port:zsocket.connect[reader; `$"tcp://",domain,":",string service]
ae[0i; rc:zstr.send[writer; "HELLO"]]
ae[0i; rc:zstr.send[writer; "1"]]
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
ae[zsockopt.hwm[socket]; 1i]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_swap[socket; 1i]
ae[zsockopt.swap[socket]; 1i]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_affinity[socket; 1i]
ae[zsockopt.affinity[socket]; 1i]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_identity[socket; "test"]
ae["test"; zsockopt.identity socket]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_rate[socket; 1i]
ae[zsockopt.rate[socket]; 1i]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_recovery_ivl[socket; 1i]
ae[1i; zsockopt.recovery_ivl[socket]]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_recovery_ivl_msec[socket; 1i]
ae[zsockopt.recovery_ivl_msec[socket]; 1i]
zsocket.destroy[ctx; socket]
socket:zsocket.new[ctx; zmq.SUB]
zsockopt.set_mcast_loop[socket; 1i]
ae[zsockopt.mcast_loop[socket]; 1i]
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
i:0; do[10; m:zstr.recv[input]; ae[m; "xxx",string i]; i+:1]
i:0; do[10; zstr.send[output;"☺⌃⌥⌘↩",string i]; i+:1]
i:0; do[10; m:zstr.recv[input]; ae[m; "☺⌃⌥⌘↩",string i]; i+:1]
zsocket.destroy[ctx; input]
zsocket.destroy[ctx; output]
zctx.destroy[ctx]
"pass"
//  --------------------------------------------------------------------------
"running zthread test"
/ detached thread
ctx:zctx.new[]
dthr:{ctx:zctx.new[]; push:zsocket.new[ctx; zmq.PUSH]; zclock.sleep[1000]; zctx.destroy[ctx]; :0N}
ae[0i; rc:zthread.new[`dthr; 0N]]
zclock.sleep 100
/attached thread
pong:{[args;ctx;pipe] zsocket.new[ctx; zmq.PUSH]; zstr.recv[pipe]; zstr.send[pipe; "pong"]}
pipe:zthread.fork[ctx; `pong; 0N]
ae[0i; ping:zstr.send[pipe; "ping"]]
ae["pong"; result:zstr.recv[pipe]]
zctx.destroy[ctx]
"pass"
//  --------------------------------------------------------------------------
"qzmq test done"
\\
caternation of czmq API's self tests translated to q.
