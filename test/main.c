//#include <sys/cdefs.h>
//__FBSDID("$FreeBSD: /repoman/r/ncvs/src/lib/libc/regex/grot/main.c,v 1.3 2002/03/22 21:52:48 obrien Exp $");

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>
#ifndef __LINUX__
#include <io.h>
#endif
#include "regex.h"

#define  NS      10
#define  MAXF    10
#define  SHORT   10
#define  NSUBS   10
#define  NSHOULD 15


int debug  = 0;
int line   = 0;
int status = 0;

/* Command option defaults */
int copts = REG_EXTENDED;    // compile options
int eopts = 0;               // execute options

regoff_t startoff = 0;       // start offset
regoff_t endoff   = 0;       // end offset

void regress(FILE *in);
void try(char *f0, char *f1, char *f2, char *f3, char *f4, int opts);
int options(int type, char *s);
int opt(int c, char *s);
void fixstr(register char *p);
char *check(char *str, regmatch_t sub, char *should);

static char *eprint(int err);
static int  efind(char *name);
static void usage( void );

extern int  split( );
extern void regprint( );

/*
 - main - do the simple case, hand off to regress() for regression
 */
int main( int argc, char *argv[] )
{
    int c;
/*
    int i;
    int err;

    size_t len;

    char erbuf[100];

    regex_t re;
    regmatch_t subs[NS];
*/
    FILE *fp;

    while( ( c = getopt( argc, argv, "c:e:S:E:xh" ) ) != -1 ) {
        switch( c ) {

        case 'c':       /* compile options */
            copts = options('c', optarg);
            break;

        case 'e':       /* execute options */
            eopts = options('e', optarg);
            break;

        case 'S':       /* start offset */
            startoff = (regoff_t)atoi(optarg);
            break;

        case 'E':       /* end offset */
            endoff = (regoff_t)atoi(optarg);
            break;

        case 'x':       /* Debugging. */
            ++debug;
            break;

        case 'h':
        default:
            usage( );
            break;
        }

    }

    if( optind == argc ) {
        fprintf( stderr, "Using stdin as input. " );
        regress( stdin );
        fprintf(stderr, "Test complete %d\n", status);
        exit( status );
    } else {
        if( (access( argv[ optind ], F_OK ) != 0 ) ) {
            fprintf( stderr, "Input file not found: %s\n", argv[ optind ] );
            usage( );
            exit( 1 );
        } else {
            fp = fopen( argv[ optind ], "r" );
            fprintf( stderr, "Using %s as input file. ", argv[ optind ] );
            regress( fp );
            fclose( fp );
            fprintf(stderr, "Test complete %d\n", status);
            exit( status );
        }
    }
/*
    fprintf( stderr, "Using cmd line as input.\n" );

    err = regcomp( &re, argv[optind++], copts );

    if( err ) {
        len = regerror( err, &re, erbuf, sizeof( erbuf ) );
        fprintf( stderr, "error %s, %d/%d `%s'\n", eprint( err ), len, sizeof(erbuf), erbuf);
        exit(status);
    }

    regprint(&re, stdout);

    if( optind >= argc ) {
        regfree( &re );
        exit( status );
    }

    if( eopts&REG_STARTEND ) {
        printf("Found REG_STARTEND\n");
        subs[0].rm_so = startoff;
        subs[0].rm_eo = strlen( argv[optind] ) - endoff;
    }

    err = regexec( &re, argv[optind], (size_t)NS, subs, eopts );

    if( err ) {
        len = regerror( err, &re, erbuf, sizeof(erbuf) );
        fprintf( stderr, "error %s, %d/%d `%s'\n", eprint(err), len, sizeof(erbuf), erbuf );
        exit( status );
    }

    if( !( copts&REG_NOSUB ) ) {
        len = (int)(subs[0].rm_eo - subs[0].rm_so);
        if (subs[0].rm_so != -1) {
            if (len != 0) printf("match `%.*s'\n", len, argv[optind] + subs[0].rm_so);
                else printf("match `'@%.1s\n", argv[optind] + subs[0].rm_so);
        }
        for( i = 1; i < NS; i++ ) if( subs[i].rm_so != -1 ) printf( "(%d) `%.*s'\n", i,
                                                              (int)(subs[i].rm_eo - subs[i].rm_so),
                                                              argv[optind] + subs[i].rm_so);
    }

    fprintf(stderr, "Test complete %d\n", status);

    exit(status);
*/
    return 0; // keep compiler happy
}


/*
 * regress - main loop of regression test
 * void regress(FILE *in);
 */
void regress( FILE *in )
{
    int nf;
    int i;

    size_t ne;

    char inbuf[1000];
    char *f[MAXF];   // Max field
    char erbuf[100];
    char *badpat = "invalid regular expression";
    char *bpname = "REG_BADPAT";

    regex_t re;

    fprintf( stderr, "Start regress....\n");
    while( fgets( inbuf, sizeof( inbuf ), in ) != NULL ) {

        line++;

        /* skip # and end of line */
        if( inbuf[0] == '#' || inbuf[0] == '\n' ) continue; /* NOTE CONTINUE */

        inbuf[strlen(inbuf)-1] = '\0';  /* get rid of stupid \n */

        if( debug ) fprintf( stderr, "Debug %d:\n", line );

        /*
         * split breaks up the input fields and places them in f[]
         */
        nf = split( inbuf, f, MAXF, "\t\t" );

        /* Input line must have at least 3 fields - see tests file header */
        if( nf < 3 ) {
            fprintf( stderr, "bad input, line %d\n", line );
            exit( 1 );
        }

        for( i = 0; i < nf; i++ ) if( strcmp(f[i], "\"\"" ) == 0 ) f[i] = "";

        if( nf <= 3 ) f[3] = NULL;
        if( nf <= 4 ) f[4] = NULL;

        /* options setup regcomp flags - first run REG_EXTENDED default */
        try( f[0], f[1], f[2], f[3], f[4], options( 'c', f[1] ) );

        /* try with either type of RE - second REG_BASIC if & */
        if( opt( '&', f[1] ) )
                try( f[0], f[1], f[2], f[3], f[4], options( 'c', f[1] ) &~ REG_EXTENDED );
    }

    ne = regerror( REG_BADPAT, (regex_t *)NULL, erbuf, sizeof( erbuf ) );
    if( strcmp( erbuf, badpat ) != 0 || ne != strlen( badpat )+1 ) {
        fprintf( stderr, "end: regerror() test gave `%s' not `%s'\n", erbuf, badpat );
        status = 1;
    }

    ne = regerror( REG_BADPAT, (regex_t *)NULL, erbuf, (size_t)SHORT );
    if( strncmp( erbuf, badpat, SHORT-1 ) != 0 ||
                        erbuf[SHORT-1] != '\0' ||
                        ne != strlen(badpat)+1) {
        fprintf( stderr, "end: regerror() short test gave `%s' not `%.*s'\n",
                                                        erbuf, SHORT-1, badpat );
        status = 1;
    }

    ne = regerror( REG_ITOA|REG_BADPAT, (regex_t *)NULL, erbuf, sizeof( erbuf ) );
    if( strcmp( erbuf, bpname) != 0 || ne != strlen( bpname )+1) {
            fprintf( stderr, "end: regerror() ITOA test gave `%s' not `%s'\n",
                                                        erbuf, bpname);
            status = 1;
    }
    re.re_endp = bpname;

    ne = regerror( REG_ATOI, &re, erbuf, sizeof( erbuf ) );
    if( atoi( erbuf ) != (int)REG_BADPAT ) {
            fprintf( stderr, "end: regerror() ATOI test gave `%s' not `%ld'\n",
                                                        erbuf, (long)REG_BADPAT );
            status = 1;
    } else {
        if( ne != strlen( erbuf )+1 ) {
            fprintf( stderr, "end: regerror() ATOI test len(`%s') = %ld\n",
                                                        erbuf, (long)REG_BADPAT );
            status = 1;
        }
    }
}


/*
 * try - try it, and report on problems
 *    void try(char *f0, char *f1, char *f2, char *f3, char *f4, int opts);
 */
void try( char *f0, char *f1, char *f2, char *f3, char *f4, int opts ) /* may not match f1 */
{
    int i;
    int nshould;
    int err;
    int len;

    char *type = (opts & REG_EXTENDED) ? "ERE" : "BRE";
    char *grump;
    char erbuf[100];
    char *should[NSHOULD];
    char f0copy[1000];
    char f2copy[1000];

    regex_t re;
    regmatch_t subs[NSUBS];

    if( debug ) {
        fprintf( stderr, "1:%s 2:%s 3:%s 4:%s 5:%s  %s\n", f0, f1,f2, f3, f4, type);
        if( opts & '^' ) fprintf( stderr, "REG_NOTBOL ");
        if( opts & '$' ) fprintf( stderr, "REG_NOTEOL ");
        if( opts & '#' ) fprintf( stderr, "REG_STARTEND ");
        if( opts & 't' ) fprintf( stderr, "REG_TRACE ");
        if( opts & 'l' ) fprintf( stderr, "REG_LARGE ");
        if( opts & 'r' ) fprintf( stderr, "REG_BACKR ");
    }

    /*
     * ***** REGCOMP Section *****
     */
    strcpy( f0copy, f0 );

    // handle REG_PEND if an option
    re.re_endp = (opts&REG_PEND) ? f0copy + strlen(f0copy) : NULL;

    fixstr( f0copy );

    // compile the RE and check result against options (ie C)
    err = regcomp( &re, f0copy, opts );
    if( debug ) fprintf( stderr, " REGCOMP %d\n", err);

    if( err != 0 && ( !opt( 'C', f1 ) || err != efind( f2 ) ) ) {
        /* unexpected error or wrong error */
        len = regerror( err, &re, erbuf, sizeof( erbuf ) );
        fprintf( stderr, "%d: %s error %s, %d/%d `%s'\n",line, type,
                                eprint(err), len, sizeof( erbuf ), erbuf );
        status = 1;
    } else {
        if( err == 0 && opt( 'C', f1 ) ) {
            /* unexpected success */
            fprintf( stderr, "%d: %s should have given REG_%s\n", line, type, f2 );
            status = 1;
            err = 1;        /* so we won't try regexec */
        }
    }

    if( err != 0 ) {
        regfree( &re );
        return;
    }
    /***** END of REGCOMP *****/

    /*
     * ***** REGEXEC Section *****
     */
    strcpy( f2copy, f2 );
    fixstr( f2copy );

    if( options( 'e', f1 ) &REG_STARTEND ) {
        if( debug ) fprintf( stderr, "REG_STARTEND\n");
        if( strchr( f2, '(' ) == NULL || strchr( f2, ')' ) == NULL )
                                  fprintf(stderr, "%d: bad STARTEND syntax\n", line);
        subs[0].rm_so = strchr( f2, '(' ) - f2 + 1;
        subs[0].rm_eo = strchr( f2, ')' ) - f2;
    }
    err = regexec( &re, f2copy, NSUBS, subs, options( 'e', f1 ) );

    if( err != 0 && ( f3 != NULL || err != REG_NOMATCH ) ) {
        /* unexpected error or wrong error */
        len = regerror( err, &re, erbuf, sizeof( erbuf ) );
        fprintf( stderr, "%d: %s exec error %s, %d/%d `%s'\n", line, type,
                                    eprint( err ), len, sizeof( erbuf ), erbuf );
        status = 1;
    } else
        if( err != 0 ) {
            /* nothing more to check */
        } else
            if( f3 == NULL ) {
                /* unexpected success */
                fprintf( stderr, "%d: %s exec should have failed\n", line, type );
                status = 1;
                err = 1;                /* just on principle */
    } else if (opts&REG_NOSUB) {
            /* nothing more to check */
    } else if ((grump = check(f2, subs[0], f3)) != NULL) {
            fprintf(stderr, "%d: %s %s\n", line, type, grump);
            status = 1;
            err = 1;
    }

    if( err != 0 || f4 == NULL ) {
        regfree( &re );
        return;
    }

    for( i = 1; i < NSHOULD; i++ ) should[i] = NULL;

    nshould = split(f4, should+1, NSHOULD-1, ",");

    if( nshould == 0 ) {
        nshould = 1;
        should[1] = "";
    }

    for( i = 1; i < NSUBS; i++ ) {
        grump = check( f2, subs[i], should[i] );
        if( grump != NULL ) {
            fprintf( stderr, "%d: %s $%d %s\n", line, type, i, grump );
            status = 1;
            err = 1;
        }
    }

    regfree( &re );
}


/*
 * options - pick options out of a regression-test string
 *
 *  'c' compile, 'e' exec
 */
int options( int type, char *s )
{
    // working with copts or eopts
    int o = (type == 'c') ? copts : eopts;

    // legal values for the type of operator
    char *legal = (type == 'c') ? "bisnmp" : "^$#tlr";

    char *p;

    for(p = s; *p != '\0'; p++) {

        if(strchr(legal, *p) != NULL) {

            switch (*p) {

            case 'b':
                o &= ~REG_EXTENDED;
                break;

            case 'i':
                o |= REG_ICASE;
                break;

            case 's':
                o |= REG_NOSUB;
                break;

            case 'n':
                o |= REG_NEWLINE;
                break;

            case 'm':
                o &= ~REG_EXTENDED;
                o |= REG_NOSPEC;
                break;

            case 'p':
                o |= REG_PEND;
                break;

            case '^':
                o |= REG_NOTBOL;
                break;

            case '$':
                o |= REG_NOTEOL;
                break;

            case '#':
                o |= REG_STARTEND;
                break;

            case 't':       /* trace */
                o |= REG_TRACE;
                break;

            case 'l':       /* force long representation */
                o |= REG_LARGE;
                break;

            case 'r':       /* force backref use */
                o |= REG_BACKR;
                break;

            }
        }
    }

    return(o);
}


/*
 - opt - is a particular option in a regression string?
 == int opt(int c, char *s);
 */
  /* predicate */
int opt( int c, char *s )
{
    return( strchr( s, c ) != NULL );
}


/*
 - fixstr - transform magic characters in strings
 == void fixstr(char *p);
 */
void fixstr( char *p )
{
    if( p == NULL ) return;

    for( ; *p != '\0'; p++ ) {
        if( *p == 'N' ) *p = '\n';
        else if( *p == 'T' ) *p = '\t';
        else if( *p == 'S' ) *p = ' ';
        else if( *p == 'Z' ) *p = '\0';
    }
}


/*
 - check - check a substring match
 == char *check(char *str, regmatch_t sub, char *should);
 */
 /* NULL or complaint */
char *check( char *str, regmatch_t sub, char *should )
{
    int len;
    int shlen;
    char *p;
    char *at = NULL;
    static char grump[500];

    if( should != NULL && strcmp( should, "-" ) == 0 ) should = NULL;

    if( should != NULL && should[0] == '@' ) {
        at = should + 1;
        should = "";
    }

    /* check rm_so and rm_eo for consistency */
    if( sub.rm_so > sub.rm_eo ||
      ( sub.rm_so == -1 && sub.rm_eo != -1 ) ||
      (sub.rm_so != -1 && sub.rm_eo == -1)   ||
      (sub.rm_so != -1 && sub.rm_so < 0)     ||
      (sub.rm_eo != -1 && sub.rm_eo < 0) ) {
        sprintf( grump, "start %ld end %ld", (long)sub.rm_so, (long)sub.rm_eo);
        return(grump);
    }

    /* check for no match */
    if( sub.rm_so == -1 && should == NULL ) return( NULL );
    if( sub.rm_so == -1 ) return( "did not match" );

    /* check for in range */
    if( sub.rm_eo > strlen( str ) ) {
        sprintf( grump, "start %ld end %ld, past end of string",
                        (long)sub.rm_so, (long)sub.rm_eo );
        return( grump );
    }

    len = (int)( sub.rm_eo - sub.rm_so );
    shlen = (int)strlen( should );
    p = str + sub.rm_so;

    /* check for not supposed to match */
    if( should == NULL ) {
        sprintf( grump, "matched `%.*s'", len, p );
        return( grump );
    }

    /* check for wrong match */
    if( len != shlen || strncmp( p, should, (size_t)shlen ) != 0) {
        sprintf( grump, "matched `%.*s' instead", len, p );
        return( grump );
    }

    if( shlen > 0 ) return( NULL );

    /* check null match in right place */
    if( at == NULL ) return( NULL );

    shlen = strlen( at );
    if( shlen == 0 ) shlen = 1;      /* force check for end-of-string */

    if( strncmp( p, at, shlen ) != 0 ) {
        sprintf( grump, "matched null at `%.20s'", p );
        return( grump );
    }

    return( NULL );
}


/*
 - eprint - convert error number to name
 == static char *eprint(int err);
 */
static char *eprint( int err )
{
    size_t len;
    static char epbuf[100];

    len = regerror( REG_ITOA|err, (regex_t *)NULL, epbuf, sizeof( epbuf ) );

    assert( len <= sizeof( epbuf ) );

    return( epbuf );
}


/*
 - efind - convert error name to number
 == static int efind(char *name);
 */
static int efind( char *name )
{
    static char efbuf[100];
    regex_t re;

    sprintf(efbuf, "REG_%s", name);

    assert(strlen(efbuf) < sizeof(efbuf));

    re.re_endp = efbuf;
    (void) regerror(REG_ATOI, &re, efbuf, sizeof(efbuf));

    return(atoi(efbuf));
}


/*
 * re.exe usage message
 *
 * I had to go to the trouble of figuring this out so
 * maybe this will help - save some time for the next
 * person.
 */
static void usage( void )
{
/* The linux build doesn't support __argv (OW specific) so just call it re */
#ifndef __LINUX__
    char PrgName[ _MAX_FNAME ];
    char PrgExt[ _MAX_EXT ];

    _splitpath( __argv[0], NULL, NULL, PrgName, PrgExt );
    fprintf( stderr, "%s%s: Test program for POSIX regex library\n\n",
                     strlwr( PrgName ), strlwr( PrgExt ) );
#else
    fprintf( stderr, "re: Test program for POSIX regex library\n\n");
#endif
    fprintf( stderr, "Usage:\n" );
    fprintf( stderr, "  -c <compile options [bisnmp] > {default REG_EXTENDED}\n" );
    fprintf( stderr, "     b - REG_EXTENDED        n - REG_NEWLINE\n" );
    fprintf( stderr, "     i - REG_ICASE           m - ~REG_EXTENDED  REG_NOSPEC\n" );
    fprintf( stderr, "     s - REG_NOSUB           p - REG_PEND\n" );
    fprintf( stderr, "              { REG_DUMP not supported }\n\n" );
    fprintf( stderr, "  -e <execute options [^$#tlr] > {default 0 - none}\n" );
    fprintf( stderr, "     ^ - REG_NOTBOL          t - REG_TRACE\n" );
    fprintf( stderr, "     $ - REG_NOTEOL          l - REG_LARGE\n" );
    fprintf( stderr, "     # - REG_STARTEND        r - REG_BACKR\n\n" );

    fprintf( stderr, "     REG_NOTBOL REG_NOTEOL REG_STARTEND REG_TRACE\n" );
    fprintf( stderr, "     REG_LARGE  REG_BACKR\n\n" );
    fprintf( stderr, "  -S <start offset>    {default 0}\n" );
    fprintf( stderr, "  -E <end offset>      {default 0}\n\n" );
    fprintf( stderr, "  -x Debugging enabled.\n" );
    fprintf( stderr, "  -h This usage message.\n" );
    exit(0);
}

