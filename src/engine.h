
#define     BOL     (OUT+1)
#define     EOL     (BOL+1)
#define     BOLEOL  (BOL+2)
#define     NOTHING (BOL+3)
#define     BOW     (BOL+4)
#define     EOW     (BOL+5)
#define     CODEMAX (BOL+5)         // highest code used

#define     NONCHAR(c)      ((c) > CHAR_MAX)
#define     NNONCHAR        (CODEMAX-CHAR_MAX)


#ifdef REDEBUG
static void print(struct match *m, char *caption, states st, int ch, FILE *d);
static void at(struct match *m, char *title, char *start, char *stop, sopno startst, sopno stopst);
static char *pchar(int ch);
#endif

static int matcher(register struct re_guts *g, char *string, size_t nmatch, regmatch_t pmatch[], int eflags);
static char *dissect(register struct match *m, char *start, char *stop, sopno startst, sopno stopst);
static char *backref(register struct match *m, char *start, char *stop, sopno startst, sopno stopst, sopno lev);
static char *fast(register struct match *m, char *start, char *stop, sopno startst, sopno stopst);
static char *slow(register struct match *m, char *start, char *stop, sopno startst, sopno stopst);
static states step(register struct re_guts *g, sopno start, sopno stop, register states bef, int ch, register states aft);
