/*
修改lookup函数，使得当链表的平均长度大于某个x值时，数组将按照y的某个因子
扩大，并重新构造散列表

For the case where the hash table has grown to "dense", my approach was to 
allocate a new table, copy the nodes from the existing table to the new table
(after computing their new hashes), and then destory the old table.
*/

#include <string.h>
#include <stdlib.h>

enum { MULTIPLIER = 31 };

int NHASH = 77;

typedef struct Nameval Nameval;
struct Nameval {
	char *name;
	int value;
	Nameval *next;
};

unsigned int hash(char *str)
{
	unsigned int h;
	unsigned char *p;
	h = 0;
	for (p = (unsigned char *)str; *p != '\0'; p++)
		h = MULTIPLIER * h + *p;
	return h % NHASH;
}

void delchain(Nameval *p)
{
	if (p->next)
		delchain(p->next);
	free(p);
}

Nameval* lookup(char *name, int create, int value, int x, int y, Nameval **symtab)
{
	static int numitems = 0;

	int h, i;
	int OLD_NHASH;
	Nameval *sym, *temp_sym, *new_sym;
	Nameval **new_symtab;

	h = hash(name);
	// lookup the requested symbol
	for (sym = symtab[h]; sym != NULL; sym = sym->next)
		if (strcmp(name, sym->name) == 0)
			return sym;
	if (create) {							// symbol not found, so create
		if ((numitems + 1) / NHASH > x)  {	// need to build a new table
			OLD_NHASH = NHASH;
			NHASH *= y;
			// allocate the new table
			new_symtab = (Nameval **)malloc(sizeof(Nameval *)*NHASH);
			for (i = 0; i < OLD_NHASH; i++) { // for each entry in the old table
				// copy chain to new table, recomputing the hash
				for (sym = symtab[i]; sym != NULL; sym = sym->next) {
					// allocate a copy of the node
					temp_sym = (Nameval *)malloc(sizeof(Nameval));
					temp_sym->name = sym->name;
					temp_sym->value = sym->value;
					h = hash(temp_sym->name);
					temp_sym->next = new_symtab[h];
					new_symtab[h] = temp_sym;
				}
			}

			for(i = 0; i < OLD_NHASH; i++) { // destory the old table
				sym = symtab[i];
				delchain(sym);
			} 
		}
		symtab = new_symtab;

		// Add the new node to the new table
		h = hash(name);
		new_sym = (Nameval *)malloc(sizeof(Nameval));
		new_sym->name = name;
		new_sym->value = value;
		new_sym->next = symtab[h];
		symtab[h] = new_sym;
		numitems++;
	}
	return sym;
}
