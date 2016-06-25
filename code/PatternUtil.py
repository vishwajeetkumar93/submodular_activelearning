class Triplets :
	def __init__(self, sentenceId, beginPos, endPos) :
		self.sentenceId = sentenceId
		self.beginPos = beginPos
		self.endPos = endPos
	def __str__(self):
		return "( "+str(self.sentenceId)+","+str(self.beginPos)+","+str(self.endPos)+" )"
	def __repr__(self):
		return self.__str__()

class Pattern:
	def __init__(self,tree,patternId,alignment,triplets, patternString):
		self.patternId = patternId
		self.tree = tree
		self.patternString = patternString
		self.alignment = alignment
		self.triplets = triplets
	
	def getPattern(self):
		return self.tree

# 1- 1 |BT| (NP (NP (NN 0)) (PP (IN 0) (S (VP (VP (VBG 0) (NP (NP (DT 0)))))))) |ET|
def getPartsTree(line) :
	id = int(line.split('- ',1)[0])
	rest = line.split('- ',1)[1]
	alignment = int(rest.split(' ',1)[0])
	tree = rest.split(' ',1)[1]
	return [id, alignment, tree]

def getPartsPattern(line) :
	id = int(line.split('- ',1)[0])
	rest = line.split('- ',1)[1]
	triplets = rest[rest.rfind(': ')+2:]
	triplets = triplets.split(")(")
	triplets[0] = triplets[0][1:]
	triplets[len(triplets)-1] = triplets[len(triplets)-1][:-1]
	triplets = [Triplets(int(i.split(',')[0]), int(i.split(',')[1]), int(i.split(',')[2]))  for i in triplets]
	pattern = rest[0 : rest.rfind(': ')]
	return [id, triplets, pattern]

def loadPatterns(loadTreeString, loadPatternString) :
	patternobjectList = []
	f_loadTreeString = open(loadTreeString)
	f_loadPatternString = open(loadPatternString)
	treeList = f_loadTreeString.readlines()
	patternList = f_loadPatternString.readlines()
	for i in range(0, len(treeList)) :
		partsTree = getPartsTree(treeList[i].rstrip('\n'))
		partsPattern = getPartsPattern(patternList[i].rstrip('\n'))
		patternobjectList.append(Pattern(partsTree[2], partsPattern[0], partsTree[1], partsPattern[1], partsPattern[2]))
	return patternobjectList

def toDictionary(patternList) :
	patternDictionary = {}
	for pattern in patternList :
		patternDictionary[pattern.patternId] = pattern
	return patternDictionary

