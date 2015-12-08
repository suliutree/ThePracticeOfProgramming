// csv.h: interface for csv library

extern char *csvgetline(FILE *f);	// read next input line
extern char *csvfield(int n);		// return field n
extern int csvfield(void);			// return number of fields
