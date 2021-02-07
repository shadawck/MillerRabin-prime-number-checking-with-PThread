#ifndef PP_TP1_FILEPARSEMPZ_HPP
#define PP_TP1_FILEPARSEMPZ_HPP

#include <tuple>
#include <vector>
#include <gmpxx.h>

using namespace std;

class FileParseMpz {
public :
    static tuple<vector<mpz_class>, vector<mpz_class>> readFile_v1(char *);

    static vector<tuple<mpz_class, mpz_class>> readFile_v2(char *);

    static void printTupleVector(const vector<tuple<mpz_class, mpz_class>> &intervals);

    static vector<tuple<mpz_class, mpz_class>> intervalsOptimisation_v2(vector<tuple<mpz_class, mpz_class>>);

    static tuple<int, vector<tuple<mpz_class, mpz_class>>>
    intervalsOptimisation_v1(tuple<vector<mpz_class>, vector<mpz_class>> splitIntervals);

private :
    static mpz_class popFront(vector<mpz_class> &aVector);

    static void printMpzVector(const vector<mpz_class> &);
};

#endif //PP_TP1_FILEPARSEMPZ_HPP
