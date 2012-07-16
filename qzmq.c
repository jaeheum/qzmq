/* qzmq - q bindings for CZMQ, the high-level C binding for 0MQ:
    http://czmq.zeromq.org.

    Copyright (c) 2012 Jaeheum Han <jaeheum@me.com>

    This file is part of qzmq.

    qzmq is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qzmq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with qzmq.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "czmq.h"
#include "k.h"

#define K3(f) K f(K x,K y,K z)
#define K4(f) K f(K x,K y,K z,K z4)
#define K5(f) K f(K x,K y,K z,K z4,K z5)
#define yG y->G0
#define yK ((K*)yG)
#define yg TX(G,y)
#define yi y->i
#define yj y->j
#define yn y->n
#define yt y->t
#define ys y->s
#define zh z->h
#define zi z->i
#define zj z->j
#define VSK(x) (V*)(intptr_t)(x->j)
#define ZTK(t,v) t*v=(t*)(intptr_t)(x->j)
#define KRR(x) krr(strerror(x))
#define CRE(x) {I r=(x);P(r!=0,KRR(errno));}
#define CSTR(x) S s;if(x->n!=1){s=(S)kC(x);s[x->n]=0;}else{s=(S)&TX(G,x);s[1]=0;}

ZI N(K x){if(xt>0)R xn;R 1;}
Z K qstr(S s){R s!=NULL?kp(s):(K)0;}
Z K ptr(V*x){if(x){R kj((intptr_t)x);}else{R(K)0;}}

static K attachedfn;
static K detachedfn;
ZV setattachedfn(K x){r1(x);attachedfn=x;}
ZV setdetachedfn(K x){r1(x);detachedfn=x;}

Z K1(zclocksleep){zclock_sleep(xi); R(K)0;}
Z K1(zclocktime){x=(K)0;R(kj(zclock_time()));}
Z K1(zclocklog){CSTR(x); zclock_log(s); R(K)0;}
Z K1(zclocktest){R ki(zclock_test(xg));}

Z K1(zctxnew){x=(K)0; zctx_t*ctx=zctx_new(); P(ctx, ptr(ctx)); R KRR(errno);} 
Z K1(zctxdestroy){ZTK(zctx_t,ctx); zctx_destroy(&ctx); R(K)0;} 
Z K2(zctxsetiothreads){zctx_set_iothreads(VSK(x), yi); R(K)0;}
Z K2(zctxsetlinger){zctx_set_linger(VSK(x), yi); R(K)0;}
Z K2(zctxsethwm){zctx_set_hwm(VSK(x), yi); R(K)0;}
Z K1(zctxgethwm){R ki(zctx_hwm(VSK(x)));}
Z K1(zctxinterrupted){x=(K)0; R kb(zctx_interrupted);}
Z K1(zctxtest){R ki(zctx_test(xg));}

// zfile `:path
Z K1(zfiledelete){R ki(zfile_delete(++xs));}
Z K1(zfilemkdir){R ki(zfile_mkdir(++xs));}
Z K1(zfileexists){R ki(zfile_exists(++xs));}
Z K1(zfilesize){R kj(zfile_size(++xs));}
Z K1(zfiletest){R ki(zfile_test(xg));}

Z K1(zframenew){P((abs(xt)!=KG&&abs(xt)!=KC), krr("type"));
    if(xt>0){zframe_t*f=zframe_new(xG, xn); P(f, ptr(f)); R(K)0;}
    else{zframe_t*f=zframe_new(&xg, 1); P(f, ptr(f)); R(K)0;}}
Z K1(zframedestroy){ZTK(zframe_t,f); zframe_destroy(&f); R(K)0;}
Z K1(zframerecv){zframe_t*f=zframe_recv(VSK(x)); P(f, ptr(f)); R(K)0;}
Z K1(zframerecvnowait){zframe_t*f=zframe_recv_nowait(VSK(x)); P(f, ptr(f)); R(K)0;}
Z K3(zframesend){ZTK(zframe_t,f); R ki(zframe_send(&f, VSK(y), zi));}
Z K1(zframesize){R kj(zframe_size(VSK(x)));}
// zframe_data() is unsafe, too low-level for q.
Z K1(zframedup){R ptr(zframe_dup(VSK(x)));}
//Z K1(zframestrhex){R qstr(zframe_strhex(VSK(x)));} // not necessary?? -9!strdup will do?
Z K1(zframestrdup){I n=zframe_size(VSK(x)); K y=ktn(KG,n); memcpy(yG, zframe_data(VSK(x)), n); R y;}
Z K1(zframemore){R ki(zframe_more(VSK(x)));}
Z K2(zframeeq){R kb(zframe_eq(VSK(x), VSK(y)));}
Z K2(zframestreq){CSTR(y); K z=kb(zframe_streq(VSK(x), s)); R z;}
Z K2(zframeprint){CSTR(y); zframe_print(VSK(x), s); R(K)0;}
Z K2(zframereset){zframe_reset(VSK(x), yG, N(y)); R(K)0;}
Z K1(zframetest){R ki(zframe_test(xg));}

static K eventfn;
static K timerfn;
ZV seteventfn(K x){r1(x);eventfn=x;}
ZV settimerfn(K x){r1(x);timerfn=x;}
Z K1(zloopnew){x=(K)0; zloop_t*l=zloop_new(); P(l, ptr(l)); R(K)0;}
Z K1(zloopdestroy){ZTK(zloop_t,l); zloop_destroy(&l); R(K)0;}
//typedef int (zloop_fn) (zloop_t *loop, zmq_pollitem_t *item, void *arg);
ZI event_loop_fn(zloop_t*loop, zmq_pollitem_t*item, V*args){
  K w=ktn(KJ,3); kK(w)[0]=ptr(loop); kK(w)[1]=ptr(item); kK(w)[2]=ptr(args);
  K x=k(0, ".", eventfn, w, (K)0);
  if(xt==-128){O("k() error: %s\n", xs);}
  R xi;}
ZV*silence(zmq_pollitem_t*item){R item;}
//typedef int (zloop_fn) (zloop_t *loop, zmq_pollitem_t *item, void *arg);
ZI timer_loop_fn(zloop_t*loop, zmq_pollitem_t*item, V*args){
  silence(item); // for the compiler
  K w=ktn(KJ,3); kK(w)[0]=ptr(loop); kK(w)[1]=kj(0); /* item is null for timer */; kK(w)[2]=ptr(args);
  K x=k(0, ".", timerfn, w, (K)0);
  if(xt==-128){O("k() error: %s\n", xs);}
  R xi;}
//    zloop_poller (zloop_t *self, zmq_pollitem_t *item, zloop_fn handler, void *arg);
Z K4(zlooppoller){
    ZTK(zloop_t, loop);
    seteventfn(z);
    if(yt==0){zmq_pollitem_t item={VSK(yK[0]), yK[1]->i, yK[2]->h,yK[3]->h};
    R ki(zloop_poller(loop, &item, event_loop_fn, z4));}
    else{O("incorrect polling items"); R ki(-1);}}
Z K2(zlooppollerend){zloop_poller_end(VSK(x), VSK(y)); R(K)0;}
// zloop_timer (zloop_t *self, size_t delay, size_t times, zloop_fn handler, void *arg)
Z K5(zlooptimer){
    ZTK(zloop_t,loop);
    settimerfn(z4);
    I rc=zloop_timer(loop, yj, zj, timer_loop_fn, z5);
    R ki(rc);}
Z K2(zlooptimerend){zloop_timer_end(VSK(x), VSK(y)); R(K)0;}
Z K2(zloopsetverbose){ZTK(zloop_t,loop); zloop_set_verbose(loop, y->g); R(K)0;}
Z K1(zloopstart){
    ZTK(zloop_t,loop);  
    I rc=zloop_start(loop);
    R ki(rc);}
Z K1(zlooptest){R ki(zloop_test(xg));}

Z K1(zmsgnew){x=(K)0; zmsg_t*m=zmsg_new(); P(m, ptr(m)); R(K)0;}
Z K1(zmsgdestroy){ZTK(zmsg_t,m); zmsg_destroy(&m); R(K)0;}
Z K1(zmsgrecv){R ptr(zmsg_recv(VSK(x)));}
Z K2(zmsgsend){ZTK(zmsg_t,m); zmsg_send(&m, VSK(y)); R(K)0;}
Z K1(zmsgsize){R kj(zmsg_size(VSK(x)));}
Z K1(zmsgcontentsize){R kj(zmsg_content_size(VSK(x)));}
Z K2(zmsgpush){zmsg_push(VSK(x), VSK(y)); R(K)0;}
Z K1(zmsgpop){P(zmsg_size(VSK(x))>0, ptr(zmsg_pop(VSK(x)))); R krr("empty");}
Z K2(zmsgadd){zmsg_add(VSK(x), VSK(y)); R(K)0;}
//Z K3(zmsgpushmem){zmsg_pushmem(VSK(x), VSK(y), N(z)); R(K)0;}
//Z K3(zmsgaddmem){zmsg_addmem(VSK(x), VSK(y), N(z)); R(K)0;}
//Z K2(zmsgpushstr){CSTR(y); zmsg_pushstr(VSK(x), s); R(K)0;}
//Z K2(zmsgaddstr){CSTR(y); zmsg_addstr(VSK(x), s); R(K)0;}
//Z K1(zmsgpopstr){R kp(zmsg_popstr(VSK(x)));}
Z K2(zmsgwrap){zmsg_wrap(VSK(x), VSK(y));R(K)0;}
Z K1(zmsgunwrap){R ptr(zmsg_unwrap(VSK(x)));}
Z K2(zmsgremove){zmsg_remove(VSK(x), VSK(y)); R(K)0;}
Z K1(zmsgfirst){P(zmsg_size(VSK(x))>0, ptr(zmsg_first(VSK(x)))); R krr("empty");}
Z K1(zmsgnext){P(zmsg_size(VSK(x))>0, ptr(zmsg_next(VSK(x))));  R krr("empty");}
Z K1(zmsglast){P(zmsg_size(VSK(x))>0, ptr(zmsg_last(VSK(x)))); R krr("empty");}
Z K2(zmsgsave){FILE*f=fopen(++ys, "w+"); I rc=zmsg_save(VSK(x), f); fclose(f); R ki(rc);}
Z K2(zmsgload){FILE*f=fopen(++ys, "r"); zmsg_t*m=zmsg_load(VSK(x), f); fclose(f); R ptr(m);}
//TODO(hjh): use kdb for saving/loading
//Z K2(zmsgencode){R kj(zmsg_encode(VSK(x), VSK(y)));}
//Z K2(zmsgdecode){R ptr(zmsg_decode(VSK(x), N(y))); R(K)0;}
Z K1(zmsgdup){zmsg_t*m=zmsg_dup(VSK(x)); P(m,ptr(m)); R(K)0;}
Z K1(zmsgdump){zmsg_dump(VSK(x)); R(K)0;}
Z K1(zmsgtest){R ki(zmsg_test(xg));}

Z K2(zsocketnew){R ptr(zsocket_new(VSK(x), yi));}
Z K2(zsocketdestroy){zsocket_destroy(VSK(x), VSK(y)); R(K)0;}
Z K2(zsocketbind){R ki(zsocket_bind(VSK(x), ys));}
Z K2(zsocketconnect){zsocket_connect(VSK(x), ys); R(K)0;}
Z K1(zsockettypestr){R ks(zsocket_type_str(VSK(x)));}

#if (ZMQ_VERSION_MAJOR == 2)
Z K1(zsockethwm){R ki(zsocket_hwm(VSK(x)));}
Z K1(zsocketswap){R ki(zsocket_swap(VSK(x)));}
Z K1(zsocketaffinity){R ki(zsocket_affinity(VSK(x)));}
Z K1(zsocketidentity){size_t n=255; S s=(S)malloc(n);  I r; K id;
    if(s){r=zmq_getsockopt(VSK(x), ZMQ_IDENTITY, s, &n);
        if(r==0){id=kpn(s,n); free(s); R r1(id);}else{R KRR(errno);}}
        else{R krr("malloc");}} 
Z K1(zsocketrate){R ki(zsocket_rate(VSK(x)));}
Z K1(zsocketrecovery_ivl){R ki(zsocket_recovery_ivl(VSK(x)));}
Z K1(zsocketrecovery_ivl_msec){R ki(zsocket_recovery_ivl_msec(VSK(x)));}
Z K1(zsocketmcast_loop){R ki(zsocket_mcast_loop(VSK(x)));}
#if (ZMQ_VERSION_MINOR == 2)
Z K1(zsocketrcvtimeo){R ki(zsocket_rcvtimeo(VSK(x)));}
#endif
#if (ZMQ_VERSION_MINOR == 2)
Z K1(zsocketsndtimeo){R ki(zsocket_sndtimeo(VSK(x)));}
#endif
Z K1(zsocketsndbuf){R ki(zsocket_sndbuf(VSK(x)));}
Z K1(zsocketrcvbuf){R ki(zsocket_sndbuf(VSK(x)));}
Z K1(zsocketlinger){R ki(zsocket_linger(VSK(x)));}
Z K1(zsocketreconnect_ivl){R ki(zsocket_reconnect_ivl(VSK(x)));}
Z K1(zsocketreconnect_ivl_max){R ki(zsocket_reconnect_ivl_max(VSK(x)));}
Z K1(zsocketbacklog){R ki(zsocket_backlog(VSK(x)));}
Z K1(zsockettype){R ki(zsocket_type(VSK(x)));}
Z K1(zsocketrcvmore){R ki(zsocket_rcvmore(VSK(x)));}
Z K1(zsocketfd){R ki(zsocket_fd(VSK(x)));}
Z K1(zsocketevents){R ki(zsocket_events(VSK(x)));}
Z K2(zsocketsethwm){zsocket_set_hwm(VSK(x), yi); R(K)0;}
Z K2(zsocketsetswap){zsocket_set_swap(VSK(x), yi); R(K)0;}
Z K2(zsocketsetaffinity){zsocket_set_affinity(VSK(x), yi); R(K)0;}
Z K2(zsocketsetidentity){CSTR(y); CRE(zmq_setsockopt(VSK(x), ZMQ_IDENTITY, s, N(y))); R(K)0;}
Z K2(zsocketsetrate){zsocket_set_rate(VSK(x), yi); R(K)0;}
Z K2(zsocketsetrecovery_ivl){zsocket_set_recovery_ivl(VSK(x), yi); R(K)0;}
Z K2(zsocketsetrecovery_ivl_msec){zsocket_set_recovery_ivl_msec(VSK(x), yi); R(K)0;}
Z K2(zsocketsetmcast_loop){zsocket_set_mcast_loop(VSK(x), yi); R(K)0;}
#if (ZMQ_VERSION_MINOR == 2)
Z K2(zsocketsetrcvtimeo){zsocket_set_rcvtimeo(VSK(x), yi); R(K)0;}
#endif
#if (ZMQ_VERSION_MINOR == 2)
Z K2(zsocketsetsndtimeo){zsocket_set_sndtimeo(VSK(x), yi); R(K)0;}
#endif
Z K2(zsocketsetsndbuf){zsocket_set_sndbuf(VSK(x), yi); R(K)0;}
Z K2(zsocketsetrcvbuf){zsocket_set_rcvbuf(VSK(x), yi); R(K)0;}
Z K2(zsocketsetlinger){zsocket_set_linger(VSK(x), yi); R(K)0;}
Z K2(zsocketsetreconnect_ivl){zsocket_set_reconnect_ivl(VSK(x), yi); R(K)0;}
Z K2(zsocketsetreconnect_ivl_max){zsocket_set_reconnect_ivl_max(VSK(x), yi); R(K)0;}
Z K2(zsocketsetbacklog){zsocket_set_backlog(VSK(x), yi); R(K)0;}
Z K2(zsocketsetsubscribe){zsocket_set_subscribe(VSK(x), ys); R(K)0;}
Z K2(zsocketsetunsubscribe){zsocket_set_unsubscribe(VSK(x), ys); R(K)0;}
#endif
#if (ZMQ_VERSION_MAJOR == 3)
Z K1(zsockettype){R ki(zsocket_type(VSK(x)));}
Z K1(zsocketsndhwm){R ki(zsocket_sndhwm(VSK(x)));}
Z K1(zsocketrcvhwm){R ki(zsocket_rcvhwm(VSK(x)));}
Z K1(zsocketaffinity){R ki(zsocket_affinity(VSK(x)));}
Z K1(zsocketidentity){size_t n=255; S s=(S)malloc(n);  I r; K id;
    if(s){r=zmq_getsockopt(VSK(x), ZMQ_IDENTITY, s, &n);
        if(r==0){id=kpn(s,n); free(s); R r1(id);}else{R KRR(errno);}}
        else{R krr("malloc");}} 
Z K1(zsocketrate){R ki(zsocket_rate(VSK(x)));}
Z K1(zsocketrecovery_ivl){R ki(zsocket_recovery_ivl(VSK(x)));}
Z K1(zsocketsndbuf){R ki(zsocket_sndbuf(VSK(x)));}
Z K1(zsocketrcvbuf){R ki(zsocket_rcvbuf(VSK(x)));}
Z K1(zsocketlinger){R ki(zsocket_linger(VSK(x)));}
Z K1(zsocketreconnect_ivl){R ki(zsocket_reconnect_ivl(VSK(x)));}
Z K1(zsocketreconnect_ivl_max){R ki(zsocket_reconnect_ivl_max(VSK(x)));}
Z K1(zsocketbacklog){R ki(zsocket_backlog(VSK(x)));}
Z K1(zsocketmaxmsgsize){R ki(zsocket_maxmsgsize(VSK(x)));}
Z K1(zsocketipv4only){R ki(zsocket_ipv4only(VSK(x)));}
Z K1(zsocketrcvmore){R ki(zsocket_rcvmore(VSK(x)));}
Z K1(zsocketfd){R ki(zsocket_fd(VSK(x)));}
Z K1(zsocket_events){R ki(zsocket_events(VSK(x)));}
Z K1(zsocket_last_endpoint){R ks(zsocket_last_endpoint(VSK(x)));}
Z K2(zsocketsetsndhwm){zsocket_set_sndhwm(VSK(x), yi); R(K)0;}
Z K2(zsocketsetrcvhwm){zsocket_set_rcvhwm(VSK(x), yi); R(K)0;}
Z K2(zsocketsetaffinity){zsocket_set_affinity(VSK(x), yi); R(K)0;}
Z K2(zsocketsetsubscribe){zsocket_set_subscribe(VSK(x), ys); R(K)0;}
Z K2(zsocketsetunsubscribe){zsocket_set_unsubscribe(VSK(x), ys); R(K)0;}
Z K2(zsocketsetidentity){CSTR(y); CRE(zmq_setsockopt(VSK(x), ZMQ_IDENTITY, s, N(y))); R(K)0;}
Z K2(zsocketsetrate){zsocket_set_rate(VSK(x), yi); R(K)0;}
Z K2(zsocketsetrecovery_ivl){zsocket_set_recovery_ivl(VSK(x), yi); R(K)0;}
Z K2(zsocketsetsndbuf){zsocket_set_sndbuf(VSK(x), yi); R(K)0;}
Z K2(zsocketsetrcvbuf){zsocket_set_rcvbuf(VSK(x), yi); R(K)0;}
Z K2(zsocketsetlinger){zsocket_set_linger(VSK(x), yi); R(K)0;}
Z K2(zsocketsetreconnect_ivl){zsocket_set_reconnect_ivl(VSK(x), yi); R(K)0;}
Z K2(zsocketsetreconnect_ivl_max){zsocket_set_reconnect_ivl_max(VSK(x), yi); R(K)0;}
Z K2(zsocketsetmulticast_hops){zsocket_set_multicast_hops(VSK(x), yi); R(K)0;}
Z K2(zsocketsetbacklog){zsocket_set_backlog(VSK(x), yi); R(K)0;}
Z K2(zsocketsetmaxmsgsize){zsocket_set_maxmsgsize(VSK(x), yi); R(K)0;}
Z K2(zsocketmulticast_hops){zsocket_multicast_hops(VSK(x), yi); R(K)0;}
Z K2(zsocketsetrcvtimeo){zsocket_set_rcvtimeo(VSK(x), yi); R(K)0;}
Z K2(zsocketsetsndtimeo){zsocket_set_sndtimeo(VSK(x), yi); R(K)0;}
Z K2(zsocketsetipv4only){zsocket_set_ipv4only(VSK(x), yi); R(K)0;}
Z K2(zsocketsetfailunroutable){zsocket_set_fail_unroutable(VSK(x), yi); R(K)0;}
Z K2(zsocketsethwm){zsocket_set_hwm(VSK(x), yi); R(K)0;}
#endif
Z K1(zsockopttest){R ki(zsockopt_test(xg));}

// send/receive q char vector (not for byte vectors or serialized objects)
// zstr_recv* returns C string with a trailing \0. Convert it to q char vector and free the poor thing.
Z K1(zstrrecv){S s=zstr_recv(VSK(x)); K qs=qstr(s); if(s)free(s); R qs;}
Z K1(zstrrecvnowait){S s=zstr_recv_nowait(VSK(x)); K qs=qstr(s); if(s)free(s); R qs;}
Z K2(zstrsend){r1(y); CSTR(y); I rc=zstr_send(VSK(x), s); R rc==0?y:(K)0;}
Z K2(zstrsendm){r1(y); CSTR(y); I rc=zstr_sendm(VSK(x), s); R rc==0?y:(K)0;}
// zero-copy or zmq_msg_init_data() takes over the ownership of kC(y) or yG buffer (but we don't let it write/free yG).
Z K2(zstrsend0){r1(y); zmq_msg_t msg; // keep y in place at least (q main thread owns y)
    if(yt>0){zmq_msg_init_data(&msg, (S)kC(y), yn, NULL, NULL);}
    else{zmq_msg_init_data(&msg, &yg, 1, NULL, NULL);}
    I rc=zmq_sendmsg (VSK(x), &msg, 0);
    zmq_msg_close (&msg);
    R rc==0?y:(K)0;}

Z K1(zstrtest){R ki(zstr_test(xg));}

//typedef void *(zthread_detached_fn) (void *args);
ZV*df(V*args){
    K x=k(0, "@", detachedfn, args, (K)0);
    if(xt==-128){O("k() error: %s\n", xs);}
    r0(x); m9(); R NULL;}
// zthread_new(zthread_detached_fn *thread_fn, void* args);
Z K2(zthreadnew){
    r1(x);
    setdetachedfn(x);
    I rc=zthread_new(df, r1(y));
    r0(x);
    R ki(rc);}

// typedef void (zthread_attached_fn) (void *args, zctx_t *ctx, void *pipe);
ZV af(V*args, zctx_t*ctx, V*pipe){
    K x=k(0, ".", attachedfn, knk(3, args, ptr(ctx), ptr(pipe)), 0);
    if(xt==-128){O("k() error: %s\n", xs);}
    r0(x); m9();}
// void* zthread_fork (zctx_t *ctx, zthread_attached_fn *thread_fn, void *args);
// From q: zthreadfork[ctx; `qaf; args;]
Z K3(zthreadfork){
    r1(y);
    setattachedfn(y);
    V*pipe=zthread_fork(VSK(r1(x)), af, r1(z));
    r0(y);
    if(pipe){R ptr(pipe);}else{R krr("fork");}}

// libzmq
Z K1(version){x=(K)0; K mnp=ktn(KI,3); zmq_version(&kI(mnp)[0],&kI(mnp)[1],&kI(mnp)[2]); R mnp;}

typedef struct {S k; V* f; I n;} czmqzpi;
Z czmqzpi zclockapi[]={
    {"sleep", zclocksleep, 1},
    {"time", zclocktime, 1},
    {"log", zclocklog, 1},
    {"test", zclocktest, 1},};
Z czmqzpi zctxapi[]={
    {"new", zctxnew, 1},
    {"destroy", zctxdestroy, 1},
    {"set_iothreads", zctxsetiothreads, 2},
    {"set_linger", zctxsetlinger, 2},
    {"set_hwm", zctxsethwm, 2},
    {"get_hwm", zctxgethwm, 1},
    {"interrupted", zctxinterrupted, 1},
    {"test", zctxtest, 1},};
Z czmqzpi zfileapi[]={
    {"delete", zfiledelete, 1},
    {"mkdir", zfilemkdir, 1},
    {"exists", zfileexists, 1},
    {"size", zfilesize, 1},
    {"test", zfiletest, 1},};
Z czmqzpi zframeapi[]={
    {"new", zframenew, 1},
    {"destroy", zframedestroy, 1},
    {"recv", zframerecv, 1},
    {"recvnowait", zframerecvnowait, 1},
    {"send", zframesend, 3},
    {"size", zframesize, 1},
//    {"data", zframedata, 1},
    {"dup", zframedup, 1},
//    {"strhex", zframestrhex, 1},
    {"strdup", zframestrdup, 1},
    {"streq", zframestreq, 2},
    {"more", zframemore, 1},
    {"eq", zframeeq, 2},
    {"print", zframeprint, 2},
    {"reset", zframereset, 2},
    {"test", zframetest, 1},};
Z czmqzpi zloopapi[]={
    {"new", zloopnew, 1},
    {"destroy", zloopdestroy, 1},
    {"poller", zlooppoller, 4},
    {"poller_end", zlooppollerend, 2},
    {"timer", zlooptimer, 5},
    {"timer_end", zlooptimerend, 2},
    {"set_verbose", zloopsetverbose, 2},
    {"start", zloopstart, 1},
    {"test", zlooptest, 1},};
Z czmqzpi zmsgapi[]={
    {"new", zmsgnew, 1},
    {"destroy", zmsgdestroy, 1},
    {"recv", zmsgrecv, 1},
    {"send", zmsgsend, 2},
    {"size", zmsgsize, 1},
    {"content_size", zmsgcontentsize, 1},
    {"push", zmsgpush, 2},
    {"pop", zmsgpop, 1},
    {"add", zmsgadd, 2},
//    {"pushmem", zmsgpushmem, 3},
//    {"addmem", zmsgaddmem, 3},
//    {"pushstr", zmsgpushstr, 2},
//    {"addstr", zmsgaddstr, 2},
//    {"popstr", zmsgpopstr, 1},
    {"wrap", zmsgwrap, 2},
    {"unwrap", zmsgunwrap, 1},
    {"remove", zmsgremove, 1},
    {"first", zmsgfirst, 1},
    {"next", zmsgnext, 1},
    {"last", zmsglast, 1},
    {"save", zmsgsave, 2},
    {"load", zmsgload, 2},
//    {"encode", zmsgencode, 2},
//    {"decode", zmsgdecode, 2},
    {"dup", zmsgdup, 1},
    {"dump", zmsgdump, 1},
    {"test", zmsgtest, 1},};
Z czmqzpi zsocketapi[]={
    {"new", zsocketnew, 2},
    {"destroy", zsocketdestroy, 2},
    {"bind", zsocketbind, 2},
    {"connect", zsocketconnect, 2},
    {"type_sym", zsockettypestr, 1},};
Z czmqzpi zsockoptapi[]={
#if (ZMQ_VERSION_MAJOR == 2)
    {"hwm", zsockethwm, 1},
    {"swap", zsocketswap, 1},
    {"affinity", zsocketaffinity, 1},
    {"identity", zsocketidentity, 1},
    {"rate", zsocketrate, 1},
    {"recovery_ivl", zsocketrecovery_ivl, 1},
    {"recovery_ivl_msec", zsocketrecovery_ivl_msec, 1},
    {"mcast_loop", zsocketmcast_loop, 1},
#   if (ZMQ_VERSION_MINOR == 2)
    {"rcvtimeo", zsocketrcvtimeo, 1},
#   endif
#   if (ZMQ_VERSION_MINOR == 2)
    {"sndtimeo", zsocketsndtimeo, 1},
#   endif
    {"sndbuf", zsocketsndbuf, 1},
    {"rcvbuf", zsocketrcvbuf, 1},
    {"linger", zsocketlinger, 1},
    {"reconnect_ivl", zsocketreconnect_ivl, 1},
    {"reconnect_ivl_max", zsocketreconnect_ivl_max, 1},
    {"backlog", zsocketbacklog, 1},
    {"type", zsockettype, 1},
    {"rcvmore", zsocketrcvmore, 1},
    {"fd", zsocketfd, 1},
    {"events", zsocketevents, 1},
    {"set_hwm", zsocketsethwm, 2},
    {"set_swap", zsocketsetswap, 2},
    {"set_affinity", zsocketsetaffinity, 2},
    {"set_identity", zsocketsetidentity, 2},
    {"set_rate", zsocketsetrate, 2},
    {"set_recovery_ivl", zsocketsetrecovery_ivl, 2},
    {"set_recovery_ivl_msec", zsocketsetrecovery_ivl_msec, 2},
    {"set_mcast_loop", zsocketsetmcast_loop, 2},
#   if (ZMQ_VERSION_MINOR == 2)
    {"set_rcvtimeo", zsocketsetrcvtimeo, 2},
#   endif
#   if (ZMQ_VERSION_MINOR == 2)
    {"set_sndtimeo", zsocketsetsndtimeo, 2},
#   endif
    {"set_sndbuf", zsocketsetsndbuf, 2},
    {"set_rcvbuf", zsocketsetrcvbuf, 2},
    {"set_linger", zsocketsetlinger, 2},
    {"set_reconnect_ivl", zsocketsetreconnect_ivl, 2},
    {"set_reconnect_ivl_max", zsocketsetreconnect_ivl_max, 2},
    {"set_backlog", zsocketsetbacklog, 2},
    {"set_subscribe", zsocketsetsubscribe, 2},
    {"set_unsubscribe", zsocketsetunsubscribe, 2},
#endif
#if (ZMQ_VERSION_MAJOR == 3)
    {"type", zsockettype, 1},
    {"sndhwm", zsocketsndhwm, 1},
    {"rcvhwm", zsocketrcvhwm, 1},
    {"affinity", zsocketaffinity, 1},
    {"identity", zsocketidentity, 1},
    {"rate", zsocketrate, 1},
    {"recovery_ivl", zsocketrecovery_ivl, 1},
    {"sndbuf", zsocketsndbuf, 1},
    {"rcvbuf", zsocketrcvbuf, 1},
    {"linger", zsocketlinger, 1},
    {"reconnect_ivl", zsocketreconnect_ivl, 1},
    {"reconnect_ivl_max", zsocketreconnect_ivl_max, 1},
    {"backlog", zsocketbacklog, 1},
    {"maxmsgsize", zsocketmaxmsgsize, 1},
    {"ipv4only", zsocketipv4only, 1},
    {"rcvmore", zsocketrcvmore, 1},
    {"fd", zsocketfd, 1},
    {"events", zsocket_events, 1},
    {"last_endpoint", zsocket_last_endpoint, 1},
    {"set_sndhwm", zsocketsetsndhwm, 2},
    {"set_rcvhwm", zsocketsetrcvhwm, 2},
    {"set_affinity", zsocketsetaffinity, 2},
    {"set_subscribe", zsocketsetsubscribe, 2},
    {"set_unsubscribe", zsocketsetunsubscribe, 2},
    {"set_identity", zsocketsetidentity, 2},
    {"set_rate", zsocketsetrate, 2},
    {"set_recovery_ivl", zsocketsetrecovery_ivl, 2},
    {"set_sndbuf", zsocketsetsndbuf, 2},
    {"set_rcvbuf", zsocketsetrcvbuf, 2},
    {"set_linger", zsocketsetlinger, 2},
    {"set_reconnect_ivl", zsocketsetreconnect_ivl, 2},
    {"set_reconnect_ivl_max", zsocketsetreconnect_ivl_max, 2},
    {"set_multicast_hops", zsocketsetmulticast_hops, 2},
    {"set_backlog", zsocketsetbacklog, 2},
    {"set_maxmsgsize", zsocketsetmaxmsgsize, 2},
    {"set_multicast_hops", zsocketmulticast_hops, 2},
    {"set_rcvtimeo", zsocketsetrcvtimeo, 2},
    {"set_sndtimeo", zsocketsetsndtimeo, 2},
    {"set_ipv4only", zsocketsetipv4only, 2},
    {"set_failunroutable", zsocketsetfailunroutable, 2},
    {"set_hwm", zsocketsethwm, 2},
#endif
    {"test", zsockopttest, 1},
};

Z czmqzpi zstrapi[]={
    {"recv", zstrrecv, 1},
    {"recv_nowait", zstrrecvnowait, 1},
    {"send", zstrsend, 2},
    {"sendm", zstrsendm, 2},
    {"send0", zstrsend0, 2},
    {"test", zstrtest, 1},};
Z czmqzpi zthreadapi[] = {
    {"new", zthreadnew, 2},
    {"fork", zthreadfork, 3},
};
Z czmqzpi libzmqapi[] = {
    {"version", version, 1},
};

#define APITAB(k,f,n) xS[i]=ss(k);kK(y)[i]=dl(f,n)
#define EXPAPI(name) K1(name){int n=tblsize(name##api);K y=ktn(0,n);x=ktn(KS,n); \
  DO(tblsize(name##api), APITAB(name##api[i].k, name##api[i].f, name##api[i].n)); \
  R xD(x,y);}

EXPAPI(zclock);
EXPAPI(zctx);
EXPAPI(zfile);
EXPAPI(zframe);
EXPAPI(zloop);
EXPAPI(zmsg);
EXPAPI(zsocket);
EXPAPI(zsockopt);
EXPAPI(zstr);
EXPAPI(zthread);
EXPAPI(libzmq);

//  Socket types, options, …
Z struct {S k; I v;} zmqopt[] = {
    {"PAIR", ZMQ_PAIR},
    {"PUB", ZMQ_PUB},
    {"SUB", ZMQ_SUB},
    {"REQ", ZMQ_REQ},
    {"REP", ZMQ_REP},
    {"DEALER", ZMQ_DEALER},
    {"ROUTER", ZMQ_ROUTER},
    {"PULL", ZMQ_PULL},
    {"PUSH", ZMQ_PUSH},
    {"XPUB", ZMQ_XPUB},
    {"XSUB", ZMQ_XSUB},
    {"XREQ", ZMQ_XREQ},
    {"XREP", ZMQ_ROUTER},
    {"UPSTREAM", ZMQ_PULL},
    {"DOWNSTREAM", ZMQ_PUSH},
    {"HWM", ZMQ_HWM},
    {"SWAP", ZMQ_SWAP},
    {"AFFINITY", ZMQ_AFFINITY},
    {"IDENTITY", ZMQ_IDENTITY},
    {"SUBSCRIBE", ZMQ_SUBSCRIBE},
    {"UNSUBSCRIBE", ZMQ_UNSUBSCRIBE},
    {"RATE", ZMQ_RATE},
    {"RECOVERY_IVL", ZMQ_RECOVERY_IVL},
    {"MCAST_LOOP", ZMQ_MCAST_LOOP},
    {"SNDBUF", ZMQ_SNDBUF},
    {"RCVBUF", ZMQ_RCVBUF},
    {"RCVMORE", ZMQ_RCVMORE},
    {"FD", ZMQ_FD},
    {"EVENTS", ZMQ_EVENTS},
    {"TYPE", ZMQ_TYPE},
    {"LINGER", ZMQ_LINGER},
    {"RECONNECT_IVL", ZMQ_RECONNECT_IVL},
    {"BACKLOG", ZMQ_BACKLOG},
    {"RECOVERY_IVL_MSEC", ZMQ_RECOVERY_IVL_MSEC},
    {"RECONNECT_IVL_MAX", ZMQ_RECONNECT_IVL_MAX},
    {"DONTWAIT", ZMQ_DONTWAIT},
    {"SNDMORE", ZMQ_SNDMORE},
    {"POLLIN", ZMQ_POLLIN},
    {"POLLOUT", ZMQ_POLLOUT},
    {"POLLERR", ZMQ_POLLERR},
    {"STREAMER", ZMQ_STREAMER},
    {"FORWARDER", ZMQ_FORWARDER},
    {"QUEUE", ZMQ_QUEUE},
    {"ZFRAME_MORE", ZFRAME_MORE},
    {"ZFRAME_REUSE", ZFRAME_REUSE},
    {"ZFRAME_DONTWAIT", ZFRAME_DONTWAIT},
    {"ZSOCKET_DYNFROM", ZSOCKET_DYNFROM},
    {"ZSOCKET_DYNTO", ZSOCKET_DYNTO},
};
#define OPTTAB(k,v) xS[i]=ss(k);kK(y)[i]=ki(v)

K1(zmq){int n=tblsize(zmqopt);
    K y=ktn(0,n);x=ktn(KS,n);
    DO(tblsize(zmqopt), OPTTAB(zmqopt[i].k, zmqopt[i].v));
    R xD(x,y);}
