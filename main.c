#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <unistd.h>
#include "sinterp.c"

int main(int argc, char** argv) {

	if (argc <= 2) {
		// todo print usage
		return 1;
	}

	if (argv[1][0] == 'c') {

		char cwd[1024];
        lua_State* L;

		getcwd(cwd, sizeof(cwd));

        L = luaL_newstate();
        luaL_openlibs(L);
        if (luaL_dofile(L, "/usr/local/share/spyre/spyre.lua")) {
            printf("\nSPYRE ERROR: could not run compiler (spyre.lua)\n\n");
            return 1;
        }
        // push entry point
        lua_getglobal(L, "main");
        // push file to be compiled
        lua_pushstring(L, argv[2]);
		// push file name
		lua_pushstring(L, argv[3]);
		// push cur dir
		lua_pushstring(L, cwd);
        // call entry point

        if (lua_pcall(L, 3, 0, 0)) {
            printf("\nSPYRE ERROR: compiler lua error:\n");
            printf("\t%s\n\n", lua_tostring(L, -1));
        }

    } else if (argv[1][0] == 'r') {
        spy_state* S;

        S = spy_newstate();
        spy_executeBinaryFile(S, argv[2]);

    }

    return 0;
}
