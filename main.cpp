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

    vector<tuple<long, long>> intervals;
    tuple<vector<long>, vector<long>> splitIntervals;
    vector<tuple<long, long>> optimizedIntervals;
    tuple<int, vector<tuple<mpz_class, mpz_class>>> optimizedIntervalsMpz;

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

    auto mr = MillerRabinSeq();
    chPrime = Chrono(true);
    vector<mpz_class> primeNumbers = mr.computeInterval(optimizedIntervalsMpz);
    chPrime.pause();
    primeNumberMessage(primeNumbers);

    chronoExecution(chInterval, chPrime);

    return 0;
}

void chronoExecution(Chrono &chInterval, Chrono &chPrime) {
    cout << "Interval Optimization time : " << chInterval.get() << " sec" << endl;
    cout << "Miller-Rabin Computing time : " << chPrime.get() << endl << " sec" << endl;;
}

void argumentMessageMpz(const tuple<int, vector<tuple<mpz_class, mpz_class>>> &optimizedIntervalsMpz, int threadNb) {
    int nbOverlap = get<0>(optimizedIntervalsMpz);
    if (nbOverlap > 0) {
        cout << "\n--- Optimized Interval ---" << endl;
        cout << nbOverlap << " Interval have been optimized." << endl;
    }
    cout << "\n--- Threads Number --- " << endl;
    cout << threadNb << endl;
}

void primeNumberMessage(const vector<mpz_class> &primeNumbers) {
    cout << primeNumbers.size() << " FOUND " << endl;
}