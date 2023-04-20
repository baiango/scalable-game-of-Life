#pragma once
#include "cdef.c"


static void _OdrawGrid(Screen *Screen, u0 *state, GameDefine *Gdef)
{	const u16 blockSize = Gdef->blockSize;
	const u16 borderSize = Gdef->borderSize;
	const Vec2i gridSize = Gdef->gridSize;

	const SDL_Color Blue = { 17, 0, 80 };
	const SDL_Color White = { 255, 255, 255 };
	SDL_Rect Rect = { 0, 0, blockSize-borderSize, blockSize-borderSize };

	for (i32 x = 0; x < gridSize.x; x++)
		for (i32 y = 0; y < gridSize.y; y++)
		{	SDL_Color fill = state[y*gridSize.x+x] ? White : Blue;
			SDL_SetRenderDrawColor(Screen->Renderer, fill.r,
					fill.g, fill.b, 0xff);

			Rect.x = x * blockSize;
			Rect.y = y * blockSize;
			SDL_RenderFillRect(Screen->Renderer, &Rect); }}
