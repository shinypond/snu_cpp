import numpy as np 

x = 10 ** 12
k = np.log(x)
def factorial(n):
    if n <= 1:
        return 1
    else:
        return n * factorial(n-1)

def f(n):
    if n <= 1:
        return k
    else:
        return f(n-1) * k * (n-1) / (n ** 2)


for i in range(1, 80):
    print(i, f(i))
