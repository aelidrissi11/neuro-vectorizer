#include "header.h"

int Output[8192][256];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<8192-1; i+=2) {
     for (j=0; j<256-1; j+=2) {
       Output[i][j] = x;
       Output[i+1][j] = x;
       Output[i][j+1] = x;
       Output[i+1][j+1] = x;

     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&Output[0][0], &Output[0][256]);
  BENCH("Example8",   example8(8), 512, digest_memory(&Output[0][0], &Output[0][256]));
 
  return 0;
}