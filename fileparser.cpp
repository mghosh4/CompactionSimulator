#include <iostream>
#include <fstream>
#include <map>
#include "fileparser.h"
#include "constants.h"

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
			map<string, long>::iterator it = keyMap.find(key);
			if (it == keyMap.end())
				keyMap.insert(pair<string, long>(key, key.size()));

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
}

string YCSBParser::parse_line(string line)
{
	// The format of operations in YCSB is <OP> usertable key <values>
	size_t firstspace = line.find(" ");
	size_t secondspace = line.find(" ", firstspace + 1);
	size_t thirdspace = line.find(" ", secondspace + 1);

	string key = line.substr(secondspace + 1, thirdspace - secondspace - 1);
	return key;
}
