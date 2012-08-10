use ZeroMQ qw/:all/;
use English;
use Digest::SHA1  qw(sha1_hex);
my $ctx = ZeroMQ::Context->new;
my $sock = $ctx->socket(ZMQ_REP);
$sock->connect("tcp://127.0.0.1:5560");
while(1) {
    my $msg = $sock->recv();
    my $data = $msg->data();
    print "perl: received \'" . $data . "\' from client.\n";
    print "perl (pid=$PID): sending back the received data: " . $data . "\n";
    my $newmsg = ZeroMQ::Message->new($data);
    $sock->send($newmsg);
}
