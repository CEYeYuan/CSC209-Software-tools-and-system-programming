#include <stdio.h>
#include <malloc.h>
#include <assert.h>

typedef struct node_t {
  int val;
  struct node_t *next;
} node;

/* For all these examples the <var>1 is the input list <var>2 is the 
   output after removing all 2s */
int a1[] = { 2, 2 };
int a2[] = { };

int b1[] = { 3, 2 };
int b2[] = { 3 };

int c1[] = { 3, 2, 4, 2 };
int c2[] = { 3, 4 };

node*  makeList( int * a, int l ) {
  int i = 1;
  node * cur = NULL;
  node * head = NULL;
  if( l == 0 )
    return NULL;
  head = malloc( sizeof( node ) );
  head->val = a[0];
  cur = head;
  while( i < l ){
    cur->next = malloc( sizeof ( node ) );
    cur = cur->next;
    cur->val = a[ i ];
    i++;
  }
  cur->next = NULL;
  return head;
}

int verifyList( node* n, int * a, int l ){
  int i = 0;
  while ( i < l && n ) {
    if( n->val != a[ i ] ){
      return 0;
    }
    i++;
    n = n->next;
  }
  return i == l && !n;
}

// Runs a test that checks that for a given input/expected array
//   we get the expected arrays when all is said and done
int check( int* a1, int l1, int*a2, int l2 ) {
  node *l1 = makeList( a1, l1 );
  node *l2 = makeList( a2, l2);
}