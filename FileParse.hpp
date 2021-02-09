#ifndef PP_TP1_FILEPARSE_HPP
#define PP_TP1_FILEPARSE_HPP

#include <tuple>
#include <vector>
#include <gmpxx.h>

using namespace std;

class FileParse {
public :
    static vector<tuple<mpz_class, mpz_class>> readFile(char *);

    static void printTupleVector(const vector<tuple<mpz_class, mpz_class>> &intervals);

    static vector<tuple<mpz_class, mpz_class>>
    intervalsOptimisation_v2(vector<tuple<mpz_class, mpz_class>> intervals, size_t THREAD_NUMBER);

private :
    static vector<tuple<mpz_class, mpz_class>>
    adaptInterval(const vector<tuple<mpz_class, mpz_class>> &intervals, size_t THREAD_NUMBER, size_t intervalSize);
};

#endif //PP_TP1_FILEPARSE_HPP
