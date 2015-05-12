// qzmq - q bindings for CZMQ, the high-level C binding for 0MQ:
//   http://czmq.zeromq.org.
//
// Copyright (c) 2012, 2015 Jaeheum Han <jay.han@gmail.com>
// This file is part of qzmq.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

\l qzmq.q
\l assert.q

show"echo_actor"
echo_actor:{[pipe;args]
    zsock.signal[pipe; 0x0];
    terminated:0b;
    while[not terminated;
        m:zmsg.recv[pipe];
        if[zmsg.is[m]; 
            [command:zmsg.popstr[m];
                if[command~`$"$TERM"; terminated:1b];
                $[command~`ECHO; zmsg.send[m; pipe];
                    [zsys.debug `$"E: invalid message to actor";
                    zmsg.destroy[m]]];
            ]]];
     if[terminated; zsys.debug `$"$TERM"]}

a:zactor.new[echo_actor; `$"Hello, World"]
rc:zstr.sendx[a; `ECHO; `$"This is a string"]
zsys.info zstr.recv[a]
rc:zstr.send[a; `$"$TERM"]
zactor.destroy[a]


show"ping/pong"
pong:{[pipe;args]
    zsys.warning[`$"pong",(string args)," waiting for the ping"];
    zsock.signal[pipe;0x0];
    while[not `ping~s:zstr.recv[pipe];
        do[1000*1000; md5 string args]];
    zstr.send[pipe; `pong]}

i:0
pong0:zactor.new[pong; i]; i+:1
pong1:zactor.new[pong; i]; i+:1
pong2:zactor.new[pong; i]; i+:1
pong3:zactor.new[pong; i]; i+:1
pong4:zactor.new[pong; i]; i+:1
pong5:zactor.new[pong; i]; i+:1
pong6:zactor.new[pong; i]; i+:1
pong7:zactor.new[pong; i]; i+:1

show""
show"Make a pong spin with ~1 thread"
do[10; zstr.send[pong0;`xtra]]

zclock.sleep 15*500

show""
show"Make pongs spin more with ~4 threads"
do[10; zstr.send[pong4;`xtra];
            zstr.send[pong5;`xtra];
            zstr.send[pong6;`xtra];
            zstr.send[pong7;`xtra]];

zclock.sleep 15*500

show""
show"Make pongs spin more with ~8 threads"
do[10; zstr.send[pong0;`xtra];
            zstr.send[pong1;`xtra];
            zstr.send[pong2;`xtra];
            zstr.send[pong3;`xtra];
            zstr.send[pong4;`xtra];
            zstr.send[pong5;`xtra];
            zstr.send[pong6;`xtra];
            zstr.send[pong7;`xtra]];

zclock.sleep 20*500

show""
show"Send `ping and stop pongs"
rc:zstr.send[pong0; `ping]
rc:zstr.send[pong1; `ping]
rc:zstr.send[pong2; `ping]
rc:zstr.send[pong3; `ping]
rc:zstr.send[pong4; `ping]
rc:zstr.send[pong5; `ping]
rc:zstr.send[pong6; `ping]
rc:zstr.send[pong7; `ping]

zsys.warning[zstr.recv[pong0]]
zsys.warning[zstr.recv[pong1]]
zsys.warning[zstr.recv[pong2]]
zsys.warning[zstr.recv[pong3]]
zsys.warning[zstr.recv[pong4]]
zsys.warning[zstr.recv[pong5]]
zsys.warning[zstr.recv[pong6]]
zsys.warning[zstr.recv[pong7]]

zactor.destroy[pong0]
zactor.destroy[pong1]
zactor.destroy[pong2]
zactor.destroy[pong3]
zactor.destroy[pong4]
zactor.destroy[pong5]
zactor.destroy[pong6]
zactor.destroy[pong7]

\\

