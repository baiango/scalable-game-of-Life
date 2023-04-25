This is sorted in alphabetical order.
cdef.c:
// SDL2
#include <SDL2/SDL.h>
#undef main
+//#include <SDL2/SDL_opengl.h>


engineu0.c:
+//static u8 getStateChanges(u8* currentState, u8* previousState)
+//{	// 00000    00100            00100
+//	// 01110 -> 00100 XOR (^) -> 01010
+//	// 00000    00100            00100
+//	// If XOR operator found 1 and 0, in other words, not the same.
+//	// turn it on. otherwise, turn off.
+//
+//}


static u8 getNeighbourCount(u0 *state, u32 x, u32 y, u32 row, u32 col)


static u0 *_processStateFull(u0 *state, GameDefine *Gdef)
{	// with memory leak, there's no easy way to deal with it.
	const i32 row = (*Gdef).gridSize.x;
	const i32 col = Gdef->gridSize.y;
	const i32 arraySize = row * col;
	
	u0 *swap = malloc(arraySize * sizeof(u8));
	_HScollectPointer(swap);

	for (i32 i = 0; i < arraySize; i++)
		swap[i] = 0;

	for (i32 x = 0; x < row; x++)
		for (i32 y = 0; y < col; y++)
		{	u8 ncout = getNeighbourCount(state, x, y, row, col);
			u32 oneDPos = y * row + x;
			swap[oneDPos] = (state[oneDPos] | ncout) == 0b11; }

	return swap; }


static u8 _processStateSingle(u0 *state, u32 x, u32 y, u32 row, u32 col)


+//#include "dep/c11threads/c11threads_win32.c"
+static void ThreadProc(void *param)
+{	int h=*((int*)param);
+	i32 n;
+	for (i32 i = 0; i < 65536; i++)
+		n = i % 7;
+	printf("%d Thread is Running!\n",h);
+	_endthread(); }
+
+
+// static u8 *_processStateMt(u0 *state, u32 row, u32 col)
+// {	
+// 
+// }


static u64 _rngi(u64 RngSeed) { return RngSeed * 0xff37f1f758180525; }


gui.c:
main.c:
	{ // For auto deleting "n".
		u64 n = 1024;
		for (i32 i = 0; i < Gdef.gridSize.x * Gdef.gridSize.y; i++)
		{	n = _rngi(n);
			state[i] = n >> 63 ? true : false; }
	}

+	// For testing new codes.
+	//_processStateMt(state, Gdef.gridSize.y, Gdef.gridSize.y);
+	//var x;
+	//_OvarSet(&x, "12");
+	//_printfOverload(&x);


memmgr.c:
// It's very hard!
-static void **registeredPointer[1024];
-static void _HScollectPointer(void *address)
-{	for (i32 i = 0; i < sizeof(u64) / sizeof(registeredPointer[0]); i++)
-		if ((void *)registeredPointer[i] == 0)
-		{	*registeredPointer[i] = &address;
-			return; }}


-static void _HSfreeAllPointer()
-{	for (i32 i = 0; i < sizeof(u64) / sizeof(registeredPointer[0]); i++)
-		if ((void*)registeredPointer[i] != 0)
-		{	void **p = registeredPointer[i];
-			#if defined(_MEMORY_LEAK_DETECTION_Info)
-			printf("Trying to free %p!\n", *p);
-			#endif
-			free(*p);
-			*registeredPointer[i] = NULL; }}


static void *_Smalloc(u64 size)


overload.c:
typedef struct {
	union {
	} uni;
} var;


+//static void _Oseti32(var* variant, i32 n)
+//{	variant->uni.i32 = n;
+//	variant->type = Ti32; }
+
+//static void _OvarSet(var *variant, char *evaluate[])
+//{
+//	char* str;
+//	//strtod()
+//	(strtol(evaluate, str, 10)); // this is for int.
+//}


+static void _printfOverload(var *variant)
+{	switch (variant->type)
+	{case Ti32:
+		printf("%d\n", variant->uni.i32);
+		break;
+	case Tf32:
+		printf("%f\n", variant->uni.f32);
+		break;
+	case Ti8:
+		printf("%c\n", variant->uni.i8);
+		break; }}
