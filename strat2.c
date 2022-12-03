// Includes
#include <stdio.h>  // Needed for printf()
#include <stdlib.h> // Needed for rand()
#include <math.h>   // Needed for log()
#include <time.h>   // Needed for srand()

// Constants
#define BUFF_SIZE 10        // Size of the Buffer
#define SIM_TIME 100        // How long the sim should run
// #define ARR_TIME 1.11111111 // Mean time between arrivals (λ)
// #define SERV_TIME 1.00      // Mean service time (μ)

#define ARR_TIME 2.11 // Mean time between arrivals (λ)
#define SERV_TIME 1.0      // Mean service time (μ)

// function declaration
double rand_exp(double lambda); // Generate a exponential RV


int assignPacket (int queue_len[])
{
   int qn = -1;
   if (queue_len[0] < queue_len[1])
            qn=0;
         else
            qn = 1;
   return qn;
}


void strategy2()
{
   printf("strategy 2\n");
   double elapsedTime = 0.0; // current time in simulation
   double arrivalTime = 0.0; // Time for next arrival
   double departureTime1 = INFINITY; // Time for next departure from server 1
   double departureTime2 = INFINITY; // Time for next departure from server 2
   int queues[2] = {0}; // initially each of the two queues are in state 0
   int droppedPackets = 0; // number of dropped packets
   int numCustomers = 0; // number of customers in the system
   int qn; // buffer number that the incoming packet will go to
   int queue_len[2] = {0};
   int numArrivals = 0;
   double SerjounTime = 0.0;
   int queueLengthSum = 0;
   // get first packet arrival time
   arrivalTime = rand_exp(ARR_TIME);   
   
   // Run The Simulation
   while (elapsedTime < SIM_TIME)
   {
      // (1) if an arrival happens before a departure
      if ( (arrivalTime < departureTime1) && (arrivalTime<departureTime2) )                
      {
         elapsedTime = arrivalTime; // update the elapsed time
         
         // Assign it to a queue randomly
         qn = assignPacket(queue_len);       // Assign it to a queue depending on min_len
          if ((qn == 0) && (queues[0]>0) ){
            queueLengthSum += (queues[0]+1);
         }
         // if there is less than 10 items in the queue, add the packet to the queue
         if ((queues[qn] < BUFF_SIZE) )
         {
            numCustomers += 1; // increase the number of customers in system
            queues[qn] += 1; // increase the number of customers in queue
            numArrivals += 1;
            queue_len[qn] += 1;
         }
         else
         {
            droppedPackets += 1; // not enough space, drop packet
         }         
         // if there is only one customer in either server 1 or 2, then service it
         if (queues[0] == 1) // QUEUE 1
         {
            departureTime1 = elapsedTime + rand_exp(SERV_TIME); // get the time of serv 1 departure
         }

         if (queues[1] == 1) // QUEUE 2
         {
            departureTime2 = elapsedTime + rand_exp(SERV_TIME); // get the time of serv 2 departure
         }
         
         arrivalTime = elapsedTime + rand_exp(ARR_TIME); // get the time of the next arrival
      }

      // (2) if the time of departure from server 1 happens before server 2
      else if (departureTime1 < departureTime2){
         elapsedTime = departureTime1; // update the elapsed time
         queues[0] -= 1; // decrease the number of customers in queue 1
         queue_len[0] -= 1;
         // if there are any packets in the first queue
         if ( queues[0] > 0 )
         {
            departureTime1 = elapsedTime + rand_exp(SERV_TIME); // get the time of the next arrival
         }
         else // the first buffer is empty
         {
            departureTime1 = INFINITY; // need to wait for another arrival
         }
      }

      // (3) server 2 departs before server 1
      else
      {
         elapsedTime = departureTime2; // update the elapsed time
         queues[1] -= 1; // decrease the number of customers in queue 2
         queue_len[1] -= 1;
         // if there are any packets in the second queue
         if ( queues[1] > 0 )
         {
            departureTime2 = elapsedTime + rand_exp(SERV_TIME); // get the time of the next arrival
         }
         else // the second buffer is empty
         {
            departureTime2 = INFINITY; // need to wait for another arrival
         }

      }
   }
   // Simulation Over
   double rho = ARR_TIME/(2*SERV_TIME);
   long double avgQueLen = (long double)queueLengthSum/(long double)numCustomers;
   double QueueLength = queue_len[0]+queue_len[1]; 
   int blockedPackets = droppedPackets/avgQueLen;
   SerjounTime = avgQueLen/ARR_TIME;

   printf("Load (rho): %f\n",ARR_TIME/(2*SERV_TIME));
   printf("Avg Customer in Queue: %.8Lf\n", avgQueLen);
   printf("Blocked Packets: %d\n", blockedPackets);
   printf("queues: [%d,%d]\n",queues[0],queues[1]);
   printf("numCustomers: %d\n",numCustomers);
   printf("droppedPackets: %d\n",droppedPackets);
   printf("SerjounTime: %f\n", SerjounTime);

 

}

void strategy1()
{
   //do nothing
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

   double y = rand() / (RAND_MAX + 1.0); // Random number from 0 to 1 excluding 1
   return (-log(1 - y) / lambda);
}

int main()
{

   srand(time(NULL)); // seed the random number
   strategy2();

   return 0;
}