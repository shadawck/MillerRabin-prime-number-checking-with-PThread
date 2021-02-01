#ifndef PP_TP1_MILLERRABINSEQ_HPP
#define PP_TP1_MILLERRABINSEQ_HPP

#include <string>
#include <vector>
#include <gmpxx.h>

using namespace std;

class MillerRabinSeq {
public:
    static vector<mpz_class> computeInterval(tuple<int, vector<tuple<mpz_class, mpz_class>>> intervals);
};


#endif //PP_TP1_MILLERRABINSEQ_HPP
