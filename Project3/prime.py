from bisect import bisect
from math import floor

def prime_sieve(n):
    sieve = [True] * (n//2) 
    for i in range(3, int(n ** 0.5) + 1, 2):
        if sieve[i//2]:
            sieve[(i*i)//2::i] = [False] * ((n-i*i-1) // (2*i) + 1)
    return [2] + [2*i+1 for i in range(1, n//2) if sieve[i]]

limit = 10 ** 5
primes = prime_sieve(limit)
phi_cache = {}

def phi(x, a):
    if (x, a) in phi_cache: return phi_cache[(x, a)]
    if a == 1:
        return (x + 1) // 2
    result = phi(x, a-1) - phi(x / primes[a-1], a-1)
    phi_cache[(x, a)] = result
    return int(result)

pi_cache = {}

def pi(x):
    x = int(x)
    if x in pi_cache: return pi_cache[x]
    if x < limit:
        result = bisect(primes, x)
        pi_cache[x] = result
        return result

    a = pi(x ** (1./4))
    b = pi(x ** (1./2))
    c = pi(x ** (1./3))

    result = phi(x, a) + (b + a - 2) * (b - a + 1) / 2

    for i in range(int(a)+1, int(b)+1):
        w = x / primes[i-1]
        b_i = pi(w ** (1./2))
        result = result - pi(w)
        if i <= c:
            for j in range(i, b_i+1):
                result = result - pi(w / primes[j-1]) + j - 1
    pi_cache[x] = result
    
    return int(result)

result = pi(10**8)
print(result)