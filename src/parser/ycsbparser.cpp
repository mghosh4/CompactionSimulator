#include <fstream>
#include <iostream>
#include <sstream>
//#include <boost/serialization/map.hpp>
//#include <boost/serialization/string.hpp>

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
	//cout << "SIZE THRESHOLD:" << consts.SIZE_THRESHOLD << endl;
	vector<string> sstable;
	string line;
	long optimalLowerBound = 0;

	ifstream myfile (mFilename.c_str());
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			string key = parse_line(line);
			if (key.size() == 0)
				continue;

			sstable.push_back(line);

			if (sstable.size() >= consts.SIZE_THRESHOLD)
			{
				dump(sstable, optimalLowerBound);
				sstable.clear();
			}

		}
		if (sstable.size())
			dump(sstable, optimalLowerBound);
		myfile.close();
	}
	else
	{
		cout << "File Open failed" << endl;
	}

	cout << "Sum of File Size (Lower Bound Compaction Cost):" << optimalLowerBound << endl;

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

void NumberParser::dump(vector<string> sstable, long &optimalLowerBound)
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
	vector<long>::iterator it = unique(sortedSStable.begin(), sortedSStable.end());
	sortedSStable.resize(distance(sortedSStable.begin(), it));
	optimalLowerBound += sortedSStable.size();
	//cout << optimalLowerBound << endl;
	mSStables.push_back(sortedSStable);
}

void FileParser::dump(vector<string> sstable, long &optimalLowerBound)
{
	map<string, string> sortedSStable;
	SStable table(mElemCount);
	ostringstream ostr;

	ostr << consts.sstablename << mNumFiles++;
	table.filename = ostr.str();
	for (vector<string>::iterator it = sstable.begin(); it != sstable.end(); it++)
	{
		string key = parse_line(*it);
		if (sortedSStable.find(key) == sortedSStable.end())
			sortedSStable.insert(pair<string, string>(key, *it));
	}

	optimalLowerBound += sortedSStable.size();
	//cout << optimalLowerBound << endl;

	ofstream fout(table.filename.c_str());
	for (map<string, string>::iterator it = sortedSStable.begin(); it != sortedSStable.end(); it++)
	{
		table.hll.add(it->first.c_str(), it->first.length());
		table.bf.insert(it->first.c_str(), it->first.length());
		fout << it->first << endl;
	}
	table.keyCount = sortedSStable.size();
	mSStables.push_back(table);
	//cout << "Cardinality:" << table.keyCount << "  " << table.hll.estimate() << " " << table.bf.cardinality() << "\n";
	fout.close();
}
