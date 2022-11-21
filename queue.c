// Includes
#include <stdio.h> // Needed for printf()
#include <stdlib.h> // Needed for rand()
#include <math.h> // Needed for log()
#include <time.h> // Needed for srand()

// Constants
#define BUFF_SIZE    10 // Size of the Buffer 
#define SIM_TIME     100 // How long the sim should run
#define ARR_TIME     1.11111111 // Mean time between arrivals (λ)
#define SERV_TIME    1.00 // Mean service time (μ)

// function declaration
double rand_exp(double lambda); // Generate a exponential RV


void strategy1(){
   
   int queues[2] = {0}; // initially each of the two queues are in state 0
   double elapsedTime = 0.0;
   int droppedPackets = 0;
   int qn = 0;

   // Run The Simulation
   while (elapsedTime < SIM_TIME){
      
      // Packet Arrived
      elapsedTime += rand_exp(ARR_TIME);

      // Assign it to a queue randomly
      qn = rand()%2; // number queue it should go into

      // if there is less than 10 items in the queue, add to it
      if(queues[qn] < BUFF_SIZE){
         queues[qn]+=1;
      }
      else droppedPackets += 1;

   }
   printf("queue 1: %i\n",queues[0]);
   printf("queue 2: %i\n",queues[1]);
   printf("dropped packets: %i\n",droppedPackets);

}

double rand_exp(double lambda)
{
   // returns a random numbers of an exponential distribution
   // our arrival process is a poisson process so the inter-arrival time follow exp dist
   // our service time for a packet is also exponentially distributed

   // https://www.eg.bucknell.edu/~xmeng/Course/CS6337/Note/master/node50.html
   // Since the CDF of Exponential distribution is y = 1-e^(-λx) 
   // We will find that x = -ln(1-y) / λ
   // where y is a uniform random distributed on (0,1)
   
   double y =  rand() / (RAND_MAX + 1.0); // Random number from 0 to 1 excluding 1
   return(-log(1-y) / lambda);
}


int main() {
   
   srand(time(NULL)); // seed the random number
   strategy1();

   return 0;
}
