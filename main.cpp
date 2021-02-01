#include <getopt.h>
#include <iostream>
#include <gmpxx.h>

#include "ArgParse.hpp"
#include "FileParseMpz.hpp"
#include "MillerRabinSeq.hpp"
#include "Chrono.hpp"

using namespace std;

void argumentMessageMpz(const tuple<int, vector<tuple<mpz_class, mpz_class>>> &optimizedIntervalsMpz, int threadNumber);

void chronoExecution(Chrono &ch);

// ./pp_tp1 -t <threadNumber> -f <file_with_interval>
int main(int argc, char **argv) {
    int args;
    int threadNumber = 0;
    Chrono ch;

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
                ch = Chrono(true);
                optimizedIntervalsMpz = FileParseMpz::intervalsOptimisation(optarg);
                ch.pause();
                break;
            default:
                abort();
        }
    }

    argumentMessageMpz(optimizedIntervalsMpz, threadNumber);
    chronoExecution(ch);

    MillerRabinSeq::testGMP("95647806479275528135733781266203904794419563064407", 25);

    return 0;
}

void chronoExecution(Chrono &ch) {
    cout << "Interval Optimization time : " << ch.get() << "sec" << endl;
    cout << "Miller-Rabin Computing time : " << "TODO" << endl;
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
