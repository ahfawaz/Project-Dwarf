// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//STL
#include <string>
#include <vector>
#include <map>
#include <d3d11_2.h>
#include <DirectXMath.h>

//Header Includes
//#include "../Utilities/ErrorChecking.h"
#include "../Utilities/TimeManager.h"

//Namespaces?
using namespace std;
using namespace DirectX;

//Tell it how many threads are involved so it knows if it's multithreaded or not
#define MULTITHREADED 1

#define SAFE_RELEASE(x) if(x) x->Release(); x = nullptr;
#define SAFE_DELETE(x) if(x) delete x; x = nullptr;
#define SAFE_SHUTDOWN(x) if(x) {x->Shutdown(); delete x;} x = nullptr;

#define MAX_DEPTH 1.0F