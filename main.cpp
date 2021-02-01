#include <getopt.h>
#include <iostream>
#include <gmpxx.h>

#include "ArgParse.hpp"
#include "FileParseMpz.hpp"
#include "MillerRabinSeq.hpp"
#include "Chrono.hpp"

using namespace std;

void argumentMessageMpz(const tuple<int, vector<tuple<mpz_class, mpz_class>>> &optimizedIntervalsMpz, int threadNb);

void chronoExecution(Chrono &ch, Chrono &chPrime);

void primeNumberMessage(const vector<mpz_class> &primeNumbers);

// ./pp_tp1 -t <threadNumber> -f <file_with_interval>
int main(int argc, char **argv) {
    int args;
    int threadNumber = 0;
    Chrono chInterval;
    Chrono chPrime;

    tuple<int, vector<tuple<mpz_class, mpz_class>>> optimizedIntervalsMpz;
    vector<tuple<mpz_class, mpz_class>> intervals;

    while ((args = getopt(argc, argv, "t:f:")) != -1) {
        auto ap = ArgParse(optarg);

        switch (args) {
            case 't' :
                threadNumber = ap.getParseInt();
                break;
            case 'f':
                chInterval = Chrono(true);
                optimizedIntervalsMpz = FileParseMpz::intervalsOptimisation(optarg);
                chInterval.pause();
                break;
            default:
                abort();
        }
    }

    argumentMessageMpz(optimizedIntervalsMpz, threadNumber);

    intervals = get<1>(optimizedIntervalsMpz);
    chPrime = Chrono(true);
    vector<mpz_class> primeNumbers = MillerRabinSeq::computeInterval(intervals);
    chPrime.pause();
    primeNumberMessage(primeNumbers);

    chronoExecution(chInterval, chPrime);

    return 0;
}

void chronoExecution(Chrono &chInterval, Chrono &chPrime) {
    cout << "Interval Optimization time : " << chInterval.get() << " sec" << endl;
    cout << "Miller-Rabin Computing time : " << chPrime.get() << " sec" << endl;
    cout << "Total execution time :" << chInterval.get() + chPrime.get() << endl;
}

void argumentMessageMpz(const tuple<int, vector<tuple<mpz_class, mpz_class>>> &optimizedIntervalsMpz, int threadNb) {
    int nbOverlap = get<0>(optimizedIntervalsMpz);
    if (nbOverlap > 0) {
        cout << "\n--- Optimized Interval ---" << endl;
        cout << nbOverlap << " Interval have been optimized." << endl;
        cout << get<1>(optimizedIntervalsMpz).size() << " Intervals to handle during prime computing" << endl;
    }
    cout << "\n--- Threads Number --- " << endl;
    cout << threadNb << endl;
}

void primeNumberMessage(const vector<mpz_class> &primeNumbers) {
    cout << "\n--- Prime Numbers ---" << endl;
    cout << primeNumbers.size() << " FOUND " << endl;
}