import sys, os, re

f=raw_input("enter file name - ")
print f
reader=open("./"+f,"r")
writer=open("./-"+f,"w")
for i, line in enumerate(reader.readlines()):
	writer.write(str(i+1)+"- "+line.strip()+"\n")
