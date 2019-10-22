
import des
import sys
from socket import socket, AF_INET, SOCK_DGRAM, gethostbyname
from RSA import generate_keypair,encrypt,decrypt
import struct
from math import ceil

SERVER_IP    = gethostbyname( 'localhost' )
PORT_NUMBER = 5000
SIZE = 1024
des_key='password'
print ("Test client sending packets to IP {0}, via port {1}\n".format(SERVER_IP, PORT_NUMBER))

mySocket = socket( AF_INET, SOCK_DGRAM )
message='hello'

#first generate the keypair
#get these two numbers from the excel file
p=1297399
q=1297853
###################################your code goes here#####################################
keypair = generate_keypair(p, q)
public_keypair = keypair[0]
private_keypair = keypair[1]

#generate public and private key from the p and q values
#send key

print("public_key: "+str(public_keypair[0])+" "+str(public_keypair[1]))

message=('public_key: %d %d' % (public_keypair[0], public_keypair[1]))
mySocket.sendto(message.encode(),(SERVER_IP,PORT_NUMBER))
#send des_key

des_key_encrypted = ''

message=('des_key')
mySocket.sendto(message.encode(),(SERVER_IP,PORT_NUMBER))
###################################your code goes here#####################################

des_encoded = ['', '', '', '', '', '', '', '']

count = 0
for char in des_key:
    des_encoded[count] = str(encrypt(private_keypair, char))
    count = count + 1

[mySocket.sendto(code.encode(),(SERVER_IP,PORT_NUMBER)) for code in des_encoded]
#read image, encode, send the encoded image binary file
file=open(r'penguin.jpg',"rb") 
data=file.read()
file.close()
###################################your code goes here#####################################
#the image is saved in the data parameter, you should encrypt it using des.py
#set cbc to False when performing encryption, you should use the des class
#coder=des.des(), use bytearray to send the encryped image through network
#r_byte is the final value you will send through socket

#decode data as latin1 encoding
#encrypt data
#convert to byte array with utf-8 encoding
coder = des.des()
r_byte=bytearray()
s=''
count=0
data = data.decode('latin1')
print(data)
ciphertext = coder.encrypt(des_key, data)
print(ciphertext)
r_byte = bytearray(ciphertext, 'utf-8')


#send image through socket
mySocket.sendto(bytes(r_byte),(SERVER_IP,PORT_NUMBER))
print('encrypted image sent!')

