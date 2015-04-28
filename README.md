# Naive Bayes Classifier
#### C++

Text-based naive bayes classifier with TF/IDF smoothing

Design Decisions:

My design is as follows. First, I instantiated 4 maps of <string, int>for each of the categories to keep track of the word and the count of the word within the training.txt. I also instantiated 4 maps of <string, double> to keep track of the probabilities of each word given each category. I read the training file, perform a getline on each line, and then process each word, adding it into its respective maps if the term is not already in the map, otherwise I increase the count by 1. I have a computeTotalWords function that finds the total number of words within each map (category). I have a computeProbability() function that computes the probability with my algorithm and stores it into the probability maps. I have a classify(queue<string> q) function that takes a queue and processes each word in the queue. The queue stores the words in a specific line in the test file. It then computes the probability for each category of that line and chooses the maximum and returns the appropriate string.

Algorithm Details:
	
For the algorithm, I chose the log TF with a smoothing factor and I used IDF. Given a particular term, my formula to calculate probability is as follows: log((1+X/(Y+m) * (1+log(A/B))) where:

X = number of times a term occurs in a given category

Y = number of words in a given category

m = smoothing factor, total number of lines in the file

A = total # of documents

B = # of documents with that term