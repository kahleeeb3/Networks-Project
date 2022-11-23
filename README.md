# Networks-Project

Compile

```
gcc -g queue.c -o queue -lm
./queue
```

Debug

```
gcc -g queue.c -o queue -lm
gdb ./queue
```

## Main Loop

```C
while (elapsedTime < SIM_TIME)
{

   // (1) if an arrival happens before a departure
   if (arrivalTime < departureTime)
   {
      elapsedTime = arrivalTime; // update the elapsed time
      numCustomers += 1; // increase the number of customers in system

      // if there is only one customer get the time of it's service
      if (numCustomers == 1)
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
```
