#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct Nameval Nameval;
struct Nameval {
	char *name;
	int value;
};

struct NVtab {
	int nval;			// 当前元素的个数
	int max;			// 已分配空间的元素的个数
	Nameval *nameval;
} nvtab;

enum { NVINIT = 1, NVGROW = 2};

int addname(Nameval newname)
{
	Nameval *nvp;
	if (nvtab.nameval == NULL) {
		nvtab.nameval = (Nameval *)malloc(NVINIT * sizeof(Nameval));
		if (nvtab.nameval == NULL)
			return -1;
		nvtab.max = NVINIT;
		nvtab.nval = 0;
	} else if (nvtab.nval >= nvtab.max) {
		nvp = (Nameval *)realloc(nvtab.nameval, (NVGROW*nvtab.max) * sizeof(Nameval));
		if (nvp == NULL)
			return -1;
		nvtab.max *= NVGROW;
		nvtab.nameval = nvp;
	}
	nvtab.nameval[nvtab.nval] = newname;
	return ++nvtab.nval;
}

int delname(char *name)
{
	int i;
	for (i = 0; i < nvtab.nval; i++)
		if (strcmp(nvtab.nameval[i].name, name) == 0) {
			memmove(nvtab.nameval+i, nvtab.nameval+1, (nvtab.nval-(i+1))*sizeof(Nameval));
			//int j;
			//for (j = i; j < nvtab.nval-1; j++)
			//	nvtab.nameval[j] = nvtab.nameval[j+i];
			nvtab.nval--;
			return 1;
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

	for (int i = 0; i < nval3; ++i)
		printf("%s ", nvtab.nameval[i].name);

	delname(nameval1.name);

	printf("\n");
	for (int i = 0; i < nval3; ++i)
		printf("%s ", nvtab.nameval[i].name);

	getch();
	return 0;
}
