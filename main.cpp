#include <getopt.h>
#include <iostream>
#include <gmpxx.h>

#include "ArgParse.hpp"
#include "FileParseMpz.hpp"
#include "MillerRabinSeq.hpp"
#include "Chrono.hpp"

using namespace std;

void chronoExecution(Chrono &chInterval, Chrono &chPrime) {
    cout << "Interval Optimization time : " << chInterval.get() << " sec" << endl;
    cout << "Miller-Rabin Computing time : " << chPrime.get() << " sec" << endl;
    cout << "Total execution time :" << chInterval.get() + chPrime.get() << endl;
}

void primeNbPrint(const vector<mpz_class> &primeNumbers) {
    cout << "\n--- Prime Numbers ---" << endl;
    cout << primeNumbers.size() << " FOUND " << endl;
}

void handleInputs(int argc, char **argv, int &THREAD_NUMBER, char *&FILEPATH) {
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

// ./pp_tp1 -t <threadNumber> -f <file_with_interval>
int main(int argc, char **argv) {
    tuple<int, vector<tuple<mpz_class, mpz_class>>> optimizedIntervalsMpz;
    vector<tuple<mpz_class, mpz_class>> intervals;
    Chrono chInterval;
    Chrono chPrime;

    int THREAD_NUMBER;
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
    chPrime = Chrono(true);
    vector<mpz_class> primeNumbers = MillerRabinSeq::computeInterval(intervals);
    chPrime.pause();

    /**
     * DISPLAY
     */
    inputPrint(optimizedIntervalsMpz, THREAD_NUMBER);
    primeNbPrint(primeNumbers);
    chronoExecution(chInterval, chPrime);

    return 0;
}

