#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <cmath>
#define N 10000LL
#define NUM_THREADS 12

using namespace std;


typedef struct longlist {
    bool is_prime[N+1]; // 0부터 시작 
} longlist;


typedef struct counter_t {
    long long value;
    pthread_mutex_t lock;
} counter_t;


counter_t jumper;
longlist primelist;


void init_primelist(longlist *pl) {
    // primelist의 초기화 함수
    for (long long i=0; i<N; ++i) {
        pl->is_prime[i] = 0;
    }
}


void erase_multiple(longlist *pl, int jumpsize) {
    // primelist의 첫 원소 0부터 시작하여,
    // jumpsize만큼 건너뛰며 해당 위치의 원소를 true -> false로 변경
    // 단, jumpsize 자체가 이미 합성수로 판명난 경우, 즉시 종료
    long long i = 0;
    for (long long i=0; i<=N; i+=jumpsize) {
        pl->is_prime[i] = 0;
    }
}


long long final_count(longlist *pl) {
    // 모든 계산 완료 후, primelist의 is_prime 배열 중 1인 값을 전부 sum
    long long sum;
    for (long long i; i<N; ++i) {
        sum += pl->is_prime[i];
    }
    return sum;
}


void init_jumper(counter_t *c) {
    // jumper의 초기화 함수
    c->value = 0; 
    pthread_mutex_init(&c->lock, NULL);
}


int inc_jumper(counter_t *c) {
    // jumper의 해당 시점 value (= jump의 jumpsize) 조회 및 1 증가
    pthread_mutex_lock(&c->lock);
    int rc = ++c->value;
    pthread_mutex_unlock(&c->lock);
    return rc;
}


int get_jumper(counter_t *c) {
    // jumper의 해당 시점 value (= jump의 jumpsize) 조회
    pthread_mutex_lock(&c->lock);
    int rc = c->value;
    pthread_mutex_unlock(&c->lock);
    return rc;
}


void *erase_composites(void *arg) {
    // jump의 jumpsize만큼 건너뛰며 그의 배수인 합성수를 지우는 역할
    int sqrt_N = pow(N, 0.5);
    while (get_jumper(&jumper) <= sqrt_N) {
        int jumpsize = inc_jumper(&jumper);

    }
    return NULL;
}


int main() {
    time_t start_time = time(NULL);

    // thread 선언
    pthread_t p_threads[NUM_THREADS];

    // primelist, jumper 초기화
    init_jumper(&jumper, 0);
    init_primelist(&primelist);

    // thread마다 task 할당
    for (int i=0; i<NUM_THREADS; ++i) {
        pthread_create(&p_threads[i], NULL, erase_composites, NULL);
    }
    for (int i=0; i<NUM_THREADS; ++i) {
        pthread_join(p_threads[i], NULL);
    }

    // 모든 thread들의 작업이 완료된 후, 최종 prime 개수 조회
    long long numberOfPrime = final_count(&primelist);

    cout << "소수의 개수: " << numberOfPrime << endl;
    time_t end_time = time(NULL);
    time_t elapsed_time = (double) (end_time - start_time);
    cout << "프로그램 수행 시간: " << elapsed_time << endl;
    return 0;
}