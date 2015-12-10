/*
Test1: Set each element to a random integer.
Test2: First half of the array elements equal to 2; second half equal to 1.
Test3: Split the array in fifths and set the elements in each fifth equal to 5,1,4,2,and3, respectively
Test4: Split the array in fifths and set the elements in each fifth equal to 5,4,3,2,and1, respectively
Test5: Set all 50,000 elements equal to 1.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys\timeb.h>
#include <conio.h>

const int ARRAY_LEN = 50000;
int array[ARRAY_LEN];

int elapsed(timeb s, timeb e)
{
	// time: s; millitm: ms
	return (int)((1000 * e.time + e.millitm) - (1000 * s.time + s.millitm));
}

int cmp(const void *i, const void *j) 
{
	return *(int *)i - *(int *)j;
}

void main()
{
	int i, j;
	int totaltime = 0;
	int ARRAY_MID = ARRAY_LEN / 2;
	timeb start, end;

	srand(time(NULL));

	/* ---------------- Test1 -------------- */
	
	for (j = 0; j < 100; j++)	
	{
		for (i = 0; i < ARRAY_LEN; i++)
			array[i] = rand();
		ftime(&start);
		qsort(array, ARRAY_LEN, sizeof(int), cmp);
		ftime(&end);
		totaltime += elapsed(start, end);
	}
	printf("Test 1: %d\n", (totaltime/100)); // average runtime(for 100 runs)

	/* ---------------- Test2 -------------- */

	for (i = 0; i < ARRAY_MID; i++)
		array[i] = 2;
	for (i = ARRAY_MID; i < ARRAY_LEN; i++)
		array[i] = 1;
	ftime(&start);
	qsort(array, ARRAY_LEN, sizeof(array[0]), cmp);
	ftime(&end);
	printf("Test 2: %d\n", elapsed(start, end));

	/* ---------------- Test3 -------------- */

	for (i = 0; i < 10000; i++) array[i] = 5;
	for (i = 10000; i < 20000; i++) array[i] = 1;
	for (i = 20000; i < 30000; i++) array[i] = 4;
	for (i = 30000; i < 40000; i++) array[i] = 2;
	for (i = 40000; i < 50000; i++) array[i] = 3;
	ftime(&start);
	qsort(array, ARRAY_LEN, sizeof(array[0]), cmp);
	ftime(&end);
	printf("Test 3: %d\n", elapsed(start, end));

	/* ---------------- Test4 -------------- */

	for (i = 0; i < 10000; i++) array[i] = 5;
	for (i = 10000; i < 20000; i++) array[i] = 4;
	for (i = 20000; i < 30000; i++) array[i] = 3;
	for (i = 30000; i < 40000; i++) array[i] = 2;
	for (i = 40000; i < 50000; i++) array[i] = 1;
	ftime(&start);
	qsort(array, ARRAY_LEN, sizeof(array[0]), cmp);
	ftime(&end);
	printf("Test 4: %d\n", elapsed(start, end));

	/* ---------------- Test5 -------------- */

	for (i = 0; i < ARRAY_LEN; i++)
		array[i] = 1;
	ftime(&start);
	qsort(array, ARRAY_LEN, sizeof(array[0]), cmp);
	ftime(&end);
	printf("Test 5: %d\n", elapsed(start, end));

	getch();
}


