
<A name="toc1-2" title="qzmq" />
# qzmq

qzmq provides [Q][q] bindings for [CZMQ][czmq], the high-level C binding for [ØMQ][zeromq].

This is version 1.2.0.RC0 of qzmq.

qzmq is written and maintained by Jaeheum Han.

qzmq is hosted at [github][qzmq] and it uses the [issue tracker][issues] for all issues and comments.

<A name="toc2-13" title="Contents" />
## Contents

&emsp;<a href="#toc2-18">License</a>
&emsp;<a href="#toc2-29">Files of qzmq</a>
&emsp;<a href="#toc2-40">Building qzmq</a>
&emsp;<a href="#toc2-52">How to use qzmq</a>
&emsp;<a href="#toc2-57">Examples</a>
&emsp;<a href="#toc2-62">Issues</a>
&emsp;<a href="#toc2-66"></a>

<A name="toc2-18" title="License" />
## License

Current version 1.2.0.RC0 of qzmq is licensed under Affero GPL. Different licenses may become possible in the future.

Copyright (c) 2012 Jaeheum Han

This file is part of qzmq.

qzmq is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

<A name="toc2-29" title="Files of qzmq" />
## Files of qzmq

* README.txt -- this file.
* README.md -- README.txt in Markdown syntax thanks to [Gitdown][gitdown].
* qzmq.c -- C bindings to be linked with CZMQ (with ØMQ) and Q (`k.h` and `c.o`).
* qzmq.q -- q code used to load the bindings.
* qzmq_test.q -- test code translated to q from CZMQ's self-test code.
* demos/* -- demos. c.f. demos/README.txt or demos/README.md.
* COPYING -- GNU Affero General Public License.

<A name="toc2-40" title="Building qzmq" />
## Building qzmq
Prerequisites: [ØMQ][zeromq] 2.2, [CZMQ][czmq] (git head) and kdb+ 2.8 `k.h` and `c.o`.

Current version 1.2.0.RC0 of qzmq has been built with 32-bit kdb+ on Mac or "m32", ØMQ 2.2 and CZMQ built from git head:

    gcc -bundle -undefined dynamic_lookup qzmq.c -o $HOME/q/m32/qzmq.so -Wall -Wextra -m32 -I$HOME/include -I/usr/local/include/ -L$HOME/q/m32 -L/usr/local/lib -L$HOME/lib -lzmq -lczmq
    
    gcc -shared -fPIC qzmq.c -o /some/where/q/l32/qzmq.so -L/usr/local/lib -I/usr/local/include -I. -Wl,-rpath -Wl,/usr/local/lib # for linux
    
C.f. [kdb+ documentation][kdbdoc] for more details.

<A name="toc2-52" title="How to use qzmq" />
## How to use qzmq

    \l qzmq.q / see qzmq_test.q to see usages.

<A name="toc2-57" title="Examples" />
## Examples

ZeroMQ [Guide][zguide] has examples in many languages. Translation of these examples to q is [under way](https://github.com/imatix/zguide/tree/master/examples/Q)  (pulled from [here](https://github.com/jaeheum/zguide/tree/master/examples/Q)). C.f. [Issue #5](https://github.com/jaeheum/qzmq/issues/5).

<A name="toc2-62" title="Issues" />
## Issues
See the [issue tracker][issues].
<A name="toc2-66" title="" />

---

`README.txt` is written in [Gitdown][gitdown].

[qzmq]: https://github.com/jaeheum/qzmq
[zeromq]: http://www.zeromq.org
[czmq]: http://czmq.zeromq.org
[q]: http://kx.com
[kdbdoc]: http://code.kx.com/wiki/Cookbook/ExtendingWithC
[issues]: https://github.com/jaeheum/qzmq/issues
[zguide]: http://zguide.zeromq.org
[gitdown]: https://github.com/imatix/gitdown
