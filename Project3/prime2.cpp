#include <iostream>
#include <vector>
#include <cmath>
#define sieve_N 1000000
#define N 10000000000
using namespace std;

vector<int> primes;

vector<int> prime_sieve(int n) {
    vector<bool> sieve(n / 2, true);
    int j;
    int sqrt_n = pow(n, 0.5) + 1;

    for (int i=3; i < sqrt_n; ++(++i)) {
        if (sieve[i / 2]) {
            j = i * i / 2;
            while (j < n / 2) {
                sieve[j] = false;
                j += i;
            }
        }
    }
    
    vector<int> result;
    result.push_back(2);
    for (int i=1; i < n / 2; ++i) {
        if (sieve[i]) {
            result.push_back(2 * i + 1);
        }
    }
    return result;
}


long long phi(long long x, long long a) {
    if (a <= 1) {
        return (int) (x + 1) / 2;
    }
    long long result = phi(x, a - 1) - phi(x / primes[a - 1], a - 1);
    return (long long) result;
}


long long pi(long long x) {
    long long result;
    long long b_i, j;
    double w;
    if (x < sieve_N) {
        result = lower_bound(primes.begin(), primes.end(), x) - primes.begin();
        return result;
    }

    long long a = pi((long long) pow(x, 1./4));
    long long b = pi((long long) pow(x, 1./2));
    long long c = pi((long long) pow(x, 1./3));
    result = phi(x, a) + (b + a - 2) * (b - a + 1) / 2;

    int start = (int) a + 1;
    int end = (int) b + 1;
    for (int i=start; i<end; ++i) {
        w = x / primes[i-1];
        b_i = pi(pow(w, 1./2));
        result -= pi(w);
        if (i <= c) {
            for (j=i; j<b_i+1; ++j) {
                result = result - pi((long long) w / primes[j-1]) + j - 1;
            }
        }
    }
    return result;
}


int main() {
    primes = prime_sieve(sieve_N);
    cout << "Prime Sieve Size : " << primes.size() << endl;
    long long result = pi(N);
    cout << result << endl;
    return 0;
}