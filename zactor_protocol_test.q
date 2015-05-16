// qzmq - q bindings for CZMQ, the high-level C binding for 0MQ:
//   http://czmq.zeromq.org.
//
// Copyright (c) 2012-2015 Jaeheum Han <jay.han@gmail.com>
// This file is part of qzmq.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

\l qzmq.q
\l assert.q

show"protocol_actor"
protocol_actor:{[pipe;protocol]
    zsock.signal[pipe; 0x0];
    terminated:0b;
    while[not terminated;
        command:zstr.recv[pipe];
        zsys.debug command;
        $[not command in (cols protocol),`$"$TERM";
            [zsys.debug[`$"E: invalid protocol!"]; terminated:1b];
            [zsys.debug[`$"legit command"]; (protocol@command)[]]];
        if[command~`$"$TERM"; terminated:1b]];
     if[terminated; zsys.debug `$"$TERM"]}

/ define a protocol or a dictionary of symbols and functions
p:`sync`ack`nack`quit!({zsys.info`sync};{zsys.info`ack};{zsys.info`nack};{terminated:1b;zsys.info`quit})

/ launch an actor with the protocol, p
a:zactor.new[protocol_actor; p]

/ launch another actor with an extended protocol.
a2:zactor.new[protocol_actor; p,(enlist `xtra)!(enlist {show zsys.info`extraspecial})]

/ talk to actors
rc:zstr.send[a; `sync];
rc:zstr.send[a; `ack];
rc:zstr.send[a; `nack]
rc:zstr.send[a; `fakecommand]
rc:zstr.send[a; `xtra]
rc:zstr.send[a2;`xtra]
rc:zstr.send[a; `$"$TERM"]
rc:zstr.send[a2;`quit]

zactor.destroy[a]
zactor.destroy[a2]

\\
