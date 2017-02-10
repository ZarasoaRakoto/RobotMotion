#include "datatypes.h"

/* Global variable */
Raw_point *BASE;

typedef Raw_point *tPoint;

/*----------Stack Structure-------------*/
typedef struct tStackCell tsStack;
typedef tsStack *tStack;
struct tStackCell {
   tPoint   p;
   tStack   next;
};


/*----------Function Prototypes-------------*/
tStack	Pop( tStack s );
tStack  Push( tPoint p, tStack top );
void  Graham( Polygon *p);
void  Squash( Polygon *p );
int     Compare( const void *tp1, const void *tp2 );
void    FindLowest( Polygon *p );
int     AreaSign(Raw_point *a, Raw_point *b, Raw_point *c);


void  Graham( Polygon *p)
{
    FindLowest(p); // find lowest and put it on the top
    BASE = &p->points[0];
    qsort( &p->points[1], p->size - 1, sizeof(Raw_point), Compare);
    Squash(p); // delete points marqued for deletion
    tStack   top = NULL;
    tPoint p1, p2;  /* Top two points on stack. */
    top = Push ( &p->points[0], top );
    top = Push ( &p->points[1], top );

    /* Bottom two elements will never be removed. */
    int i = 2;
    while ( i < p->size ) {
        assert(top->next); // ensure at least 2 elements always remain
        p1 = top->next->p;
        p2 = top->p;
        if ( AreaSign(p1, p2, &p->points[i]) > 0) { // left turn
            top = Push (&p->points[i], top );
            i++;
        } 
        else {
            p2->key = 0; // mark for deletion
            top = Pop( top );
        }    
    }
    Squash(p); // delete points marqued for deletion
    // re-flling the Polygon

}

void   FindLowest( Polygon *p ){
   int i;
   int m = 0;   /* Index of lowest so far. */
   for ( i = 1; i < p->size; i++ )
      if ( (p->points[i].y <  p->points[m].y) ||
          ((p->points[i].y == p->points[m].y) && (p->points[i].x > p->points[m].x)) ) 
         m = i;
   
    if (m) {   /* Swap P[0] and P[m] */
        Raw_point rp = p->points[0];
        p->points[0] = p->points[m];
        p->points[m] = rp;
    }
}

/*---------------------------------------------------------------------
Compare: returns -1,0,+1 if p1 < p2, =, or > respectively;
here "<" means smaller angle.  Follows the conventions of qsort.
---------------------------------------------------------------------*/
int   Compare( const void *tpi, const void *tpj ){
   int a;             /* area */
   int x, y;          /* projections of ri & rj in 1st quadrant */
   Raw_point *pi, *pj;
   pi = (Raw_point *)tpi;
   pj = (Raw_point *)tpj;

   a = AreaSign(BASE, pi, pj );
   if (a > 0)
      return -1;
   else if (a < 0)
      return 1;
   else { /* Collinear with BASE */
      x =  abs( pi->x - BASE->x ) - abs( pj->x - BASE->x );
      y =  abs( pi->y - BASE->y ) - abs( pj->y - BASE->y );
      if ( (x < 0) || (y < 0) ) {
         pi->key = 0;  // mark for deletion
         return -1;
      }
      else if ( (x > 0) || (y > 0) ) {
         pj->key = 0;  // mark for deletion
         return 1;
      }
      else { /* points are coincident */
         if (pi->key) pj->key = 0; // ensure one point remain
         return 0;
      }
   }
}

tStack   Pop( tStack s ) {
    tStack top = NULL;
    if(s){
        if(s->next) top = s->next;
        free(s); s = NULL;
    }
   return top;
}

tStack   Push( tPoint p, tStack top ) {
   tStack s = malloc(sizeof(tsStack)); assert(s);
   s->p = p;
   s->next = top;
   return s;
}

/*---------------------------------------------------------------------
Squash removes all elements from P marked delete.
---------------------------------------------------------------------*/
void   Squash( Polygon *p){
   int i=0, j=0;
   while ( i < p->size ) {
      if ( p->points[i].key ) { /* if not marked for deletion */
         p->points[j].x = p->points[i].x;
         p->points[j].y = p->points[i].y;
         p->points[j].key = p->points[i].key;
         j++;
      }
      /* else do nothing: delete by skipping. */
      i++;
   }
   p->size = j;
   p->points = realloc(p->points, p->size * sizeof(Raw_point));
   assert(p->points);
}

int AreaSign( Raw_point *a, Raw_point *b, Raw_point *c ){
    double area2 = ( b->x - a->x ) * (double)( c->y - a->y ) -
                   ( c->x - a->x ) * (double)( b->y - a->y );
    if      ( area2 >  0.5 ) return  1;
    else if ( area2 < -0.5 ) return -1;
    else                     return  0;
}
