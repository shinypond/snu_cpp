#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <cmath>
#define N 100000000LL
#define NUM_THREADS 12

using namespace std;

/*
counter_t 구조체 : 값과 뮤텍스 락 여부로 구성된 구조체
    primesearch : prime 여부 조사 대상을 값으로 저장
    primecount : 현 시점까지 나온 prime 개수를 값으로 저장

[Note]
위 구조체들의 선언은 최초 1회만 이루어지며,
각각의 thread들은 매번 여력이 될 때 (배정된 값 prime 여부 계산 완료)
현재까지 prime 여부 파악이 완료된 수보다 하나 더 큰 수를 가져간다.
이 때 primesearch를 lock하고, value를 올린 후, unlock을 함으로써
서로 다른 thread들 간의 순서가 꼬이는 것을 방지한다.
*/

typedef struct counter_t {
    long long value;
    pthread_mutex_t lock;
} counter_t;


counter_t primesearch; 
counter_t primecount; 


void init_counter(counter_t *c, long long init_value) {
    // counter_t의 초기화 함수
    c->value = init_value;
    pthread_mutex_init(&c->lock, NULL);
}


int inc_counter(counter_t *c) {
    // 해당 시점 값을 조회 + 1 증가시키는 함수
    pthread_mutex_lock(&c->lock);
    int rc = ++c->value;
    pthread_mutex_unlock(&c->lock);
    return rc;
}


int get_counter(counter_t *c) {
    // 해당 시점 값을 조회하는 함수
    pthread_mutex_lock(&c->lock);
    int rc = c->value;
    pthread_mutex_unlock(&c->lock);
    return rc;
}


void check_prime() {
    // primesearch로부터 prime 여부 판단해야 하는 최신 대상 숫자 조회 및 판단
    // prime이라면, primecount의 값 1 증가
    int n = inc_counter(&primesearch); // prime 여부 판단해야 하는 최신 대상 숫자 조회
    int sqrt_n = pow(n, 0.5);
    for (int i=2; i<=sqrt_n; ++i) {
        if (n % i == 0) {
            return;
        }
    }
    inc_counter(&primecount); // prime으로 판정 -> primecount 값 1 증가
    return;
}


void *count_primes(void *arg) {
    while (get_counter(&primesearch) < N) {
        check_prime();
    }
    return NULL;
}


int main() {
    time_t start_time = time(NULL);

    // thread 선언
    pthread_t p_threads[NUM_THREADS];

    // primesearch, primecount 초기화
    init_counter(&primesearch, 1); // 1을 소수로 세는 것 방지
    init_counter(&primecount, 0);

    // thread마다 task 할당
    for (int i=0; i<NUM_THREADS; ++i) {
        pthread_create(&p_threads[i], NULL, count_primes, NULL);
    }
    for (int i=0; i<NUM_THREADS; ++i) {
        pthread_join(p_threads[i], NULL);
    }

    // 모든 thread들의 작업이 완료된 후, 최종 prime 개수 조회
    long long numberOfPrime = get_counter(&primecount);

    cout << "소수의 개수: " << numberOfPrime << endl;
    time_t end_time = time(NULL);
    time_t elapsed_time = (double) (end_time - start_time);
    cout << "프로그램 수행 시간: " << elapsed_time << endl;
    return 0;
}