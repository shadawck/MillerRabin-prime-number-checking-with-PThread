#include <iostream>
#include "MillerRabinPar.hpp"
#include <pthread.h>
using namespace std;


MillerRabinPar::MillerRabinPar(int threadNumber, const vector<tuple<mpz_class, mpz_class>>& intervals) {
}


/***
 * Each threads handle one Interval. Thus, in this version interval are not splitted
 * @param intervals
 */
void *MillerRabinPar::workerOnIntervals(void * intervalsId) {

}

/***
 * Number of threads are cut in half. A half handle the intervals. And the other half handle the splitting of intervals.
 * @param intervals
 */
void MillerRabinPar::workerOnIntervalsWithSplit() {

}

void MillerRabinPar::pthread_exec(vector<tuple<mpz_class, mpz_class>> intervals) {
}
