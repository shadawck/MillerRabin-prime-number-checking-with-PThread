#include <getopt.h>
#include <iostream>
#include <gmpxx.h>
#include <pthread.h>
#include <thread>
#include <chrono>

#include "ArgParse.hpp"
#include "FileParse.hpp"
#include "MillerRabinSeq.hpp"
#include "Chrono.hpp"

using namespace std;

void chronoExecution(Chrono &chInterval, Chrono &chSeq, Chrono &chPara) {
    double chronoSeq = chInterval.get() + chSeq.get();
    double chronoPar = chInterval.get() + chPara.get();

    cout << "\n--- Execution Time ---" << endl;

    cout << "Interval Optimization time : " << chInterval.get() << " sec" << endl;

    fprintf(stderr, "Total Sequential execution time : %f \n", chronoSeq);
    fprintf(stderr, "Total Parallel execution time : %f \n", chronoPar);

    cout << "SPEEDUP : " << chSeq.get() / chPara.get() << endl;
}

void printPrimeNumber(const vector<mpz_class> &primeNumbersPar) {
    for (const mpz_class& prime : primeNumbersPar) {
        cout << prime << " ";
    }
    cout << endl;
}

void primeNbDisplay(const vector<mpz_class> &primeNumbersSeq, const vector<mpz_class> &primeNumbersPar) {
    cout << "\n--- Prime Numbers ---" << endl;
    cout << primeNumbersSeq.size() << " Prime number found with sequential method" << endl;
    cout << primeNumbersPar.size() << " Prime number found with Parallel method" << endl << endl;

    printPrimeNumber(primeNumbersPar);
}

void handleInputs(int argc, char **argv, size_t &THREAD_NUMBER, char *&FILEPATH) {
    THREAD_NUMBER = 0;
    int args;
    int FILE_FLAG = 0;
    int THREAD_FLAG = 0;
    while ((args = getopt(argc, argv, "t:f:")) != -1) {
        auto ap = ArgParse(optarg);

        switch (args) {
            case 't' :
                THREAD_FLAG = 1;
                THREAD_NUMBER = ap.getParseInt();
                break;
            case 'f':
                FILE_FLAG = 1;
                FILEPATH = optarg;
                break;
            default:
                abort();
        }
    }
    if (!FILE_FLAG) {
        cout << "Please use -f <file_path> to load file." << endl;
        exit(0);
    }
    if (!THREAD_FLAG) {
        cout << "Default number of flag is 1." << endl;
    }
}

void inputPrint(const vector<tuple<mpz_class, mpz_class>> &intervals, int threadNb) {
    cout << "\n--- Optimized Interval ---" << endl;
    cout << intervals.size() << " Intervals to handle during prime computing" << endl;

    cout << "\n--- Threads Number --- " << endl;
    cout << threadNb << endl;
}

typedef struct thread_data {
    vector<tuple<mpz_class, mpz_class>> intervals;
    vector<mpz_class> result;
} thread_data;

/***
 * Each threads handle one Interval. Thus, in this version interval are not splitted
 * @param intervals
 */
void *workerOnIntervals(void *threadData) {
    auto *tData = (struct thread_data *) threadData;
    vector<mpz_class> primeNumbers;
    vector<tuple<mpz_class, mpz_class>> intervals = tData->intervals;

    primeNumbers = MillerRabinSeq::computePrime(intervals);

    for(const mpz_class& i : primeNumbers){
        tData->result.push_back(i);
    }
    return nullptr;
}

template<typename T>
vector<vector<T>> SplitVector(const vector<T> &vec, size_t n) {
    vector<vector<T>> outVec;

    size_t length = vec.size() / n;
    size_t remain = vec.size() % n;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; i < min(n, vec.size()); ++i) {
        end += (remain > 0) ? (length + !!(remain--)) : length;
        outVec.push_back(vector<T>(vec.begin() + begin, vec.begin() + end));
        begin = end;
    }
    return outVec;
}

/// ./pp_tp1 -t <threadNumber> -f <file_with_interval>
int main(int argc, char **argv) {
    vector<tuple<mpz_class, mpz_class>> INTERVALS;
    vector<tuple<mpz_class, mpz_class>>  readIntervals;

    size_t THREAD_NUMBER;
    char *FILEPATH;

    handleInputs(argc, argv, THREAD_NUMBER, FILEPATH);

    readIntervals = FileParse::readFile(FILEPATH);
    auto chInterval = Chrono(true);
    INTERVALS = FileParse::intervalsOptimisation_v2(readIntervals, THREAD_NUMBER);
    chInterval.pause();

    /**
     * MillerRabin Sequential
     */
    auto chSeq = Chrono(true);
    vector<mpz_class> primeNumbersSeq = MillerRabinSeq::computePrime(INTERVALS);
    chSeq.pause();

    /**
     * SOLUTION - Miller rabin Parallel
     */
    struct thread_data thread_data_array[THREAD_NUMBER];
    pthread_t threads[THREAD_NUMBER];

    vector<vector<tuple<mpz_class, mpz_class>>> splitVector = SplitVector(INTERVALS, THREAD_NUMBER);


    /// Use a struct to pass INTERVALS data to worker function
    auto chPar = Chrono(true);
    for (size_t t = 0; t < THREAD_NUMBER; t++) {
        /// Split in even interval
        thread_data_array[t].intervals = splitVector[t];
        pthread_create(&threads[t], nullptr, workerOnIntervals, (void *) &thread_data_array[t]);
    }

    for (size_t i = 0; i < THREAD_NUMBER; i++) {
        pthread_join(threads[i], nullptr);
    }

    vector<mpz_class> primeNumbersPar;
    for (size_t t = 0; t < THREAD_NUMBER; t++) {
        primeNumbersPar.insert(primeNumbersPar.end(), thread_data_array[t].result.begin(), thread_data_array[t].result.end());
    }
    chPar.pause();

    /**
     * DISPLAY
     */
    inputPrint(INTERVALS, THREAD_NUMBER);
    chronoExecution(chInterval, chSeq, chPar);
    primeNbDisplay(primeNumbersSeq, primeNumbersPar);

    return 0;
}