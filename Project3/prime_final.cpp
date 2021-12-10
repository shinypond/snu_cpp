#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <cmath>
#define N 100000000LL
#define NUM_THREADS 16
typedef long long LL;
using namespace std;


// sqrt(1조) = 100만 이하의 소수 리스트를 얻는 것이 목표.
typedef struct short_list {
    int size = (int) sqrt(N) + 1;
    bool *is_prime = new bool[size]; // 소수이면 1, 아니면 0. (k번째는 k를 의미)
} short_list;


typedef struct short_jumper {
    int jumpsize;
    pthread_mutex_t lock;
} short_jumper;


short_list sl;
short_jumper sj;

// 1조 이하의 소수 개수를 얻는 것이 목표.
typedef struct long_list {
    LL size = N + 1;
    bool *is_prime = new bool[size]; // 소수이면 1, 아니면 0. (k번째는 k를 의미)
} long_list;


typedef struct long_jumper {
    int short_pl_len = 1.5 * sqrt(N) / log(sqrt(N));
    int *short_pl = new int[short_pl_len]; // 100만 이하 소수 리스트
    int idx; // short_pl 내에서의 index
    pthread_mutex_t lock;
} long_jumper;


long_list ll;
long_jumper lj;


// Short list 전용 함수
void init_short_list(short_list *_sl) {
    // short_list의 초기화 함수
    for (int i=0; i<(_sl->size); ++i) {
        _sl->is_prime[i] = 1; // 1, ..., 1 꼴로 초기화
    }
    // 0, 1은 소수가 아님
    _sl->is_prime[0] = 0;
    _sl->is_prime[1] = 0;
}


void erase_multiple_short(short_list *_sl, int jumpsize) {
    // short_list의 첫 원소 0부터 시작하여
    // jumpsize만큼 건너뛰며 해당 위치 원소를 1 -> 0으로 변경
    // 단, jumpsize 자체가 이미 합성수로 판명된 경우, 즉시 종료
    if (_sl->is_prime[jumpsize] == 0) {
        return;
    }
    for (int i=2*jumpsize; i<(_sl->size); i+=jumpsize) {
        _sl->is_prime[i] = 0;
    }
}


void copy_short_primelist(short_list *_sl, long_jumper *_lj) {
    // short_list에서의 에라토스테네스 체 작업이 완료된 후,
    // 소수들을 모아서 long_jumper의 short_pl에 복사 (100만 이하 소수 총 78498개)
    // 단, short_pl의 동적 할당 크기는 n 이하 소수 개수의 근삿값인 n / log(n)의 1.5배로 지정한다.
    // (메모리 절감 + 유효한 상계)

    int idx = 0;

    // 초기화
    for (int i=0; i<_lj->short_pl_len; ++i) {
        _lj->short_pl[i] = 0;
    }
    // 소수이면, 결과 리스트에 추가
    // 단, 2는 제외 (추후 final_count에서 홀수 번째만 셀 예정)
    for (int i=3; i<(_sl->size); ++i) {
        if (_sl->is_prime[i]) {
            _lj->short_pl[idx] = i;
            idx++;
        }
    }
}


void init_short_jumper(short_jumper *_sj) {
    // short_jumper의 초기화 함수
    _sj->jumpsize = 1; 
    pthread_mutex_init(&_sj->lock, NULL);
}


int inc_short_jumper(short_jumper *_sj) {
    // short_jumper의 해당 시점 jumpsize 조회 및 1 증가
    pthread_mutex_lock(&_sj->lock);
    int js = ++_sj->jumpsize;
    pthread_mutex_unlock(&_sj->lock);
    return js;
}


int get_short_jumper(short_jumper *_sj) {
    // short_jumper의 해당 시점 jumpsize 조회
    pthread_mutex_lock(&_sj->lock);
    int js = _sj->jumpsize;
    pthread_mutex_unlock(&_sj->lock);
    return js;
}


void *erase_short(void *arg) {
    // short_list에 에라토스테네스 체를 적용
    // jumpsize만큼 건너뛰며 그의 배수인 합성수를 지우는 역할
    int n = sqrt(sl.size);
    int jumpsize;
    while (get_short_jumper(&sj) <= n) {
        jumpsize = inc_short_jumper(&sj);
        erase_multiple_short(&sl, jumpsize);
    }
    return NULL;
}


void *init_long_list(void *arg) {
    // long_list의 초기화 함수
    // 오래 걸리기 때문에 별도의 쓰레드에게 맡김
    for (LL i=0; i<(ll.size); ++i) {
        ll.is_prime[i] = 1; // 1, ..., 1 꼴로 초기화
    }
    // 0, 1은 소수가 아님
    ll.is_prime[0] = 0;
    ll.is_prime[1] = 0;
    return NULL;
}


void erase_multiple_long(long_list *_ll, int jumpsize) {
    // long_list의 첫 원소 0부터 시작하여
    // jumpsize만큼 건너뛰며 해당 위치 원소를 1 -> 0으로 변경
    // 참고 : jumpsize는 이미 소수임이 공인됨 (short_list 때 확인)
    for (LL i=2*jumpsize; i<(_ll->size); i+=jumpsize) {
        _ll->is_prime[i] = 0;
    }
}


void init_long_jumper(long_jumper *_lj) {
    // long_jumper의 초기화 함수
    _lj->idx = 0; 
    pthread_mutex_init(&_lj->lock, NULL);
}


int inc_long_jumper(long_jumper *_lj) {
    // long_jumper의 해당 시점 idx 조회 및 1 증가
    // 반환 결과는 해당 idx 번째의 소수 (jumpsize로 사용 예정)
    pthread_mutex_lock(&_lj->lock);
    int pl_idx = _lj->idx++;
    pthread_mutex_unlock(&_lj->lock);
    return _lj->short_pl[pl_idx];
}


int get_long_jumper(long_jumper *_lj) {
    // long_jumper의 해당 시점 jumpsize 조회
    pthread_mutex_lock(&_lj->lock);
    int pl_idx = _lj->idx;
    pthread_mutex_unlock(&_lj->lock);
    return _lj->short_pl[pl_idx];
}


void *erase_long(void *arg) {
    // long_list에 에라토스테네스 체를 적용
    // jumpsize만큼 건너뛰며 그의 배수인 합성수를 지우는 역할
    int n = sqrt(ll.size);
    int jumpsize;
    while (get_long_jumper(&lj) != 0) {
        jumpsize = inc_long_jumper(&lj);
        erase_multiple_long(&ll, jumpsize);
    }
    return NULL;
}


LL final_count(long_list *_ll) {
    // long_list에 대한 작업이 완료된 후,
    // 1인 것들의 개수를 덧셈 -> 최종 소수 개수
    // 단, 3부터 시작하여 2칸씩 움직임
    LL sum = 1;
    for (LL i=3; i<(_ll->size); ++(++i)) {
        sum += _ll->is_prime[i];
    }
    return sum;
}


int main() {
    time_t start_time = time(NULL);

    /* ################################### */
    /* ############## START ############## */
    /* ################################### */

    // thread 선언
    pthread_t p_threads[NUM_THREADS];

    // short_list, short_jumper 초기화
    init_short_list(&sl);
    init_short_jumper(&sj);

    // thread마다 jump task 할당
    for (int i=0; i<NUM_THREADS-1; ++i) {
        pthread_create(&p_threads[i], NULL, erase_short, NULL);
    }

    // last thread에는 long_list 초기화 task 할당
    pthread_create(&p_threads[NUM_THREADS-1], NULL, init_long_list, NULL);

    // thread 결합
    for (int i=0; i<NUM_THREADS; ++i) {
        pthread_join(p_threads[i], NULL);
    }

    // short_list에 대한 에라토스테네스 체 작업 완료 후
    // 100만 이하 소수 리스트 short_pl 획득
    copy_short_primelist(&sl, &lj);

    // short_list에 대한 메모리 할당 해제
    delete[] sl.is_prime;

    // long_jumper 초기화
    init_long_jumper(&lj);

    // thread마다 jump task 할당
    for (int i=0; i<NUM_THREADS; ++i) {
        pthread_create(&p_threads[i], NULL, erase_long, NULL);
    }
    for (int i=0; i<NUM_THREADS; ++i) {
        pthread_join(p_threads[i], NULL);
    }

    // 최종 소수 개수 count
    LL numberOfPrime = final_count(&ll);

    // 메모리 할당 해제
    delete[] lj.short_pl;
    delete[] ll.is_prime;

    /* ################################### */
    /* ##############  END  ############## */
    /* ################################### */

    cout << "소수의 개수: " << numberOfPrime << endl;
    time_t end_time = time(NULL);
    time_t elapsed_time = (double) (end_time - start_time);
    cout << "프로그램 수행 시간: " << elapsed_time << endl;
    return 0;
}