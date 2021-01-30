#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "ArgParse.hpp"
#include "FileParse.hpp"

void argumentMessage(vector<tuple<long, long>> vector, int number);

using namespace std;

// ./pp_tp1 -t <threadNumber> -f <file_with_interval>
int main(int argc, char **argv) {
    int args;
    int threadNumber = 0;

    vector<tuple<long, long>> intervals;
    tuple<vector<long>, vector<long>> splitIntervals;
    vector<tuple<long, long>> optimizedIntervals;

    while ((args = getopt(argc, argv, "t:f:")) != -1) { // while there is arguments to parse
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

    return 0;
}

void argumentMessage(vector<tuple<long, long>> optimizedIntervals, int threadNumber) {
    cout << "\n--- Optimized Interval ---" << endl;
    FileParse::printTupleVector(optimizedIntervals);

    cout << "\n--- Threads Number --- " << endl;
    cout << threadNumber << endl;
}
