#include <iostream>
#include <vector>
#include <complex>
#include <time.h>
#include "zeta.h"

#define N 1000000000000
#define GAMMA 0.57721566490153286060 // Euler-Mascheroni Constant
#define CACHE_CAPACITY 80
typedef unsigned long long ULL;
using namespace std;

const double LN_N = log(N);
vector<double> term_cache(1);


void save_li_last_term1() {
    // Auxiliary function for li(n)
    double result;
    term_cache.push_back(LN_N);
    for (int i=2; i<CACHE_CAPACITY; ++i) {
        result = term_cache.back() * LN_N * (i - 1) / (i * i);
        term_cache.push_back(result);
    }
}

double get_li_last_term2(int n) {
    // Auxiliary function for li(n)
    ULL temp = (ULL) n;
    double sum = 0;
    for (int k=1; k<term_cache.size(); ++k) {
        sum += term_cache[k] / temp;
        temp *= n;
    }
    return sum;
}

double li(int n) {
    // Calculate LI(N ^ (1/n)) (Log Integral) using an identity
    // (refer to Bromwich and MacRobert 1991, p.334)
    return GAMMA + log(LN_N) - log(n) + get_li_last_term2(n);
}


int main() {
    time_t start_time = time(NULL);
    save_li_last_term1();
    double numberOfPrime = li(1);
    for (int i=0; i<term_cache.size(); ++i) {
        cout << term_cache[i] << endl;
    }
    complex<LD> z1(0, 1);
    complex<LD> a = zeta(z1);
    cout.precision(13);
    cout << "소수의 개수: " << numberOfPrime << endl;
    time_t end_time = time(NULL);
    time_t elapsed_time = (double) (end_time - start_time);
    cout << "프로그램 수행 시간: " << elapsed_time << endl;
    return 0;
}