#include <iostream>
#include <cmath>
#include <complex>
typedef long double LD;
using namespace std;

const LD LOWER_THRESHOLD = 1.0e-6;
const LD UPPER_BOUND = 1.0e+4;
const int MAXNUM = 100;

complex<LD> zeta(const complex<LD> &s);