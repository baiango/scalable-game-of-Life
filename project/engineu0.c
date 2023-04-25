#pragma once
#include "cdef.c"
#include "memmgr.c"
//#include <immintrin.h> // avx2 todo


static u8 getNeighbourCount(u0 *state, u32 x, u32 y, u32 row, u32 col)
{	// % is modulo.
	// modulo formula is "a-floor(a/b)*b".
	// or "a-(a//b)*b" in python.

	// the formula for the top left is
	// "((y-1) * row + x-1 + arraySize) % arraySize".
	// or y * row + x just for array index.
	u8 ret = 0;
	const u32 oneDPosY = y * row;
	const u32 mov1Y = 1 * row;
	const u32 arraySize = row * col;
	// top left
	ret += state[(oneDPosY-mov1Y + x-1 + arraySize) % arraySize];
	ret += state[(oneDPosY-mov1Y + x   + arraySize) % arraySize];
	ret += state[(oneDPosY-mov1Y + x+1 + arraySize) % arraySize];
	// left
	ret += state[(oneDPosY       + x-1 + arraySize) % arraySize];
	ret += state[(oneDPosY       + x+1 + arraySize) % arraySize];
	// bottom left
	ret += state[(oneDPosY+mov1Y + x-1 + arraySize) % arraySize];
	ret += state[(oneDPosY+mov1Y + x   + arraySize) % arraySize];
	ret += state[(oneDPosY+mov1Y + x+1 + arraySize) % arraySize];
	return ret; }

/*
example:
#include <stdio.h>

int arrSize(int *x) { return sizeof(x) / sizeof((x)[0]); }

void prtIntArr(int *arr)
{	for (int i=0;i<=arrSize(arr);i++)
		printf("%i prtIntArr()\n", arr[i]); }

void prtPlus2(int *num) { printf("%i prtPlus2()\n", *num+2); }
void prt(int *num)
{	printf("%i prt()\n", *num);
	prtPlus2(num); }

int main()
{	int nArr[] = { 123, 234, 345 };
	int n = 987;
	int *p = &n;
	prtIntArr(nArr);
	prt(&n);
	prt(p); }

You pass anything with a pointer to functions.
Once you make a variable to pointer,
You don't have to make it into double pointer to pass in functions.
You just leave it as is and pass to another functions again.
*/
static void _OprocessStateFull(u0 *state, u0* swap, GameDefine *Gdef)
{	const i32 row = (*Gdef).gridSize.x;
	const i32 col = Gdef->gridSize.y;
	// hint: this is last line that Gdef was accessed in this function.
	// the function should be received "gridSize" instead to reduce scope.
	// but for the sake of learning arrow operator(->), I kept it.
	//  Gdef ->(.) gridSize ->(.) x
	//  (*)         ^
	//   ^          |
	//   |          |
	// *Gdef ->(->)
	// arrow operator(->) gives you indirect access to struct members.
	// if you use it from pointers.
	// otherwise, use the dot operator(.) to access directly.
	const i32 arraySize = row * col;
	for (i32 i = 0; i < arraySize; i++)
		swap[i] = 0;

	for (i32 x = 0; x < row; x++)
		for (i32 y = 0; y < col; y++)
		{	// 1 turn the 1nd switch on.                 0001
			// 2 turn the 2nd switch on.                 0010
			// 3 turn the 2nd and 1st switch on.         0011
			// 4 turn the 3rd switch on.                 0100
			// 5 turn the 3rd and 1st switch on.         0101
			// 6 turn the 3rd and 2nd switch on.         0110
			// 7 turn the 3rd and 2nd and 1st switch on. 0111
			// 8 turn the 4th switch on.                 1000
			// 9 turn the 4th and 1st switch on.         1001
			u8 ncout = getNeighbourCount(state, x, y, row, col);
			u32 oneDPos = y * row + x;
			// Any live cell with two or three live neighbours survives.
			// Any dead cell with three live neighbours becomes a live cell.
			// Coincidentally, it fits very well with bitwise OR(|) operators.
			// If bitwise OR(|) operators found one of the switch on...
			// Then turn on, otherwise turn off. for example:
			// 11100000 10001111 11101011 01101001 00000111 11111100 10001110 11011011
			// 10111110 00110100 10100100 01101011 11000010 11010010 11101100 10100111
			// Equals to:
			// 11111110 10111111 11101111 01101011 11000111 11111110 11101110 11111111
			// Alive if (state | ncout = 3) else Dead
			// If dead  and 3 neighbours (00 | 11=11)
			// If alive and 3 neighbours (01 | 11=11)
			// If alive and 2 neighbours (01 | 10=11)
			swap[oneDPos] = (state[oneDPos] | ncout) == 0b11; }
/*
# Try this in Python!
r = 1023
print(bin(r)[2:])
a = r * 0xff37f1f758180525 & 0xffffffffffffffff
print(bin(a)[2:])
b = a * 0xff37f1f758180525 & 0xffffffffffffffff
print(bin(b)[2:])
print(bin(a | b)[2:])
*/	/*
	C:
	static u0 *_processStateFull(u0 *state, GameDefine *Gdef)
	{	u0 *swap = malloc(arraySize * sizeof(u8));
		return swap; }
	
	int init()
	{	u0 *state = malloc(arraySize * sizeof(u8));
		state = _processStateFull(state, &Gdef); }

	// You need to free the "swap". But you can't free "swap" before returning it.
	// So I found a way to deal with it.
	// You just paste the swap to state to prevent memory leaks.
	C:
	static u0 *_OprocessStateFull(u0 *state, u0 *swap, GameDefine *Gdef)
	{	for (i32 i = 0; i < arraySize; i++)
			state[i] = swap[i];
		return swap; }

	int init()
	{	u0 *state = malloc(arraySize * sizeof(u8));
		u0 *swap = malloc(arraySize * sizeof(u8));
		_OprocessStateFull(state, swap, &Gdef); }
	*/
	for (i32 i = 0; i < arraySize; i++)
		state[i] = swap[i]; }


static u8 _processStateSingle(u0 *state, u32 x, u32 y, u32 row, u32 col)
{	u8 ncout = getNeighbourCount(state, x, y, row, col);
	return (state[y * row + x] | ncout) == 0b11; }


static u64 _rngi(u64 RngSeed) { return RngSeed * 0xff37'f1f7'5818'0525; }


static void _OprocessKeyDown(GameEvent *Evt, u0 *state, GameDefine *Gdef)
{	const Vec2i gridSize = Gdef->gridSize;
	u64 *rngSeed = &Gdef->rngSeed;

	switch (Evt->Event.key.keysym.sym)
	case SDLK_F5:
		for (i32 i = 0; i < gridSize.x * gridSize.y; i++)
		{	*rngSeed = _rngi(*rngSeed);
			state[i] = *rngSeed >> 63; }}
			// Surprisingly, "& 1" bitwise AND
			// doesn't work well with this RNG. and "% 2" modulo too!
			// ">>" is bitwise right shift.
			// "<<" is bitwise left shift.
			// 10011101 = 157
			// if you do ">> 2", it removes the last 2 numbers.
			// 100111   = 39
			// Now if you do "<< 2", it just add 2 zeros to the back.
			// 10011100 = 156
			/*
# Try this in Python!
print(0b10011101)
print(bin(0b10011101))
a = 0b10011101 >> 2
print(a)
print(bin(a))
b = 0b100111 << 2
print(b)
print(bin(b))
			*/


static void _OprocessInput(GameEvent *Evt, u0 *state, GameDefine *Gdef)
{	while (SDL_PollEvent(&Evt->Event))
	{	switch (Evt->Event.type)
		{case SDL_QUIT:
			Evt->quit = true;
		case SDL_KEYDOWN:
			_OprocessKeyDown(Evt, state, Gdef); }}}
