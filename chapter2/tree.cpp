#include <stdio.h>
#include <string.h>
#include <conio.h>

#include "eprintf.h"

typedef struct Nameval Nameval;
struct Nameval {
	char *name;
	int value;
	Nameval *left;	// lesser
	Nameval *right;	// greater
};


Nameval htmlchars[] = {
	"AA",	0x00c6, NULL, NULL,
	"AB",	0x00c1, NULL, NULL,
	"AC",	0x00c2, NULL, NULL,
	"BA",	0x11b1, NULL, NULL,
	"BB",	0x11b2, NULL, NULL,
	"BC",	0x11b3, NULL, NULL,
	"BB",	0x11b2, NULL, NULL 
};

#define NELEMS(array) (sizeof(array) / sizeof(array[0]))

// insert: insert newp in treep, return treep
Nameval *insert(Nameval *treep, Nameval *newp)
{
	int cmp;
	if (treep == NULL)
		return newp;
	cmp = strcmp(newp->name, treep->name);
	if (cmp == 0)
		weprintf("insert: duplicate entry %s ignored", newp->name);
	else if (cmp < 0)
		treep->left = insert(treep->left, newp);
	else
		treep->right = insert(treep->right, newp);
	return treep;
}

// lookup: look up name in tree treep
Nameval *lookup(Nameval *treep, char *name)
{
	int cmp;
	if (treep == NULL)
		return NULL;
	cmp = strcmp(name, treep->name);
	if (cmp == 0)
		return treep;
	else if (cmp < 0)
		return lookup(treep->left, name);
	else
		return lookup(treep->right, name);
}

// nrlookup: non-recursively look up name in tree treep
Nameval *nrlookup(Nameval *treep, char *name)
{
	int cmp;
	while (treep != NULL) {
		cmp = strcmp(name, treep->name);
		if (cmp == 0)
			return treep;
		else if (cmp < 0)
			treep = treep->left;
		else
			treep = treep->right;
	}
	return NULL;
}

// applyinorder: inorder application of fn to treep
void applyinorder(Nameval *treep, void (*fn)(Nameval*, void*), void *arg)
{
	if (treep == NULL)
		return;
	applyinorder(treep->left, fn, arg);
	(*fn)(treep, arg);
	applyinorder(treep->right, fn, arg);
}

// applyinorder(treep, printnv, "%s: %x\n");

// applypostorder: postorder application of fn to treep
void applypostorder(Nameval *treep, void (*fn)(Nameval*, void*), void *arg)
{
	if (treep == NULL)
		return;
	applypostorder(treep->left, fn, arg);
	applypostorder(treep->right, fn, arg);
	(*fn)(treep, arg);
}

// printnv: print name and value using format in arg
void printnv(Nameval *p, void *arg)
{
	char *fmt;
	fmt = (char *)arg;
	printf(fmt, p->name, p->value);
}

int main()
{
	Nameval *nv = NULL;
	nv = insert(nv, &htmlchars[0]);
	for (int i = 1; i < NELEMS(htmlchars); ++i) {
		insert(nv, &htmlchars[i]);
	}

	printf("inorder:\n");
	applyinorder(nv, printnv, "%s: %x\n");

	printf("postorder:\n");
	applypostorder(nv, printnv, "%s: %x\n");

	printf("look up the string: ");
	char str[256];
	gets(str);
	if (lookup(nv, str) != NULL)
		printf("found the string: %s", str);
	else
		printf("not found the string: %s", str);

	getch();
	return 0;
}
