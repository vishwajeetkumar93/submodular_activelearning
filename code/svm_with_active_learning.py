import os,sys,random
from operator import itemgetter
import math
import PatternUtil as pt

# file which are read once in starting

testingfile = "../data/KernelDataForPattern.test"
treefile = "../data/KernelDataForPattern"
patternfile = "../data/Patterns"

#-----------------------------Threshold Parameter----------------------------------------------------------------------------------------------

InitialLabeledPoolSize = 500
ConfidenceThresholdForClassifier = 2000000
CoverageThreshold = 0.75

#----------------------------------------------------------------------------------------------------------------------------------------------

corpusfile = "../sfo/data/CorpusFile.txt"
submodularpatterninputfile = "../sfo/data/pattern"
submodularSelectedPatternfile = "../finalset/final"
submodularoutputfile = "../sfo/data/outputpattern"
model_file = "../model/model"
trainingdatafile = "../temp/trainingdata"
unlabledpoolpredictionfile = "../temp/unlabledpoolpredictionfile"
SVMPredictionfile = "svm_predictions"
accuracyfile = "../accuracy/accuracy"

#-----------------------------------------------------------------------------------------------------------------------------------------------------

BitCorpus = []
Dictpatterns = {}
ListlabeledPool = []
ListunlabedlePool = []
coverage = 0.1
loopcounter = 10
BoolTrainClassifier = True
BoolPerformLazyGreedy = True
ListH = [] # final set
TotalWordsInCorpus = 0

#----------------------------------------------------------------------------------------------------------------

def deleteContent(fName):
    with open(fName, "w"):
        pass

def totalWords():
	count = 0
	for item in BitCorpus:
		count = count + len(item)
	return count

def loadBitCorpus():
	BitCorpus.append([0]*10)
	with open(corpusfile) as f:
		for line in f:
			line = line.strip("\n")
			length = int(line.split(" ")[1])
			BitCorpus.append([0]*length)

def computeSVMConfidence():
	os.system("../svm-light-TK-1.2/svm-light-TK-1.2.1/svm_classify -f 1 "+ testingfile + " "+ model_file)
	confidence = 0
	with open(SVMPredictionfile) as f:
		for line in f:
			line = line.strip("\n")
			confidence = confidence + abs(float(line))
	return confidence

def updateCoverage():
	for patternid in ListH:
		p = Dictpatterns[patternid]
		for triple in p.triplets:
			sentence = BitCorpus[triple.sentenceId]
			for i in range(triple.beginPos,triple.endPos+1):
				sentence[i] = 1

def computeCoverage():
	count = 0 
	for patternid in ListH:
		p = Dictpatterns[patternid]
		for triple in p.triplets:
			sentence = BitCorpus[triple.sentenceId]
			for i in range(triple.beginPos,triple.endPos+1):
				if sentence[i]==1:
					count += 1;
	return count

def generateLabeled_UnlabeledPool():
	global ListlabeledPool
	count = len(Dictpatterns)
	ListlabeledPool = random.sample(range(0,count),InitialLabeledPoolSize)
	for item in Dictpatterns:
		if item not in ListlabeledPool:
			ListunlabedlePool.append(item)


# this function writes the labeled pool into the file for learning classifier
def writeTrainingDataForClassifier():
	f_trainingdatafile = open(trainingdatafile,"w")
	for patternid in ListlabeledPool:
		p  = Dictpatterns[patternid]
		f_trainingdatafile.write(str(p.alignment)+" "+ p.tree+"\n")
		
def writeUnlabelPoolForClassifier():
	f_unlabledpoolpredictionfile = open(unlabledpoolpredictionfile,"w")
	for id in ListunlabedlePool:
		pattern = Dictpatterns[id]
		f_unlabledpoolpredictionfile.write(str(pattern.alignment)+" "+ pattern.tree+"\n")

def writeUnlabeledPoolForSubmodular():
	f_submodularpatterninputfile = open(submodularpatterninputfile,"w")
	for index in ListunlabedlePool:
		pattern = Dictpatterns[index]
		f_submodularpatterninputfile.write(str(index)+"- "+pattern.patternString+" : "+" ".join([str(i) for i in pattern.triplets])+"\n")

def writeFinalSetPatterns():
	f_submodularSelectedPatternfile = open(submodularSelectedPatternfile,"w")
	for index in ListH:
		pattern = Dictpatterns[index]
		f_submodularSelectedPatternfile.write(str(index)+"- "+pattern.patternString+" : "+" ".join([str(i) for i in pattern.triplets])+"\n")

def readNewBatch():
	X = []
	with open(submodularoutputfile) as f:
		for line in f:
			X.append(int(line.split("- ",1)[0]))
	return X 

#------------------------------------------------------------------------------------------------------------------

deleteContent(submodularSelectedPatternfile)
#load patterns
print "Reading Patterns......"
Dictpatterns = pt.toDictionary(pt.loadPatterns(treefile,patternfile))
print str(len(Dictpatterns))+"patterns read......"
#load bitcorpus
loadBitCorpus()
print "Bit Corpus size "+str(len(BitCorpus))
TotalWordsInCorpus = totalWords()
#create labeled and unlabeled pool
generateLabeled_UnlabeledPool()
print "Initial Labeled Pool size "+ str(len(ListlabeledPool))
print "Initial Unlabeled Pool size "+ str(len(ListunlabedlePool))


while(True):

	if BoolTrainClassifier:
		if(os.path.isfile(model_file)):
			os.remove(model_file)

		writeTrainingDataForClassifier()
		os.system("../svm-light-TK-1.2/svm-light-TK-1.2.1/svm_learn -t 5 -c 3 "+trainingdatafile + " "+ model_file)
		if not(os.path.isfile(model_file)):
			print "Error while training the svm."
			sys.exit(1)
	#classify unlabled pool
	print "Writing Unlabeled pool data for Classification...."
	writeUnlabelPoolForClassifier()
	print "Writing Unlabeled pool data for Classification ends......"
	print "Classifying Unlabeled Data...."
	os.system("../svm-light-TK-1.2/svm-light-TK-1.2.1/svm_classify -f 1 "+ unlabledpoolpredictionfile + " "+ model_file+" > " + accuracyfile)
	print "Classifying Unlabeled Data ends...."


	if BoolPerformLazyGreedy:
		#write unlabled pool for submodular
		print "Writing Unlabled pattern for submodular....."
		writeUnlabeledPoolForSubmodular()
		print "Writing Unlabled pattern for submodular ends....."

		# call sfo (SC) opimization
		print "Starting submodular optimization......"
		os.system("../sfo/a.out "+str(coverage)+" 0 > "+ submodularoutputfile)
		print "Submodular optimization ends..."

	# Now read the pattern set X generated by above optimization algorithm 
	sfooutput = readNewBatch()
	#X = [x for x in sfooutput if x not in ListH]
	Y = [] # label of the patterns X
	#compute the confidence of classfier
	confidence = computeSVMConfidence()
	if confidence > ConfidenceThresholdForClassifier:
		#get the label from classifer itself

		BTrainClassifier = True
	else:
		# get the alignment of pattern in X from the user
		for item in sfooutput:
			pattern = Dictpatterns[item]
			Y.append(pattern.alignment)
		# add X to labelded pool
		for item in sfooutput:
			ListlabeledPool.append(item)
		# remove X from unlabled pool
		for item in sfooutput:
			ListunlabedlePool.remove(item)
	# add all the aligned pattern into H
	for index in range(len(sfooutput)):
		if Y[index] == 1:
			ListH.append(sfooutput[index])
	print "Size of Labled Pool"+str(len(ListlabeledPool))
	print "Size of Final Set"+str(len(ListH))

	# write selected final set of patterns in file
	print "Writing final set of pattern for submodular..."
	writeFinalSetPatterns()
	print "Writing final set of pattern for submodular ends..."

	#update the coverage
	updateCoverage()

	#compute the coverage on corpus
	currentCoverage = computeCoverage()
	currentCoverage = float(currentCoverage)/TotalWordsInCorpus
	if currentCoverage >= CoverageThreshold:
		break
	coverage += 0.1

















