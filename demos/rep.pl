use ZeroMQ qw/:all/;
use English;
use Digest::SHA1  qw(sha1_hex);
my $ctx = ZeroMQ::Context->new;
my $sock = $ctx->socket(ZMQ_REP);
$sock->bind("tcp://127.0.0.1:5556");
while(1) {
    my $msg = $sock->recv();
    print "perl: received \'" . $msg->data() . "\' from client.\n";
    $digest = sha1_hex($msg->data());
    print "perl (pid=$PID): sending back sha1 of the received data: " . $digest . "\n";
    my $newmsg = ZeroMQ::Message->new($digest);
    $sock->send($newmsg);
}
