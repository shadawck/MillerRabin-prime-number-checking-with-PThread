#include <tuple>
#include <vector>

using namespace std;

#ifndef PP_TP1_FILEPARSE_H
#define PP_TP1_FILEPARSE_H

class FileParse {
public:
    FileParse(char *FILENAME);

    static void printTupleVector(vector<tuple<int, int>>);

    const vector<tuple<int, int>> &getIntervals() const;

private:
    vector<tuple<int, int>> intervals;
};

#endif //PP_TP1_FILEPARSE_H
