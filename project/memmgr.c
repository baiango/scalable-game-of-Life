#pragma once
// It can't show call stacks.
// So try not to warp them into a functions like _Smalloc()
/* example to get around call stack issue:
#define _MEMORY_LEAK_DETECTION_CRTDebug

#if defined(_MEMORY_LEAK_DETECTION_CRTDebug)
u0* swap = malloc(arraySize * sizeof(u8));
#else
u0* swap = _Smalloc(arraySize * sizeof(u8));
#endif
*/
// SDL2 will leak 48 bytes per frame.
// If you leave the game for full day, it will leak 237.3 MiB.
// (48*60 * 60*60*24 / 1024 / 1024)
// memory leak detection
// It will switch _Smalloc() with malloc() to get rid call stack.
#define _MEMORY_LEAK_DETECTION_CRTDebug
//#define _MEMORY_LEAK_DETECTION_Info

#if defined(_MEMORY_LEAK_DETECTION_CRTDebug)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


enum { Undef, OOM, Window, Renderer };
static void _OraiseError(i32 errorCode)
{	switch (errorCode)
	{case Undef:
	{	printf("Undefined error! Quitting now...");
		break; }
	case OOM:
	{	printf("Out of memory! Quitting now...");
		break; }
	case Window:
	{	printf("Window not created! Quitting now...");
		break; }
	case Renderer:
	{	printf("Renderer not created! Quitting now...");
		break; }}

	SDL_Quit();
	#if defined(_MEMORY_LEAK_DETECTION_CRTDebug)
	_CrtDumpMemoryLeaks();
	#endif
	exit(EXIT_FAILURE); }


static void *registeredPointer[1024];
static void _HScollectPointer(void *address)
{	for (int i = 0; i < sizeof(registeredPointer) / sizeof(registeredPointer[0]); i++)
		if (registeredPointer[i] == NULL)
		{	registeredPointer[i] = address;
			return; }}


static void _HSfreeAllPointer()
{	for (int i = 0; i < sizeof(registeredPointer) / sizeof(registeredPointer[0]); i++)
		if (registeredPointer[i] != NULL)
		{	void *p = registeredPointer[i];
			#if defined(_MEMORY_LEAK_DETECTION_Info)
			printf("Trying to free %p.\n", p);
			#endif
			free(p);
			registeredPointer[i] = NULL; }}


static void *_Smalloc(u64 size)
{	void *mem = malloc(size);
	if (!mem)
		_OraiseError(OOM);
	_HScollectPointer(mem);
	#if defined(_MEMORY_LEAK_DETECTION_Info) 
	printf("_Smalloc allocated %p with %i bytes.\n", mem, size);
	#endif
	return mem; }
