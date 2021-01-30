#include <tuple>
#include <vector>

using namespace std;

#ifndef PP_TP1_FILEPARSE_H
#define PP_TP1_FILEPARSE_H

class FileParse {
public:
    FileParse(char *filename);

    static tuple<vector<long>, vector<long>> FileParseTri(char*);

    static void printTupleVector(const vector<tuple<long, long>>&);

    static vector<tuple<long, long>> intervalsOptimisation(char *);

private:
    static int cmp(long a, long b);

    static long popFront(vector<long> &aVector) ;
};

#endif //PP_TP1_FILEPARSE_H
