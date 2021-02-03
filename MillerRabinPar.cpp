#include <iostream>
#include "MillerRabinPar.hpp"
#include "MillerRabinSeq.hpp"
#include <pthread.h>

using namespace std;

//typedef struct thread_data {
//    vector<tuple<mpz_class, mpz_class>> intervals;
//    vector<mpz_class> result;
//} thread_data;
//
//
///***
// * Each threads handle one Interval. Thus, in this version interval are not splitted
// * @param intervals
// */
//void *MillerRabinPar::workerOnIntervals(void *threadData) {
//    auto *tData = (thread_data *) threadData;
//    vector<mpz_class> primeNumbers;
//    vector<tuple<mpz_class, mpz_class>> intervals = tData->intervals;
//
//    primeNumbers = MillerRabinSeq::computePrime(intervals);
//    // TODO stock prime Number in tData->result
//
//    return nullptr;
//}
//
//template<typename T>
//vector<vector<T>> MillerRabinPar::splitVector(const vector<T> &vec, size_t n) {
//    vector<vector<T>> outVec;
//
//    size_t length = vec.size() / n;
//    size_t remain = vec.size() % n;
//
//    size_t begin = 0;
//    size_t end = 0;
//
//    for (size_t i = 0; i < min(n, vec.size()); ++i) {
//        end += (remain > 0) ? (length + !!(remain--)) : length;
//        outVec.push_back(vector<T>(vec.begin() + begin, vec.begin() + end));
//        begin = end;
//    }
//
//    return outVec;
//}
//
//thread_data MillerRabinPar::pthreadExec(int THREAD_NUMBER, const vector<tuple<mpz_class, mpz_class>> &intervals) {
//    pthread_t threads[THREAD_NUMBER];
//    thread_data thread_data_array[THREAD_NUMBER];
//
//    vector<vector<tuple<mpz_class, mpz_class>>> sp = splitVector(intervals, THREAD_NUMBER);
//    // adapt threadNumber to number of intervals
//    if (sp.size() < THREAD_NUMBER)
//        THREAD_NUMBER = sp.size();
//
//    for (int t = 0; t < THREAD_NUMBER; t++) {
//        /// Split in even interval
//        thread_data_array[t].intervals = sp[t];
//        pthread_create(&threads[t], nullptr, workerOnIntervals, (void *) &thread_data_array[t]);
//    }
//
//    for (int i = 0; i < THREAD_NUMBER; i++) {
//        pthread_join(threads[i], nullptr);
//    }
//}
//
//
///***
// * Number of threads are cut in half. A half handle the intervals. And the other half handle the splitting of intervals.
// * @param intervals
// */
//void MillerRabinPar::workerOnIntervalsWithSplit() {
//
//}



