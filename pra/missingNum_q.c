#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int a[] = { 4,5,7,8,9 };
int b[] = { 4,5,6,8,9,10};
int c[] = { 4,6,7,8,9,10};
int d[] = { 4,5,6,7,9,10};
int e[] = { 4,5,6,7,8,9};


int findMissing( int* arr, int len ) {
    int first,middle,last;
    first = 0;
    last = len-1;
    while (first < last) {
         middle = (first+last)/2;
        if ((arr[middle]-arr[first]) != (middle - first)) {
            /* there is a hole in the first half */
            if ((middle-first) == 1 && (arr[middle]-arr[first] > 1)) {
                return (arr[middle]-1);
            }   
            last = middle;
        }   
        else if ((arr[last]-arr[middle]) != (last-middle)) {
            /* there is a hole in the second half */
            if ((last-middle) == 1 && (arr[last]-arr[middle] > 1)) {
                return (arr[middle]+1);
            }   
            first = middle;
        }   
        else
            return arr[0]+len;  
    }   
}
main() {

   assert( findMissing( a, 5 ) == 6 );
   assert( findMissing( b, 6 ) == 7 );
   assert( findMissing( c, 6 ) == 5 );
   assert( findMissing( d, 6 ) == 8 );
   assert( findMissing( e, 6 ) == 10 );
}
