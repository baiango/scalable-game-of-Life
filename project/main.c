#include "cdef.c"
#include "memmgr.c"
#include "engineu0.c"
#include "guimgr.c"
#include "overload.c"
#include <stdio.h>
// There's 3 important parts of the program.
// main.c as the controller.
// engineu0.c as the calculator or the "model".
// guimgr.c as the graphic interface manager or the "view".


const u64 DEFAULT_RNG_SEED = 1023;
const SDL_Color CHINESE_BLACK = { 0x12, 0x12, 0x12, 0xff };


i32 main()
{	Screen Screen = { NULL_WINDOW, NULL_RENDER, 640, 480 };
	GameDefine Gdef = { DEFAULT_RNG_SEED, 5, 1,
			{ Screen.sizeX / 5, Screen.sizeY / 1 } };
	const i32 arraySize = Gdef.gridSize.x * Gdef.gridSize.y;

	SDL_CreateWindowAndRenderer(Screen.sizeX, Screen.sizeY,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC,
			&Screen.Window, &Screen.Renderer);
	if (!Screen.Window)
		_OraiseError(Window);
	if (!Screen.Renderer)
		_OraiseError(Renderer);

	SDL_SetRenderDrawColor(Screen.Renderer, CHINESE_BLACK.r,
			CHINESE_BLACK.g, CHINESE_BLACK.b, 0xff);
	SDL_RenderClear(Screen.Renderer);
	SDL_RenderPresent(Screen.Renderer);

	// Please do not use vector<bool> if you port it in C++.
	// It will slow it down. Use vector<char> instead.
	// It might be because vector<bool> store in char(8-bit datatype),
	// and then decode as bool(1-bit datatype). which is bad.

	// C++ standard library tends to access as char or byte(8 bits pair).
	// Because of CPU designed to modify in bytes than bits.
	// (you can use this lost advantage to modify 8 bools at same times.)
	// They might be encode that bool in char for extra overheads.
	// It just way easier to encode bool into char than create a new
	// algorithm optimizing for.

	// Also, there's no performance difference between bool and char in C.
	// because it's optimized for bits rather than bytes.
	// Actually, idk why. I just made it up.
	#if defined(_MEMORY_LEAK_DETECTION_CRTDebug)
	u0* state = malloc(arraySize * sizeof(u8));
	_HScollectPointer(state);
	u0* swap = malloc(arraySize * sizeof(u8));
	_HScollectPointer(swap);
	#else
	u0 *state = _Smalloc(arraySize * sizeof(u8));
	u0 *swap = _Smalloc(arraySize * sizeof(u8));
	#endif
	//_HScollectPointer(state);

	{ // For auto deleting "n".
		u64 n = 1024;
		for (i32 i = 0; i < Gdef.gridSize.x * Gdef.gridSize.y; i++)
		{	n = _rngi(n);
			state[i] = n >> 63 ? true : false; }
	}

	GameEvent Evt = { NULL_EVENT, NOT_QUIT };
	while (!Evt.quit)
	{	// Controller
		_OprocessInput(&Evt, state, &Gdef);
		// Model
		_OprocessStateFull(state, swap, &Gdef);
		// View
		_OdrawGrid(&Screen, state, &Gdef);
		SDL_RenderPresent(Screen.Renderer); }

	SDL_Quit();
	_HSfreeAllPointer();
	#if defined(_MEMORY_LEAK_DETECTION_CRTDebug)
	_CrtDumpMemoryLeaks();
	#endif
	return 0; }
