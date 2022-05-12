/* Test of Knuth's algorithm for generating Poisson-distributed random numbers
 
 See: https://en.wikipedia.org/wiki/Poisson_distribution#Generating_Poisson-distributed_random_variables
 
 For small lambdas that we assume for photon-starved communications, Knuth's algorithm will usually
 call rand() only once, so it'll be just about as computationally efficient as more sophisticated
 algorithms (such as inversion by sequential search).

Ian Morrison
March 2021

*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>

#include "poisson.h"


void usage ()
{
  fprintf(stdout, "poisson-test [options]\n"
    "\n"
    "  Generate some Poisson-distributed numbers for specified lambda\n"
    "\n"
    "  -n <num>    number of numbers to generate [default 100]\n"
    "  -l <num>    lambda (= mean and variance) [default 0.2]\n"
    "\n"
  );
}


int main(int argc, char **argv)
{
    int i;
    uint32_t num_numbers = 100;
    double lambda = 0.2;
    double L;           // e^^(-lambda)
    double p;           // intermediate probability value
    double u;           // uniform RV
    int32_t k;          // the Poisson RV to be returned
    int32_t sum_k = 0;  // for checking mean of generated values (should be close to lambda)
    int32_t max_k = 0;  // for checking max value generated
    
    srand(5979229);
    
    // parse command line options
    int arg = 0;
    while ((arg = getopt(argc, argv, "n:l:")) != -1)
    {
      switch (arg)
      {
        case 'n':
          num_numbers = (uint32_t)atoi(optarg);
          break;

        case 'l':
          lambda = (double)atof(optarg);
          break;

        default:
          usage ();
          return 0;
      }
    }
   
    
    // initialise L
    L = exp(-lambda);
    
    for (i=0; i<num_numbers; i++)
    {
        k = poisson(L);   // function in poisson.c to generate a single poisson-distributed number

        printf("k = %d\n", k);
        sum_k+= k;
        if (k > max_k) max_k = k;
    }
    
    
#if 0  // in-line version
    
    // generate the Poisson-distributed numbers
    
    for (i=0; i<num_numbers; i++)
    {
        p = 1.0;
        k = -1;
        do
        {
            k++;
            u = (double)rand()/(double)RAND_MAX;  // uniformly disributed random number in range [0,1]
            p = p * u;
        }
        while (p > L);
        
        printf("k = %d\n", k);
        sum_k+= k;
        if (k > max_k) max_k = k;
    }
#endif
    
    printf("total of all k values = %d\n", sum_k);
    printf("mean of k = %f\n", (double)sum_k/(double)num_numbers);
    printf("max value of k = %d\n", max_k);
    
    return 0;
  }
