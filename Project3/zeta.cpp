#include "zeta.h"

complex<double> zeta(complex<double> s) {
    complex<double> a_arr[MAXNUM + 1];
    complex<double> half(0.5, 0.0);
    complex<double> one(1.0, 0.0);
    complex<double> two(2.0, 0.0);
    complex<double> rev(-1.0, 0.0);
    complex<double> sum(0.0, 0.0);
    complex<double> prev(1.0e+20, 0.0);

    a_arr[0] = half / (one - pow(two, (one - s)));
    sum += a_arr[0];

    for (int n=1; n<=MAXNUM; n++) {
        complex<double> nCplx(n, 0.0);
        for (int k=0; k<n; k++) {
            complex<double> kCplx(k, 0.0);
            a_arr[k] *= half * (nCplx / (nCplx - kCplx));
            sum += a_arr[k];
        }
        a_arr[n] = (rev * a_arr[n-1] * pow((nCplx / (nCplx + one)), s) / nCplx);
        sum += a_arr[n];
        if (abs(prev - sum) < LOWER_THRESHOdouble) {
            break;
        }
        if (abs(sum) > UPPER_BOUND) {
            break;
        }
        prev = sum;
    }
    return sum;
}
