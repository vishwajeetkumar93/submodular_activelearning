import csv,sys,random

inputfile1 = sys.argv[1]
inputfile2 = sys.argv[2]
labeledpoolfile1 = inputfile1 + ".labledpool"
unlabeledpoolfile1 = inputfile1 + ".unlabledpool"
labeledpoolfile2 = inputfile2 + ".labledpool"
unlabeledpoolfile2 = inputfile2 + ".unlabledpool"
svmlighttrees = []
patterns = []

with open(inputfile1) as f:
	for line in f:
		svmlighttrees.append(line)
with open(inputfile2) as f:
	for line in f:
		patterns.append(line)
count = len(svmlighttrees)
print "Total Number of Data Points are "+ str(count)

line = raw_input("Enter number of data point in labled data: ")
labeled_data_points = int(line.strip())
random_list = random.sample(range(0,count),labeled_data_points)
f_labeledpool1 = open(labeledpoolfile1,"w")
f_unlabeledpool1 = open(unlabeledpoolfile1,"w")
f_labeledpool2 = open(labeledpoolfile2,"w")
f_unlabeledpool2 = open(unlabeledpoolfile2,"w")
for item in random_list:
	f_labeledpool1.write(svmlighttrees[item])
	f_labeledpool2.write(patterns[item])
for item in range(0,count):
	if(item not in random_list):
		f_unlabeledpool1.write(svmlighttrees[item])
		f_unlabeledpool2.write(patterns[item])