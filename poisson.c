/* Function to generate a Poisson-distributed random number using Knuth's algorithm
 
 See: https://en.wikipedia.org/wiki/Poisson_distribution#Generating_Poisson-distributed_random_variables
 
 For small lambdas that we assume for photon-starved communications, Knuth's algorithm will usually call rand() only once, so it'll be just about as computationally efficient as more sophisticated algorithms (such as inversion by sequential search).

Ian Morrison
March 2021

*/
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


int32_t poisson(double L)
{
    double p = 1.0;     // intermediate probability value
    double u;           // uniform RV
    int32_t k = -1;     // the Poisson RV to be returned
    
    // generate a Poisson-distributed number
    do
    {
        k++;
        u = (double)rand()/(double)RAND_MAX;  // uniformly disributed random number in range [0,1]
        p = p * u;
    }
    while (p > L);
    
    return k;
  }
