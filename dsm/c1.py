# you should know how XOR works!
# and about random seed, too

import random
import time

t = time.time()

def enc(x):
    rval = ""
    random.seed(t) # initialize the seed
    for i in range(len(x)):
        rval += chr(ord(x[i])^random.randint(0,255)) # XOR!!
    return rval

flag = "x0r_t0_findMEEE" # flag to auth.

plain = # something from GET['input']

ct  = map(enc,[plain,flag])
print ct # print it!