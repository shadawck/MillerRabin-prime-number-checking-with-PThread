#include "MillerRabinSeq.hpp"
#include <gmpxx.h>

using namespace std;

int REPETITION_NUMBER = 10;

vector<mpz_class> MillerRabinSeq::computeInterval(tuple<int, vector<tuple<mpz_class, mpz_class>>> intervalsTuple){
    vector<tuple<mpz_class, mpz_class>> intervals = get<1>(intervalsTuple);
    vector<mpz_class> primeNumbers;

    for(tuple<mpz_class, mpz_class> t : intervals){
        mpz_class borneMin = get<0>(t);
        mpz_class borneMax = get<1>(t);

        while(borneMin < borneMax){
            int primeProbability = mpz_probab_prime_p(borneMin.get_mpz_t(), REPETITION_NUMBER);
            if(primeProbability > 0){
                primeNumbers.emplace_back(borneMin);
            }
            borneMin += 1;
        }
    }

    return primeNumbers;
}