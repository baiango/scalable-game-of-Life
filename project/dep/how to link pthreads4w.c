// Debug -> "yourProjectName" Debug Properties
// Configuration Properties -> VC++ Directories
// Include Directories <- $(ProjectDir)dep\PTHREADS-BUILT\include
// Library Directories <- $(ProjectDir)dep\PTHREADS-BUILT\lib
// Linker -> Input
// Additional Dependencies <- libpthreadVC3d.lib
// (libpthreadVC3.lib for release version)

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	int threadNum;
	int arrSize;
} Args;

#define NUM_THREADS 4


void* ThreadFunction(void* arg)
{	Args *cmd = arg;
	printf("Thread %d is online!\n", cmd->threadNum);

	// To test if did load your CPU to 100%.
	//int x;
	//for (unsigned long long i = 0; i < 1'000'000'000'0ULL; i++)
	//	x = 1000 * 1000 % 100;

	for (int i = cmd->threadNum; i < cmd->arrSize; i += NUM_THREADS)
		printf("Thread %d print %d\n", cmd->threadNum, i);

	free(arg);
	return NULL; }


int main()
{	pthread_t threads[NUM_THREADS];
	int arrSize = 19;

	for (int i = 0; i < NUM_THREADS; i++)
	{	Args *cmd = malloc(sizeof(Args));
		if (cmd)
		{	cmd->threadNum = i;
			cmd->arrSize = arrSize; }
		else
		{	printf("cmd are not created\n");
			exit(-1); }
		pthread_create(&threads[i], NULL, ThreadFunction, cmd); }

	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);

	return 0; }
