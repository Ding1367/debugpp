#include <lauxlib.h>
#include <lualib.h>
#include <lua.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#define WINDOWS 1
#else
#include <unistd.h>
#define WINDOWS 0
#endif

int dbpp_archsize(lua_State *L){
    lua_pushinteger(L, sizeof(void*)*8);
    return 1;
}

int dbpp_iswindows(lua_State *L){
    lua_pushboolean(L, WINDOWS);
    return 1;
}

int dbpp_getregistry(lua_State *L){
    lua_pushvalue(L, LUA_REGISTRYINDEX);
    return 1;
}

int dbpp_getmetatable(lua_State *L){
    lua_getmetatable(L, 1);
    return 1;
}

int dbpp_getgmetatable(lua_State *L){
    luaL_getmetatable(L, lua_tostring(L, 1));
    return 1;
}

int dbpp_setmetatable(lua_State *L){
    lua_pushvalue(L, 2);
    lua_setmetatable(L, 1);
    return 1;
}

int dbpp_create(lua_State *L){
    lua_newuserdata(L, (size_t)luaL_checkinteger(L, 1));
    return 1;
}

int dbpp_readfrom(lua_State *L){
    int tt = lua_type(L, 1);
    if (tt == LUA_TUSERDATA || tt == LUA_TLIGHTUSERDATA)
        lua_pushinteger(L, *(char*)lua_touserdata(L, 1));
    else
        lua_pushnil(L);
    return 1;
}

int dbpp_writeto(lua_State *L){
    int tt = lua_type(L, 1);
    if (tt == LUA_TUSERDATA || tt == LUA_TLIGHTUSERDATA)
        lua_pushinteger(L, *(unsigned char*)lua_touserdata(L, 1) = luaL_checkinteger(L, 2) & 0xFF);
    else
        lua_pushnil(L);
    return 1;
}

int dbpp_getnum(lua_State *L){
    lua_pushinteger(L, (long long)lua_touserdata(L, 1));
    return 1;
}

int dbpp_getptr(lua_State *L){
    if (lua_isinteger(L, 1))
        lua_pushlightuserdata(L, (void*)lua_tointeger(L, 1));
    else
        lua_pushlightuserdata(L, (void*)lua_topointer(L, 1));
    return 1;
}

int dbpp_getfd(lua_State *L){
    lua_pushinteger(L, fileno(*(FILE**)luaL_checkudata(L, 1, "FILE*")));
    return 1;
}

#include <errno.h>

int dbpp_close(lua_State *L){
    if (WINDOWS || lua_isuserdata(L, 1)){
        if (fclose(*(FILE**)luaL_checkudata(L, 1, "FILE*")) == EOF)
            lua_pushstring(L, strerror(errno));
        else
            lua_pushnil(L);
        return 1;
    }
    if (close(luaL_checkinteger(L, 1)) == -1)
        lua_pushstring(L, strerror(errno));
    else
        lua_pushnil(L);
    return 1;
}

const luaL_Reg debugppfuncs[] = {
    {"archsize", dbpp_archsize},
    {"iswindows", dbpp_iswindows},
    {"getgmetatable", dbpp_getgmetatable},
    {"getmetatable", dbpp_getmetatable},
    {"setmetatable", dbpp_setmetatable},
    {"getregistry", dbpp_getregistry},
    {"create", dbpp_create},
    {"readfrom", dbpp_readfrom},
    {"writeto", dbpp_writeto},
    {"getnum", dbpp_getnum},
    {"getptr", dbpp_getptr},
    {"close", dbpp_close},
    {NULL, NULL}
};

int luaopen_debugpp(lua_State *L){
    luaL_newlib(L, debugppfuncs);
    return 1;
}
