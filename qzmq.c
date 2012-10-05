/* qzmq - q bindings for CZMQ, the high-level C binding for 0MQ:
    http://czmq.zeromq.org.

    Copyright (c) 2012 Jaeheum Han <jaeheum@me.com>

    This file is part of qzmq.

    qzmq is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qzmq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with qzmq.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "czmq.h"
#include "k.h"

#define K0(f) K f(void)
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
#define CSTR(x) S s;if(x->t>0){s=(S)kC(x);s[x->n]=0;}else{s=(S)&TX(G,x);s[1]=0;} // xn can be unset.
#define TC(x,T) P(x->t!=T,krr("type"))
#define TC2(x,T,T2) P(x->t!=T&&x->t!=T2,krr("type"))
#define PC(x) TC(x,-KJ)

ZI N(K x){if(xt>0)R xn;R 1;}
Z K qstr(S s){R s!=NULL?kp(s):(K)0;}
Z K ptr(V*x){if(x){R kj((intptr_t)x);}else{R(K)0;}}

Z K1(zclocksleep){TC(x,-KI); zclock_sleep(xi); R(K)0;}
Z K0(zclocktime){R(kj(zclock_time()));}
Z K1(zclocklog){TC(x,KC); CSTR(x); zclock_log(s); R(K)0;}

Z K0(zctxnew){zctx_t*ctx=zctx_new(); P(ctx, ptr(ctx)); R KRR(errno);} 
Z K1(zctxdestroy){PC(x); ZTK(zctx_t,ctx); zctx_destroy(&ctx); R(K)0;} 
Z K2(zctxsetiothreads){PC(x); TC(y,-KI); zctx_set_iothreads(VSK(x), yi); R(K)0;}
Z K2(zctxsetlinger){PC(x); TC(y,-KI); zctx_set_linger(VSK(x), yi); R(K)0;}
Z K2(zctxsethwm){PC(x); TC(y,-KI); zctx_set_hwm(VSK(x), yi); R(K)0;}
Z K1(zctxgethwm){PC(x); R ki(zctx_hwm(VSK(x)));}
Z K0(zctxinterrupted){R kb(zctx_interrupted);}
Z K0(zctxunderlying){R krr("nyi");}

// zfile `:path
Z K1(zfiledelete){TC(x,-KS); R ki(zfile_delete(++xs));}
Z K1(zfilemkdir){TC(x,-KS); R ki(zfile_mkdir(++xs));}
Z K1(zfileexists){TC(x,-KS); R ki(zfile_exists(++xs));}
Z K1(zfilesize){TC(x,-KS); R kj(zfile_size(++xs));}

Z K1(zframenew){P((abs(xt)!=KG&&abs(xt)!=KC), krr("type"));
    if(xt>0){zframe_t*f=zframe_new(xG, xn); P(f, ptr(f)); R(K)0;}
    else{zframe_t*f=zframe_new(&xg, 1); P(f, ptr(f)); R(K)0;}}
Z K0(zframenewzerocopy){R krr("nyi");}
Z K1(zframedestroy){PC(x); ZTK(zframe_t,f); zframe_destroy(&f); R(K)0;}
Z K1(zframerecv){PC(x); zframe_t*f=zframe_recv(VSK(x)); P(f, ptr(f)); R(K)0;}
Z K1(zframerecvnowait){PC(x); zframe_t*f=zframe_recv_nowait(VSK(x)); P(f, ptr(f)); R(K)0;}
Z K3(zframesend){PC(x); PC(y); TC(z,-KI);  ZTK(zframe_t,f); R ki(zframe_send(&f, VSK(y), zi));}
Z K1(zframesize){PC(x); R kj(zframe_size(VSK(x)));}
Z K0(zframedata){R krr("nyi");}// zframe_data() is unsafe, too low-level for q.
Z K1(zframedup){PC(x); R ptr(zframe_dup(VSK(x)));}
Z K0(zframestrhex){R krr("nyi");}
Z K1(zframestrdup){PC(x); I n=zframe_size(VSK(x)); K y=ktn(KG,n); memcpy(yG, zframe_data(VSK(x)), n); R y;}
Z K2(zframestreq){PC(x); TC2(y,KC,KG); CSTR(y); K z=kb(zframe_streq(VSK(x), s)); R z;}
Z K0(zframezerocopy){R krr("nyi");}
Z K1(zframemore){PC(x); R ki(zframe_more(VSK(x)));}
Z K2(zframeeq){PC(x); PC(y); R kb(zframe_eq(VSK(x), VSK(y)));}
Z K2(zframeprint){TC(y,KC); CSTR(y); zframe_print(VSK(x), s); R(K)0;}
Z K2(zframereset){zframe_reset(VSK(x), yG, N(y)); R(K)0;}

static K eventfn;
static K timerfn;
ZV seteventfn(K x){r1(x);eventfn=x;}
ZV settimerfn(K x){r1(x);timerfn=x;}
Z K0(zloopnew){zloop_t*l=zloop_new(); P(l, ptr(l)); R(K)0;}
Z K1(zloopdestroy){PC(x); ZTK(zloop_t,l); zloop_destroy(&l); R(K)0;}
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
Z K4(zlooppoller){PC(x); TC(y,0); TC(z,-KS);
    ZTK(zloop_t, loop);
    seteventfn(z);
    zmq_pollitem_t item={VSK(yK[0]), yK[1]->i, yK[2]->h,yK[3]->h};
    R ki(zloop_poller(loop, &item, event_loop_fn, z4));}
Z K2(zlooppollerend){PC(x); PC(y); zloop_poller_end(VSK(x), VSK(y)); R(K)0;}
// zloop_timer (zloop_t *self, size_t delay, size_t times, zloop_fn handler, void *arg)
Z K5(zlooptimer){PC(x); TC2(y,-KI,-KJ); TC2(z,-KI,-KJ); TC(z4,-KS); 
    ZTK(zloop_t,loop);
    settimerfn(z4);
    I rc=zloop_timer(loop, yj, zj, timer_loop_fn, z5);
    R ki(rc);}
Z K2(zlooptimerend){PC(x); PC(y); zloop_timer_end(VSK(x), VSK(y)); R(K)0;}
Z K2(zloopsetverbose){PC(x); TC(y,-KB); ZTK(zloop_t,loop); zloop_set_verbose(loop, y->g); R(K)0;}
Z K1(zloopstart){PC(x);
    ZTK(zloop_t,loop);  
    I rc=zloop_start(loop);
    R ki(rc);}

Z K0(zmsgnew){zmsg_t*m=zmsg_new(); P(m, ptr(m)); R(K)0;}
Z K1(zmsgdestroy){PC(x); ZTK(zmsg_t,m); zmsg_destroy(&m); R(K)0;}
Z K1(zmsgrecv){PC(x); R ptr(zmsg_recv(VSK(x)));}
Z K2(zmsgsend){PC(x); PC(y); ZTK(zmsg_t,m); R ki(zmsg_send(&m, VSK(y)));}
Z K1(zmsgsize){PC(x); R kj(zmsg_size(VSK(x)));}
Z K1(zmsgcontentsize){PC(x); R kj(zmsg_content_size(VSK(x)));}
Z K2(zmsgpush){PC(x); PC(y); zmsg_push(VSK(x), VSK(y)); R(K)0;}
Z K1(zmsgpop){PC(x); P(zmsg_size(VSK(x))>0, ptr(zmsg_pop(VSK(x)))); R krr("empty");}
Z K2(zmsgadd){PC(x); PC(y); zmsg_add(VSK(x), VSK(y)); R(K)0;}
Z K0(zmsgpushmem){R krr("nyi");}//{zmsg_pushmem(VSK(x), VSK(y), N(z)); R(K)0;}
Z K0(zmsgaddmem){R krr("nyi");}//{zmsg_addmem(VSK(x), VSK(y), N(z)); R(K)0;}
Z K0(zmsgpushstr){R krr("nyi");}//{CSTR(y); zmsg_pushstr(VSK(x), s); R(K)0;}
Z K0(zmsgaddstr){R krr("nyi");}//{CSTR(y); zmsg_addstr(VSK(x), s); R(K)0;}
Z K0(zmsgpopstr){R krr("nyi");}//{R kp(zmsg_popstr(VSK(x)));}
Z K0(zmsgwrap){R krr("nyi");}//{PC(x); PC(y); zmsg_wrap(VSK(x), VSK(y));R(K)0;}
Z K0(zmsgunwrap){R krr("nyi");}//{PC(x); R ptr(zmsg_unwrap(VSK(x)));}
Z K0(zmsgremove){R krr("nyi");}//{PC(x); PC(y); zmsg_remove(VSK(x), VSK(y)); R(K)0;}
Z K1(zmsgfirst){PC(x); P(zmsg_size(VSK(x))>0, ptr(zmsg_first(VSK(x)))); R krr("empty");}
Z K1(zmsgnext){PC(x); P(zmsg_size(VSK(x))>0, ptr(zmsg_next(VSK(x))));  R krr("empty");}
Z K1(zmsglast){PC(x); P(zmsg_size(VSK(x))>0, ptr(zmsg_last(VSK(x)))); R krr("empty");}
Z K2(zmsgsave){PC(x); TC(y,-KS); FILE*f=fopen(++ys, "w+"); I rc=zmsg_save(VSK(x), f); fclose(f); R ki(rc);}
Z K2(zmsgload){PC(x); TC(y,-KS); FILE*f=fopen(++ys, "r"); zmsg_t*m=zmsg_load(VSK(x), f); fclose(f); R ptr(m);}
//TODO(hjh): use kdb for saving/loading
Z K0(zmsgencode){R krr("nyi");}//{R kj(zmsg_encode(VSK(x), VSK(y)));}
Z K0(zmsgdecode){R krr("nyi");}//{R ptr(zmsg_decode(VSK(x), N(y))); R(K)0;}
Z K1(zmsgdup){PC(x); zmsg_t*m=zmsg_dup(VSK(x)); P(m,ptr(m)); R(K)0;}
Z K1(zmsgdump){PC(x); zmsg_dump(VSK(x)); R(K)0;}

Z K2(zsocketnew){PC(x); TC(y,-KI); R ptr(zsocket_new(VSK(x), yi));}
Z K2(zsocketdestroy){PC(x); PC(y); zsocket_destroy(VSK(x), VSK(y)); R(K)0;}
Z K2(zsocketbind){PC(x); TC(y,-KS); R ki(zsocket_bind(VSK(x), ys));}
Z K2(zsocketconnect){PC(x); TC(y,-KS); R ki(zsocket_connect(VSK(x), ys));}
Z K1(zsockettypestr){PC(x); R ks(zsocket_type_str(VSK(x)));}

#if (ZMQ_VERSION_MAJOR == 2)
Z K1(zsockethwm){PC(x); R ki(zsocket_hwm(VSK(x)));}
Z K1(zsocketswap){PC(x); R ki(zsocket_swap(VSK(x)));}
Z K1(zsocketaffinity){PC(x); R ki(zsocket_affinity(VSK(x)));}
Z K1(zsocketidentity){PC(x); size_t n=255; S s=(S)malloc(n); I r; K id;
    if(s){r=zmq_getsockopt(VSK(x), ZMQ_IDENTITY, s, &n);
        if(r==0){id=kpn(s,n); free(s); R r1(id);}else{free(s); R KRR(errno);}}
        else{R krr("malloc");}} 
Z K1(zsocketrate){PC(x); R ki(zsocket_rate(VSK(x)));}
Z K1(zsocketrecovery_ivl){PC(x); R ki(zsocket_recovery_ivl(VSK(x)));}
Z K1(zsocketrecovery_ivl_msec){PC(x); R ki(zsocket_recovery_ivl_msec(VSK(x)));}
Z K1(zsocketmcast_loop){PC(x); R ki(zsocket_mcast_loop(VSK(x)));}
#if (ZMQ_VERSION_MINOR == 2)
Z K1(zsocketrcvtimeo){PC(x); R ki(zsocket_rcvtimeo(VSK(x)));}
#endif
#if (ZMQ_VERSION_MINOR == 2)
Z K1(zsocketsndtimeo){PC(x); R ki(zsocket_sndtimeo(VSK(x)));}
#endif
Z K1(zsocketsndbuf){PC(x); R ki(zsocket_sndbuf(VSK(x)));}
Z K1(zsocketrcvbuf){PC(x); R ki(zsocket_sndbuf(VSK(x)));}
Z K1(zsocketlinger){PC(x); R ki(zsocket_linger(VSK(x)));}
Z K1(zsocketreconnect_ivl){PC(x); R ki(zsocket_reconnect_ivl(VSK(x)));}
Z K1(zsocketreconnect_ivl_max){PC(x); R ki(zsocket_reconnect_ivl_max(VSK(x)));}
Z K1(zsocketbacklog){PC(x); R ki(zsocket_backlog(VSK(x)));}
Z K1(zsockettype){PC(x); R ki(zsocket_type(VSK(x)));}
Z K1(zsocketrcvmore){PC(x); R ki(zsocket_rcvmore(VSK(x)));}
Z K1(zsocketfd){PC(x); R ki(zsocket_fd(VSK(x)));}
Z K1(zsocketevents){PC(x); R ki(zsocket_events(VSK(x)));}
Z K2(zsocketsethwm){PC(x); TC(y,-KI); zsocket_set_hwm(VSK(x), yi); R(K)0;}
Z K2(zsocketsetswap){PC(x); TC(y,-KI); zsocket_set_swap(VSK(x), yi); R(K)0;}
Z K2(zsocketsetaffinity){PC(x); TC(y,-KI); zsocket_set_affinity(VSK(x), yi); R(K)0;}
Z K2(zsocketsetidentity){PC(x); TC2(y,KC,-KC); CSTR(y); CRE(zmq_setsockopt(VSK(x), ZMQ_IDENTITY, s, N(y))); R(K)0;}
Z K2(zsocketsetrate){PC(x); TC(y,-KI); zsocket_set_rate(VSK(x), yi); R(K)0;}
Z K2(zsocketsetrecovery_ivl){PC(x); TC(y,-KI); zsocket_set_recovery_ivl(VSK(x), yi); R(K)0;}
Z K2(zsocketsetrecovery_ivl_msec){PC(x); TC(y,-KI); zsocket_set_recovery_ivl_msec(VSK(x), yi); R(K)0;}
Z K2(zsocketsetmcast_loop){PC(x); TC(y,-KI); zsocket_set_mcast_loop(VSK(x), yi); R(K)0;}
#if (ZMQ_VERSION_MINOR == 2)
Z K2(zsocketsetrcvtimeo){PC(x); TC(y,-KI); zsocket_set_rcvtimeo(VSK(x), yi); R(K)0;}
#endif
#if (ZMQ_VERSION_MINOR == 2)
Z K2(zsocketsetsndtimeo){PC(x); TC(y,-KI); zsocket_set_sndtimeo(VSK(x), yi); R(K)0;}
#endif
Z K2(zsocketsetsndbuf){PC(x); TC(y,-KI); zsocket_set_sndbuf(VSK(x), yi); R(K)0;}
Z K2(zsocketsetrcvbuf){PC(x); TC(y,-KI); zsocket_set_rcvbuf(VSK(x), yi); R(K)0;}
Z K2(zsocketsetlinger){PC(x); TC(y,-KI); zsocket_set_linger(VSK(x), yi); R(K)0;}
Z K2(zsocketsetreconnect_ivl){PC(x); TC(y,-KI); zsocket_set_reconnect_ivl(VSK(x), yi); R(K)0;}
Z K2(zsocketsetreconnect_ivl_max){PC(x); TC(y,-KI); zsocket_set_reconnect_ivl_max(VSK(x), yi); R(K)0;}
Z K2(zsocketsetbacklog){PC(x); TC(y,-KI); zsocket_set_backlog(VSK(x), yi); R(K)0;}
Z K2(zsocketsetsubscribe){PC(x); TC(y,-KS); zsocket_set_subscribe(VSK(x), ys); R(K)0;}
Z K2(zsocketsetunsubscribe){PC(x); TC(y,-KS); zsocket_set_unsubscribe(VSK(x), ys); R(K)0;}
#endif
#if (ZMQ_VERSION_MAJOR == 3)
Z K1(zsockettype){PC(x); R ki(zsocket_type(VSK(x)));}
Z K1(zsocketsndhwm){PC(x); R ki(zsocket_sndhwm(VSK(x)));}
Z K1(zsocketrcvhwm){PC(x); R ki(zsocket_rcvhwm(VSK(x)));}
Z K1(zsocketaffinity){PC(x); R ki(zsocket_affinity(VSK(x)));}
Z K1(zsocketidentity){PC(x); size_t n=255; S s=(S)malloc(n); I r; K id;
    if(s){r=zmq_getsockopt(VSK(x), ZMQ_IDENTITY, s, &n);
        if(r==0){id=kpn(s,n); free(s); R r1(id);}else{free(s); R KRR(errno);}}
        else{R krr("malloc");}} 
Z K1(zsocketrate){PC(x); R ki(zsocket_rate(VSK(x)));}
Z K1(zsocketrecovery_ivl){PC(x); R ki(zsocket_recovery_ivl(VSK(x)));}
Z K1(zsocketsndbuf){PC(x); R ki(zsocket_sndbuf(VSK(x)));}
Z K1(zsocketrcvbuf){PC(x); R ki(zsocket_rcvbuf(VSK(x)));}
Z K1(zsocketlinger){PC(x); R ki(zsocket_linger(VSK(x)));}
Z K1(zsocketreconnect_ivl){PC(x); R ki(zsocket_reconnect_ivl(VSK(x)));}
Z K1(zsocketreconnect_ivl_max){PC(x); R ki(zsocket_reconnect_ivl_max(VSK(x)));}
Z K1(zsocketbacklog){PC(x); R ki(zsocket_backlog(VSK(x)));}
Z K1(zsocketmaxmsgsize){PC(x); R ki(zsocket_maxmsgsize(VSK(x)));}
Z K1(zsocketipv4only){PC(x); R ki(zsocket_ipv4only(VSK(x)));}
Z K1(zsocketrcvmore){PC(x); R ki(zsocket_rcvmore(VSK(x)));}
Z K1(zsocketfd){PC(x); R ki(zsocket_fd(VSK(x)));}
Z K1(zsocket_events){PC(x); R ki(zsocket_events(VSK(x)));}
Z K1(zsocket_last_endpoint){PC(x); R ks(zsocket_last_endpoint(VSK(x)));}
Z K2(zsocketsetsndhwm){PC(x); TC(y,-KI); zsocket_set_sndhwm(VSK(x), yi); R(K)0;}
Z K2(zsocketsetrcvhwm){PC(x); TC(y,-KI); zsocket_set_rcvhwm(VSK(x), yi); R(K)0;}
Z K2(zsocketsetaffinity){PC(x); TC(y,-KI); zsocket_set_affinity(VSK(x), yi); R(K)0;}
Z K2(zsocketsetsubscribe){PC(x); TC(y,-KS); zsocket_set_subscribe(VSK(x), ys); R(K)0;}
Z K2(zsocketsetunsubscribe){PC(x); TC(y,-KS); zsocket_set_unsubscribe(VSK(x), ys); R(K)0;}
Z K2(zsocketsetidentity){PC(x); TC2(y,KC,-KC); CSTR(y); CRE(zmq_setsockopt(VSK(x), ZMQ_IDENTITY, s, N(y))); R(K)0;}
Z K2(zsocketsetrate){PC(x); TC(y,-KI); zsocket_set_rate(VSK(x), yi); R(K)0;}
Z K2(zsocketsetrecovery_ivl){PC(x); TC(y,-KI); zsocket_set_recovery_ivl(VSK(x), yi); R(K)0;}
Z K2(zsocketsetsndbuf){PC(x); TC(y,-KI); zsocket_set_sndbuf(VSK(x), yi); R(K)0;}
Z K2(zsocketsetrcvbuf){PC(x); TC(y,-KI); zsocket_set_rcvbuf(VSK(x), yi); R(K)0;}
Z K2(zsocketsetlinger){PC(x); TC(y,-KI); zsocket_set_linger(VSK(x), yi); R(K)0;}
Z K2(zsocketsetreconnect_ivl){PC(x); TC(y,-KI); zsocket_set_reconnect_ivl(VSK(x), yi); R(K)0;}
Z K2(zsocketsetreconnect_ivl_max){PC(x); TC(y,-KI); zsocket_set_reconnect_ivl_max(VSK(x), yi); R(K)0;}
Z K2(zsocketsetmulticast_hops){PC(x); TC(y,-KI); zsocket_set_multicast_hops(VSK(x), yi); R(K)0;}
Z K2(zsocketsetbacklog){PC(x); TC(y,-KI); zsocket_set_backlog(VSK(x), yi); R(K)0;}
Z K2(zsocketsetmaxmsgsize){PC(x); TC(y,-KI); zsocket_set_maxmsgsize(VSK(x), yi); R(K)0;}
Z K2(zsocketmulticast_hops){PC(x); TC(y,-KI); zsocket_multicast_hops(VSK(x), yi); R(K)0;}
Z K2(zsocketsetrcvtimeo){PC(x); TC(y,-KI); zsocket_set_rcvtimeo(VSK(x), yi); R(K)0;}
Z K2(zsocketsetsndtimeo){PC(x); TC(y,-KI); zsocket_set_sndtimeo(VSK(x), yi); R(K)0;}
Z K2(zsocketsetipv4only){PC(x); TC(y,-KI); zsocket_set_ipv4only(VSK(x), yi); R(K)0;}
Z K2(zsocketsetfailunroutable){PC(x); TC(y,-KI); zsocket_set_fail_unroutable(VSK(x), yi); R(K)0;}
Z K2(zsocketsethwm){PC(x); TC(y,-KI); zsocket_set_hwm(VSK(x), yi); R(K)0;}
#endif

// send/receive q char vector (not for byte vectors or serialized objects)
// zstr_recv* returns C string with a trailing \0. Convert it to q char vector and free the poor thing.
Z K1(zstrrecv){PC(x); S s=zstr_recv(VSK(x)); K qs=qstr(s); if(s)free(s); R qs;}
Z K1(zstrrecvnowait){PC(x); S s=zstr_recv_nowait(VSK(x)); K qs=qstr(s); if(s)free(s); R qs;}
Z K2(zstrsend){PC(x); TC2(y,KC,-KC); r1(y); CSTR(y); I rc=zstr_send(VSK(x), s); R rc==0?y:(K)0;}
Z K2(zstrsendm){PC(x); TC2(y,KC,-KC); r1(y); CSTR(y); I rc=zstr_sendm(VSK(x), s); R rc==0?y:(K)0;}

static K attachedfn;
static K detachedfn;
ZV setattachedfn(K x){r1(x);attachedfn=x;}
ZV setdetachedfn(K x){r1(x);detachedfn=x;}

//typedef void *(zthread_detached_fn) (void *args);
ZV*df(V*args){
    K x=k(0, "@", detachedfn, args, (K)0);
    if(xt==-128){O("k() error: %s\n", xs);}
    r0(x); m9(); R NULL;}
// zthread_new(zthread_detached_fn *thread_fn, void* args);
Z K2(zthreadnew){TC(x,-KS);
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
Z K3(zthreadfork){PC(x); TC(y,-KS); 
    r1(y);
    setattachedfn(y);
    V*pipe=zthread_fork(VSK(r1(x)), af, r1(z));
    r0(y);
    if(pipe){R ptr(pipe);}else{R krr("fork");}}

// libzmq
Z K0(version){K mnp=ktn(KI,3); zmq_version(&kI(mnp)[0],&kI(mnp)[1],&kI(mnp)[2]); R mnp;}
Z K3(device){TC(x,-KI); PC(y); PC(z); R ki(zmq_device(xi, VSK(y), VSK(z)));}

typedef struct {S apiname; S fnname; V* fn; I argc; S docstring;} czmqzpi; // apiname because reflection is impossible.
Z czmqzpi zclockapi[]={
    {"zclock", "sleep", zclocksleep, 1, "sleeps for x milliseconds (-6h)."},
    {"zclock", "time", zclocktime, 0, "returns the current timestamp in milliseconds (-7h)."},
    {"zclock", "log", zclocklog, 1, "prints YY-mm-dd, followed by x (10h)."},
    {NULL,NULL,NULL,0,NULL}};
Z czmqzpi zctxapi[]={
    {"zctx", "new", zctxnew, 0, "creates and returns a new zctx."},
    {"zctx", "destroy", zctxdestroy, 1, "destroys the zctx x."},
    {"zctx", "set_iothreads", zctxsetiothreads, 2, "sets number of threads for the zctx x to be y (-6h)."},
    {"zctx", "set_linger", zctxsetlinger, 2, "sets y milliseconds (-6h) to flush zsockets in the zctx x."},
    {"zctx", "set_hwm", zctxsethwm, 2, "sets HWM value y (-6h) for the zctx x."},
    {"zctx", "get_hwm", zctxgethwm, 1, "returns HWM value (-6h) for the zctx x."},
    {"zctx", "interrupted", zctxinterrupted, 0, "returns 1b if interrupted by SIGINT/SIGTERM, 0b otherwise."},
    {"zctx", "underlying", zctxunderlying, 0, "nyi."},
    {NULL,NULL,NULL,0,NULL}};
Z czmqzpi zfileapi[]={
    {"zfile", "delete", zfiledelete, 1, "deletes the `:file x; returns 0i if successful, -1i otherwise."},
    {"zfile", "mkdir", zfilemkdir, 1, "makes a `:directory named x; returns 0i if successful, -1i otherwise."},
    {"zfile", "exists", zfileexists, 1, "returns 1i if the `:file x exists, 0i otherwise."},
    {"zfile", "size", zfilesize, 1, "returns the size (-7h) of the `:file x."},
    {NULL,NULL,NULL,0,NULL}};
Z czmqzpi zframeapi[]={
    {"zframe", "new", zframenew, 1, "creates and returns a new zframe from x (10h|4h); returns nothing on failure."},
    {"zframe", "new_zero_copy", zframenewzerocopy, 0, "nyi."},
    {"zframe", "destroy", zframedestroy, 1, "destroys the zframe x."},
    {"zframe", "recv", zframerecv, 1, "receives, returns the zframe from the zsocket x."},
    {"zframe", "recvnowait", zframerecvnowait, 1, "receives, returns the zframe from the zsocket x without waiting."},
    {"zframe", "send", zframesend, 3, "sends the zframe x to the zsocket y with flag z (-6h); returns non-zero on failure (-6h)."},
    {"zframe", "size", zframesize, 1, "returns the byte count of the zframe x."},
    {"zframe", "data", zframedata, 0, "nyi."},
    {"zframe", "dup", zframedup, 1, "returns a duplicate of the zframe x."},
    {"zframe", "strhex", zframestrhex, 0, "nyi."},
    {"zframe", "strdup", zframestrdup, 1, "returns a 10h of the zframe x's data."},
    {"zframe", "streq", zframestreq, 2, "returns whether the body of zframe x=y (10h)."},
    {"zframe", "zero_copy", zframezerocopy, 0, "nyi."},
    {"zframe", "more", zframemore, 1, "returns the zframe x's more property (-6h)."},
    {"zframe", "eq", zframeeq, 2, "returns 1b if zframes x=y, 0b otherwise."},
    {"zframe", "print", zframeprint, 2, "prints the content of the zframe x prefixed by y (10h) to stderr."},
    {"zframe", "reset", zframereset, 2, "resets the zframe x with the new data y."},
    {NULL,NULL,NULL,0,NULL}};
Z czmqzpi zloopapi[]={
    {"zloop", "new", zloopnew, 0, "creates a new zloop."},
    {"zloop", "destroy", zloopdestroy, 1, "destroys the zloop x."},
    {"zloop", "poller", zlooppoller, 4, "registers pollitem y with the zloop x; the handler z (-11h) is called with z4 when y is ready; returns 0i if there is no error, -1i otherwise."},
    {"zloop", "poller_end", zlooppollerend, 2, "cancels the pollitem y from the zloop x."},
    // zloop_timer (zloop_t *self, size_t delay, size_t times, zloop_fn handler, void *arg)
    {"zloop", "timer", zlooptimer, 5, "registers a timer to the zloop x, with the delay y, the repetitions z, the handler z4 (-11h), and the arguments z5. see `zloop for a full documentation."},
    {"zloop", "timer_end", zlooptimerend, 2, "cancels all timers for the zloop x and y."},
    {"zloop", "set_verbose", zloopsetverbose, 2, "verbose tracing of the zloop x to y, 1b or 0b."},
    {"zloop", "start", zloopstart, 1, "starts zloop x; returns 0i if interrupted, -1i if cancelled by a handler."},
    {NULL,NULL,NULL,0,NULL}};
Z czmqzpi zmsgapi[]={
    {"zmsg", "new", zmsgnew, 0, "creates a new empty zmsg."},
    {"zmsg", "destroy", zmsgdestroy, 1, "destroys the zmsg x."},
    {"zmsg", "recv", zmsgrecv, 1, "receives the zmsg from zsocket x."},
    {"zmsg", "send", zmsgsend, 2, "sends the zmsg x to the zsocket y."},
    {"zmsg", "size", zmsgsize, 1, "returns number of zframes in the zmsg x."},
    {"zmsg", "content_size", zmsgcontentsize, 1, "returns the combined size of all zframes in the zmsg x."},
    {"zmsg", "push", zmsgpush, 2, "pushes the zframe y to the front of the zmsg x."},
    {"zmsg", "pop", zmsgpop, 1, "pops the zframe y from the front of the zmsg x or raises 'empty."},
    {"zmsg", "add", zmsgadd, 2, "adds the zframe y to the end of the zmsg x."},
    {"zmsg", "pushmem", zmsgpushmem, 0, "nyi."},
    {"zmsg", "addmem", zmsgaddmem, 0, "nyi."},
    {"zmsg", "pushstr", zmsgpushstr, 0, "nyi."},
    {"zmsg", "addstr", zmsgaddstr, 0, "nyi."},
    {"zmsg", "popstr", zmsgpopstr, 0, "nyi."},
    {"zmsg", "wrap", zmsgwrap, 0, "nyi."},
    {"zmsg", "unwrap", zmsgunwrap, 0, "nyi."},
    {"zmsg", "remove", zmsgremove, 0, "nyi."},
    {"zmsg", "first", zmsgfirst, 1, "returns the first zframe from the zmsg x or raises 'empty."},
    {"zmsg", "next", zmsgnext, 1, "returns the next zframe from the zmsg x or raises 'empty."},
    {"zmsg", "last", zmsglast, 1, "returns the last zframe from the zmsg x or raises 'empty."},
    {"zmsg", "save", zmsgsave, 2, "saves the zmsg x to the `:file y."},
    {"zmsg", "load", zmsgload, 2, "loads the zmsg x from the `:file y."},
    {"zmsg", "encode", zmsgencode, 0, "nyi."},
    {"zmsg", "decode", zmsgdecode, 0, "nyi."},
    {"zmsg", "dup", zmsgdup, 1, "returns a duplicate of the zmsg x."},
    {"zmsg", "dump", zmsgdump, 1, "dumps the content of the zmsg x to stderr."},
    {NULL,NULL,NULL,0,NULL}};
Z czmqzpi zsocketapi[]={
    {"zsocket", "new", zsocketnew, 2, "returns the new zsocket of the zmq socket option y (-6h) for the zctx x."},
    {"zsocket", "destroy", zsocketdestroy, 2, "destroys the zsocket y for the zctx x."},
    {"zsocket", "bind", zsocketbind, 2, "binds the zsocket x to the `transport://endpoint y; returns the bound port number (-6h)."},
    {"zsocket", "connect", zsocketconnect, 2, "connects the zsocket x to the `transport://endpoint y; returns 0i if successul, -1i otherwise."},
    {"zsocket", "type_sym", zsockettypestr, 1, "prints the `option of the zsocket x. (N.B. czmq type_str())"},
    {NULL,NULL,NULL,0,NULL}};
Z czmqzpi zsockoptapi[]={
#if (ZMQ_VERSION_MAJOR == 2)
    {"zsockopt", "hwm", zsockethwm, 1, ""},
    {"zsockopt", "swap", zsocketswap, 1, ""},
    {"zsockopt", "affinity", zsocketaffinity, 1, ""},
    {"zsockopt", "identity", zsocketidentity, 1, ""},
    {"zsockopt", "rate", zsocketrate, 1, ""},
    {"zsockopt", "recovery_ivl", zsocketrecovery_ivl, 1, ""},
    {"zsockopt", "recovery_ivl_msec", zsocketrecovery_ivl_msec, 1, ""},
    {"zsockopt", "mcast_loop", zsocketmcast_loop, 1, ""},
#   if (ZMQ_VERSION_MINOR == 2)
    {"zsockopt", "rcvtimeo", zsocketrcvtimeo, 1, ""},
#   endif
#   if (ZMQ_VERSION_MINOR == 2)
    {"zsockopt", "sndtimeo", zsocketsndtimeo, 1, ""},
#   endif
    {"zsockopt", "sndbuf", zsocketsndbuf, 1, ""},
    {"zsockopt", "rcvbuf", zsocketrcvbuf, 1, ""},
    {"zsockopt", "linger", zsocketlinger, 1, ""},
    {"zsockopt", "reconnect_ivl", zsocketreconnect_ivl, 1, ""},
    {"zsockopt", "reconnect_ivl_max", zsocketreconnect_ivl_max, 1, ""},
    {"zsockopt", "backlog", zsocketbacklog, 1, ""},
    {"zsockopt", "type", zsockettype, 1, ""},
    {"zsockopt", "rcvmore", zsocketrcvmore, 1, ""},
    {"zsockopt", "fd", zsocketfd, 1, ""},
    {"zsockopt", "events", zsocketevents, 1, ""},
    {"zsockopt", "set_hwm", zsocketsethwm, 2, ""},
    {"zsockopt", "set_swap", zsocketsetswap, 2, ""},
    {"zsockopt", "set_affinity", zsocketsetaffinity, 2, ""},
    {"zsockopt", "set_identity", zsocketsetidentity, 2, ""},
    {"zsockopt", "set_rate", zsocketsetrate, 2, ""},
    {"zsockopt", "set_recovery_ivl", zsocketsetrecovery_ivl, 2, ""},
    {"zsockopt", "set_recovery_ivl_msec", zsocketsetrecovery_ivl_msec, 2, ""},
    {"zsockopt", "set_mcast_loop", zsocketsetmcast_loop, 2, ""},
#   if (ZMQ_VERSION_MINOR == 2)
    {"zsockopt", "set_rcvtimeo", zsocketsetrcvtimeo, 2, ""},
#   endif
#   if (ZMQ_VERSION_MINOR == 2)
    {"zsockopt", "set_sndtimeo", zsocketsetsndtimeo, 2, ""},
#   endif
    {"zsockopt", "set_sndbuf", zsocketsetsndbuf, 2, ""},
    {"zsockopt", "set_rcvbuf", zsocketsetrcvbuf, 2, ""},
    {"zsockopt", "set_linger", zsocketsetlinger, 2, ""},
    {"zsockopt", "set_reconnect_ivl", zsocketsetreconnect_ivl, 2, ""},
    {"zsockopt", "set_reconnect_ivl_max", zsocketsetreconnect_ivl_max, 2, ""},
    {"zsockopt", "set_backlog", zsocketsetbacklog, 2, ""},
    {"zsockopt", "set_subscribe", zsocketsetsubscribe, 2, ""},
    {"zsockopt", "set_unsubscribe", zsocketsetunsubscribe, 2, ""},
#endif
#if (ZMQ_VERSION_MAJOR == 3)
    {"zsockopt", "type", zsockettype, 1, ""},
    {"zsockopt", "sndhwm", zsocketsndhwm, 1, ""},
    {"zsockopt", "rcvhwm", zsocketrcvhwm, 1, ""},
    {"zsockopt", "affinity", zsocketaffinity, 1, ""},
    {"zsockopt", "identity", zsocketidentity, 1, ""},
    {"zsockopt", "rate", zsocketrate, 1, ""},
    {"zsockopt", "recovery_ivl", zsocketrecovery_ivl, 1, ""},
    {"zsockopt", "sndbuf", zsocketsndbuf, 1, ""},
    {"zsockopt", "rcvbuf", zsocketrcvbuf, 1, ""},
    {"zsockopt", "linger", zsocketlinger, 1, ""},
    {"zsockopt", "reconnect_ivl", zsocketreconnect_ivl, 1, ""},
    {"zsockopt", "reconnect_ivl_max", zsocketreconnect_ivl_max, 1, ""},
    {"zsockopt", "backlog", zsocketbacklog, 1, ""},
    {"zsockopt", "maxmsgsize", zsocketmaxmsgsize, 1, ""},
    {"zsockopt", "ipv4only", zsocketipv4only, 1, ""},
    {"zsockopt", "rcvmore", zsocketrcvmore, 1, ""},
    {"zsockopt", "fd", zsocketfd, 1, ""},
    {"zsockopt", "events", zsocket_events, 1, ""},
    {"zsockopt", "last_endpoint", zsocket_last_endpoint, 1, ""},
    {"zsockopt", "set_sndhwm", zsocketsetsndhwm, 2, ""},
    {"zsockopt", "set_rcvhwm", zsocketsetrcvhwm, 2, ""},
    {"zsockopt", "set_affinity", zsocketsetaffinity, 2, ""},
    {"zsockopt", "set_subscribe", zsocketsetsubscribe, 2, ""},
    {"zsockopt", "set_unsubscribe", zsocketsetunsubscribe, 2, ""},
    {"zsockopt", "set_identity", zsocketsetidentity, 2, ""},
    {"zsockopt", "set_rate", zsocketsetrate, 2, ""},
    {"zsockopt", "set_recovery_ivl", zsocketsetrecovery_ivl, 2, ""},
    {"zsockopt", "set_sndbuf", zsocketsetsndbuf, 2, ""},
    {"zsockopt", "set_rcvbuf", zsocketsetrcvbuf, 2, ""},
    {"zsockopt", "set_linger", zsocketsetlinger, 2, ""},
    {"zsockopt", "set_reconnect_ivl", zsocketsetreconnect_ivl, 2, ""},
    {"zsockopt", "set_reconnect_ivl_max", zsocketsetreconnect_ivl_max, 2, ""},
    {"zsockopt", "set_multicast_hops", zsocketsetmulticast_hops, 2, ""},
    {"zsockopt", "set_backlog", zsocketsetbacklog, 2, ""},
    {"zsockopt", "set_maxmsgsize", zsocketsetmaxmsgsize, 2, ""},
    {"zsockopt", "set_multicast_hops", zsocketmulticast_hops, 2, ""},
    {"zsockopt", "set_rcvtimeo", zsocketsetrcvtimeo, 2, ""},
    {"zsockopt", "set_sndtimeo", zsocketsetsndtimeo, 2, ""},
    {"zsockopt", "set_ipv4only", zsocketsetipv4only, 2, ""},
    {"zsockopt", "set_failunroutable", zsocketsetfailunroutable, 2, ""},
    {"zsockopt", "set_hwm", zsocketsethwm, 2, ""},
#endif
    {NULL,NULL,NULL,0,NULL}};
Z czmqzpi zstrapi[]={
    {"zstr", "recv", zstrrecv, 1, "returns a 10h received from the zsocket x."},
    {"zstr", "recv_nowait", zstrrecvnowait, 1, "returns a 10h received from the zsocket x without waiting."},
    {"zstr", "send", zstrsend, 2, "sends y (10h) to the zsocket x."},
    {"zstr", "sendm", zstrsendm, 2, "sends y (10h) to the zsocket x with MORE flag."},
    {NULL,NULL,NULL,0,NULL}};
Z czmqzpi zthreadapi[] = {
    {"zthread", "new", zthreadnew, 2, "creates a detached thread running the function x (-11h) with the argument y."},
    {"zthread", "fork", zthreadfork, 3, "creates an attached thread running the function y (-11h) with the argument z in the zctx x."},
    {NULL,NULL,NULL,0,NULL},};
Z czmqzpi libzmqapi[] = {
    {"libzmq", "version", version, 0, "returns major, minor, patch version numbers (6h) of libzmq."},
    {"libzmq", "device", device, 3, "sets a zmq_device for the zctx x, the frontend y and the backend z, both zsocket. see 'man zmq_device'"},
    {NULL,NULL,NULL,0,NULL}};

// if argc==0, set it to 1 otherwise 'rank
#define APITAB(fnname,fn,argc) xS[i]=ss(fnname);kK(y)[i]=dl(fn,argc>0?argc:1)
// notice the -1 for the NULL sentinals in *api[]
#define EXPAPI(name) K1(name){int n=tblsize(name##api)-1; K y=ktn(0,n);x=ktn(KS,n); \
    DO(n, APITAB(name##api[i].fnname, name##api[i].fn, name##api[i].argc)); \
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

// generate args list based on argc.
ZS printargs(I argc){SW(argc){CS(0,R"[]")CS(1,R"[x]")CS(2,R"[x;y]")CS(3,R"[x;y;z]")CS(4,R"[x;y;z;z4]")CS(5,R"[x;y;z;z4;z5]")CD:R"";}}

// doc is a table keyed by `api.fnname with args and docstring columns
K0(makedoc){
    czmqzpi* apis[] = {zclockapi, zctxapi, zfileapi, zframeapi, zloopapi, zmsgapi, zsocketapi, /*zsockoptapi*/ zstrapi, zthreadapi, libzmqapi};
    czmqzpi **each = apis;
    C s[40]; C see[100]; I i,j;
    K key,kc,kd, val, vc, argz, docstrings;
    kc=ktn(KS,1); kS(kc)[0]=ss("name"); kd=ktn(KS,0);
    vc=ktn(KS,2); kS(vc)[0]=ss("args"); kS(vc)[1]=ss("docstring");
    argz=ktn(KS,0); docstrings=ktn(KS,0);
    // special cases for qzmq, czmq, zmq(3) and zsockopt(7); zhash and zlist are not included in qzmq.
    js(&kd, ss("qzmq")); js(&argz, ss("")); js(&docstrings, ss("see https://github.com/jaeheum/qzmq/blob/master/README.md."));
    js(&kd, ss("czmq")); js(&argz, ss("")); js(&docstrings, ss("see http://czmq.zeromq.org or 'man czmq'."));
    js(&kd, ss("zmq")); js(&argz, ss("")); js(&docstrings, ss("keys/values of zmq (99h) match 'zmq.h', 'man zmq_socket', 'man zmq_getsockopt', `zsockopt and `zsocket.type_sym."));
    js(&kd, ss("zsockopt")); js(&argz, ss("")); js(&docstrings, ss("see http://czmq.zeromq.org/manual:zsockopt or 'man zsockopt'. see also `zmq and `zsocket.type_sym."));
    js(&kd, ss("zhash")); js(&argz, ss("")); js(&docstrings, ss("nyi."));
    js(&kd, ss("zlist")); js(&argz, ss("")); js(&docstrings, ss("nyi."));
    for(i=0; i < (int)tblsize(apis); i++) {
        js(&kd, ss(each[i][0].apiname)); js(&argz, ss(""));
        if(strcmp(each[i][0].apiname, "libzmq")==0){snprintf(see, sizeof(see), "see http://api.zeromq.org or 'man zmq'.");}
        else{snprintf(see, sizeof(see), "see http://czmq.zeromq.org/manual:%s or 'man %s'.", each[i][0].apiname, each[i][0].apiname);}
        js(&docstrings, ss(see));
        for (j=0; each[i][j].fnname != NULL; j++) {
            snprintf(s, sizeof(s), "%s.%s", each[i][j].apiname, each[i][j].fnname);
            js(&kd,ss(s));
            js(&argz, ss(printargs(each[i][j].argc)));
            js(&docstrings,ss(each[i][j].docstring));}}
    key=xT(xD(kc, knk(1,kd))); val=xT(xD(vc, knk(2,argz,docstrings)));
    R xD(key,val);}

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
    {"QUEUE", ZMQ_QUEUE},
    {"FORWARDER", ZMQ_FORWARDER},
    {"STREAMER", ZMQ_STREAMER},
};
#define OPTTAB(k,v) xS[i]=ss(k);kK(y)[i]=ki(v)

K1(zmq){int n=tblsize(zmqopt);
    K y=ktn(0,n);x=ktn(KS,n);
    DO(tblsize(zmqopt), OPTTAB(zmqopt[i].k, zmqopt[i].v));
    R xD(x,y);}
