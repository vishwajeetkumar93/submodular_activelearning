import sys, os

reader=open("Input5000fa","r")
writer=open("corpusFile.txt","w")
x=reader.readlines()
for i in range(1,len(x)):
	line = x[i].strip("\n")
	ids = line.split("- ",1)[0]
	sen = line.split("- ",1)[1]
	size = len(sen.split(" "))
	print ids + " " + str(size)


		
