// Includes
#include <stdio.h>  // Needed for printf()
#include <stdlib.h> // Needed for rand()
#include <math.h>   // Needed for log()
#include <time.h>   // Needed for srand()

// Constants
#define BUFF_SIZE 10        // Size of the Buffer
#define SIM_TIME 100        // How long the sim should run
#define ARR_TIME 1.11111111 // Mean time between arrivals (λ)
#define SERV_TIME 1.00      // Mean service time (μ)

// function declaration
double rand_exp(double lambda); // Generate a exponential RV

void strategy2()
{
   printf("strategy 2\n");
}

void strategy1()
{
   // 1) Random selection, which assigns an incoming packet uniformly randomly to one of the queues.
   // This means a packet has an equal chance of being in either subqueue.

   double elapsedTime = 0.0; // current time in simulation
   double arrivalTime = 0.0; // Time for next arrival
   double departureTime = INFINITY; // Time for next departure

   int queues[2] = {0}; // initially each of sthe two queues are in state 0
   int droppedPackets = 0; // number of dropped packets
   int numCustomers = 0; // number of customers in the system
   int qn; // buffer number that the incoming packet will go to

   // wait for the first incoming packet
   arrivalTime = rand_exp(ARR_TIME);
   elapsedTime += arrivalTime;
   
   
   // Run The Simulation
   while (elapsedTime < SIM_TIME)
   {
    
      // (1) if an arrival happens before a departure
      if (arrivalTime < departureTime)                
      {
         elapsedTime = arrivalTime; // update the elapsed time
         
         // Assign it to a queue randomly
         qn = rand() % 2; // number queue it should go into

         // if there is less than 10 items in the queue, add the packet to the queue
         if (queues[qn] < BUFF_SIZE)
         {
            numCustomers += 1; // increase the number of customers in system
            queues[qn] += 1; // increase the number of customers in queue
         }
         else
            droppedPackets += 1;

         if (numCustomers == 1) // if there is only one customer, service it
         {
            departureTime = elapsedTime + rand_exp(SERV_TIME); // get the time after service completes
         }
         
         arrivalTime = elapsedTime + rand_exp(ARR_TIME); // get the time of the next arrival
      }

      // (2) if a departure happens before an arrival
      else
      {
         elapsedTime = departureTime; // update the elapsed time
         numCustomers -= 1; // decrease the number of customers in system

         // NEED TO REMOVE FROM ONE QUEUE

         // if there are any packets in the server
         if (numCustomers > 0)
         {
            departureTime = elapsedTime + rand_exp(SERV_TIME); // get the time of the next arrival
         }
         else
         {
            departureTime = INFINITY; // need to wait for another arrival
         }

      }
   }
   // Simulation Over
   printf("queues:[%d,%d]\n",queues[0],queues[1]);
   printf("numCustomers:%d\n",numCustomers);
   printf("droppedPackets:%d\n",droppedPackets);
}
      
      
      
      
      
      
      
      
      
      // Packet Arrived
      // elapsedTime += rand_exp(ARR_TIME);

double rand_exp(double lambda)
{
   // returns a random numbers of an exponential distribution
   // our arrival process is a poisson process so the inter-arrival time follow exp dist
   // our service time for a packet is also exponentially distributed

   // https://www.eg.bucknell.edu/~xmeng/Course/CS6337/Note/master/node50.html
   // Since the CDF of Exponential distribution is y = 1-e^(-λx)
   // We will find that x = -ln(1-y) / λ
   // where y is a uniform random distributed on (0,1)

   double y = rand() / (RAND_MAX + 1.0); // Random number from 0 to 1 excluding 1
   return (-log(1 - y) / lambda);
}

int main()
{

   srand(time(NULL)); // seed the random number
   strategy1();
   // strategy2();

   return 0;
}
