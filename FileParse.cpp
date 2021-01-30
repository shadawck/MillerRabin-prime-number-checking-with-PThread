#include <fstream>
#include <iostream>

#include "FileParse.hpp"

using namespace std;

/**
 *
 * @param FILENAME
 * @return Tuple of Intervals vectors : ([beginValues], [endValues])
 */
tuple<vector<long>, vector<long>> FileParse::FileParseTri(char *FILENAME) {
    vector<long> intervalsBegin;
    vector<long> intervalsEnd;

    fstream file(FILENAME);

    int x, y;
    while (file >> x >> y) {
        intervalsBegin.emplace_back(x);
        intervalsEnd.emplace_back(y);
    }

    return make_tuple(intervalsBegin, intervalsEnd);
}

/**
 * Make Intervals union to get reduced intervals
 * @param splitIntervals Tuple of Intervals vectors : ([beginValues], [endValues])
 * @return
 */
vector<tuple<long, long>> FileParse::intervalsOptimisation(char *FILENAME) {
    tuple<vector<long>, vector<long>> splitIntervals = FileParseTri(FILENAME);

    vector<long> intervalsBegin = get<0>(splitIntervals);
    vector<long> intervalsEnd = get<1>(splitIntervals);
    sort(intervalsBegin.begin(), intervalsBegin.end());
    sort(intervalsEnd.begin(), intervalsEnd.end());

    int nbOverlap = 0;
    long beginCurrentInterval = 0;
    long beginPos;
    long endPos;
    vector<tuple<long, long>> optimizedInterval;

    while (!intervalsBegin.empty()) {
        int beginEndOrder = cmp(intervalsBegin[0], intervalsEnd[0]);
        if (beginEndOrder == -1) {
            beginPos = popFront(intervalsBegin);

            if (nbOverlap == 0) {
                beginCurrentInterval = beginPos;
            }
            nbOverlap += 1;
        } else if (beginEndOrder == 1) {
            endPos = popFront(intervalsEnd);
            nbOverlap -= 1;

            if (nbOverlap == 0) {
                tuple<long, long> newInterval{beginCurrentInterval, endPos};
                optimizedInterval.emplace_back(newInterval);
            }
        } else {
            intervalsBegin.erase(intervalsBegin.begin());
            intervalsEnd.erase(intervalsEnd.begin());
        }
    }

    if (!intervalsEnd.empty()) {
        endPos = intervalsEnd.back();
        tuple<long, long> newInterval{beginCurrentInterval, endPos};
        optimizedInterval.emplace_back(newInterval);
    }

    return optimizedInterval;
}

/**
 * Immitate python Pop function: Get first element of the vector and erase him from the vector.
 * @param aVector
 * @return First element of the vector.
 */
long FileParse::popFront(vector<long> &aVector) {
    long position;
    position = aVector.front();
    aVector.erase(aVector.begin());
    return position;
}

/**
 * Compare two long
 * @param a
 * @param b
 * @return 1 if a>b, 0 if a==b, else -1
 */
int FileParse::cmp(long a, long b) {
    if (a < b) {
        return -1;
    } else if (a == b) {
        return 0;
    } else {
        return 1;
    }
}

void FileParse::printTupleVector(const vector<tuple<long, long>> &intervals) {
    int ind = 0;
    for (const auto &i : intervals) {
        cout << "Interval " << ind << " : [" << get<0>(i) << ", " << get<1>(i) << "]" << endl;
        ind++;
    }
}
