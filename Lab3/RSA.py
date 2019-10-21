
import random
from math import floor

#fnction for finding gcd of two numbers using euclidean algorithm
def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

#uses extened euclidean algorithm to get the d value
#for more info look here: https://crypto.stackexchange.com/questions/5889/calculating-rsa-private-exponent-when-given-public-exponent-and-the-modulus-fact
#will also be explained in class
def get_d(e, z):
    ###################################your code goes here#####################################
    a = (1,0)
    b = (0,1)
    output = 0
    
    while (output != 1):

        # Anonymous function to reduce tuple to numeric value
        value = lambda (x,y): x*e+y*z

        # val is simply amodb
        output = value(a)%value(b)
        # Get the value of c by dividing a/b without the remainder
        c = floor(value(a)/value(b))

        #Do tuple subtraction a-c*b, where c is the number of times b can go into a
        r = tuple(x-c*y for x, y in zip(a, b))
        
        #print(str(a) + " - " + str(b) + " = " + str(val))
        a = b
        b = r

    return r[1]

def is_prime (num):
    if num > 1: 
      
        # Iterate from 2 to n / 2  
       for i in range(2, num//2): 
         
           # If num is divisible by any number between  
           # 2 and n / 2, it is not prime  
           if (num % i) == 0: 
               return False 
               break
           else: 
               return True 
  
    else: 
        return False


def generate_keypair(p, q):
    if not (is_prime(p) and is_prime(q)):
        raise ValueError('Both numbers must be prime.')
    elif p == q:
        raise ValueError('p and q cannot be equal')
    ###################################your code goes here#####################################

    e=0
    n=0
    d=0
    return ((e, n), (d, n))

def encrypt(pk, plaintext):
    ###################################your code goes here#####################################
    #plaintext is a single character
    #cipher is a decimal number which is the encrypted version of plaintext
    #the pow function is much faster in calculating power compared to the ** symbol !!!
    cipher=0;
    return cipher

def decrypt(pk, ciphertext):
    ###################################your code goes here#####################################
    #ciphertext is a single decimal number
    #the returned value is a character that is the decryption of ciphertext
    plain='a'
    return ''.join(plain)
