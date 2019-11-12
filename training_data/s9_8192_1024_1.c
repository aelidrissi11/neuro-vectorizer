#include "header.h"

int Out[8192][1024];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<8192; i++) {
     for (j=0; j<1024; j++) {
       Out[i][j] = x;
     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&Out[0][0], &Out[0][1024]);
  BENCH("Example8",   example8(8), 128, digest_memory(&Out[0][0], &Out[0][1024]));
 
  return 0;
}