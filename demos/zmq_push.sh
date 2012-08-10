# Derived from http://www.zeromq.org/bindings:bash
# Creative Commons Attribution-Share Alike 3.0 License.
#
# This implementation assumes there is a port open at 127.0.0.1:5555
zmq_push () 
{ 
    m=$(cat) && echo -e $(printf '\\x01\\x00\\x%02x\\x00%s' $((1 + ${#m})) "$m") | nc 127.0.0.1 5555 $@;
}
