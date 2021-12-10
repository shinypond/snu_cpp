from mpmath import *
from tqdm import tqdm

with open('zetazeros.txt', 'w') as f:
    zeros = []
    for i in tqdm(range(1, 10001)):
        zero = float(zetazero(i).imag)
        zeros.append(str(zero))
    f.write('\n'.join(zeros))