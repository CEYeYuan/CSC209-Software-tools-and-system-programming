
#include <stdio.h>

/* Write a quick program to tell me the size of this structure: */

struct s {
   double f;
   int i;
   char c[3];
   void *p;
   int x[0];
}

int main( int argc, char ** argv ) {

}

============================================================================
/* What does this do?
#define ABC(x) aaa ## x

ABC(b)
*/

============================================================================
/* Find the size */
int main() {
   const char *s1 = "hello";
   char s2[] = "world";

   printf( "sizeof s1 : %d sizeof s2 : %d\n", sizeof(s1), sizeof(s2) );
   printf( "lenof s2 : %d\n", strlen(s2) );
}

============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char* argv[] ){
    char* s1 = "STRING1";
    char s2[] = "STRING2";
    char *s3 = NULL;

    s1[ 1 ] = 'P';
    s2[ 1 ] = 'P';

    s3 = malloc(5);
    strcpy(s3, "test");
    s3[1]= 'P';
}

============================================================================
/* What is the difference between these variables? */                                                           

int i1;
int j=0;

int main() {
   const char *s1 = "hello world";
   char s2[] = "hellow world";
   static int i2;
   static int k=1;
}
