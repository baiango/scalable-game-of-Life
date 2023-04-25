#pragma once
#include "cdef.c"
#include "memmgr.c"
#include "engineu0.c"
#include <pthread.h>
#include <stdlib.h>


// Reference
/*
static void _OprocessStateFullmt(u0 *state, u0* swap, GameDefine *Gdef)
{	const i32 row = Gdef->gridSize.x;
	const i32 col = Gdef->gridSize.y;

	const i32 arrSize = row * col;
	for (i32 i = 0; i < arrSize; i++)
		swap[i] = 0;

	i32 *threadID = malloc(sizeof(i32));
	for (i32 x = 0; x < row; x++)
		for (i32 y = 0; y < col; y++)
			swap[y*row + x] = _processStateSingle(state, x, y, row, col);

	for (i32 i = 0; i < arrSize; i++)
		state[i] = swap[i]; }
*/


typedef struct {
	i32 threadNum;
	pthread_mutex_t *mtx;
	u0 *state;
	u0 *swap;
	i32 row;
	i32 col;
} Args;


static u8 processStateSingle(u0 *state, u32 x, u32 y, u32 row, u32 col)
{	u8 ncout = _getNeighbourCount(state, x, y, row, col);
	return (state[y * row + x] | ncout) == 0b11; }


#define NUMBER_OF_THREADS 4


static void *HthreadProcess(void *arg)
{	Args *cmd = arg;
	i32 row = cmd->row;
	i32 col = cmd->col;
	i32 arrSize = cmd->row * cmd->col;

	//printf("Thread %d is online from HthreadProcess()!\n", cmd->threadNum);
	for (i32 i = cmd->threadNum; i < arrSize; i += NUMBER_OF_THREADS)
	{	cmd->swap[i] = processStateSingle(cmd->state, i%row, i/row,
				row, col); }

	free(arg);
	return NULL; }


static void *HmemCp(void *arg)
{	Args *cmd = arg;
	i32 arrSize = cmd->row * cmd->col;

	//printf("Thread %d is online from HmemCp()!\n", cmd->threadNum);
	for (i32 i = cmd->threadNum; i < arrSize; i += NUMBER_OF_THREADS)
		cmd->state[i] = cmd->swap[i];

	free(arg);
	return NULL; }


// It's 33.3% slower than single threaded.
// It's too easy for cores to compute! That's why it slowed down.
// Because cores spend more time waiting than computing.
static void _OprocessStateFullMT(u0* state, u0* swap, GameDefine* Gdef)
{	i32 row = Gdef->gridSize.x;
	i32 col = Gdef->gridSize.y;
	
	pthread_t threads[NUMBER_OF_THREADS];

	for (i32 i = 0; i < NUMBER_OF_THREADS; i++)
	{	Args *cmd = malloc(sizeof(Args));
		if (cmd)
		{	cmd->threadNum = i;
			cmd->state = state;
			cmd->swap = swap;
			cmd->row = row;
			cmd->col = col; }
		else
			_OraiseError(OOM);
		pthread_create(&threads[i], NULL, HthreadProcess, cmd); }

	for (i32 i = 0; i < NUMBER_OF_THREADS; i++)
		pthread_join(threads[i], NULL);
	
	for (i32 i = 0; i < NUMBER_OF_THREADS; i++)
	{	Args *cmd = malloc(sizeof(Args));
		if (cmd)
		{	cmd->threadNum = i;
			cmd->state = state;
			cmd->swap = swap;
			cmd->row = row;
			cmd->col = col; }
		else
			_OraiseError(OOM);
		pthread_create(&threads[i], NULL, HmemCp, cmd); }
	
	for (i32 i = 0; i < NUMBER_OF_THREADS; i++)
		pthread_join(threads[i], NULL); }
