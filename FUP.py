#-*-coding:utf-8-*-
"""
Description     : Simple Python implementation of the Apriori Algorithm

Usage:
    $python apriori.py -f DATASET.csv -s minSupport  -c minConfidence

    $python apriori.py -f DATASET.csv -s 0.15 -c 0.6
"""

import sys
from collections import Counter
from itertools import chain, combinations
from collections import defaultdict
from optparse import OptionParser
import datetime
import csv

def subsets(arr):
    """ Returns non empty subsets of arr"""
    return chain(*[combinations(arr, i + 1) for i, a in enumerate(arr)])



def returnItemsWithMinSupport(itemSet, transactionList, minSupport, freqSet):
        """calculates the support for items in the itemSet and returns a subset
       of the itemSet each of whose elements satisfies the minimum support"""
        _itemSet = set()
        localSet = defaultdict(int)

        for item in itemSet:
                for transaction in transactionList:
                        if item.issubset(transaction):
                                freqSet[item] += 1
                                localSet[item] += 1

        for item, count in localSet.items():
                support = float(count)/len(transactionList)

                if support >= minSupport:
                        _itemSet.add(item)

        return _itemSet


def joinSet(itemSet, length):
        """Join a set with itself and returns the n-element itemsets"""
        return set([i.union(j) for i in itemSet for j in itemSet if len(i.union(j)) == length])


def getItemSetTransactionList(data_iterator):
    transactionList = list()
    itemSet = set()
    for record in data_iterator:
        transaction = frozenset(record)
        transactionList.append(transaction)
        for item in transaction:
            itemSet.add(frozenset([item]))              # Generate 1-itemSets
    return itemSet, transactionList


def getFreItem(data_iter, minSupport):
    """
    run the apriori algorithm. data_iter is a record iterator
    Return both:
     - items (tuple, support)
     - rules ((pretuple, posttuple), confidence)
    """
    itemSet, transactionList = getItemSetTransactionList(data_iter)

    freqSet = defaultdict(int)
    largeSet = dict()
    # Global dictionary which stores (key=n-itemSets,value=support)
    # which satisfy minSupport

    assocRules = dict()
    # Dictionary which stores Association Rules

    oneCSet = returnItemsWithMinSupport(itemSet,
                                        transactionList,
                                        minSupport,
                                        freqSet)

    currentLSet = oneCSet
    k = 2
    while(currentLSet != set([])):
        largeSet[k-1] = currentLSet
        currentLSet = joinSet(currentLSet, k)
        currentCSet = returnItemsWithMinSupport(currentLSet,
                                                transactionList,
                                                minSupport,
                                                freqSet)
        currentLSet = currentCSet
        k = k + 1
    return largeSet,freqSet,transactionList

def getSupport(item,freqSet,transactionList):
        """local function which Returns the support of an item"""
        return float(freqSet[item])/len(transactionList)

def ruleGenerate(largeSet,minConfidence,freqSet,transactionList):
    toRetItems = []
    for key, value in largeSet.items():
        toRetItems.extend([(tuple(item), getSupport(item,freqSet,transactionList))
                           for item in value])

    toRetRules = []
    for key, value in largeSet.items()[1:]:
        for item in value:
            _subsets = map(frozenset, [x for x in subsets(item)])
            for element in _subsets:
                remain = item.difference(element)
                if len(remain) > 0:
                    confidence = getSupport(item,freqSet,transactionList)/getSupport(element,freqSet,transactionList)
                    if confidence >= minConfidence:
                        toRetRules.append(((tuple(element), tuple(remain)),
                                           confidence))
    return toRetItems, toRetRules


def printResults(items, rules,path):
    """prints the generated itemsets sorted by support and the confidence rules sorted by confidence"""
    path = path+'\\'+'rule.txt'
    f = open(path,'w')
    for item, support in sorted(items, key=lambda (item, support): support):
        print "item: %s , %.3f" % (str(item), support)
        f.write("item: %s , %.3f\n" % (str(item), support))
    print "\n------------------------ RULES:"
    for rule, confidence in sorted(rules, key=lambda (rule, confidence): confidence):
        pre, post = rule
        print "Rule: %s ==> %s , %.3f" % (str(pre), str(post), confidence)
        f.write("Rule: %s ==> %s , %.3f\n" % (str(pre), str(post), confidence))
    f.close()


def dataFromFile(fname):
        """Function which reads from the file and yields a generator"""
        file_iter = open(fname, 'rU')
        for line in file_iter:
                line = line.strip().rstrip(',')                         # Remove trailing comma
                record = frozenset(line.split(','))
                yield record

def getComFreItem(largeSetDB,freqSetDB,transactionListDB,largeSetdb,freqSetdb,transactionListdb,minSupport):
    # 生成联合字典
    largeSet = dict()
    freqSet = defaultdict(int)
    freqSetUnion = []
    lenDBtra = len(transactionListDB)
    lendbtra = len(transactionListdb)
    lenDBlargeSet = len(largeSetDB)
    lendblargeSet = len(largeSetdb)
    minlen = min(lendblargeSet,lenDBlargeSet)
    for i in range(1,minlen+1):
        # 曲两个数据库频繁项集拼接
        # 对数据取交和取并之后取余操作
        unionfreitem = set()
        unionfreitem = largeSetDB[i]&largeSetdb[i]
        difffreDB = largeSetDB[i]-largeSetdb[i]
        for elefre in difffreDB:
            countDB = freqSetDB[elefre]
            coundb = freqSetdb[elefre]
            if (countDB+coundb)/float(lenDBtra+lendbtra)>=minSupport:
                unionfreitem.add(elefre)
        difffredb = largeSetdb[i]-largeSetDB[i]
        for elefre in difffredb:
            countDB = freqSetDB[elefre]
            coundb = freqSetdb[elefre]
            if (countDB+coundb)/float(lenDBtra+lendbtra)>=minSupport:
                unionfreitem.add(elefre)
        largeSet[i] = unionfreitem
    singleFreitem = set()
    if lenDBlargeSet > lendblargeSet:
        for i in range(lendblargeSet+1,lenDBlargeSet):
            for elefre in largeSetDB[i]:
                countDB = freqSetDB[elefre]
                if countDB/float(lenDBtra+lendbtra)>=minSupport:
                    singleFreitem.add(elefre)
        largeSet[i] = singleFreitem
    if lendblargeSet > lenDBlargeSet:
        for i in range(lenDBlargeSet+1,lendblargeSet):
            for elefre in largeSetdb[i]:
                countdb = freqSetDB[elefre]
                if countdb/float(lenDBtra+lendbtra)>=minSupport:
                    singleFreitem.add(elefre)
        largeSet[i] = singleFreitem
    freqSet = dict(Counter(freqSetDB)+Counter(freqSetdb))
    transactionList = transactionListdb+transactionListDB
    return largeSet,freqSet,transactionList

def Fileclose(f):
    f.close()
    return 1

if __name__ == "__main__":

    optparser = OptionParser()
    optparser.add_option('-f', '--inputFile',
                         dest='input',
                         help='filename containing csv',
                         default=None)
    optparser.add_option('-s', '--minSupport',
                         dest='minS',
                         help='minimum support value',
                         default=0.15,
                         type='float')
    optparser.add_option('-c', '--minConfidence',
                         dest='minC',
                         help='minimum confidence value',
                         default=0.6,
                         type='float')

    (options, args) = optparser.parse_args()
    inFile = dataFromFile('k2.csv')
    if options.input is None:
            inFile = sys.stdin
    elif options.input is not None:
            inFile = dataFromFile(options.input)
    else:
            print 'No dataset filename specified, system with exit\n'
            sys.exit('System will exit')
    minSupport = 0.4
    minConfidence = 0.6
    starttime = datetime.datetime.now()
    avgpath = 'D:\\csv\\tranlist10.csv'
    f = open(avgpath, 'r')
    mark = Fileclose(f)
    inputData = csv.reader(open(avgpath, 'r'))
    next(inputData)  # filter the first row
    # start the algorithm using Apriori
    largeSetDB,freqSetDB,transactionListDB = getFreItem(inputData, minSupport)
    increPath =  'D:\\csv\\tranlist20.csv'
    increInputData = csv.reader(open(increPath, 'r'))
    next(increInputData)
    largeSetdb,freqSetdb,transactionListdb = getFreItem(increInputData, minSupport)
    # set1 = largeSet[1]
    # item = list(set1)
    # count = freqSet[item[1]]
    # toRetItems, toRetRules = ruleGenerate(largeSet,minConfidence,freqSet,transactionList)
    # t = zip(*itemsStar)
    largeSet,freqSet,transactionList = getComFreItem(largeSetDB,freqSetDB,transactionListDB,largeSetdb,freqSetdb,transactionListdb,minSupport)
    toRetItems, toRetRules = ruleGenerate(largeSet,minConfidence,freqSet,transactionList)
    endtime = datetime.datetime.now()
    print (endtime-starttime).seconds
    path = 'D:\csv'
    printResults(toRetItems, toRetRules,path)
    # 循环输入部分
    # 读取文件夹下文件做监控