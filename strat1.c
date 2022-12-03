// Includes
#include <stdio.h>  // Needed for printf()
#include <stdlib.h> // Needed for rand()
#include <math.h>   // Needed for log()
#include <time.h>   // Needed for srand()

// Constants
#define BUFF_SIZE 10        // Size of the Buffer
// #define ARR_TIME 1.11111111 // Mean time between arrivals (λ)
// #define SERV_TIME 1.00      // Mean service time (μ)

#define ARR_TIME 1.00 // Mean time between arrivals (λ)
#define SERV_TIME 0.10  // Mean service time (μ)

// function declaration
double rand_exp(double lambda); // Generate a exponential RV

void strategy1()
{
   // 1) Random selection, which assigns an incoming packet uniformly randomly to one of the queues.
   // This means a packet has an equal chance of being in either subqueue.

   double elapsedTime = 0.0; // current time in simulation
   double arrivalTime = 0.0; // Time for next arrival
   double departureTime1 = INFINITY; // Time for next departure from server 1
   double departureTime2 = INFINITY; // Time for next departure from server 2

   int queues[2] = {0}; // initially each of the two queues are in state 0
   int droppedPackets = 0; // number of dropped packets
   int numCustomers = 0; // number of customers in the system
   int qn; // buffer number that the incoming packet will go to


   long double queLenSum = 0; // sum of que length
   long double t1 = 0.0;
   long double t2 = 0.0;
   long double v1 = 0.0;
   long double v2 = 0.0;
   long double A1 = 0.0;
   long double A2 = 0.0;

   
   long double ArrTimes[2][10]={0}; // stores the arrival times of packets
   long double sojournTime = 0; // stores the sojourn Time of a packet

   // get first packet arrival time
   // arrivalTime = rand_exp(ARR_TIME);   
   
   // Run The Simulation
   while ((numCustomers+droppedPackets)<1E4)
   {
    
      // (1) if an arrival happens before a departure
      if ( (arrivalTime < departureTime1) && (arrivalTime<departureTime2) )                
      {
         
         elapsedTime = arrivalTime; // update the elapsed time

         /////////////////////////////////////////////////////////////////////////////////
         t2 = elapsedTime;
         v2 = queues[0];
         
         A1 = (t2-t1)*v1; // square section
         A2 = (t2-t1)*(v2-v1)/2; // triangle section

         queLenSum += (A1 + A2);

         t1 = elapsedTime;
         v1 = queues[0];
         /////////////////////////////////////////////////////////////////////////////////
         
         
         // Assign it to a queue randomly
         qn = rand() % 2; // number queue it should go into
          
         // if there is less than 10 items in the queue, add the packet to the queue
         if (queues[qn] < BUFF_SIZE)
         {
            numCustomers += 1; // increase the number of customers in system
            queues[qn] += 1; // increase the number of customers in queue
            push(ArrTimes[qn],arrivalTime);
         }
         else
            droppedPackets += 1; // not enough space, drop packet
         
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

         /////////////////////////////////////////////////////////////////////////////////
         t2 = elapsedTime;
         v2 = queues[0];
         
         A1 = (t2-t1)*v1; // square section
         A2 = (t2-t1)*(v2-v1)/2; // triangle section

         queLenSum += (A1 + A2);

         t1 = elapsedTime;
         v1 = queues[0];
         /////////////////////////////////////////////////////////////////////////////////

         queues[0] -= 1; // decrease the number of customers in queue 1

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

          /////////////////////////////////////////////////////////////////////////////////
         t2 = elapsedTime;
         v2 = queues[0];
         
         A1 = (t2-t1)*v1; // square section
         A2 = (t2-t1)*(v2-v1)/2; // triangle section

         queLenSum += (A1 + A2);

         t1 = elapsedTime;
         v1 = queues[0];
         /////////////////////////////////////////////////////////////////////////////////


         queues[1] -= 1; // decrease the number of customers in queue 2
         sojournTime += elapsedTime - pop(ArrTimes[1]);

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

   long double blockProb = (long double)droppedPackets/(long double)(numCustomers+droppedPackets);
   long double avgQueLen = queLenSum/elapsedTime;
   long double AvgSojournTime = queLenSum/numCustomers;

   
   // printf("queues:[%d,%d]\n",queues[0],queues[1]);
   // printf("Run Time: %f\n",elapsedTime);
   
   // printf("Blocking Probability %i/%i\n", droppedPackets,(numCustomers+droppedPackets));
   // printf("Blocking Probability %.8Lf\n", blockProb);

   // printf("Avg Que Length %Lf/%f\n", queLenSum,elapsedTime);
   
   // printf("Avg Que Length %.8Lf\n", avgQueLen);

   // long double AvgSojournTime = sojournTime/(numCustomers);
   // printf("AvgSojournTime: %.8Lf\n", AvgSojournTime);


   // printf("Load (rho): %f\n",ARR_TIME/(2*SERV_TIME));
   // printf("%.8Lf, ", blockProb);
   // printf("%.8Lf, ", avgQueLen);
   printf("%.8Lf, ", AvgSojournTime);
   
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

   int loops = 1; // number of trials to run

   int i;
   for(i = 0; i<loops; i++){
      strategy1();
   }
   printf("\n");
   return 0;
}