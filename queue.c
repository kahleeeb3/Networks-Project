#include <stdio.h>

void print_array(int *a){
   int i;
   int n = sizeof a;
   printf("[");
   for(i=0; i<n; i++){

      printf("%d",*a);
      if(i<n-1) printf(", ",*a);

      a++;          //for incrementing the position of array.
   }
   printf("]\n");
}

int main() {

   // Create the Queues
   int q1[10] = {0}; // queue 1
   int q2[10] = {0}; // queue 2

   int numPackets = 10; // specify the number of packets to arrive

   // recv the packets
   int i; // decelare i for the iteration
   for(i = 1;i<=numPackets;i++){
      
      // 50/50 chance
      if ((rand() % 2) + 1 ==  2){
         q1[i] = 1;
      }

   }

   print_array(q1);
   

   return 0;
}
