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

show"Attached thread writes local data"
ctx:zctx.new[]

txtwriter:{[file; ctx; pipe]
    while[not `ping~zstr.recv[pipe]; zstr.send[pipe; file]];
    t:([]x:til 16;y:md5 string file);
    file 0:.h.tx[`txt;t]; / save in txt format
    zstr.send[pipe; `pong]}
dbwriter:{[file; ctx; pipe]
    while[not `ping~zstr.recv[pipe]; zstr.send[pipe; file]];
    t:([]x:til 16;y:md5 string file);
    file set t;
    zstr.send[pipe; `pong]}
pipe:zthread.fork[ctx; txtwriter; `$":deleteme.txt"]
pipe2:zthread.fork[ctx; txtwriter; `$":deleteme2.txt"]
pipe3:zthread.fork[ctx; dbwriter; `$":deleteme.db"]
pipe4:zthread.fork[ctx; dbwriter; `$":deleteme2.db"]
do[10; zstr.send[pipe; `notping]]
ae[0; ping:zstr.send[pipe; `ping]]
ae[`:deleteme.txt; result:zstr.recv[pipe]]
do[10; zstr.send[pipe2; `notping]]
ae[0; ping:zstr.send[pipe2; `ping]]
ae[`:deleteme2.txt; result:zstr.recv[pipe2]]
do[10; zstr.send[pipe3; `notping]]
ae[0; ping:zstr.send[pipe3; `ping]]
ae[`:deleteme.db; result:zstr.recv[pipe3]]
do[10; zstr.send[pipe4; `notping]]
ae[0; ping:zstr.send[pipe4; `ping]]
ae[`:deleteme2.db; result:zstr.recv[pipe4]]
/ comment out these four lines to see output files
ae[0; rc:zfile.delete `:deleteme.txt]
ae[0; rc:zfile.delete `:deleteme2.txt]
ae[0; rc:zfile.delete `:deleteme.db]
ae[0; rc:zfile.delete `:deleteme2.db]
zctx.destroy[ctx]

\\

