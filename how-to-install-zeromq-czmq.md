
Title: how-to-install-zeromq-czmq

<A name="toc1-4" title="Install ZeroMQ and CZMQ" />
# Install ZeroMQ and CZMQ

<A name="toc2-7" title="Version of ZeroMQ and CZMQ" />
## Version of ZeroMQ and CZMQ

- [ZeroMQ][zeromq] version 2.2
- [CZMQ][czmq] version 1.2.0

<A name="toc2-13" title="Installing ZeroMQ and CZMQ Without Compiling" />
## Installing ZeroMQ and CZMQ Without Compiling
<A name="toc3-15" title="Linux" />
### Linux
See [http://www.zeromq.org/distro:_start](http://www.zeromq.org/distro:_start)

<A name="toc4-19" title="RHEL, CentOS (RPM based distributions)" />
#### RHEL, CentOS (RPM based distributions)
See [http://www.zeromq.org/distro:centos](http://www.zeromq.org/distro:centos)

As root, first [add a repository][yum] holding the ZeroMQ 2.2 to `yum`, then run `yum` to install ZeroMQ and CZMQ.

    sudo yum-config-manager --add-repo http://download.opensuse.org/repositories/home:/fengshuo:/zeromq/CentOS_CentOS-6/home:fengshuo:zeromq.repo
    sudo yum install zeromq.i686 # do.it is it i686 or i586?
    sudo yum install zeromq-devel.i686
    sudo yum install zeromq
    sudo yum install zeromq-devel
    sudo yum install libczmq.i686
    sudo yum install libczmq
    sudo yum install libczmq-devel.i686
    sudo yum install libczmq-devel

`man zmq` and it should display 0MQ 2.2.0 at the bottom of the man page.

<A name="toc4-37" title="Debian, Ubuntu (deb based distributions)" />
#### Debian, Ubuntu (deb based distributions)
    # first two commands install `add-apt-repository` (skip if installed).
    sudo apt-get update
    sudo apt-get install python-software-properties
    # see https://launchpad.net/~chris-lea/+archive/zeromq
    sudo add-apt-repository ppa:chris-lea/zeromq
    sudo apt-get update
    sudo apt-get install  libzmq-dev
    # see https://launchpad.net/~ori-livneh/+archive/e3
    sudo add-apt-repository  ppa:ori-livneh/e3
    sudo apt-get install  libczmq-dev


<A name="toc3-51" title="Mac OS X" />
### Mac OS X
There is no installer with prebuilt binaries. There are two ways to install ZeroMQ on Mac OS X:

* Install [homebrew]. Then run
    * `brew install --universal zeromq` # for both 32-bit and 64-bit versions.
    * `brew install --universal czmq` # ditto
* Install ZeroMQ from source code (see the next section).

<A name="toc3-60" title="Windows" />
### Windows
See [http://www.zeromq.org/distro:microsoft-windows](http://www.zeromq.org/distro:microsoft-windows). 

<A name="toc2-64" title="Installing ZeroMQ From Source Code" />
## Installing ZeroMQ From Source Code

See [http://www.zeromq.org/intro:get-the-software](http://www.zeromq.org/intro:get-the-software) -- you need to have modern day development tools like  libtool, autoconf, automake and gcc tool-chain.

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
[yum]: https://access.redhat.com/knowledge/docs/en-US/Red_Hat_Enterprise_Linux/6/html/Deployment_Guide/sec-Managing_Yum_Repositories.html
