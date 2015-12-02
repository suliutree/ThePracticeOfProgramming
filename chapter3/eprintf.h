/* eprintf.h: error wrapper function */
extern void eprintf(char *, ...);
extern void weprintf(char *, ...);
extern char *estrdup(char *);
extern void *emalloc(size_t);
extern char *progname(void);
extern void setprogname(char *);

#define NELEMS(a)	(sizeof(a) / sizeof(a[0]))
