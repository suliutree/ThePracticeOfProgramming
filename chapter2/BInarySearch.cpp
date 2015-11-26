#include <stdio.h>
#include <string.h>
#include <conio.h>

/**
struct S { int x; }; 
typedef struct S S;

对于第1行的定义，如果要定义S的变量：
void f( struct S argument ); // struct is required here

如果加了第二行，则可以这样定义：
void f( S argument ); // struct keyword no longer needed
*/

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

int lookup(char *name, Nameval tab[], int ntab)
{
	int low, high, mid, cmp;

	low = 0;
	high = ntab - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		cmp = strcmp(name, tab[mid].name);
		if (cmp < 0)
			high = mid - 1;
		else if (cmp > 0)
			low = mid + 1;
		else
			return mid;
	}
	return -1;
}

int main()
{
	//printf("The HTML table has %d words\n", NELEMS(htmlchars));

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
