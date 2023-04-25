#pragma once
// It can't show call stacks.
// So try not to warp them into a functions like _Smalloc()
/* example to get around call stack issue:
#define _MEMORY_LEAK_DETECTION_CRTDebug

#if defined(_MEMORY_LEAK_DETECTION_CRTDebug)
u0* swap = malloc(arrSize * sizeof(u8));
#else
u0* swap = _Smalloc(arrSize * sizeof(u8));
#endif
*/
// SDL2 will leak 48 bytes per frame.
// If you leave the game for full day, it will leak 237.3 MiB.
// (48*60 * 60*60*24 / 1024 / 1024)
// memory leak detection
// It will switch _Smalloc() with malloc() to get rid call stack.
//#define _MEMORY_LEAK_DETECTION_CRTDebug
#define _MEMORY_LEAK_DETECTION_Info
#define SHOW_ERROR_MESSAGE

#if defined(_MEMORY_LEAK_DETECTION_CRTDebug)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


enum { Undef, OOM, Window, Renderer, Thread, Mutex };
static void _OraiseError(i32 errorCode)
{	switch (errorCode)
	{case Undef:
	{	printf("E001\n");
		#if defined(SHOW_ERROR_MESSAGE)
		printf("\tUndefined error! ");
		#endif
		break; }
	case OOM:
	{	printf("E002\n");
		#if defined(SHOW_ERROR_MESSAGE)
		printf("\tOut of memory! ");
		#endif
		break; }
	case Window:
	{	printf("E003\n");
		#if defined(SHOW_ERROR_MESSAGE)
		printf("\tWindow not created! ");
		#endif
		break; }
	case Renderer:
	{	printf("E004\n");
		#if defined(SHOW_ERROR_MESSAGE)
		printf("\tRenderer not created! ");
		#endif
		break; }
	case Thread:
	{	printf("E005\n");
		#if defined(SHOW_ERROR_MESSAGE)
		printf("\tThread are not created! ");
		#endif
		break; }
	case Mutex:
	{	printf("E006\n");
		#if defined(SHOW_ERROR_MESSAGE)
		printf("\tMutex are not initalized! ");
		#endif
		break; }}

	#if defined(SHOW_ERROR_MESSAGE)
	printf("Quitting now...\n");
	#endif
	SDL_Quit();
	#if defined(_MEMORY_LEAK_DETECTION_CRTDebug)
	_CrtDumpMemoryLeaks();
	#endif
	exit(EXIT_FAILURE); }


//static void *ptrArr[POINTER_ARRAY_SIZE];
#define POINTER_ARRAY_SIZE 1024 // Still better than exposed global arrays!
static void _OScollectPointer(void **ptrArr, void *address)
{	for (int i = 0; i < POINTER_ARRAY_SIZE; i++)
		if (ptrArr[i] == NULL)
		{	ptrArr[i] = address;
			#if defined(_MEMORY_LEAK_DETECTION_Info)
			printf("Added %p to unknown 'void *arr[]' array.\n", ptrArr[i]);
			#endif
			return; }}


static void _OSfreeAllPointer(void **ptrArr)
{	for (int i = 0; i < POINTER_ARRAY_SIZE; i++)
		if (ptrArr[i] != NULL)
		{	void *p = ptrArr[i];
			#if defined(_MEMORY_LEAK_DETECTION_Info)
			printf("Trying to free %p.\n", p);
			#endif
			free(p);
			ptrArr[i] = NULL; }}


static void *_Smalloc(void **ptrArr, u64 size)
{	void *mem = malloc(size);
	if (!mem)
		_OraiseError(OOM);
	_OScollectPointer(ptrArr, mem);
	#if defined(_MEMORY_LEAK_DETECTION_Info)
	printf("_Smalloc allocated %p with %lli bytes.\n", mem, size);
	#endif
	return mem; }
