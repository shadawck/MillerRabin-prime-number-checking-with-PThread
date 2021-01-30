#include "MillerRabinSeq.hpp"
#include <gmpxx.h>
#include <iostream>
#include <string>

using namespace std;

int MillerRabinSeq::testGMP(string numberToTest, int nbRepetition){
    mpz_class n(numberToTest);
    int primeProbability = mpz_probab_prime_p(n.get_mpz_t(),nbRepetition);
    cout << "Prime probability of " << numberToTest << " is: " <<primeProbability << endl;
    mpz_clear;

    return primeProbability;
}
