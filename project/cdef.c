#pragma once
#include <stdbool.h>


typedef bool u0; typedef unsigned char u8; typedef unsigned short u16;
typedef unsigned u32; typedef unsigned long long u64;
typedef char i8; typedef short i16; typedef int i32; typedef long long i64;
typedef float f32; typedef double f64;
// leaving out "typedef long double f80;" because they are extremely slow.
// and does not play well with bitwise operators as they are not in power of 2.

typedef struct { f32 x, y; } Vec2;
typedef struct { i32 x, y; } Vec2i;

// SDL2
#include <SDL2/SDL.h>
#undef main


static const void *NULL_WINDOW = NULL, *NULL_RENDER = NULL;
static const SDL_Event *NULL_EVENT = NULL;
static const u0 NOT_QUIT = false;


typedef struct {
	SDL_Window* Window;
	SDL_Renderer* Renderer;
	i32 sizeX;
	i32 sizeY;
} Screen;

typedef struct {
	SDL_Event Event;
	u0 quit;
	u64 totalFrame;
	u64 lastRecordedframe;
	u64 prtTpsNextTimeout;
} GameEvent;

typedef struct {
	u64 rngSeed;
	u16 blockSize;
	u16 borderSize;
	Vec2i gridSize;
} GameDefine;
