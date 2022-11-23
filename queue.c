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
   double departureTime = SIM_TIME; // Time for next departure

   int queues[2] = {0}; // initially each of sthe two queues are in state 0
   int droppedPackets = 0; // number of dropped packets
   int numCustomers = 0; // number of customers in the system
   int qn; // buffer number that the incoming packet will go to

   // Run The Simulation
   while (elapsedTime < SIM_TIME)
   {
    
      // (1) if an arrival happens before a departure
      if (arrivalTime < departureTime)                
      {
         elapsedTime = arrivalTime; // update the elapsed time
         numCustomers += 1; // increase the number of customers in system
         arrivalTime = elapsedTime + rand_exp(ARR_TIME); // get the time of the next arrival
         if (numCustomers == 1) // if there is only one customer, service it
         {
            departureTime = elapsedTime + rand_exp(ARR_TIME); // get the time after service completes
         }
      }

      // (2) if a departure happens before an arrival
      else
      {
         elapsedTime = arrivalTime; // update the elapsed time
         numCustomers -= 1; // decrease the number of customers in system

         // if there are any packets in the server
         if (numCustomers > 0)
         {
            arrivalTime = elapsedTime + rand_exp(ARR_TIME); // get the time of the next arrival
         }
         else
         {
            arrivalTime = SIM_TIME;
         }

      }
   }
}
      
      
      
      
      
      
      
      
      
      // Packet Arrived
      // elapsedTime += rand_exp(ARR_TIME);

      // Assign it to a queue randomly
      // qn = rand() % 2; // number queue it should go into

      // if there is less than 10 items in the queue, add the packet to the queue
      // if (queues[qn] < BUFF_SIZE)
      // {
      //    queues[qn] += 1;
      // }
      // else
      //    droppedPackets += 1;

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

   return 0;
}
