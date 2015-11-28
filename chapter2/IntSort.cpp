#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

enum {N = 10};

int icmp(const void *p1, const void *p2)
{
	int v1, v2;

	v1 = *(int *)p1;
	v2 = *(int *)p2;
	if (v1 < v2)
		return -1;
	else if (v1 == v2)
		return 0;
	else
		return 1;
}

int main()
{
	int arr[N];
	for (int i = 0; i < N; ++i)
		scanf("%d", &arr[i]);

	// 数组、数组长度、被排序元素的大小、比较函数
	qsort(arr, N, sizeof(arr[0]), icmp);

	for (int i = 0; i < N; ++i)
		printf("%d ", arr[i]);

	getch();
	return 0;
}
