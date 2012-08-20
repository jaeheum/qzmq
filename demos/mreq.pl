use ZeroMQ qw/:all/;
use English;
use String::Random;
my $ctx = ZeroMQ::Context->new;
my $req = $ctx->socket(ZMQ_REQ);
$req->connect("tcp://127.0.0.1:5559");
while(1) {
    sleep 1;
    $sr = new String::Random;
    $data = $sr->randregex('[qwertyuiop]{10}'); 
    my $msg = ZeroMQ::Message->new($data);
    $req->send($msg);
    my $newmsg = $req->recv();
    my $newdata = $newmsg->data();
    print "perl (pid=$PID): sending back the original data: " . $newdata . "\n";
}
