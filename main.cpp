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

void chronoExecution(Chrono &chInterval, Chrono &chPrime, Chrono &chPara) {
    cout << "Interval Optimization time : " << chInterval.get() << " sec" << endl << endl;

    cout << "Miller-Rabin Sequential execution time : " << chPrime.get() << " sec" << endl;
    cout << "Total Sequential execution time :" << chInterval.get() + chPrime.get() << endl << endl;

    cout << "First Parallel Method execution time : " << chPara.get() << " sec" << endl;
    cout << "Total Parallel execution time : " << chInterval.get() + chPara.get() << " sec" << endl;

}

void primeNbPrint(const vector<mpz_class> &primeNumbersSeq, const vector<mpz_class> &primeNumbersPar) {
    cout << "\n--- Prime Numbers ---" << endl;
    cout << primeNumbersSeq.size() << " Prime number found with sequential method" << endl << endl;
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

void inputPrint(const tuple<int, vector<tuple<mpz_class, mpz_class>>> &optiIntervals, int threadNb) {
    int nbOverlap = get<0>(optiIntervals);
    if (nbOverlap > 0) {
        cout << "\n--- Optimized Interval ---" << endl;
        cout << nbOverlap << " Interval have been optimized." << endl;
        cout << get<1>(optiIntervals).size() << " Intervals to handle during prime computing" << endl;
    }
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
    // TODO stock prime Number in tData->result

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
    tuple<int, vector<tuple<mpz_class, mpz_class>>> optimizedIntervalsMpz;
    vector<tuple<mpz_class, mpz_class>> intervals;
    Chrono chInterval;
    Chrono chSeq;
    Chrono chPara;

    size_t THREAD_NUMBER;
    char *FILEPATH;
    handleInputs(argc, argv, THREAD_NUMBER, FILEPATH);

    /**
     * Interval Optimization
     */
    chInterval = Chrono(true);
    optimizedIntervalsMpz = FileParseMpz::intervalsOptimisation(FILEPATH);
    chInterval.pause();

    intervals = get<1>(optimizedIntervalsMpz);

    /**
     * MillerRabin Sequential
     */
    chSeq = Chrono(true);
    vector<mpz_class> primeNumbers = MillerRabinSeq::computePrime(intervals);
    chSeq.pause();

    /**
     * Miller rabin Parallel
     */
    struct thread_data thread_data_array[THREAD_NUMBER];
    pthread_t threads[THREAD_NUMBER];

    vector<vector<tuple<mpz_class, mpz_class>>> sp = SplitVector(intervals, THREAD_NUMBER);
    // adapt threadNumber to number of intervals
    if (sp.size() < THREAD_NUMBER)
        THREAD_NUMBER = sp.size();

    /// Use a struct to pass intervals data to worker function
    chPara = Chrono(true);
    for (size_t t = 0; t < THREAD_NUMBER; t++) {
        /// Split in even interval
        thread_data_array[t].intervals = sp[t];
        pthread_create(&threads[t], nullptr, workerOnIntervals, (void *) &thread_data_array[t]);
    }

    for (size_t i = 0; i < THREAD_NUMBER; i++) {
        pthread_join(threads[i], nullptr);
    }
    chPara.pause();

    /**
     * DISPLAY
     */
    inputPrint(optimizedIntervalsMpz, THREAD_NUMBER);
    primeNbPrint(primeNumbers, thread_data_array->result);
    chronoExecution(chInterval, chSeq, chPara);

    return 0;
}

