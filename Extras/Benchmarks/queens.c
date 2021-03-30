#include <stdio.h>			/* Need standard I/O functions */
#include <stdlib.h>			/* Need exit() routine interface */
#include <string.h>			/* Need strcmp() interface */
#ifdef	MPW				/* Macintosh MPW ONLY */
#   include <CursorCtl.h>		/* Need cursor control interfaces */
#endif

#define MAXQUEENS 100 			/* Max number of queens */
#define MAXRANKS  MAXQUEENS		/* Max number of ranks (rows) */
#define MAXFILES  MAXQUEENS		/* Max number of files (columns) */
#define MAXDIAGS  (MAXRANKS+MAXFILES-1)	/* Max number of diagonals */
#define EMPTY	  (MAXQUEENS+1)		/* Marks unoccupied file or diagonal */

/* GLOBAL VARIABLES */

int queens;			/* Number of queens to place */
int ranks;			/* Number of ranks (rows) */
int files;			/* Number of files (columns) */
int printing = 1;		/* TRUE if printing positions */
int findall = 0;		/* TRUE if finding all solutions */

unsigned long solutions = 0;	/* Number of solutions found */
int queen[MAXRANKS];		/* File on which each queen is located */
int file[MAXFILES]; 		/* Which queen 'owns' each file */
int fordiag[MAXDIAGS];		/* Which queen 'owns' forward diagonals */
int bakdiag[MAXDIAGS];		/* Which queen 'owns' reverse diagonals */
char *progname = NULL;		/* The name of this program */


/* -------------------------- PROTOTYPES ----------------------- */

void pboard(void);
void find(register int level);


/*-------------------------- main() ----------------------------
**  MAIN program.  The main purpose of this routine is to deal
**  with decoding the command line arguments, initializing the
**  various arrays, and starting the recursive search routine.
*/
int main(int argc, char **argv)
{
   register int  i;				/* Loop variable */
   register char *p;				/* Ptr to argument */
   char *usage =
"Usage:  %s [-ac] n\n\
\tn\tNumber of queens (rows and columns). An integer from 1 to 100.\n\
\t-a\tFind and print all solutions.\n\
\t-c\tCount all solutions, but do not print them.\n";

#ifdef	MPW					/* Macintosh MPW ONLY */
   InitCursorCtl(0);				/* Enable cursor control */
#endif

   progname = argv[0];				/* Name of the program */
   if(argc<2)
   {  
      queens=12;
      printing = 1;
      findall = 1;
   }
   else
   /****   DECODE COMMAND LINE ARGUMENTS   ****/
   {
      for(i = 1; i < argc; ++i) {			/* Scan through arguments */
         p = argv[i];				/* Ptr to base of argument */
         if(*p == '-') {				/* Command line option? */
            while(*++p) {				/* Loop through characters */
               switch(*p) {			/* What is the character */
                  case 'c':			/* '-c' option */
                     printing = 0;			/* Counting, not printing */
                  case 'a':			/* '-a' option */
                     findall = 1;			/* Find all solutions */
                     break;
                  default:				/* Illegal option */
                     fprintf(stderr,"%s: Illegal option '%s'\n",progname,argv[i]);
                     fprintf(stderr,usage,progname);
                     exit(-1);
               }					/* End of switch */
            }					/* End of loop */
         }						/* End of option test */
         else {
            if(sscanf(p,"%d",&queens) != 1) {	/* Read integer argument */
               fprintf(stderr,"%s: Non-integer argument '%s'\n",progname,p);
               exit(-1);
            
            }
            if(queens <= 0) {			/* N must be positive */
               fprintf(stderr,"%s: n must be positive integer\n",progname);
               exit(-1);
            }
            if(queens > MAXQUEENS) {		/* N can't be too large */
               fprintf(stderr,"%s: Can't have more than %d queens\n",
                  progname, MAXQUEENS);
               exit(-1);
            }
         }						/* End of argument test */
      }						/* End of argument scan loop */
   }
   if(!queens) {
      fprintf(stderr,"%s: Missing n argument\n",progname);
      fprintf(stderr,usage,progname);
      exit(-1);
   }

   ranks = files = queens;			/* NxN board for N queens */
   printf("%d queen%s on a %dx%d board...\n",
      queens, queens > 1 ? "s" : "", ranks, files);
   fflush(stdout);

   /* Initialization */
   solutions = 0;				/* No solutions yet */
   for(i = 0; i < MAXFILES; ++i) file[i] = EMPTY;
   for(i = 0; i < MAXDIAGS; ++i) fordiag[i] = bakdiag[i] = EMPTY;

   /* Find all solutions (begin recursion) */
   find(0);
   if(printing && solutions) putchar('\n');

   /* Report results */
   if(solutions == 1) printf("...there is 1 solution\n");
   else printf("...there are %ld solutions\n", solutions);

   exit(0);					/* No errors */
}


/***********************/
/****	ROUTINES    ****/
/***********************/

/*------------------------- pboard() ---------------------------
**  This routines prints the board for a particular solution.
**  The output is sent to stdout.
*/
void pboard(void)
{
   register int i, j;				/* Rank/File indices */

   if(findall)  				/* Only if searching for all */
      printf("\nSolution #%lu:\n",solutions);	/* Print solution number */

   for(i = 0; i < ranks; ++i) {			/* Loop through all ranks */
      for(j = 0; j < files; ++j) {		/* Loop through all files */
         putchar(' ');				/* Output a space */
         if(j == queen[i]) putchar('Q');	/* Output Q for queen... */
         else putchar('-');			/* or '-' if empty */
      }
      putchar('\n');				/* Break line */
   }
   fflush(stdout);				/* Flush solution to output */
}


/*-------------------------- find() ----------------------------
**  FIND is the recursive heart of the program, and finds all
**  solutions given a set of level-1 fixed queen positions.
**  The routine moves a single queen through all files (columns)
**  at the current rank (recursion level).  As the queen is moved,
**  conflict tests are made.  If the queen can be placed without
**  conflict, then the routine recurses to the next level.  When
**  all queens have been placed without conflict, a solution is
**  counted and reported.
*/
void find(register int level)
{
   register int f;			/* Indexes through files */
   register int *fp, *fdp, *bdp;	/* Ptrs to file/diagonal entries */

#ifdef	MPW				/* Macintosh MPW ONLY */
   if(level & 7 == 0)			/* Periodically break for... */
      SpinCursor(1);			/* background processing */
#endif

   if(level == queens) {		/* Placed all queens? Stop. */
      ++solutions;			/* This is a solution! */
      if(printing) pboard();		/* Print board if printing */
      if(!findall) exit(0);		/* May stop after first solution */
#ifdef	MPW				/* Macintosh MPW ONLY */
      SpinCursor(1);			/* background processing */
#endif
   }
   else {				/* Not at final level yet */
      for(				/* Move queen through all files */
         f = 0,				/* Queen starts at left (file 0) */
         fp = file,			/* Ptr to base of file array */
         fdp = &fordiag[level],		/* Ptr to first fwd diag entry */
         bdp = &bakdiag[level+files-1]	/* Ptr to first bak diag entry */
         ;
         f < files			/* Loop through all files */
         ;
         ++f,				/* Advance index */
         ++fp, ++fdp, --bdp		/* Advance pointers */
      ) {
         if(*fp >= level && 		/* No queen on the file? */
            *fdp >= level && *bdp >= level	/* No queens on diagonals? */
	 ) {
            queen[level] = f;		/* Note new position of queen */
            *fp = *fdp = *bdp = level;	/* Place queen on file & diags */
            find(level+1);		/* This level OK, recurse to next */
            *fp = *fdp = *bdp = EMPTY;	/* Remove queen from file & diags */
         }				/* End of conflict test */
      }					/* End of file loop */
   }					/* End if (level == queens) */
}


