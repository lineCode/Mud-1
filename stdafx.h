// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <string>
#include <map>
#include <vector>
#include <deque>
#include <iostream>
#include <list>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <sys/timeb.h>
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <process.h>
#include <mysql++.h>
#define ZLIB_WINAPI
#include "zlib.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#define SOL_CHECK_ARGUMENTS 1
#define SOL_USING_CXX_LUA
#include <sol.hpp>


// TODO: reference additional headers your program requires here
