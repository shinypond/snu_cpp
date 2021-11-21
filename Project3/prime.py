from random import randint
from math import sqrt, ceil, floor, log
import time
import sys

def sieveOfAtkin(end):
    end += 1
    lng = ((end/2)-1+end%2)
    sieve = [False] * int(lng + 1)

    x_max, x2, xd = int(sqrt((end-1)/4.0)), 0, 4
    for xd in range(4, 8*x_max + 2, 8):
        x2 += xd
        y_max = int(sqrt(end-x2))
        n, n_diff = x2 + y_max**2, (y_max << 1) - 1
        if n%2 == 0:
            n -= n_diff
            n_diff -= 2
        for d in range((n_diff - 1) << 1, -1, -8):
            m = n%12
            if (m == 1 or m == 5):
                    m = n >> 1
                    sieve[m] = not sieve[m]
            n -= d
                
    x_max, x2, xd = int(sqrt((end-1)/3.0)), 0, 3
    for xd in range(3, 6*x_max + 2, 6):
        x2 += xd
        y_max = int(sqrt(end-x2))
        n, n_diff = x2 + y_max**2, (y_max << 1) - 1
        if n%2 == 0:
            n -= n_diff
            n_diff -= 2
        for d in range((n_diff - 1) << 1, -1, -8):
            if (n%12 == 7):
                    m = n >> 1
                    sieve[m] = not sieve[m]
            n -= d
                
    x_max, y_min, x2, xd = int((2 + sqrt(4-8*(1-end)))/4), -1, 0, 3
    for x in range(1, x_max + 1):
        x2 += xd
        xd += 6
        if x2 >= end: y_min = (((int(ceil(sqrt(x2 - end))) - 1) << 1) - 2) << 1
        n, n_diff = ((x**2 + x) << 1) - 1, (((x-1) << 1) - 2) << 1
        for d in range(n_diff, y_min, -8):
            if (n%12 == 11):
                    m = n >> 1
                    sieve[m] = not sieve[m]
            n += d

    primes = [2,3]
    if end <= 3 : return primes[:max(0,end-2)]
    
    for n in range(5 >> 1, (int(sqrt(end))+1) >> 1):
        if sieve[n]:
            primes.append((n << 1) + 1)
            for k in range(((n << 1) + 1)**2, end, 2*((n << 1) + 1)**2):
                sieve[k >> 1] = False

    s  = int(sqrt(end)) + 1
    if s%2 == 0: s += 1
    primes.extend([ i for i in range(s, end, 2) if sieve[i >> 1]])
    
    return primes
    
def sieveOfErat(end):
    if end < 2: return []

    #The array doesn't need to include even numbers
    lng = ((end/2)-1+end%2)
    
    # Create array and assume all numbers in array are prime
    sieve = [True]*int(lng+1)

    # In the following code, you're going to see some funky
    # bit shifting and stuff, this is just transforming i and j
    # so that they represent the proper elements in the array
    
    # Only go up to square root of the end
    for i in range(int(sqrt(end)) >> 1):
            
        # Skip numbers that aren't marked as prime
        if not sieve[i]: continue

        # Unmark all multiples of i, starting at i**2
        for j in range( (i*(i + 3) << 1) + 3, int(lng), (i << 1) + 3):
            sieve[j] = False
            
    # Don't forget 2!
    primes = [2]

    # Gather all the primes into a list, leaving out the composite numbers
    primes.extend([(i << 1) + 3 for i in range(int(lng)) if sieve[i]])

    return primes


for j in range(5,100000006, 1000000):
        print(j)
        
        a = time.time()
        sieveOfErat(j)
        print(time.time()-a)
        
        a = time.time()
        sieveOfAtkin(j)
        print(time.time()-a)