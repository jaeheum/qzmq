\l qzmq.q

ctx:zctx.new[]
pull:zsocket.new[ctx; zmq`PULL]
source:zsocket.bind[pull; `tcp://127.0.0.1:5555]
plpy:zsocket.new[ctx; zmq`REQ]
zsocket.connect[plpy; `tcp://127.0.0.1:5556]
while[1b and not zctx.interrupted[];
    zclock.log "q received: '",(msg:zstr.recv[pull]),"'";
    sent:zstr.send[plpy; msg];
    zclock.log "q (pid=",(string .z.i),") received sha1 from perl or python: ",sha1:zstr.recv[plpy]]
zsocket.destroy[ctx; pull]
zsocket.destroy[ctx; plpy]
zctx.destroy[ctx]
\\
