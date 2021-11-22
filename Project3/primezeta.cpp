#include <iostream>
#include "primezeta.h"

vector<CD> aux_last_term(CD x) {
    // Auxiliary function for li_last_term(x, n)
    vector<CD> term_cache(1);
    CD result;
    CD ln_x = log(x);
    term_cache.push_back(ln_x);
    for (double i=2; i<CACHE_CAPACITY; ++i) {
        result = term_cache.back() * ln_x * (i - 1) / (i * i);
        term_cache.push_back(result);
    }
    return term_cache;
}

CD li_last_term(CD x, int n) {
    // Auxiliary function for li(x, n)
    vector<CD> term_cache = aux_last_term(x);
    CD temp(n, 0.);
    CD sum(0.);
    for (int k=1; k<term_cache.size(); ++k) {
        sum += term_cache[k] / temp;
        temp *= n;
    }
    return sum;
}

CD li(CD x, int n) {
    // Calculate li(x^(1/n)) (Log Integral)
    // Refer to Bromwich and MacRobert 1991, p.334.
    return GAMMA + log(log(x)) - log(n) + li_last_term(x, n);
}

double second_li(CD x, int zero_idx) {
    // Calculate sum(li(x^rho))
    // The second term in the Riemann (1859) formula
    CD zero1(0.5, zeros[zero_idx]);
    CD zero2(0.5, -zeros[zero_idx]);
    double result = (li(pow(x, zero1), 1) + li(pow(x, zero2), 1)).real();
    return result;
}

double riemann_f(CD x) {
    // Calculate Riemann (1859) formula:
    // f(x) = li(x) - sum(li(x^rho)) - ln(2) + \int_{x}^{\infty} 1 / t(t^2 - 1)(ln t) dt
    // where rho's indicate the non-trivial zeros of Riemann-zeta function.
    // The first term is obtained from li(x, 1)
    // The second term is obtained from the sum of second_li(x, rho) 
    // The third and fourth terms are constants (and tiny).
    double result = li(x, 1).real();
    for (int i=0; i<zeros.size(); ++i) {
        result -= second_li(x, i);
    }
    result -= log(2);
    result += FOURTH_TERM;
    return result;
}

bool is_prime(int n) {
    // Auxiliary function for mobius_mu(n)
    // Naive primality test
    if (n < 2) {
        return false;
    }
    int sqrt_n = pow(n, 0.5);
    for (int i=2; i<sqrt_n+1; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int mobius_mu(int n) {
    // Calculate Möbius mu function
    if (n == 1) {
        return 1;
    }
    int sqrt_n = pow(n, 0.5);
    int num_p = 0; // number of prime factors of n
    for (int i=2; i<sqrt_n+1; ++i) {
        if (is_prime(i)) {
            if (n % (i * i) == 0) {
                return 0; // if n is not square-free
            }
            num_p++;
        }
    }
    if (num_p % 2 == 0) {
        return 1; // if n has even prime factors
    }
    return -1; // if n has odd prime factors
}

int pi(int x) {
    // Prime Counting function
    CD _x((double) x, 0.);
    CD result(0., 0.);
    CD term;
    double coeff;
    int n = 1;
    while (true) {
        coeff = (double) mobius_mu(n) / n;
        term = coeff * riemann_f(pow(_x, 1./n));
        cout << n << term << endl;
        result += term;
        n++;
        if (norm(term) < 1e-5) {
            break;
        }
    }
    return result.real();
}

int main() {
    time_t start_time = time(NULL);

    CD temp(100., 0.);
    cout << riemann_f(temp) << endl;

    int x = 100;
    int numberOfPrime = pi(x);
    // cout.precision(13);
    cout << "소수의 개수: " << numberOfPrime << endl;

    time_t end_time = time(NULL);
    time_t elapsed_time = (double) (end_time - start_time);
    cout << "프로그램 수행 시간: " << elapsed_time << endl;
    return 0;
}