<A name="toc1-1" title="Installing ZeroMQ and Language Bindings" />
# Installing ZeroMQ and Language Bindings

ZeroMQ is written in C but there are many bindings for other  [languages][languages]. Before running demos, install ZeroMQ and the necessary language bindings.

Summary:

* Install ZeroMQ version 2.2.
* Install Python ZeroMQ bindings.
* Install Perl one for extra credit.
* Install others as needed (at the price of mental aggrevation).

<A name="toc2-13" title="ZeroMQ" />
## ZeroMQ

<A name="toc3-16" title="Version of ZeroMQ" />
### Version of ZeroMQ
ZeroMQ version 2.2 is recommended for the demos because Python and Perl bindings use it.

<A name="toc3-20" title="Installing ZeroMQ Without Compiling" />
### Installing ZeroMQ Without Compiling
<A name="toc4-22" title="Linux" />
#### Linux
See [http://www.zeromq.org/distro:_start](http://www.zeromq.org/distro:_start)

For example, RHEL [http://www.zeromq.org/distro:centos](http://www.zeromq.org/distro:centos)

<A name="toc4-28" title="Mac OS X" />
#### Mac OS X
There is no installer with prebuilt binaries. There are two ways to install ZeroMQ on Mac OS X:

* Install [homebrew]. Then run `brew install zeromq`. This will install ZeroMQ matching your OS/hardware. Another option is to run `brew install --universal zeromq` for both 32-bit and 64-bit versions.
* Install ZeroMQ from source code (see the next section).

<A name="toc4-35" title="Windows" />
#### Windows
Note that Windows is not used nor tested for the demos but mentioned for the sake of completeness: [http://www.zeromq.org/distro:microsoft-windows](http://www.zeromq.org/distro:microsoft-windows). 

<A name="toc3-39" title="Installing ZeroMQ From Source Code" />
### Installing ZeroMQ From Source Code

[http://www.zeromq.org/intro:get-the-software](http://www.zeromq.org/intro:get-the-software) -- you need to have modern day development tools like  libtool, autoconf, automake and gcc tool-chain.

<A name="toc2-44" title="Instaling Language Bindings" />
## Instaling Language Bindings
ZeroMQ must be installed prior to installing language bindings.

<A name="toc2-48" title="Python" />
## Python
Install [pyzmq][pyzmq]:

    sudo easy_install pyzmq

One quick check for successful installation:

    $ python
    >>> import zmq
    >>> help(zmq) # to read the documentation
    >>> zmq.pyzmq_version()
    '2.2.0'
    >>> zmq.zmq_version()
    '2.2.0'
    # either '2.2.0' or '2.2.0.1' is acceptable.

<A name="toc2-65" title="Perl" />
## Perl
Install [ZeroMQ.pm][perl] from CPAN:

    sudo cpan -i ZeroMQ # add -f flag if necessary

One quick check for successful installation:

    $ perl -e 'use ZeroMQ qw/:all/; print "ZeroMQ version is: " . ZeroMQ::version() . "\n";'
    ZeroMQ version is: 2.2.0
    
<A name="toc2-76" title="Java" />
## Java
Currently the demo does not include any Java code. (An exercise for the readers.)

See [http://www.zeromq.org/bindings:java](http://www.zeromq.org/bindings:java) for installation instructions and documentation of Java bindings.

<A name="toc2-82" title="Q" />
## Q
[qzmq][qzmq] installation is [still rough around edges][issue6]. In addition to ZeroMQ, qzmq uses [Czmq][czmq] which is currently only installable from source code.


[qzmq]: https://github.com/jaeheum/qzmq
[issues]: https://github.com/jaeheum/qzmq/issues
[zeromq]: http://www.zeromq.org
[czmq]: http://czmq.zeromq.org
[q]: http://kx.com
[zguide]: http://zguide.zeromq.org
[gitdown]: https://github.com/imatix/gitdown
[broker]: http://zguide.zeromq.org/page:all#A-Request-Reply-Broker
[brokerless]: http://www.zeromq.org/whitepapers:brokerless
[mitm]: http://en.wikipedia.org/wiki/MiTM
[perl]: http://www.zeromq.org/bindings:perl
[pyzmq]: http://www.zeromq.org/bindings:python
[languages]: http://www.zeromq.org/bindings:_start
[gangnam]: http://www.youtube.com/watch?v=9bZkp7q19f0&feature=related
[homebrew]: http://mxcl.github.com/homebrew/
[issue6]: https://github.com/jaeheum/qzmq/issues/6
