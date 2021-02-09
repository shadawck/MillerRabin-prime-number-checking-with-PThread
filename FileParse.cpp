#include <fstream>
#include <iostream>
#include <tuple>

#include "FileParse.hpp"

using namespace std;

vector<tuple<mpz_class, mpz_class>> FileParse::readFile(char *FILENAME) {
    vector<tuple<mpz_class, mpz_class>> intervalsMpz;
    fstream file(FILENAME);
    mpz_class xMpz,yMpz;
    while (file >> xMpz >> yMpz) {
        intervalsMpz.emplace_back(xMpz, yMpz);
    }
    fstream close(FILENAME);

    return intervalsMpz;
}

vector<tuple<mpz_class, mpz_class>> SplitTuple(tuple<mpz_class, mpz_class> tup, size_t n) {
    vector<tuple<mpz_class, mpz_class>> outTup;

    mpz_class tupSize = get<1>(tup) - get<0>(tup);
    size_t s = tupSize.get_ui();

    size_t length = s / n;
    size_t remain = s % n;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; i < min(n, s); ++i) {
        end += (remain > 0) ? (length + !!(remain--)) : length;
        outTup.emplace_back(get<0>(tup) + begin, get<0>(tup) + end);
        begin = end + 1;
    }

    return outTup;
}

vector<tuple<mpz_class, mpz_class>>
FileParse::intervalsOptimisation_v2(vector<tuple<mpz_class, mpz_class>> intervals, size_t THREAD_NUMBER) {
    size_t intervalSize = intervals.size();
    sort(intervals.begin(), intervals.end());
    int index = 0;

    if (intervals.size() < THREAD_NUMBER) {
        return adaptInterval(intervals, THREAD_NUMBER, intervalSize);
    }

    for (size_t i = 1; i < intervalSize; i++) {
        if (get<1>(intervals[index]) >= get<0>(intervals[i])) {
            get<1>(intervals[index]) = max(get<1>(intervals[index]), get<1>(intervals[i]));
            get<0>(intervals[index]) = min(get<0>(intervals[index]), get<0>(intervals[i]));
        } else {
            intervals[index] = intervals[i];
            index++;
        }
    }

    intervals.erase(intervals.begin() + index + 1, intervals.end());
    return intervals;
}

vector<tuple<mpz_class, mpz_class>>
FileParse::adaptInterval(const vector<tuple<mpz_class, mpz_class>> &intervals, size_t THREAD_NUMBER,
                         size_t intervalSize) {
    vector<tuple<mpz_class, mpz_class>> adaptedInterval;

    for (size_t i = 0; i < intervalSize; i++) {
        for (const tuple<mpz_class, mpz_class> &tup : SplitTuple(intervals[i], THREAD_NUMBER))
            adaptedInterval.emplace_back(tup);
    }

    return adaptedInterval;
}

void FileParse::printTupleVector(const vector<tuple<mpz_class, mpz_class>> &intervals) {
    int ind = 0;
    for (const auto &i : intervals) {
        cout << "Interval " << ind << " : [" << get<0>(i) << ", " << get<1>(i) << "]" << endl;
        ind++;
    }
}
