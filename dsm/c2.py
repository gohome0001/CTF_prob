import random
import time
import string

next_key = 0
priv_key = 0
data = ""

def enc(x,k): # ROT-k encryption
    rs = ""
    for i in x:
        rs += chr(65+((ord(i)-65+k)%26))
    return rs

def enc_2(p,k): # xor encryption
    rs = ""
    for c in p:
        rs += chr(ord(c)^k)
    return rs

key_sum = 0
random.seed(time.time())
flag =  # ???

f = open("out.txt","w")
        
for i in range(1000):
    key_sum += next_key

    next_key = random.randint(0,25)
    data = enc(''.join(random.choice(string.ascii_uppercase) for k in range(random.randint(1,50)))+"THENEXTKEYIS"+chr(next_key+65)+''.join(random.choice(string.ascii_uppercase) for k in range(random.randint(1,30))),priv_key)
    f.write(data+"\r\n")
    priv_key = next_key

next_key = key_sum % 256
f.write(enc_2(enc(flag,next_key),next_key)+"\r\n")
f.close()

# flag to auth.

