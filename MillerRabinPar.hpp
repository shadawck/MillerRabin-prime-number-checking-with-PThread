#include <gmpxx.h>
#include <vector>
#include <tuple>
#include <pthread.h>

using namespace std;

#ifndef PP_TP1_MILLERRABINPAR_HPP
#define PP_TP1_MILLERRABINPAR_HPP


class MillerRabinPar {
public:
    explicit MillerRabinPar(int threadNumber, const vector<tuple<mpz_class, mpz_class>> &intervals);

    static void *workerOnIntervals(void *intervals);

private:
    int threadNumber{};
    vector<tuple<mpz_class, mpz_class>> intervals;

    void workerOnIntervalsWithSplit();

    void pthread_exec(vector<tuple<mpz_class, mpz_class>> intervals);
};


#endif //PP_TP1_MILLERRABINPAR_HPP
