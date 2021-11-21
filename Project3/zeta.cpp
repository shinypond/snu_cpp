#include "zeta.h"

complex<LD> zeta(const complex<LD> &s) {
    complex<LD> a_arr[MAXNUM + 1];
    complex<LD> half(0.5, 0.0);
    complex<LD> one(1.0, 0.0);
    complex<LD> two(2.0, 0.0);
    complex<LD> rev(-1.0, 0.0);
    complex<LD> sum(0.0, 0.0);
    complex<LD> prev(1.0e+20, 0.0);

    a_arr[0] = half / (one - pow(two, (one - s)));
    sum += a_arr[0];

    for (int n=1; n<=MAXNUM; n++) {
        complex<LD> nCplx(n, 0.0);
        for (int k=0; k<n; k++) {
            complex<LD> kCplx(k, 0.0);
            a_arr[k] *= half * (nCplx / (nCplx - kCplx));
            sum += a_arr[k];
        }
        a_arr[n] = (rev * a_arr[n-1] * pow((nCplx / (nCplx + one)), s) / nCplx);
        sum += a_arr[n];
        if (abs(prev - sum) < LOWER_THRESHOLD) {
            break;
        }
        if (abs(sum) > UPPER_BOUND) {
            break;
        }
        prev = sum;
    }
    return sum;
}
