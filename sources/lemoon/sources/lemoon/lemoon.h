/*!
 * \file abi.h
 * \date 2014/04/29 14:39
 *
 * \author yayan
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef LEMOON_H
#define LEMOON_H
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
#ifdef __cplusplus
}
#endif //__cplusplus

#include <lemoon/configure.h>


//////////////////////////////////////////////////////////////////////////
#define LEMOON_MODULE                               "lemoon"
#define LEMOON_TIMER                                "timer"
#define LEMOON_IO                                   "io"
#define LEMOON_SOCK                                 "sock"
#define LEMOON_SOCKADDR                             "sockaddr"
#define LEMOON_FILE                                 "file"
#define LEMOON_FS                                   "fs"
#define LEMOON_REG(N)                               LEMOON_MODULE "." N



//////////////////////////////////////////////////////////////////////////
//lemoon core APIs

/*
 * open the lemoon lua c extend library
 */
LEMOON_API int luaopen_lemoon(lua_State *L);

LEMOON_API void lemoon_newio(lua_State *L);
LEMOON_API void lemoon_dispatch(lua_State *L,int index,size_t timeout);
LEMOON_API void lemoon_newtimewheel(lua_State *L,int millisecondsOfTick);
LEMOON_API void lemoon_tick(lua_State *L,int index);
LEMOON_API void lemoon_timeout(lua_State *L, int index,size_t timeout);
LEMOON_API void lemoon_getaddrinfo(lua_State *L,const char* host, const char* service, int af, int type, int flags);
LEMOON_API void lemoon_pushsockaddr(lua_State *L, struct sockaddr * addr,size_t addrlen);
LEMOON_API struct sockaddr* lemoonL_testsockaddr(lua_State *L, int index, size_t *len);
LEMOON_API struct sockaddr* lemoon_tosockaddr(lua_State *L, int index,size_t *len);
//socket APIs
LEMOON_API void lemoon_newsock(lua_State *L, int index, int domain, int type, int protocol);
LEMOON_API void lemoon_closesock(lua_State *L);
LEMOON_API void lemoon_bind(lua_State *L, int index, struct sockaddr * addr, size_t addrlen);
LEMOON_API void lemoon_listen(lua_State *L, int index);
LEMOON_API void lemoon_accept(lua_State *L, int index);
LEMOON_API void lemoon_connect(lua_State *L, int index, struct sockaddr * addr, size_t addrlen);
LEMOON_API void lemoon_send(lua_State *L,int sock, int func, const char * buff, size_t len, int flags, int timeout);
LEMOON_API void lemoon_recv(lua_State *L, int sock, int func,size_t len, int flags, int timeout);
LEMOON_API void lemoon_recv_some(lua_State *L, int sock, int func, size_t len, int flags, int timeout);
LEMOON_API void lemoon_sendto(lua_State *L, int sock, int func, const char * buff, size_t len, struct sockaddr * addr, size_t addrlen,int flags, int timeout);
LEMOON_API void lemoon_recvfrom(lua_State *L, int sock, int func, size_t len, int flags, int timeout);
//file APIs
LEMOON_API void lemoon_newfile(lua_State *L, const char * path, const char* mode);
LEMOON_API void lemoon_closefile();
LEMOON_API void lemoon_write(lua_State *L, int fd, int func, const char * buff, size_t len, int timeout);
LEMOON_API void lemoon_read(lua_State *L, int fd, int func, size_t len, int timeout);
LEMOON_API void lemoon_read_some(lua_State *L, int sock, int func, size_t len, int timeout);

//Debug APIs
LEMOON_API int lemoon_error(lua_State *L,const char * file, int lines, const char* msg, ...);
LEMOON_API void lemoon_pushsysmerror(lua_State *L,int errcode, const char * file, int lines, const char* msg,...);
LEMOON_API int lemoon_sysmerror(lua_State *L, int errcode, const char * file, int lines, const char* msg, ...);



#define lemoonL_error(L,msg,...) lemoon_error(L,__FILE__,__LINE__,msg,__VA_ARGS__)
#define lemoonL_pushsysmerror(L,errcode,msg,...) lemoon_pushsysmerror(L,errcode,__FILE__,__LINE__,msg,__VA_ARGS__)
#define lemoonL_sysmerror(L,errcode,msg,...) lemoon_sysmerror(L,errcode,__FILE__,__LINE__,msg,__VA_ARGS__)

#endif // LEMOON_H