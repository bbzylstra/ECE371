
import random
#fnction for finding gcd of two numbers using euclidean algorithm
def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

#uses extened euclidean algorithm to get the d value
#for more info look here: https://crypto.stackexchange.com/questions/5889/calculating-rsa-private-exponent-when-given-public-exponent-and-the-modulus-fact
#will also be explained in class
def get_d(z,e):
    ###################################your code goes here#####################################
    a = (1,0)
    b = (0,1)
    output = 0
    
    while (output != 1):
        # Anonymous function to reduce tuple to numeric value
        value = lambda r: r[0]*z+r[1]*e

        # val is simply amodb
        output = value(a)%value(b)
        
        # Get the value of c by dividing a/b without the remainder
        c = value(a)//value(b)

        #Do tuple subtraction a-c*b, where c is the number of times b can go into a
        r = tuple(x-c*y for x, y in zip(a, b))
        
        #print(str(a) + " - " + str(b) + " = " + str(val))
        a = b
        b = r
    d = r[1]
    while d < 0:
        d+=z
    return d


def is_prime (num):
    if num > 1: 
      
        # Iterate from 2 to n / 2  
       for i in range(2, num//2): 
         
           # If num is divisible by any number between  
           # 2 and n / 2, it is not prime  
           if (num % i) == 0:
               return False 
               break
       return True
    else: 
        return False

def rel_prime(z, n):
    """
    Returns a random, relatively prime number to z in the range 1:n
    """
    # Had to comment this out due to time and memory errors.
    # Instead will get first 100 unique values
    # num_list = [i for i in range(1,n) if z%i!=0]

    rel_prime_list = []

    # Start from 5 to avoid the edge cases of 2,3, and 4 (each make the inner loop not run)
    for i in range(5,n):
        k = True
        for j in range(2,i//2):
            # if there is a number that divides both the checked number and z, then dont include
            if (z%j==0 and i%j==0 or z%i==0):
                k = False
                break
        if(k):
            rel_prime_list.append(i)
        if(len(rel_prime_list) > 100):
            break
    return rel_prime_list[random.randint(0,len(rel_prime_list)-1)]

def generate_keypair(p, q):
    if not (is_prime(p) and is_prime(q)):
        raise ValueError('Both numbers must be prime.')
    elif p == q:
        raise ValueError('p and q cannot be equal')
    ###################################your code goes here#####################################

    z = (p-1)*(q-1)
    n=p*q
    e = rel_prime(z,n)
    d=get_d(z,e)
    return ((e, n), (d, n))

def encrypt(pk, plaintext):
    ###################################your code goes here#####################################
    #plaintext is a single character
    #cipher is a decimal number which is the encrypted version of plaintext
    #the pow function is much faster in calculating power compared to the ** symbol !!!
    cipher=pow(ord(plaintext),pk[0],pk[1])
    return cipher

def decrypt(pk, ciphertext):
    ###################################your code goes here#####################################
    #ciphertext is a single decimal number
    #the returned value is a character that is the decryption of ciphertext
    plain=pow(ciphertext,pk[0],pk[1])
    return chr(plain)
