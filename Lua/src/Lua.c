#pragma once
#include <stdio.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "Player.h"

// Function to call from lua
int lua_CallMe(lua_State* L) {
	float a = (float)lua_tonumber(L, 1);
	float b = (float)lua_tonumber(L, 2);

	printf("[C] CallMe(%f, %f) was called\n", a, b);

	float result = a * b + 10;
	lua_pushnumber(L, result);

	return 1;
}

int main() {
	// Init a lua virtual machine
	lua_State* L = luaL_newstate();
	// Allow usage of simple libs
	luaL_openlibs(L);

	Player player;

	// execute a lua script file
	int r = luaL_dofile(L, "config.lua");

	// checks for errors
	if (r == LUA_OK) {
		// if found - puts global a the stack top
		lua_getglobal(L, "a");
		// check if can be converted to number
		if (lua_isnumber(L, -1)) {
			int a_in_cpp = (int)lua_tonumber(L, -1);
			printf("a_in_cpp = %d\n", a_in_cpp);
		}

		lua_getglobal(L, "player");
		if (lua_istable(L, -1)) {
			// push the key to search for in the table
			lua_pushstring(L, "Name");
			// table is now bellow the key
			lua_gettable(L, -2);
			strcpy_s(player.name, sizeof(player.name), lua_tostring(L, -1));
			// align back the stack 
			lua_pop(L, 1);

			lua_pushstring(L, "Age");
			lua_gettable(L, -2);
			player.age = lua_tointeger(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "Level");
			lua_gettable(L, -2);
			player.level = lua_tointeger(L, -1);
			lua_pop(L, 1);
		}

		printf("Player {Name = %s, Age = %d, Level = %d\n", player.name, player.age, player.level);

		lua_getglobal(L, "AddStuff");
		if (lua_isfunction(L, -1)) {
			// push function params
			lua_pushnumber(L, 1.3f);
			lua_pushnumber(L, 4.4f);

			// call function with two parameters and one returned value
			lua_pcall(L, 2, 1, 0);
			printf("[C] called a function from lua -> got %f in return\n", (float)lua_tonumber(L, -1));
		}

		// connect local C function to a name in lua
		lua_register(L, "HostFunction", lua_CallMe);
		lua_getglobal(L, "HostCallMe");
		if (lua_isfunction(L, -1)) {
			lua_pushnumber(L, 1.3f);
			lua_pushnumber(L, 4.4f);

			lua_pcall(L, 2, 1, 0);
			printf("[C] called a function from lua -> got %f in return\n", (float)lua_tonumber(L, -1));
		}
	}
	else {
		// get errors from stack top
		char* error_msg = lua_tostring(L, -1);
		printf("%s\n", error_msg);
	}

	return 0;
}