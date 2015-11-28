/*
用库函数bsearch重写HTML查询函数
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct Nameval Nameval;
struct Nameval {
	char *name;
	int value;
};

Nameval htmlchars[] = {
	"AA",	0x00c6, 
	"AB",	0x00c1,
	"AC",	0x00c2,
	"BA",	0x11b1,
	"BB",	0x11b2,
	"BC",	0x11b3
};

#define NELEMS(array) (sizeof(array) / sizeof(array[0]))

int nvcmp(const void *va, const void *vb);

int lookup(char *name, Nameval tab[], int ntab)
{
	Nameval key, *np;

	key.name = name;
	key.value = 0;

	// 返回一个指针指向检索到的元素，如果没有检索到元素，bsearch返回NULL
	np = (Nameval *)bsearch(&key, tab, ntab, sizeof(tab[0]), nvcmp);

	if (np == NULL)
		return -1;
	else
		return np-tab;
}

int nvcmp(const void *va, const void *vb)
{
	const Nameval *a, *b;

	a = (Nameval *)va;
	b = (Nameval *)vb;
	return strcmp(a->name, b->name);
}

int main()
{
	char str[256];
	gets(str);

	int index = lookup(str, htmlchars, NELEMS(htmlchars));

	if (index >= 0)
		printf("Found %s", htmlchars[index].name);
	else
		printf("Not found %s", str);

	getch();
	return 0;
}
