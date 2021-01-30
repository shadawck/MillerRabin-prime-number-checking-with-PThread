#include <getopt.h>
#include <stdlib.h>
#include <iostream>
#include <gmpxx.h>

#include "ArgParse.hpp"
#include "FileParseMpz.hpp"
#include "MillerRabinSeq.hpp"

using namespace std;

void argumentMessageMpz(const vector<tuple<mpz_class, mpz_class>> &optimizedIntervalsMpz, int threadNumber);

// ./pp_tp1 -t <threadNumber> -f <file_with_interval>
int main(int argc, char **argv) {
    int args;
    int threadNumber = 0;

    vector<tuple<long, long>> intervals;
    tuple<vector<long>, vector<long>> splitIntervals;
    vector<tuple<long, long>> optimizedIntervals;
    vector<tuple<mpz_class, mpz_class>> optimizedIntervalsMpz;

    while ((args = getopt(argc, argv, "t:f:")) != -1) {
        auto ap = ArgParse(optarg);

        switch (args) {
            case 't' :
                threadNumber = ap.getParseInt();
                break;
            case 'f':
                optimizedIntervalsMpz = FileParseMpz::intervalsOptimisation(optarg);
                break;
            default:
                abort();
        }
    }

    argumentMessageMpz(optimizedIntervalsMpz, threadNumber);

    MillerRabinSeq::testGMP("95647806479275528135733781266203904794419563064407", 25);

    return 0;
}

void argumentMessageMpz(const vector<tuple<mpz_class, mpz_class>> &optimizedIntervalsMpz, int threadNumber) {
    cout << "\n--- Optimized Interval ---" << endl;
    FileParseMpz::printTupleVector(optimizedIntervalsMpz);

    cout << "\n--- Threads Number --- " << endl;
    cout << threadNumber << endl;
}
