#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define _MEMORY_LEAK_DETECTION_Info


static void *registeredPointer[1024];
static void _HScollectPointer(void *address)
{	printf("%p e\n", address);
	for (int i = 0; i < sizeof(registeredPointer) / sizeof(registeredPointer[0]); i++)
		if (registeredPointer[i] == NULL)
		{	registeredPointer[i] = address;
			return; }}


static void _HSfreeAllPointer()
{   for (int i = 0; i < sizeof(registeredPointer) / sizeof(registeredPointer[0]); i++)
		if (registeredPointer[i] != NULL)
		{	void *p = registeredPointer[i];
			#if defined(_MEMORY_LEAK_DETECTION_Info)
			printf("Trying to free %p\n", p);
			#endif
			free(p);
			registeredPointer[i] = NULL; }}


int main()
{	int *arr = malloc(10 * sizeof(int));
	arr[0] = 123;
	_HScollectPointer(arr);
	printf("%i a\n", arr[0]);
	printf("%p b\n", (void *)arr);

	for (int i = 0; i < 5; i++)
		printf("%p\n", registeredPointer[i]);
	_HSfreeAllPointer();
	printf("%i c\n", arr[0]);
	printf("%p d\n", (void *)arr);

	return 0; }
