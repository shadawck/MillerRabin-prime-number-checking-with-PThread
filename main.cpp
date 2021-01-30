#include <getopt.h>
#include <stdlib.h>
#include <iostream>
#include <gmpxx.h>

#include "ArgParse.hpp"
#include "FileParse.hpp"
#include "MillerRabinSeq.hpp"

using namespace std;

void argumentMessage(vector<tuple<long, long>> optimizedIntervals, int number);

// ./pp_tp1 -t <threadNumber> -f <file_with_interval>
int main(int argc, char **argv) {
    int args;
    int threadNumber = 0;

    vector<tuple<long, long>> intervals;
    tuple<vector<long>, vector<long>> splitIntervals;
    vector<tuple<long, long>> optimizedIntervals;

    while ((args = getopt(argc, argv, "t:f:")) != -1) {
        auto ap = ArgParse(optarg);

        switch (args) {
            case 't' :
                threadNumber = ap.getParseInt();
                break;
            case 'f':
                optimizedIntervals = FileParse::intervalsOptimisation(optarg);
                break;
            default:
                abort();
        }
    }

    argumentMessage(optimizedIntervals, threadNumber);

    MillerRabinSeq::testGMP("95647806479275528135733781266203904794419563064407",25);

    return 0;
}

void argumentMessage(vector<tuple<long, long>> optimizedIntervals, int threadNumber) {
    cout << "\n--- Optimized Interval ---" << endl;
    FileParse::printTupleVector(optimizedIntervals);

    cout << "\n--- Threads Number --- " << endl;
    cout << threadNumber << endl;
}
