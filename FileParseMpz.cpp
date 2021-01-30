#include "FileParseMpz.hpp"
#include <fstream>
#include <iostream>
#include <tuple>

using namespace std;

/**
 *
 * @param FILENAME
 * @return Tuple of Intervals vectors : ([beginValues], [endValues])
 */
tuple<vector<mpz_class>, vector<mpz_class>> FileParseMpz::FileParseTri(char *FILENAME) {
    vector<mpz_class> intervalMpzBegin;
    vector<mpz_class> intervalMpzEnd;

    fstream file(FILENAME);

    mpz_class xMpz;
    mpz_class yMpz;
    while (file >> xMpz >> yMpz) {
        intervalMpzBegin.emplace_back(xMpz);
        intervalMpzEnd.emplace_back(yMpz);
    }

    fstream close(FILENAME);
    return make_tuple(intervalMpzBegin, intervalMpzEnd);
}

/**
 * Make Intervals union to get reduced intervals
 * @param splitIntervals Tuple of Intervals vectors : ([beginValues], [endValues])
 * @return
 */
vector<tuple<mpz_class, mpz_class>> FileParseMpz::intervalsOptimisation(char *FILENAME) {
    tuple<vector<mpz_class>, vector<mpz_class>> splitIntervals = FileParseTri(FILENAME);

    vector<mpz_class> intervalMpzBegin = get<0>(splitIntervals);
    vector<mpz_class> intervalMpzEnd = get<1>(splitIntervals);

    sort(intervalMpzBegin.begin(), intervalMpzBegin.end());
    sort(intervalMpzEnd.begin(), intervalMpzEnd.end());

    int nbOverlap = 0;
    mpz_class beginCurrentInterval(0);
    mpz_class beginPos;
    mpz_class endPos;
    vector<tuple<mpz_class, mpz_class>> optimizedInterval;

    while (!intervalMpzBegin.empty()) {
        long beginEndOrder = mpz_cmp(intervalMpzBegin[0].get_mpz_t(), intervalMpzEnd[0].get_mpz_t());

        if (beginEndOrder == -1) {
            beginPos = popFront(intervalMpzBegin);

            if (nbOverlap == 0) {
                beginCurrentInterval = beginPos;
            }
            nbOverlap += 1;
        } else if (beginEndOrder == 1) {
            endPos = popFront(intervalMpzEnd);
            nbOverlap -= 1;

            if (nbOverlap == 0) {
                tuple<mpz_class, mpz_class> newInterval{beginCurrentInterval, endPos};
                optimizedInterval.emplace_back(newInterval);
            }
        } else {
            intervalMpzBegin.erase(intervalMpzBegin.begin());
            intervalMpzEnd.erase(intervalMpzEnd.begin());
        }
    }

    if (!intervalMpzEnd.empty()) {
        endPos = intervalMpzEnd.back();
        tuple<mpz_class, mpz_class> newInterval{beginCurrentInterval, endPos};
        optimizedInterval.emplace_back(newInterval);
    }

    return optimizedInterval;
}

/**
 * Immitate python Pop function: Get first element of the vector and erase him from the vector.
 * @param aVector
 * @return First element of the vector.
 */
mpz_class FileParseMpz::popFront(vector<mpz_class> &aVector) {
    mpz_class position;
    position = aVector.front();
    aVector.erase(aVector.begin());
    return position;
}

void FileParseMpz::printTupleVector(const vector<tuple<mpz_class, mpz_class>> &intervals) {
    int ind = 0;
    for (const auto &i : intervals) {
        cout << "Interval " << ind << " : [" << get<0>(i) << ", " << get<1>(i) << "]" << endl;
        ind++;
    }
}

void FileParseMpz::printMpzVector(const vector<mpz_class> &intervals) {
    int ind = 0;
    for (const auto &i : intervals) {
        cout << i << " ";
        ind++;
    }
    cout << endl;
}