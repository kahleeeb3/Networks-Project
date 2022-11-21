// TAKEN From https://cse.usf.edu/~kchriste/tools/mm1.c

//=========================================================== file = mm1.c =====
//=  A simple M/M/1 queue simulation                                           =
//==============================================================================
//=  Notes:                                                                    =
//=   1) This program is adapted from Figure 1.6 in Simulating Computer        =
//=      Systems, Techniqyes and Tools by M. H. MacDougall (1987).             =
//=   2) The values of SIM_TIME, ARR_TIME, and SERV_TIME need to be set.       =
//=----------------------------------------------------------------------------=
//=  Build: Standard C build                                                   =
//=----------------------------------------------------------------------------=
//=  Execute: mm1                                                              =
//=----------------------------------------------------------------------------=
//=  History: KJC (03/09/99) - Genesis                                         =
//=           KJC (05/23/09) - Added RNG function (so not to use compiler RNG) =
//=           KJC (05/24/09) - Added updated of busy time at end of main loop  =
//=           KJC (12/08/19) - Fixed total simulation time to be time          =
//==============================================================================
//----- Include files ----------------------------------------------------------
#include <stdio.h>              // Needed for printf()
#include <stdlib.h>             // Needed for exit() and rand()
#include <math.h>               // Needed for log()

//----- Constants --------------------------------------------------------------
#define SIM_TIME   1.0e7        // Simulation time
#define ARR_TIME   1.11111111   // Mean time between arrivals
#define SERV_TIME  1.00         // Mean service time

//----- Function prototypes ----------------------------------------------------
double rand_val(int seed);      // RNG for unif(0,1)
double exponential(double x);   // Generate exponential RV with mean x

//===== Main program ===========================================================
int main(void)
{
  double   end_time = SIM_TIME; // Total time to simulate
  double   Ta = ARR_TIME;       // Mean time between arrivals
  double   Ts = SERV_TIME;      // Mean service time
  double   time = 0.0;          // Simulation time
  double   t1 = 0.0;            // Time for next event #1 (arrival)
  double   t2 = SIM_TIME;       // Time for next event #2 (departure)
  unsigned int n = 0;           // Number of customers in the system
  unsigned int c = 0;           // Number of service completions
  double   b = 0.0;             // Total busy time
  double   s = 0.0;             // Area of number of customers in system
  double   tn = time;           // Variable for "last event time"
  double   tb;                  // Variable for "last start of busy time"
  double   x;                   // Throughput
  double   u;                   // Utilization
  double   l;                   // Mean number in the system
  double   w;                   // Mean residence time

  // Seed the RNG
  rand_val(1);

  // Main simulation loop
  while (time < end_time)
  {
    if (t1 < t2)                // *** Event #1 (arrival) ***
    {
      time = t1;
      s = s + n * (time - tn);  // Update area under "s" curve
      n++;
      tn = time;                // tn = "last event time" for next event
      t1 = time + exponential(Ta);
      if (n == 1)
      {
        tb = time;              // Set "last start of busy time"
        t2 = time + exponential(Ts);
      }
    }
    else                        // *** Event #2 (departure) ***
    {
      time = t2;
      s = s + n * (time - tn);  // Update area under "s" curve
      n--;
      tn = time;                // tn = "last event time" for next event
      c++;                      // Increment number of completions
      if (n > 0)
        t2 = time + exponential(Ts);
      else
      {
        t2 = SIM_TIME;
        b = b + time - tb;      // Update busy time sum if empty
      }
    }
  }

  // End of simulation so update busy time sum
  b = b + time - tb;

  // Compute outputs
  x = c / time;   // Compute throughput rate
  u = b / time;   // Compute server utilization
  l = s / time;   // Compute mean number in system
  w = l / x;      // Compute mean residence or system time

  // Output results
  printf("=============================================================== \n");
  printf("=            *** Results from M/M/1 simulation ***            = \n");
  printf("=============================================================== \n");
  printf("=  Total simulated time         = %f sec     \n", time);
  printf("=============================================================== \n");
  printf("=  INPUTS:                                    \n");
  printf("=    Mean time between arrivals = %f sec      \n", Ta);
  printf("=    Mean service time          = %f sec      \n", Ts);
  printf("=============================================================== \n");
  printf("=  OUTPUTS:                                   \n");
  printf("=    Number of completions      = %d cust     \n", c);
  printf("=    Throughput rate            = %f cust/sec \n", x);
  printf("=    Server utilization         = %f %%       \n", 100.0 * u);
  printf("=    Mean number in system      = %f cust     \n", l);
  printf("=    Mean residence time        = %f sec      \n", w);
  printf("=============================================================== \n");

  return(0);
}

//=========================================================================
//= Multiplicative LCG for generating uniform(0.0, 1.0) random numbers    =
//=   - x_n = 7^5*x_(n-1)mod(2^31 - 1)                                    =
//=   - With x seeded to 1 the 10000th x value should be 1043618065       =
//=   - From R. Jain, "The Art of Computer Systems Performance Analysis," =
//=     John Wiley & Sons, 1991. (Page 443, Figure 26.2)                  =
//=   - Seed the RNG if seed > 0, return a unif(0,1) if seed == 0         =
//=========================================================================
double rand_val(int seed)
{
  const long  a =      16807;  // Multiplier
  const long  m = 2147483647;  // Modulus
  const long  q =     127773;  // m div a
  const long  r =       2836;  // m mod a
  static long x;               // Random int value (seed is set to 1)
  long        x_div_q;         // x divided by q
  long        x_mod_q;         // x modulo q
  long        x_new;           // New x value

  // Seed the RNG
  if (seed != 0) x = seed;

  // RNG using integer arithmetic
  x_div_q = x / q;
  x_mod_q = x % q;
  x_new = (a * x_mod_q) - (r * x_div_q);
  if (x_new > 0)
    x = x_new;
  else
    x = x_new + m;

  // Return a random value between 0.0 and 1.0
  return((double) x / m);
}

//==============================================================================
//=  Function to generate exponentially distributed RVs using inverse method   =
//=    - Input:  x (mean value of distribution)                                =
//=    - Output: Returns with exponential RV                                   =
//==============================================================================
double exponential(double x)
{
  double z;                    // Uniform random number from 0 to 1

  // Pull a uniform RV (0 < z < 1)
  do
  {
    z = rand_val(0);
  }
  while ((z == 0) || (z == 1));

  return(-x * log(z));
}