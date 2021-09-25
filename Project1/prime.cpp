/* Written by Changyeon Yoon */

#include <iostream>
#include <vector>
#include <time.h>

int countprime(int N) {
    /*
    Count the number of primes less than or equal to N
    using the "Eratosthenes Sieve"
    
    Define a list, named "num_list", corresponding to integers 0 ~ N
    An i-th element in the list indicates whether i is prime or not
    0 : composite, 1 : prime
    
    < Pseudo-code >
    (1) Initialize the list as [1, 1, ..., 1]
    (2) For all i < sqrt(N) and k > 1, put 0 (composite) on the (k*i)-th position of the list
    (3) Finally, take the sum of the list
    */
    
	std::vector<int> num_list(N+1, 1);
	int j;
	for (int i=2; i*i<=N; i++) { // Start from 2 (just pass 0, 1)
		if (num_list[i] == 0) { // If i-th element is already 0 (i.e, composite) then skip
			continue;
		}
		j = 2 * i; // Skip i itself and start from 2*i = i + i
		while (j <= N) {
			num_list[j] = 0;
			j += i;
		}
	}
    
	int sum = 0;
	for (int i=2; i<=N; i++) {
		sum += num_list[i];
	}
	return sum;
}

int main() {
    clock_t startTime = clock();
    
	int n = 1000000000;
    int numberOfPrime = countprime(n);
    
	std::cout << "소수의 개수: " << numberOfPrime << std::endl;
	clock_t endTime = clock();
	double elapsedTime = (double) (endTime - startTime);
	std::cout << "프로그램 수행 시간: " << elapsedTime << std::endl;
	return 0;
}

