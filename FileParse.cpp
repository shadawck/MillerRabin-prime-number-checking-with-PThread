#include <fstream>
#include <iostream>

#include "FileParse.hpp"

using namespace std;

FileParse::FileParse(char *FILENAME) {
    vector<tuple<int, int>> intervals;
    fstream file(FILENAME);
    int x, y;

    while (file >> x >> y) {
        intervals.emplace_back(x, y);
    }

    this->intervals = intervals;
}

const vector<tuple<int, int>> &FileParse::getIntervals() const {
    return intervals;
}

void FileParse::printTupleVector(vector<tuple<int, int>> intervals) {
    int ind = 0;
    for (const auto &i : intervals) {
        cout << "Interval " << ind << " : " << get<0>(i) << ", " << get<1>(i) << endl;
        ind++;
    }
}
