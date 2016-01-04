#ifndef _REGEX_H_
#define _REGEX_H_

#ifndef __TYPES_H_INCLUDED
#include <sys/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* regcomp() cflags */
#define REG_BASIC       0000    /** POSIX **/
#define REG_EXTENDED    0001    /** POSIX **/
#define REG_ICASE       0002    /** POSIX **/
#define REG_NOSUB       0004    /** POSIX **/
#define REG_NEWLINE     0010    /** POSIX **/
#define REG_NOSPEC      0020
#define REG_PEND        0040
#define REG_DUMP        0200

/* regexec.c eflags */
#define REG_NOTBOL      00001   /** POSIX **/
#define REG_NOTEOL      00002   /** POSIX **/
#define REG_STARTEND    00004
#define REG_TRACE       00400   /* tracing of execution */
#define REG_LARGE       01000   /* force large representation */
#define REG_BACKR       02000   /* force use of backref code */

/* regerror() flags */
#define REG_ENOSYS      (-1)    /** POSIX Obsolescent **/
#define REG_OKAY         0
#define REG_NOMATCH      1      /** POSIX **/
#define REG_BADPAT       2      /** POSIX **/
#define REG_ECOLLATE     3      /** POSIX **/
#define REG_ECTYPE       4      /** POSIX **/
#define REG_EESCAPE      5      /** POSIX **/
#define REG_ESUBREG      6      /** POSIX **/
#define REG_EBRACK       7      /** POSIX **/
#define REG_EPAREN       8      /** POSIX **/
#define REG_EBRACE       9      /** POSIX **/
#define REG_BADBR       10      /** POSIX **/
#define REG_ERANGE      11      /** POSIX **/
#define REG_ESPACE      12      /** POSIX **/
#define REG_BADRPT      13      /** POSIX **/
#define REG_EMPTY       14
#define REG_ASSERT      15
#define REG_INVARG      16
#define REG_ILLSEQ      17

#define REG_ATOI        255     /* convert name to number (!) */
#define REG_ITOA        0400    /* convert number to name (!) */

typedef off_t regoff_t;

typedef struct {
    int  re_magic;
    size_t re_nsub;         /* number of parenthesized subexpressions */
    const char *re_endp;    /* end pointer for REG_PEND */
    struct re_guts *re_g;   /* none of your business :-) */
} regex_t;

typedef struct {
    regoff_t rm_so;         /* start of match */
    regoff_t rm_eo;         /* end of match */
} regmatch_t;


extern int regcomp(regex_t *, const char *, int);
extern int regexec(const regex_t *, const char *, size_t, regmatch_t [], int);
extern size_t regerror(int, const regex_t *, char *, size_t);
extern void regfree(regex_t *);


//extern int regcomp(regex_t * __restrict, const char * __restrict, int);
//extern size_t regerror(int, const regex_t * __restrict, char * __restrict, size_t);
/*
 * XXX forth parameter should be `regmatch_t [__restrict]', but isn't because
 * of a bug in GCC 3.2 (when -std=c99 is specified) which perceives this as a
 * syntax error.
 */
//extern int regexec(const regex_t * __restrict, const char * __restrict, size_t, regmatch_t * __restrict, int);
//extern void regfree( regex_t * );


#ifdef __cplusplus
}
#endif

#endif
