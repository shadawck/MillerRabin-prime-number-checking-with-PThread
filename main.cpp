#include <getopt.h>
#include <iostream>
#include <gmpxx.h>
#include <pthread.h>
#include <thread>
#include <chrono>

#include "ArgParse.hpp"
#include "FileParseMpz.hpp"
#include "MillerRabinSeq.hpp"
#include "Chrono.hpp"
#include "MillerRabinPar.hpp"

using namespace std;

void chronoExecution(Chrono &chInterval, Chrono &chInterval2, Chrono &chPrime, Chrono &chPara) {
    cout << "Interval V1 Optimization time : " << chInterval.get() << " sec" << endl;
    cout << "Interval V2 Optimization time : " << chInterval2.get() << " sec" << endl << endl;

    cout << "Total Sequential execution time : " << chInterval2.get() + chPrime.get() << endl;
    cout << "Total Parallel execution time : " << chInterval2.get() + chPara.get() << " sec" << endl << endl;

    cout << "SPEEDUP : " << chPrime.get() / chPara.get() << endl;
}

void primeNbPrint(const vector<mpz_class> &primeNumbersSeq, const vector<mpz_class> &primeNumbersPar) {
    cout << "\n--- Prime Numbers ---" << endl;
    cout << primeNumbersSeq.size() << " Prime number found with sequential method" << endl;
    cout << primeNumbersPar.size() << " Prime number found with Parallel method" << endl << endl;
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

void wait() {
    chrono::milliseconds timespan(1000);
    this_thread::sleep_for(timespan);
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

    tuple<vector<mpz_class>, vector<mpz_class>> readIntervals_v1;
    vector<tuple<mpz_class, mpz_class>>  readIntervals_v2;

    size_t THREAD_NUMBER;
    char *FILEPATH;

    handleInputs(argc, argv, THREAD_NUMBER, FILEPATH);

    /**
     * Interval Optimization
     */
    readIntervals_v1 = FileParseMpz::readFile_v1(FILEPATH);
    auto chInterval = Chrono(true);
    get<1>(FileParseMpz::intervalsOptimisation_v1(readIntervals_v1));
    chInterval.pause();

    readIntervals_v2 = FileParseMpz::readFile_v2(FILEPATH);
    auto chInterval2 = Chrono(true);
    INTERVALS = FileParseMpz::intervalsOptimisation_v2(readIntervals_v2);
    chInterval2.pause();

    /**
     * MillerRabin Sequential
     */
    auto chSeq = Chrono(true);
    vector<mpz_class> primeNumbers = MillerRabinSeq::computePrime(INTERVALS);
    chSeq.pause();

    /**
     * Miller rabin Parallel
     */
    struct thread_data thread_data_array[THREAD_NUMBER];
    pthread_t threads[THREAD_NUMBER];

    vector<vector<tuple<mpz_class, mpz_class>>> sp = SplitVector(INTERVALS, THREAD_NUMBER);
    /// adapt threadNumber to number of INTERVALS
    if (sp.size() < THREAD_NUMBER)
        THREAD_NUMBER = sp.size();

    /// Use a struct to pass INTERVALS data to worker function
    auto chPara = Chrono(true);
    for (size_t t = 0; t < THREAD_NUMBER; t++) {
        /// Split in even interval
        thread_data_array[t].intervals = sp[t];
        pthread_create(&threads[t], nullptr, workerOnIntervals, (void *) &thread_data_array[t]);
    }

    for (size_t i = 0; i < THREAD_NUMBER; i++) {
        pthread_join(threads[i], nullptr);
    }
    chPara.pause();

    /// reassemble prime number from thread result
    vector<mpz_class> prime;
    for (size_t t = 0; t < THREAD_NUMBER; t++) {
        prime.insert(prime.end(), thread_data_array[t].result.begin(), thread_data_array[t].result.end());
    }

    /**
     * DISPLAY
     */
    inputPrint(INTERVALS, THREAD_NUMBER);
    primeNbPrint(primeNumbers, prime);
    chronoExecution(chInterval, chInterval2, chSeq, chPara);

    return 0;
}