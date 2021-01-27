#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "ArgParse.hpp"
#include "FileParse.hpp"

using namespace std;

// ./pp_tp1 -t <threadNumber> -f <file_with_interval>
int main(int argc, char **argv) {
    int args;
    int threadNumber = 0;

    vector<tuple<int, int>> intervals;

    while ((args = getopt(argc, argv, "t:f:")) != -1) { // while there is arguments to parse
        ArgParse ap = ArgParse(optarg);
        FileParse fp = FileParse(optarg);

        switch (args) {
            case 't' :
                threadNumber = ap.getParseInt();
                break;
            case 'f':
                intervals = fp.getIntervals();
                break;
            default:
                abort();
        }
    }

    cout << "You have chosen " << threadNumber << "threads" << endl;
    FileParse::printTupleVector(intervals);

    return 0;
}
