#define MAX_VALUE 10000

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct Number {
   int num;
   int true; // true = prime/unchecked, false = non-prime
} Number;

int main() {
   int max = MAX_VALUE, mult = 1;
   int print = 1, i, j, count = 0;
   Number *list;
   clock_t begin, end;
   double time;


   list = malloc(sizeof(Number) * max);

   for(i = 0; i < max; i++) {
      list[i].num = i;
      list[i].true = 1;
   }  
   
   begin = clock(); // for timing

   /* This is the actual algorithm!
    * Printing stuff is included here as well */
   for(i = 2; i < sqrt(max); i++) {
      if(list[i].true) {
         for(j = i * i; j < max; j = (i * i) + (mult++ * i)) {
            list[j].true = 0;
         }
         mult = 1;
      }
   }
   
   end = clock(); // for timing
   
   if (print)
      printf("\nPrime numbers below %d:\n", max);
   for(i = 0; i < max; i++) {
      if(list[i].true) {
         if (print)
            printf("%d\n", list[i].num);
         count++;
      }
   } 
   if (print)
      printf("\n");
   
   time = (double)(end - begin) / CLOCKS_PER_SEC;

   printf("Calculation time: %.4lf seconds\n", time);
   printf("Total primes: %d\n", count);

   return 0;
}