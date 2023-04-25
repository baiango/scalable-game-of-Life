#pragma once
#include "cdef.c"


typedef enum {
	Tu0, Tu8, Tu16, Tu32, Tu64,
	Ti8, Ti16, Ti32, Ti64,
	Tf32, Tf64,
	TVec2, TVec2i
} type;

typedef struct {
	type type;
	union {
		u0 u0; u8 u8; u16 u16; u32 u32; u64 u64;
		i8 i8; i16 i16; i32 i32; i64 i64;
		f32 f32; f64 f64;
		Vec2 vec2; Vec2i vec2i;
	} uni;
} var;
