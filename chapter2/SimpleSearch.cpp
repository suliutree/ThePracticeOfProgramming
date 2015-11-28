#include <stdio.h>
#include <string.h>
#include <conio.h>

char *flab[] = {
	"actually",
	"just",
	"quite",
	"really",
	NULL
};

int lookup(char *word, char *array[])
{
	int i;
	for (i = 0; array[i] != NULL; i++)
		if (strcmp(word, array[i]) == 0)
			return i;
	return -1;
}

int main()
{
	char str[256];
	gets(str);

	int index = lookup(str, flab);
	if (index >= 0)
		printf("found %s", flab[index]);
	else
		printf("Not found %s", str);
	getch();
	return 0;
}
