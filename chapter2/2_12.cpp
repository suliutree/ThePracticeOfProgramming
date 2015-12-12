/*
I sorted an array of 1,000,000 integers using the binary search tree method, the
standard library qsort() function provided by Visual Stdio 2010, and the PoP 
quicksort. The sort times were:
Binary search tree: 3119 ms
Library Quicksort: 500 ms
PoP BOOK Quicksort: 473 ms

The essential parts of the binary search tree sort are:
Lookup from original array: O(1)
Insert into binary search tree: O(logn) for each element, multiplied by n elements = O(nlogn)
Inorder traversal of binary search tree: O(n)
Insert into array: O(1)

So the overall complexity is O(n*n*logn), which is not good.
In the code below, I did not free the tree after the sort completed. This would be
an O(n) operation, making the overall sort be O(n*n*n*logn).
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys\timeb.h>
#include <conio.h>

const int ARRAY_LEN = 1000000;
int array1[ARRAY_LEN], array2[ARRAY_LEN], array3[ARRAY_LEN];
// arrays are global to avoid stack overflow

typedef struct intp intp;
struct intp {
	int data;
	intp *left;
	intp *right;
};

int cmp(const void* a, const void* b)
{
	return (*(int *)a - *(int *)b);
}

void swap(int v[], int i, int j)
{
	int temp;
	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

int elapsed(timeb s, timeb e)
{
	return (int)((1000 * e.time + e.millitm) - (1000 * s.time + s.millitm));
}

intp *insert(intp *treep, int newint)
{
	intp* newnode;
	if (treep == NULL)
	{
		newnode = (intp*)malloc(sizeof(intp));
		if (newnode != NULL) {
			newnode->data = newint;
			newnode->left = NULL;
			newnode->right = NULL;
			return newnode;
		}
		else
			return NULL;
	}
	if (newint <= treep->data)
		treep->left = insert(treep->left, newint);
	else
		treep->right = insert(treep->right, newint);

	return treep;
}

void traverseinorder(intp* treep, int *sortedarray, int* index)
{
	if (treep == NULL)
		return;
	traverseinorder(treep->left, sortedarray, index);
	sortedarray[(*index)++] = treep->data;
	traverseinorder(treep->right, sortedarray, index);
}

void quicksort(int v[], int n)
{
	int i, last;
	if (n <= 1)
		return;
	swap(v, 0, rand() % n);
	last = 0;
	for (i = 1; i < n; i++)
		if (v[i] < v[0])
			swap(v, ++last, i);
	swap(v, 0, last);
	quicksort(v, last);
	quicksort(v+last+1, n-last-1);
}

int main()
{
	int i;
	int dummy = 0;
	timeb start, end;

	srand(time(NULL));								// Reseed the random number generator

	for (i = 0; i < ARRAY_LEN; i++)
		array1[i] = array2[i] = array3[i] = rand();	// Populate test array

	/* ------------ Sort using binary search tree method -------------- */

	ftime(&start);
	intp* head = insert(NULL, array1[0]);
	for (i = 0; i < ARRAY_LEN; ++i)
		insert(head, array1[i]);
	traverseinorder(head, array1, &dummy);
	ftime(&end);
	printf("Binary search tree: %d\n", elapsed(start, end));

	/* ------------ Sort using library quicksort ---------------------- */

	ftime(&start);
	qsort(array2, ARRAY_LEN, sizeof(int), cmp);
	ftime(&end);
	printf("Library QuickSort: %d\n", elapsed(start, end));

	/* ------------ Sort using PoP Book quicksort ---------------------- */

	ftime(&start);
	quicksort(array3, ARRAY_LEN);
	ftime(&end);
	printf("PoP QuickSort: %d\n", elapsed(start, end));

	getch();
	return 0;
}
