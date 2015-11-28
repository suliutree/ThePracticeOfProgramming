#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

enum {length = 10};

 void swap(int v[], int i, int j);

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

 void swap(int v[], int i, int j)
 {
	 int temp;

	 temp = v[i];
	 v[i] = v[j];
	 v[j] = temp;
 }

 int main()
 {
	 int array[length];
	 for (int i = 0; i < length; ++i) 
		scanf("%d", &array[i]);	

	 quicksort(array, length);

	 for (int i = 0; i < length; ++i) 
		printf("%d ", array[i]);
	 printf("\n");

	 getch();
	 return 0;
 }
