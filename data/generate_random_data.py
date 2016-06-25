import sys,csv,random

inputfilename=sys.argv[1]
rdatapoint = int(sys.argv[2])

data = []
with open(inputfilename) as f:
	for line in f:
		data.append(line)
datapoint = len(data)
random_list = random.sample(range(0,datapoint),rdatapoint);
for item in random_list:
	print data[item],

