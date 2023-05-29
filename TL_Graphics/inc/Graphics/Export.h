#pragma once


#ifdef AJW_GRAPHICS

#define AJW_GRAPHICS_DLLEXPORT __declspec(dllexport)

#else

//#pragma comment (lib, AJW_GRAPHICS_LIBNAME)
#define AJW_GRAPHICS_DLLEXPORT __declspec(dllimport)

#endif
