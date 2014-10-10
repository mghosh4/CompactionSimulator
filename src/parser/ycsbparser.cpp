#include <fstream>
#include <iostream>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>

#include "ycsbparser.h"
#include "../utilities/constants.h"
#include "../utilities/utilities.h"

void print_set(vector< vector<long> > sstables)
{
	for (vector< vector<long> >::iterator it = sstables.begin(); it != sstables.end(); it++)
	{
		vector<long> set = *it;
		for (vector<long>::iterator bit = set.begin(); bit != set.end(); bit++)
		{
			cout << *bit << " ";
		}
		cout << "\n";
	}
}

void YCSBParser::parse()
{
	vector<string> sstable;
	string line;

	ifstream myfile (mFilename.c_str());
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			string key = parse_line(line);
			if (key.size() == 0)
				continue;

			sstable.push_back(line);

			if (sstable.size() >= SIZE_THRESHOLD)
			{
				dump(sstable);
				sstable.clear();
			}

		}
		myfile.close();
	}
	else
	{
		cout << "File Open failed" << endl;
	}

	//print_set(mSStables);
}

string YCSBParser::parse_line(string line)
{
	// The format of operations in YCSB is <OP> usertable key <values>
	size_t firstspace = line.find(" ");
	size_t secondspace = line.find(" ", firstspace + 1);
	size_t thirdspace = line.find(" ", secondspace + 1);

	string operation = line.substr(0, firstspace);
	string key;
	if (!operation.compare("INSERT") || !operation.compare("UPDATE"))
		key = line.substr(secondspace + 1, thirdspace - secondspace - 1);
	return key;
}

void NumberParser::dump(vector<string> sstable)
{
	vector<long> sortedSStable;
	for (vector<string>::iterator it = sstable.begin(); it != sstable.end(); it++)
	{
		string key = parse_line(*it);
		map<string, long>::iterator it = mKeyMap.find(key);
		if (it == mKeyMap.end())
			mKeyMap.insert(pair<string, long>(key, mKeyMap.size()));

		sortedSStable.push_back(mKeyMap.find(key)->second);
	}
	sort(sortedSStable.begin(), sortedSStable.end());
	mSStables.push_back(sortedSStable);
}

struct classcomp
{
	bool operator()(string str1, string str2)
	{
		return str1.compare(str2);
	}
};	

void FileParser::dump(vector<string> sstable)
{
	map<string, string, classcomp> sortedSStable;
	for (vector<string>::iterator it = sstable.begin(); it != sstable.end(); it++)
	{
		string key = parse_line(*it);
		sortedSStable.insert(pair<string, string>(key, *it));
	}

	char filename[100];
	sprintf(filename, "File-%d.db", mNumFiles++);
	Save<map<string,string, classcomp> >(string(filename), sortedSStable);			
}
