
#include "header.h"
int b[2048] ALIGNED16;
__attribute__((noinline))
void example2a (int x) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 2048-3 ; i+=4) {
      b[i] = x;
b[i+1] = -x;
b[i+2]=x;
b[i+3]=-x;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&b[0], &b[2048]);
  BENCH("Example2a",  example2a(2), Mi*4/2048*512, digest_memory(&b[0], &b[2048]));
  
  return 0;
}