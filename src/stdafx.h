// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
#include <SDKDDKVer.h>

#include "json.hpp"

#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <memory>
#include <list>
#include <random>
#include <deque>
#include <bitset>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <sys/timeb.h>
#include <time.h>
#include <process.h>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#define SOL_CHECK_ARGUMENTS
#define SOL_PRINT_ERRORS
#include <sol.hpp>

#define ZLIB_WINAPI
#include "zlib.h"