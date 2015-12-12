#include <stdio.h>
#include <string.h>
#include <conio.h>

#include "eprintf.h"

typedef struct Nameval Nameval;
struct Nameval {
	char *name;
	int value;
	Nameval *next;
};

const int NHASH = 7;
Nameval *symtab[NHASH]; // a symbol table

Nameval htmlchars[] = {
	"AA",	0x00c6, NULL,
	"AB",	0x00c1, NULL,
	"AC",	0x00c2, NULL,
	"BA",	0x11b1, NULL,
	"BB",	0x11b2, NULL,
	"BC",	0x11b3, NULL,
	"BB",	0x11b2, NULL
};

#define NELEMS(array) (sizeof(array) / sizeof(array[0]))

unsigned int hash(char *str);

// lookup: find name in symtab, with optional create
Nameval* lookup(char* name, int create, int value)
{
	int h;
	Nameval *sym;

	h = hash(name);
	for (sym = symtab[h]; sym != NULL; sym = sym->next)
		if (strcmp(name, sym->name) == 0)
			return sym;
	if (create) {
		sym = (Nameval *)emalloc(sizeof(Nameval));
		sym->name = name; // assumed allocated elsewhere
		sym->value = value;
		sym->next = symtab[h];
		symtab[h] = sym;
	}
	return sym;
}

enum { MULTIPLITER = 31 };

// hash: compute hash value of string
unsigned int hash(char *str)
{
	unsigned int h;
	unsigned char *p;

	h = 0;
	for (p = (unsigned char *)str; *p != '\0'; p++)
		h = MULTIPLITER * h + *p;
	return h % NHASH;
}


int main()
{
	for (int i = 0; i < NELEMS(htmlchars); ++i) {
		lookup(htmlchars[i].name, 1, htmlchars[i].value);
	}

	Nameval *nv = NULL; 
	nv = lookup(htmlchars[3].name, 0, htmlchars[3].value);
	if (nv != NULL)
		printf("found: %s\n", nv->name);

	nv = NULL; 
	nv = lookup("github", 0, 0);
	if (nv != NULL)
		printf("found: %s\n", nv->name);
	else
		printf("not found github\n");

	nv = NULL; 
	nv = lookup("github", 1, 0);
	if (nv != NULL)
		printf("found: %s\n", nv->name);
	else
		printf("not found: %s\n", nv->name);
	
	getch();
	return 0;
}
