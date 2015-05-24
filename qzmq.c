// qzmq - q bindings for CZMQ, the high-level C binding for 0MQ:
//   http://czmq.zeromq.org.
//
// Copyright (c) 2012-2015 Jaeheum Han <jay.han@gmail.com>
// This file is part of qzmq.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "czmq.h"
#include "k.h"

#define RZ R(K)0
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
#define zi z->i
#define zj z->j
#define VSK(x) (V*)(intptr_t)(x->j) // void* from K; dual: ptr(V*). c.f. ZTK.
ZK ptr(V*x){if(x){R kj((intptr_t)x);}else{RZ;}} // K from opaque types e.g. void*, zctx_t, etc; dual: VSK(x).
#define ZTK(t,v) t*v=(t*)(intptr_t)(x->j)
#define KRR(x) krr(strerror(x))
#define TC(x,T) P(x->t!=T,krr("type")) // typechecker
#define TC2(x,T,T2) P(x->t!=T&&x->t!=T2,krr("type"))
#define IC(x) P(x->t==-KJ&&x->j>wi,krr("type"));if(x->t==-KJ)x=ki((int)x->j); // j from i (=<0Wi).
#define PC(x) TC(x,-KJ) // pointer check; implementation dependent -- see ptr(V*).
ZI N(K x){if(xt>0)R xn;R 1;} // x->n may be unset for n=1.
// this macro was defined in czmq_prelude.h before 1.4 until fd79227edf2b18136bd0d1e246382f01c9e7c635 1 parent 74f8307
#define tblsize(x)      (sizeof (x) / sizeof ((x) [0]))

#ifdef DEBUG
#define D(x) zsys_debug("%s:%s:%s:%s:%s\n", x, __FILE__, __LINE__,  __FUNCTION__, strerror(errno))
#else
#define D(x)
#endif

//zactor
ZK actorfn;
sem_t actorfn_sem;
//typedef void (zactor_fn) (zsock_t *pipe, void *args);
ZV task(zsock_t *pipe, void *args){
    sem_post(&actorfn_sem);
    K x=dot(actorfn, knk(2, ptr(pipe), d9(args)));
    if(xt==-128){O("k() error: %s\n", xs);}
    r0(x);
    m9(); R;}
// zactor_new(zactor_fn *task, void *args)
Z K2(zactornew){
    sem_init(&actorfn_sem,0,0);
    actorfn=x;
    zactor_t*a=zactor_new(task,b9(-1,y));
    sem_wait(&actorfn_sem);
    sem_destroy(&actorfn_sem);
    if(a){R ptr(a);}else{R krr("zactor_new");}
}
Z K1(zactordestroy){PC(x); ZTK(zactor_t,a); zactor_destroy(&a); RZ;}
Z K2(zactorsend){PC(x); PC(y); zmsg_t*m=(zmsg_t*)(intptr_t)(y->j);
    R kj(zactor_send(VSK(x), &m));}
Z K1(zactorrecv){PC(x); R ptr(zactor_recv(VSK(x)));}
Z K1(zactoris){PC(x); R kb(zactor_is(VSK(x)));}
Z K1(zactorresolve){PC(x); R ptr(zactor_resolve(VSK(x)));}
Z K1(zactorsock){PC(x); R ptr(zactor_sock(VSK(x)));}

//zclock (kept for zthread & testing)
Z K1(zclocksleep){TC2(x,-KI,-KJ); IC(x); zclock_sleep(xi); RZ;}

//zctx (kept for zthread)
Z K0(zctxnew){zctx_t*ctx=zctx_new(); P(ctx, ptr(ctx)); R KRR(errno);}
Z K1(zctxdestroy){PC(x); ZTK(zctx_t,ctx); zctx_destroy(&ctx); RZ;}
Z K2(zctxsetiothreads){PC(x); TC2(y,-KI,-KJ); IC(y); zctx_set_iothreads(VSK(x), yi); RZ;}
Z K2(zctxsetlinger){PC(x); TC2(y,-KI,-KJ); IC(y); zctx_set_linger(VSK(x), yi); RZ;}
Z K0(zctxinterrupted){R kb(zctx_interrupted);}

//zfile `:path (kept for zthread & testing)
Z K1(zfiledelete){TC(x,-KS); R kj(zfile_delete(++xs));}
Z K1(zfilemkdir){TC(x,-KS); R kj(zfile_mkdir(++xs));}
Z K1(zfileexists){TC(x,-KS); R kj(zfile_exists(++xs));}
Z K1(zfilesize){TC(x,-KS); R kj(zfile_size(++xs));}

//zframe
Z K1(zframenew){P((abs(xt)!=KG&&abs(xt)!=KC), krr("type"));
    if(xt>0){zframe_t*f=zframe_new(xG, xn); P(f, ptr(f)); RZ;}
    else{zframe_t*f=zframe_new(&xg, 1); P(f, ptr(f)); RZ;}}
Z K0(zframeempty){zframe_t*f=zframe_new_empty(); P(f, ptr(f)); RZ;}
Z K1(zframedestroy){PC(x); ZTK(zframe_t,f); zframe_destroy(&f); RZ;}
Z K1(zframerecv){PC(x); zframe_t*f=zframe_recv(VSK(x)); P(f, ptr(f)); RZ;}
Z K3(zframesend){PC(x); PC(y); TC2(z,-KI,-KJ); IC(z); ZTK(zframe_t,f); R kj(zframe_send(&f, VSK(y), zi));}
Z K1(zframedup){PC(x); R ptr(zframe_dup(VSK(x)));}
Z K1(zframestrdup){PC(x); R ks(zframe_strdup(VSK(x)));}
Z K2(zframestreq){PC(x);  K z=kb(zframe_streq(VSK(x), ys)); R z;}
Z K1(zframemore){PC(x); R kj(zframe_more(VSK(x)));}
Z K1(zframesize){PC(x); R kj(zframe_size(VSK(x)));}
Z K2(zframeeq){PC(x); PC(y); R kb(zframe_eq(VSK(x), VSK(y)));}
Z K2(zframereset){zframe_reset(VSK(x), yG, N(y)); RZ;}
Z K2(zframeprint){PC(x); zframe_print(VSK(x), ys); RZ;}
Z K1(zframeis){PC(x); R kb(zframe_is(VSK(x)));}

//zmsg
Z K0(zmsgnew){zmsg_t*m=zmsg_new(); P(m, ptr(m)); RZ;}
Z K1(zmsgdestroy){PC(x); ZTK(zmsg_t,m); zmsg_destroy(&m); RZ;}
Z K1(zmsgrecv){PC(x); R ptr(zmsg_recv(VSK(x)));}
Z K2(zmsgsend){PC(x); PC(y); ZTK(zmsg_t,m); R kj(zmsg_send(&m, VSK(y)));}
Z K1(zmsgsize){PC(x); R kj(zmsg_size(VSK(x)));}
Z K1(zmsgcontentsize){PC(x); R kj(zmsg_content_size(VSK(x)));}
Z K2(zmsgprepend){PC(x); PC(y); zframe_t*f=(zframe_t*)(intptr_t)yj;  R kj(zmsg_prepend(VSK(x), &f));}
Z K2(zmsgappend){PC(x); PC(y); zframe_t*f=(zframe_t*)(intptr_t)yj;  zmsg_append(VSK(x), &f); RZ;}
Z K1(zmsgpop){PC(x); P(zmsg_size(VSK(x))>0, ptr(zmsg_pop(VSK(x)))); R krr("empty");}

Z K1(zmsgpushmem){PC(x); R krr("nyi");}
Z K1(zmsgaddmem){PC(x); R krr("nyi");}

Z K2(zmsgpushstr){PC(x); R kj(zmsg_pushstr(VSK(x), ys));}
Z K2(zmsgaddstr){PC(x); R kj(zmsg_addstr(VSK(x), ys));}
Z K2(zmsgpushstrf){PC(x); R krr("nyi");}
Z K2(zmsgaddstrf){PC(x); R krr("nyi");}
Z K1(zmsgpopstr){PC(x); R ks(zmsg_popstr(VSK(x)));}

Z K1(zmsgfirst){PC(x); P(zmsg_size(VSK(x))>0, ptr(zmsg_first(VSK(x)))); R krr("empty");}
Z K1(zmsgnext){PC(x); P(zmsg_size(VSK(x))>0, ptr(zmsg_next(VSK(x))));  R krr("empty");}
Z K1(zmsglast){PC(x); P(zmsg_size(VSK(x))>0, ptr(zmsg_last(VSK(x)))); R krr("empty");}
Z K2(zmsgsave){PC(x); TC(y,-KS); FILE*f=fopen(++ys, "w+"); I rc=zmsg_save(VSK(x), f); fclose(f); R kj(rc);}
Z K2(zmsgload){PC(x); TC(y,-KS); FILE*f=fopen(++ys, "r"); zmsg_t*m=zmsg_load(VSK(x), f); fclose(f); R ptr(m);}
Z K1(zmsgdup){PC(x); zmsg_t*m=zmsg_dup(VSK(x)); P(m,ptr(m)); RZ;}
Z K1(zmsgprint){PC(x); zmsg_print(VSK(x)); RZ;}
Z K1(zmsgis){PC(x); R kb(zmsg_is(VSK(x)));}

//zsock
Z K1(zsocknew){R ptr(zsock_new(xi));}
Z K1(zsocknewpub){R ptr(zsock_new_pub(xs));}
Z K2(zsocknewsub){R ptr(zsock_new_sub(xs,ys));}
Z K1(zsocknewreq){R ptr(zsock_new_req(xs));}
Z K1(zsocknewrep){R ptr(zsock_new_rep(xs));}
Z K1(zsocknewdealer){R ptr(zsock_new_dealer(xs));}
Z K1(zsocknewrouter){R ptr(zsock_new_router(xs));}
Z K1(zsocknewpull){R ptr(zsock_new_pull(xs));}
Z K1(zsocknewpush){R ptr(zsock_new_push(xs));}
Z K1(zsocknewxpub){R ptr(zsock_new_xpub(xs));}
Z K1(zsocknewxsub){R ptr(zsock_new_xsub(xs));}
Z K1(zsocknewpair){R ptr(zsock_new_pair(xs));}
Z K1(zsocknewstream){R ptr(zsock_new_stream(xs));}
Z K1(zsockdestroy){PC(x); ZTK(zsock_t,s); zsock_destroy(&s); RZ;}
Z K2(zsockbind){PC(x); TC(y,-KS); R kj(zsock_bind(VSK(x),ys));}
Z K1(zsockendpoint){PC(x); R ks((const S)zsock_endpoint(VSK(x)));}
Z K2(zsockunbind){PC(x); TC(y,-KS); R kj(zsock_unbind(VSK(x),ys));}
Z K2(zsockconnect){PC(x); TC(y,-KS); R kj(zsock_connect(VSK(x),ys));}
Z K2(zsockdisconnect){PC(x); TC(y,-KS); R kj(zsock_disconnect(VSK(x),ys));}
Z K3(zsockattach){PC(x); TC(y,-KS); R kj(zsock_attach(VSK(x),ys, z->g));}
Z K1(zsocktypestr){PC(x); R ks((const S)zsock_type_str(VSK(x)));}
Z K2(zsocksend){PC(x); TC(y,-KS); R kj(zsock_send(VSK(x), ys));}
Z K2(zsockrecv){PC(x); TC(y,-KS); R kj(zsock_recv(VSK(x), ys));}
Z K2(zsockbsend){PC(x); TC(y,-KS); R kj(zsock_bsend(VSK(x), ys));}
Z K2(zsockbrecv){PC(x); TC(y,-KS); R kj(zsock_brecv(VSK(x), ys));}
Z K1(zsocksetunbounded){PC(x);zsock_set_unbounded(VSK(x)); RZ;}
Z K2(zsocksignal){PC(x); R kj(zsock_signal(VSK(x), y->g));}
Z K1(zsockwait){PC(x); R kj(zsock_wait(VSK(x)));}
Z K1(zsockflush){PC(x); zsock_flush(VSK(x)); RZ;}
Z K1(zsockis){PC(x); R kb(zsock_is(VSK(x)));}
Z K1(zsockresolve){PC(x); R ptr(zsock_resolve(VSK(x)));}
//zstr
Z K1(zstrrecv){PC(x); S s=zstr_recv(VSK(x)); K is=ks(s); zstr_free(&s); R is;}
Z K2(zstrsend){PC(x); R kj(zstr_send(VSK(x), ys));}
Z K2(zstrsendm){PC(x); R kj(zstr_sendm(VSK(x), ys));}
Z K3(zstrsendx){PC(x); R kj(zstr_sendx(VSK(x), ys, z->s, NULL));}

//zsys
Z K1(zsyserror){zsys_error(xs); RZ;}
Z K1(zsyswarning){zsys_warning(xs); RZ;}
Z K1(zsysnotice){zsys_notice(xs); RZ;}
Z K1(zsysinfo){zsys_info(xs); RZ;}
Z K1(zsysdebug){zsys_debug(xs); RZ;}

// libzmq
Z K0(version){K mnp=ktn(KI,3); zmq_version(&kI(mnp)[0],&kI(mnp)[1],&kI(mnp)[2]); R mnp;}
Z K3(device){TC2(x,-KI,-KJ); IC(x); PC(y); PC(z); R kj(zmq_device(xi, VSK(y), VSK(z)));}

// apiname because reflection is impossible.
typedef struct {S apiname; S fnname; V* fn; I argc;} czmqzpi;
Z czmqzpi zactorapi[]={
    {"zactor", "new", zactornew, 2},
    {"zactor", "destroy", zactordestroy, 1},
    {"zactor", "send", zactorsend, 2},
    {"zactor", "recv", zactorrecv, 1},
    {"zactor", "is", zactoris, 1},
    {"zactor", "resolve", zactorresolve, 1},
    {"zactor", "sock", zactorsock, 1},
    {NULL,NULL,NULL,0}};    
Z czmqzpi zctxapi[]={
    {"zctx", "new", zctxnew, 0},
    {"zctx", "destroy", zctxdestroy, 1},
    {"zctx", "set_iothreads", zctxsetiothreads, 2},
    {"zctx", "set_linger", zctxsetlinger, 2},
    {"zctx", "interrupted", zctxinterrupted, 0},
    {NULL,NULL,NULL,0}};
Z czmqzpi zclockapi[]={
    {"zclock", "sleep", zclocksleep, 1},
    {NULL,NULL,NULL,0}};
Z czmqzpi zfileapi[]={
    {"zfile", "delete", zfiledelete, 1,},
    {"zfile", "mkdir", zfilemkdir, 1},
    {"zfile", "exists", zfileexists, 1},
    {"zfile", "size", zfilesize, 1,},
    {NULL,NULL,NULL,0}};
Z czmqzpi zframeapi[]={
    {"zframe", "new", zframenew, 1},
    {"zframe", "empty", zframeempty, 0},
    {"zframe", "destroy", zframedestroy, 1},
    {"zframe", "recv", zframerecv, 1},
    {"zframe", "send", zframesend, 3},
    {"zframe", "size", zframesize, 1},
    {"zframe", "dup", zframedup, 1},
    {"zframe", "strdup", zframestrdup, 1},
    {"zframe", "streq", zframestreq, 2},
    {"zframe", "more", zframemore, 1},
    {"zframe", "eq", zframeeq, 2},
    {"zframe", "print", zframeprint, 2},
    {"zframe", "reset", zframereset, 2},
    {"zframe", "is", zframeis, 1},
    {NULL,NULL,NULL,0}};
Z czmqzpi zmsgapi[]={
    {"zmsg", "new", zmsgnew, 0},
    {"zmsg", "destroy", zmsgdestroy, 1},
    {"zmsg", "recv", zmsgrecv, 1},
    {"zmsg", "send", zmsgsend, 2},
    {"zmsg", "size", zmsgsize, 1},
    {"zmsg", "content_size", zmsgcontentsize, 1},
    {"zmsg", "prepend", zmsgprepend, 2},
    {"zmsg", "append", zmsgappend, 2},
    {"zmsg", "pop", zmsgpop, 1},
    {"zmsg", "pushmem", zmsgpushmem, 1},
    {"zmsg", "addmem", zmsgaddmem, 1},
    {"zmsg", "pushstr", zmsgpushstr, 1},
    {"zmsg", "addstr", zmsgaddstr, 1},
    {"zmsg", "pushstrf", zmsgpushstrf, 1},
    {"zmsg", "addstrf", zmsgaddstrf, 2},
    {"zmsg", "popstr", zmsgpopstr, 1},

    {"zmsg", "first", zmsgfirst, 1},
    {"zmsg", "next", zmsgnext, 1},
    {"zmsg", "last", zmsglast, 1},
    {"zmsg", "save", zmsgsave, 2},
    {"zmsg", "load", zmsgload, 2},
    {"zmsg", "dup", zmsgdup, 1},
    {"zmsg", "print", zmsgprint, 1},
    {"zmsg", "is", zmsgis, 1},
    {NULL,NULL,NULL,0}};
Z czmqzpi zsockapi[]={
    {"zsock", "new", zsocknew, 1},
    {"zsock", "new_pub", zsocknewpub, 1},
    {"zsock", "new_sub", zsocknewsub, 2},
    {"zsock", "new_req", zsocknewreq, 1},
    {"zsock", "new_rep", zsocknewrep, 1},
    {"zsock", "new_dealer", zsocknewdealer, 1},
    {"zsock", "new_router", zsocknewrouter, 1},
    {"zsock", "new_pull", zsocknewpull, 1},
    {"zsock", "new_push", zsocknewpush, 1},
    {"zsock", "new_xpub", zsocknewxpub, 1},
    {"zsock", "new_xsub", zsocknewxsub, 1},
    {"zsock", "new_pair", zsocknewpair, 1},
    {"zsock", "new_stream", zsocknewstream, 1},
    {"zsock", "destroy", zsockdestroy, 1},
    {"zsock", "bind", zsockbind, 2},
    {"zsock", "unbind", zsockunbind, 2},
    {"zsock", "connect", zsockconnect, 2},
    {"zsock", "endpoint", zsockendpoint, 1},
    {"zsock", "disconnect", zsockdisconnect, 2},
    {"zsock", "attach", zsockattach, 2},
    {"zsock", "type_str", zsocktypestr, 1},
    {"zsock", "send", zsocksend, 1},
    {"zsock", "recv", zsockrecv, 1},
    {"zsock", "bsend", zsockbsend, 1},
    {"zsock", "brecv", zsockbrecv, 1},
    {"zsock", "set_unbounded", zsocksetunbounded, 1},
    {"zsock", "signal", zsocksignal, 2},
    {"zsock", "wait", zsockwait, 1},
    {"zsock", "flush", zsockflush, 1},
    {"zsock", "is", zsockis, 1},
    {"zsock", "resolve", zsockresolve, 1},
    {NULL,NULL,NULL,0}};
Z czmqzpi zstrapi[]={
    {"zstr", "recv", zstrrecv, 1},
    {"zstr", "send", zstrsend, 2},
    {"zstr", "sendm", zstrsendm, 2},
    {"zstr", "sendx", zstrsendx, 3},
    {NULL,NULL,NULL,0}};
Z czmqzpi libzmqapi[] = {
    {"libzmq", "version", version, 0},
    {"libzmq", "device", device, 3},
    {NULL,NULL,NULL,0}};
Z czmqzpi zsysapi[]={
    {"zsys", "error", zsyserror, 1},
    {"zsys", "warning", zsyswarning, 1},
    {"zsys", "notice", zsysnotice, 1},
    {"zsys", "info", zsysinfo, 1},
    {"zsys", "debug", zsysdebug, 1},
    {"zsys", "warning", zsyswarning, 1},
    {NULL,NULL,NULL,0}};
// if argc==0, set it to 1 otherwise 'rank
#define APITAB(fnname,fn,argc) xS[i]=ss(fnname);kK(y)[i]=dl(fn,argc>0?argc:1)
// notice the -1 for the NULL sentinals in *api[]
#define EXPAPI(name) K1(name){int n=tblsize(name##api)-1; K y=ktn(0,n);x=ktn(KS,n); \
    DO(n, APITAB(name##api[i].fnname, name##api[i].fn, name##api[i].argc)); \
    R xD(x,y);}

EXPAPI(zactor);
EXPAPI(zctx);
EXPAPI(zclock);
EXPAPI(zfile);
EXPAPI(zframe);
EXPAPI(zmsg);
EXPAPI(zsock);
EXPAPI(zstr);
EXPAPI(zsys);
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
    {"AFFINITY", ZMQ_AFFINITY},
    {"IDENTITY", ZMQ_IDENTITY},
    {"SUBSCRIBE", ZMQ_SUBSCRIBE},
    {"UNSUBSCRIBE", ZMQ_UNSUBSCRIBE},
    {"RATE", ZMQ_RATE},
    {"RECOVERY_IVL", ZMQ_RECOVERY_IVL},
    {"SNDBUF", ZMQ_SNDBUF},
    {"RCVBUF", ZMQ_RCVBUF},
    {"RCVMORE", ZMQ_RCVMORE},
    {"FD", ZMQ_FD},
    {"EVENTS", ZMQ_EVENTS},
    {"TYPE", ZMQ_TYPE},
    {"LINGER", ZMQ_LINGER},
    {"RECONNECT_IVL", ZMQ_RECONNECT_IVL},
    {"BACKLOG", ZMQ_BACKLOG},
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
    {"QUEUE", ZMQ_QUEUE},
    {"FORWARDER", ZMQ_FORWARDER},
    {"STREAMER", ZMQ_STREAMER},
};
#define OPTTAB(k,v) xS[i]=ss(k);kK(y)[i]=ki(v)

K1(zmq){I n=tblsize(zmqopt);
    K y=ktn(0,n);x=ktn(KS,n);
    DO(tblsize(zmqopt), OPTTAB(zmqopt[i].k, zmqopt[i].v));
    R xD(x,y);}
