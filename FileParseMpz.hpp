#ifndef PP_TP1_FILEPARSEMPZ_HPP
#define PP_TP1_FILEPARSEMPZ_HPP

#include <tuple>
#include <vector>
#include <gmpxx.h>

using namespace std;

class FileParseMpz {
public :
    static tuple<vector<mpz_class>, vector<mpz_class>> FileParseTri(char *);

    static tuple<int, vector<tuple<mpz_class, mpz_class>>> intervalsOptimisation(char *);

    static void printTupleVector(const vector<tuple<mpz_class, mpz_class>> &intervals);

    static vector<tuple<mpz_class, mpz_class>> merge(char *FILENAME);

private :
    static mpz_class popFront(vector<mpz_class> &aVector);

    static void printMpzVector(const vector<mpz_class> &);


};

#endif //PP_TP1_FILEPARSEMPZ_HPP
