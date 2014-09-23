#include <iostream>
#include <fstream>
#include <map>
#include "fileparser.h"
#include "constants.h"

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
	vector<long> sstable;
	string line;
	map<string, long> keyMap;

	ifstream myfile (mFilename.c_str());
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			string key = parse_line(line);
			if (key.size() == 0)
				continue;

			map<string, long>::iterator it = keyMap.find(key);
			if (it == keyMap.end())
				keyMap.insert(pair<string, long>(key, keyMap.size()));

			sstable.push_back(keyMap.find(key)->second);
			if (sstable.size() >= SIZE_THRESHOLD)
			{
				sort(sstable.begin(), sstable.end());
				mSStables.push_back(sstable);
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
