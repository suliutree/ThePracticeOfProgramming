#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define FALSE 0
#define TRUE 1

typedef struct Nameval Nameval;
struct Nameval {
	char *name;
	int value;
};

struct NVtab {
	int nval;			// current number of values
	int max;			// allocated number of values
	Nameval *nameval;	// array of name-value pairs
} nvtab;

enum { NVINIT = 1, NVGROW = 2 };

int addname(Nameval newname)
{
	Nameval *nvp;
	int addpos = 0;			// position where the new element will be added
	int emptyslot = FALSE;	// flag to indicate whether an empty slot is available

	if (nvtab.nameval == NULL) {	// first time
		nvtab.nameval = (Nameval *)malloc(NVINIT * sizeof(Nameval));
		if (nvtab.nameval == NULL)
			return -1;
		nvtab.max = NVINIT;
		nvtab.nval = 0; ///////////////////////////////
		addpos = 0;
	} else {
		for (int i = 0; i < nvtab.max; i++) // look for an empty slot
			//if (strcmp(nvtab.nameval[i].name, (char *)(NULL)) == 0) {
			if (nvtab.nameval[i].name == NULL) {
				addpos = i;
				emptyslot = TRUE;
				break;
			}
		if (!emptyslot) {	// no empty slot found, must grow the array
			addpos = nvtab.max;
			nvp = (Nameval *)realloc(nvtab.nameval, (NVGROW * nvtab.max) * sizeof (Nameval));
			if (nvp == NULL)
				return -1;
			nvtab.max *= NVGROW;
			nvtab.nameval = nvp;
		}
	}
	nvtab.nameval[addpos] = newname;
	return ++nvtab.nval;
}

int delname(char *name)
{
	int i;
	for (i = 0; i < nvtab.nval; i++)
		if (nvtab.nameval[i].name != NULL) {
			if (strcmp(nvtab.nameval[i].name, name) == 0) {
				nvtab.nameval[i].name = (char *)(NULL);
				nvtab.nval--;
				return 1;
			}
		}
		return 0;
}

int main()
{
	Nameval nameval1, nameval2, nameval3;
	nameval1.name = "github";
	nameval1.value = 0;
	nameval2.name = "google";
	nameval2.value = 1;
	nameval3.name = "facebook";
	nameval3.value = 2;

	int nval1 = addname(nameval1);
	printf("length1: %d\n", nval1);
	printf("max1: %d\n", nvtab.max);

	int nval2 = addname(nameval2);
	printf("length2: %d\n", nval2);
	printf("max2: %d\n", nvtab.max);

	int nval3 = addname(nameval3);
	printf("length3: %d\n", nval3);
	printf("max3: %d\n", nvtab.max);

	for (int i = 0; i < nval3; ++i) {
		if (nvtab.nameval[i].name == NULL)
			printf("NULL ");
		else
			printf("%s ", nvtab.nameval[i].name);
	}

	delname(nameval1.name);
	delname(nameval2.name);

	printf("\n");
	for (int i = 0; i < nval3; ++i) {
		if (nvtab.nameval[i].name == NULL)
			printf("NULL ");
		else
			printf("%s ", nvtab.nameval[i].name);
	}

	getch();
	return 0;
}
