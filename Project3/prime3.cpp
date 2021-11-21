#include <iostream>
#include <vector>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <cmath>
#define N 1000000000
#define NUM_THREADS 128

using namespace std;

vector<bool> is_prime(N+1);

int s[16] = {
    1, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 49, 53, 59
};

long long sieve_of_atkin() {
    long long n = 0;
    int x, y, ix1, ix2;
    long long w, c, v;
    int x_end1 = pow(N / 4, 0.5);
    int x_end2 = pow(N / 3, 0.5);
    int x_end3 = pow(N / 2, 0.5);
    int y_end;
    long long w_end = N / 60;
    long long v_end;

    for (x=1; x<=x_end1; ++x) {
        y_end = pow(N - 4 * x * x, 0.5);
        for (y=1; y<=y_end; ++(++y)) {
            n = 4 * x * x + y * y;
            if (n > N) {
                break;
            }
            switch (n % 60) {
                case 1:
                case 13:
                case 17:
                case 29:
                case 37:
                case 41:
                case 49:
                case 53:
                    is_prime[n] = !is_prime[n];
                    break;
                default:
                    break;
            }
        }
    }
    for (x=1; x<=x_end2; ++(++x)) {
        y_end = pow(N - 3 * x * x, 0.5);
        for (y=2; y<=y_end; ++(++y)) {
            n = 3 * x * x + y * y;
            if (n > N) {
                break;
            }
            switch (n % 60) {
                case 7:
                case 19:
                case 31:
                case 43:
                    is_prime[n] = !is_prime[n];
                    break;
                default:
                    break;
            }
        }
    }
    for (x=2; x<=x_end3; ++x) {
        for (y=x-1; y>=1; --(--y)) {
            n = 3 * x * x - y * y;
            if (n > N) {
                break;
            }
            switch (n % 60) {
                case 11:
                case 23:
                case 47:
                case 59:
                    is_prime[n] = !is_prime[n];
                    break;
                default:
                    break;
            }
        }
    }

    for (w=0; w<=w_end; ++w) {
        for (ix1=0; ix1<16; ++ix1) {
            n = 60 * w + s[ix1];
            if (n < 7 || n * n > N) {
                continue;
            }
            if (is_prime[n]) {
                v_end = N / (60 * n * n);
                for (v=0; v<=v_end; ++v) {
                    for (ix2=0; ix2<16; ++ix2) {
                        c = n * n * (60 * v + s[ix2]);
                        if (c > N) {
                            break;
                        }
                        is_prime[c] = 0;
                    }
                }
            }
        }
    }
    // Final output (including 2, 3, 5)
    long long result = 3;
    for (w=0; w<=w_end; ++w) {
        for (ix1=0; ix1<16; ++ix1) {
            n = 60 * w + s[ix1];
            if (n < 7) {
                continue;
            }
            if (n > N) {
                break;
            }
            if (is_prime[n]) {
                result += 1;
            }
        }
    }
    return result;
}

int main() {
    time_t start_time = time(NULL);
    long long numberOfPrime = sieve_of_atkin();
    cout << "소수의 개수: " << numberOfPrime << endl;
    time_t end_time = time(NULL);
    time_t elapsed_time = (double) (end_time - start_time);
    cout << "프로그램 수행 시간: " << elapsed_time << endl;
    return 0;
}