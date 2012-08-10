\l qzmq.q

ctx:zctx.new[]
req:zsocket.new[ctx; zmq`REQ]
zsocket.connect[req; `tcp://127.0.0.1:5559]
while[1b and not zctx.interrupted[];
    zclock.sleep 1000;
    sent:zstr.send[req; msg:(10?"qwertyuiop")," (from client pid=",(string .z.i),")"];
    zclock.log "q (pid=",(string .z.i),") received the original data: ",zstr.recv[req]]
zsocket.destroy[ctx; req]
zctx.destroy[ctx]
\\
