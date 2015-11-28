#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

enum {N = 5};

int scmp(const void *p1, const void *p2)
{
	char *v1, *v2;

	v1 = *(char **)p1;
	v2 = *(char **)p2;
	return strcmp(v1, v2);
}

int main()
{
	char *str[N], buf[256];
	for (int i = 0; i < N; ++i)
	{
		gets(buf);
		str[i] = (char *)malloc(strlen(buf) + 1);
		strcpy(str[i], buf);
	}

	// 数组、数组长度、被排序元素（数组元素）的大小、比较函数
	qsort(str, N, sizeof(str[0]), scmp);

	printf("\n");
	for (int i = 0; i < N; ++i)
		printf("%s\n", str[i]);

	getch();
	return 0;
}
