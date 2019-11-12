
#include "header.h"
int b[4096] ALIGNED16;
__attribute__((noinline))
void example2a (int x) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 4096-1 ; i+=2) {
      b[i] = x;
b[i+1] = -x;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&b[0], &b[4096]);
  BENCH("Example2a",  example2a(2), Mi*4/4096*512, digest_memory(&b[0], &b[4096]));
  
  return 0;
}