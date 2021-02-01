#include "MillerRabinSeq.hpp"
#include <gmpxx.h>

using namespace std;

int REPETITION_NUMBER = 10;

vector<mpz_class> MillerRabinSeq::computeInterval(const vector<tuple<mpz_class, mpz_class>>& intervals){
    vector<mpz_class> primeNumbers;
    int primeProbability;

    for(tuple<mpz_class, mpz_class> t : intervals){
        mpz_class borneMin = get<0>(t);
        mpz_class borneMax = get<1>(t);

        if(borneMin <= 2){
            borneMin = 2;
            primeNumbers.emplace_back(2);
        }

        borneMin = checkOddity(borneMin);

        for(; borneMin <= borneMax; borneMin = borneMin + 2){
            primeProbability = mpz_probab_prime_p(borneMin.get_mpz_t(), REPETITION_NUMBER);
            if(primeProbability > 0){
                primeNumbers.emplace_back(borneMin);
            }
        }
    }

    return primeNumbers;
}

mpz_class &MillerRabinSeq::checkOddity(mpz_class &borneMin) {
    if (borneMin % 2 == 0)
        borneMin++;
    return borneMin;
}
