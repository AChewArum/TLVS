#pragma once

#ifdef AJW_KEYBOARDDLL
	#define AJW_KEYBOARD_DLLEXPORT __declspec(dllexport)
#else
	#define AJW_KEYBOARD_DLLEXPORT __declspec(dllimport)
#endif