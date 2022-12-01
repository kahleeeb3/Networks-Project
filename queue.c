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
int assignPacket (int queue_len[]);
void strategy1();
void strategy2();
void Initilization();
void Arrival(double arrivalTime);
void Departure(double arrivalTime);

int main()
{

   srand(time(NULL)); // seed the random number
   strategy1();
   strategy2();

   return 0;
}
double elapsedTime = 0.0;        // current time in simulation
double arrivalTime = 0.0;        // Time for next arrival
double departureTime = INFINITY; // Time for next departure
double servicetime = 0.0;        // total service time
double totalwaitingTime;         // total waiting  time
double utilization;              // Utilization of serve
double throughput;               // Throughput
double timeNextEvent;
int    queues[2] = {0};          // initially each of sthe two queues are in state 0
int    qn;                       // buffer number that the incoming packet will go to
int    droppedPackets = 0;       // number of dropped packets
int    numCustomers = 0;         // number of customers in the system
int    CompletedService = 0;     // number of service completions, departured
int    numArrivals = 0; 
int    waitingCust = 0;          // customers who had to wait in line(counter)
   

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
   
   // get first packet arrival time
   arrivalTime = rand_exp(ARR_TIME);   
   timeNextEvent = math.min(arrivalTime, departureTime);
   while (elapsedTime < SIM_TIME)
   {
      if (arrivalTime < departureTime)           // (1) if a customer is arrival, when arrival time < departure time               
         Arrival(arrivalTime);
      else                                     // (2) if a customer is departured, if departure time < arrival time
         Departure(arrivalTime);
   
}
      Simulation();

}

void Arrival (double arrivalTime){
   qn = assignPacket(queue_len);       // Assign it to a queue depending on min_len
   totalwaitingTime = totalwaitingTime + queue_len[qn] * (timeNextEvent - elapsedTime);
   elapsedTime = timeNextEvent;
   if (queues[qn] < BUFF_SIZE)         // if there is less than 10 items in the queue, add the packet to the queue
   {
      numCustomers += 1;               // increase the number of customers in system
      numArrivals += 1;
      queues[qn] += 1;                 // increase the number of customers in queue
      queue_len[qn] += 1;              // increase the length of the queue 
      waitingCust += 1;
   }
   else
      droppedPackets += 1;             //if the buffer is full, drop it

   if (numCustomers == 1)                                         // if there is only one customer, service it
   {
      double rand = rand_exp(SERV_TIME);
      departureTime = elapsedTime + rand;          // get the time after this service completes
      totalwaitingTime += rand;                   ///?? check again
   }   
   arrivalTime = elapsedTime + rand_exp(ARR_TIME);                // get the time of the next arrival
}

void Departure(double arrivalTime){
   numCustomers -= 1;                     // decrease the number of customers in system
   queue_len[qn] -= 1;                    // NEED TO REMOVE FROM ONE QUEUE
   CompletedService+=1;                   // Increase number of completed service by 1 when it departured 
   if (numCustomers > 0)                  // if there are any packets in the server
   {
      double rand = rand_exp(SERV_TIME);
      departureTime = elapsedTime + rand;         // get the time of the next arrival
      totalwaitingTime +=  rand;
   }  
   else
      departureTime = INFINITY;           // need to wait for another arrival
}

void Simulation(){
   throughput  = CompletedService/ elapsedTime;
   AvgInterarrivalTime = elapsedTime/numArrivals;
   AvgServiceTime = CompletedService/ CompletedService;
   waitingTime = totalwaitingTime;

   // Simulation Over
   printf("queues:[%d,%d]\n",queues[0],queues[1]);
   printf("numCustomers:%d\n",numCustomers);
   printf("droppedPackets:%d\n",droppedPackets);
   printf("throughput:%d\n",throughput);
   printf("AvgServiceTime:%d\n",AvgServiceTime);
   printf("waitingTime:%d\n",waitingTime);
   printf("AvgInterarrivalTime:%d\n",AvgInterarrivalTime);
   
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

   // get first packet arrival time
   arrivalTime = rand_exp(ARR_TIME);   
   
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

