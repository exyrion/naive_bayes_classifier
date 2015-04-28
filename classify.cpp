//Justin Liang
//CS165A
//MP-2
//SPRING 2013

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <queue>
#include <map>
using namespace std;

map<string, int> project;
map<string, int> faculty;
map<string, int> course;
map<string, int> student;

map<string, int> total;
map<string, int> docfreq;

map<string, double> projectProb;
map<string, double> facultyProb;
map<string, double> courseProb;
map<string, double> studentProb;

int projectTotal = 0;
int facultyTotal = 0;
int courseTotal = 0;
int studentTotal = 0;

int totalDocs;
int totalWords = 0;
double averageWordsPerLine = 0;

//Computes total number of words for each category
void computeTotalWords()
{
	for(map<string, int>::iterator it = project.begin(); it != project.end(); it++)
	{
		projectTotal = projectTotal + it->second;
	}
	for(map<string, int>::iterator it = faculty.begin(); it != faculty.end(); it++)
	{
		facultyTotal = facultyTotal + it->second;
	}
	for(map<string, int>::iterator it = course.begin(); it != course.end(); it++)
	{
		courseTotal = courseTotal + it->second;
	}
	for(map<string, int>::iterator it = student.begin(); it != student.end(); it++)
	{
		studentTotal = studentTotal + it->second;
	}
}

//Compute the probability for each term, where F is feature, and C is label
//Uses the function log((1+#oftimestermoccursincategory/(#ofwordsincategory + m))
//*(1+log(totaldocs/#ofdocswithterm))) where m is a smoothing factor
//Uses log TF multiplied by IDF
void computeProbability()
{
	for(map<string, int>::iterator it = project.begin(); it != project.end(); it++)
	{
		double t = double(it->second);
		double idf = 1 + log(totalDocs/docfreq[it->first]);
		projectProb[it->first] = log((1+t/double(projectTotal+total.size()))*idf); //0.1158
	}
	for(map<string, int>::iterator it = faculty.begin(); it != faculty.end(); it++)
	{
		double t = double(it->second);
		double idf = 1 + log(totalDocs/docfreq[it->first]);
		facultyProb[it->first] = log((1+t/double(facultyTotal+total.size()))*idf); //0.2376
	}
	for(map<string, int>::iterator it = course.begin(); it != course.end(); it++)
	{
		double t = double(it->second);
		double idf = 1 + log(totalDocs/docfreq[it->first]);
		courseProb[it->first] = log((1+t/double(courseTotal+total.size()))*idf); //0.2762
	}
	for(map<string, int>::iterator it = student.begin(); it != student.end(); it++)
	{
		double t = double(it->second);
		double idf = 1 + log(totalDocs/docfreq[it->first]);
		studentProb[it->first] = log((1+t/double(studentTotal+total.size()))*idf); //0.3705
	}	
}

//Classifies the queue (line)
//Sums up the probability of each term in that line for each class label and returns
//the class label with the maximum probability
string classify(queue<string> q)
{
	double projectArray[q.size()];
	double facultyArray[q.size()];
	double courseArray[q.size()];
	double studentArray[q.size()];

	int i = 0;
	int length = q.size();
	while (!q.empty())
  	{
		string temp = q.front();
		projectArray[i] = projectProb[temp];
		facultyArray[i] = facultyProb[temp];
		courseArray[i] = courseProb[temp];
		studentArray[i] = studentProb[temp];
		q.pop();
		i++;
	}

	double projectSum = 0;
	double facultySum = 0;
	double courseSum = 0;
	double studentSum = 0;

	for(int x = 0; x < length; x++)
	{
		projectSum += projectArray[x];
		facultySum += facultyArray[x];
		courseSum += courseArray[x];
		studentSum += studentArray[x];		
	}
	// cout << projectSum << " " << facultySum << " " << courseSum << " " << studentSum << endl;
	if(projectSum >= facultySum && projectSum >= courseSum && projectSum >= studentSum)
	{
		return "project";
	}
	else if(facultySum >= projectSum && facultySum >= courseSum && facultySum >= studentSum)
	{
		return "faculty";
	}
	else if(courseSum >= projectSum && courseSum >= facultySum && courseSum >= studentSum)
	{
		return "course";
	}
	else if(studentSum >= projectSum && studentSum >= facultySum && studentSum >= courseSum)
	{
		return "student";
	}
}

int main(int argc, char* argv[])
{	
	//Opens the training file
	ifstream trainfile;
	trainfile.open(argv[1]);
	if(trainfile.is_open())
	{
		while(trainfile.good())
		{
			string line;
			getline(trainfile, line); //Gets each line from file
			totalDocs++; //Increase the count of each line	
			int start = 0;
			int end = 0;
			string category;
			map <string, int> checker;
			for(int i = 0; i < line.length(); i++) //Goes through each word in the line and adds them to the maps
			{
				//Checks first word of each line to assign to category
				if(line.at(i) == '\t')
				{
					end = i;
					category = line.substr(start,end-start);
					start = i+1;
				}

				//Goes through each word in the line
				if(line.at(i) == ' ')
				{						
					end = i;
					string temp = line.substr(start,end-start);
					start = i+1;

					//Adds word to the total map (for smoothing)
					if(total.find(temp) == total.end())
						total[temp] = 1;
					else
						total[temp]++;

					//Adds word to docfreq (for computing IDF)
					if(checker.find(temp) == checker.end())
						checker[temp] = 1;
					else
						checker[temp]++;

					if(docfreq.find(temp) == docfreq.end())
						docfreq[temp] = 1;
					else
					{
						if(checker[temp] == 1)
							docfreq[temp]++;
					}

					//Adds word to respective categories defined by first word in line
					if(category == "project")
					{
						if(project.find(temp) == project.end())
							project[temp] = 1;
						else
							project[temp]++;							
					}
					if(category == "faculty")
					{
						if(faculty.find(temp) == faculty.end())
							faculty[temp] = 1;
						else
							faculty[temp]++;							
					}
					if(category == "course")
					{
						if(course.find(temp) == course.end())
							course[temp] = 1;
						else
							course[temp]++;							
					}
					if(category == "student")
					{
						if(student.find(temp) == student.end())
							student[temp] = 1;
						else
							student[temp]++;							
					}
				}

				if(i == line.length()-1)
				{
					string temp = line.substr(start,line.length());

					//Adds word to the total map (for smoothing)
					if(total.find(temp) == total.end())
						total[temp] = 1;
					else
						total[temp]++;
					//Adds word to docfreq (for computing IDF)
					if(checker.find(temp) == checker.end())
						checker[temp] = 1;
					else
						checker[temp]++;

					if(docfreq.find(temp) == docfreq.end())
						docfreq[temp] = 1;
					else
					{
						if(checker[temp] == 1)
							docfreq[temp]++;
					}
					//Adds word to respective category
					if(category == "project")
					{
						if(project.find(temp) == project.end())
							project[temp] = 1;
						else
							project[temp]++;							
					}
					if(category == "faculty")
					{
						if(faculty.find(temp) == faculty.end())
							faculty[temp] = 1;
						else
							faculty[temp]++;							
					}
					if(category == "course")
					{
						if(course.find(temp) == course.end())
							course[temp] = 1;
						else
							course[temp]++;							
					}
					if(category == "student")
					{
						if(student.find(temp) == student.end())
							student[temp] = 1;
						else
							student[temp]++;							
					}
				}
			}
		}
	}
	trainfile.close();

	computeTotalWords();
	computeProbability();

	//Commented out. This is for printing out the maps for error checking
	/*
	for(map<string, int>::iterator it = project.begin(); it != project.end(); it++)
	{
		cout << "WORD: " << it->first << " | " << it->second << endl;
	}
	
	for(map<string, double>::iterator it = projectProb.begin(); it != projectProb.end(); it++)
	{
		cout << "WORD: " << it->first << " | " << it->second << endl;
	}

	for(map<string, int>::iterator it = docfreq.begin(); it != docfreq.end(); it++)
	{
		cout << "WORD: " << it->first << " | " << 1+log(double(totalDocs)/double(it->second)) << endl;
	}	
	cout << projectTotal << endl;
	*/

	//Opens the testing file
	ifstream testfile;
	testfile.open(argv[2]);
	ofstream predictions ("predictions.txt");
	if(testfile.is_open())
	{
		while(testfile.good())
		{
			string line;
			getline(testfile, line); //Gets each line from file
			int start = 0;
			int end = 0;
			queue<string> q; //Uses a queue to store the words of each line
			for(int i = 0; i < line.length(); i++) //Goes through each word and adds them to queue
			{
				if(line.at(i) == ' ')
				{						
					end = i;
					string temp = line.substr(start,end-start);
					start = i+1;
					q.push(temp); //Push current word into queue
				}

				if(i == line.length()-1)
				{
					string temp = line.substr(start,line.length());
					q.push(temp); //Push current word into queue
				}
			}
			//Outputs to predictions.txt
			predictions << classify(q); //Classifies current line (doc)
			predictions << endl;
		}
	}
}
