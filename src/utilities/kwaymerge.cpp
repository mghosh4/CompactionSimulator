#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "kwaymerge.h"
#include "../utilities/utilities.h"
#include "../utilities/constants.h"

typedef struct
{
	vector<long>::iterator end;
	vector<long>::iterator next;
}Iterator;

void print_set2(vector< vector<long> > sets)
{
	for (vector< vector<long> >::iterator it = sets.begin(); it != sets.end(); it++)
	{
		vector<long> set = *it;
		for (vector<long>::iterator bit = set.begin(); bit != set.end(); bit++)
		{
			cout << *bit << " ";
		}
		cout << "\n";
	}
}

void print_set3(vector<long> set)
{
	for (vector<long>::iterator it = set.begin(); it != set.end(); it++)
	{
		cout << *it << " ";
	}
	cout << "\n";
}

/*vector< vector<long> > unique(vector< vector<long> > allTables, long &cost)
{
	vector< vector<long> > uniqSStables;
	for (vector< vector<long> >::iterator vit = allTables.begin(); vit != allTables.end(); vit++)
	{
		vector<long> a = *vit;
		vector<long> final;
		for (vector<long>::iterator it = a.begin(); it != a.end(); it++)
		{
			if (final.size() == 0 || *it != final[final.size() - 1])
				final.push_back(*it);
		}
		cost += final.size();
		uniqSStables.push_back(final);
	}

	//print_set2(uniqSStables);
	return uniqSStables;
}*/

void printMap(map<long, Iterator> numberMap)
{
	for (map<long, Iterator>::iterator it = numberMap.begin(); it != numberMap.end(); it++)
		cout << it->first << " ";
	cout << "\n";
}

vector<long> KWayNumberMerge::merge(vector< vector<long> > sstables, long &cost)
{
	map<long, Iterator> numberMap;
	//vector< vector<long> > sstables = unique(sets, cost);
	vector<long> output;
	//cout << "Cost:" << cost << " " << sstables.size() << endl;

	//cout << "Input Count ";
	for (vector< vector<long> >::iterator it = sstables.begin(); it != sstables.end(); it++)
	{
		//cout << it->size() << " ";
		cost += it->size();
	}
	//cout << "\n";

	for (int i = 0; i < sstables.size(); i++)
	{
		vector<long>::iterator it = sstables[i].begin();
		while (it != sstables[i].end() && numberMap.find(*it) != numberMap.end())
			it++;
		if (it != sstables[i].end())
		{
			Iterator itr;
			itr.next = it;
			itr.end = sstables[i].end();
			numberMap.insert(pair<long, Iterator>(*it, itr));
		}
	}

	//printMap(numberMap);

	map<long, Iterator>::iterator mit = numberMap.begin();
	while(mit != numberMap.end())
	{
		output.push_back(mit->first);
		vector<long>::iterator incrIt = mit->second.next + 1;
		vector<long>::iterator endIt = mit->second.end;
		numberMap.erase(mit);
		//cout << "Before erase";
		while (incrIt != endIt && numberMap.find(*incrIt) != numberMap.end())
			incrIt++;
		if (incrIt != endIt)
		{
			Iterator itr;
			itr.next = incrIt;
			itr.end = endIt;
			numberMap.insert(pair<long, Iterator>(*incrIt, itr));
		}
		//printMap(numberMap);
		mit = numberMap.begin();
	}

	//print_set3(output);
	//cost += output.size();
	//cout << "Cost:" << output.size() << endl;

	return output;
}

SStable KWayFileMerge::merge(vector<SStable> sstables, long numFiles, long &cost)
{
	map<string, ifstream*> keyMap;
	ifstream *fin[sstables.size()];
	string line;
	bool flag = false;
	SStable output;

	ostringstream ostr;
	ostr << sstablename << numFiles;
	output.filename = ostr.str();
	ofstream fout(output.filename.c_str());
	output.keyCount = 0;

	//cout << "Input Size ";
	for (vector<SStable>::iterator it = sstables.begin(); it != sstables.end(); it++)
	{
		//cout << it->keyCount << " ";
		cost += it->keyCount;
	}
	//cout << "\n";

	for (int i = 0; i < sstables.size(); i++)
	{
		//cout << "File Name:" << sstables[i].filename.c_str() << "\n";
		fin[i] = new ifstream(sstables[i].filename.c_str());
		flag = false;
		while (getline(*fin[i], line))
		{
			//cout << "Possible:" << line << "\n";
			if (keyMap.find(line) == keyMap.end())
			{
				flag = true;
				break;
			}
		}

		if (flag)
		{
			//cout << "First:" << line << "\n";
			keyMap.insert(pair<string, ifstream*>(line, fin[i]));
		}
	}

	map<string, ifstream*>::iterator mit = keyMap.begin();
	while(mit != keyMap.end())
	{
		//cout << "Selected:" << mit->first << "\n";
		fout << mit->first << endl;
		output.keyCount++;
		output.hll.add(mit->first.c_str(), mit->first.length());

		ifstream* fin = mit->second;
		flag = false;
		while (getline(*fin, line))
		{
			//cout << "Possible:" << line << "\n";
			if (keyMap.find(line) == keyMap.end())
			{
				flag = true;
				break;
			}
		}
			
		if (flag)
		{
			//cout << "First:" << line << "\n";
			keyMap.insert(pair<string, ifstream*>(line, fin));
		}

		//printMap(keyMap);
		keyMap.erase(mit);
		mit = keyMap.begin();
	}

	for (int i = 0; i < sstables.size(); i++)
	{
		fin[i]->close();
		delete fin[i];
	}

	//cout << "Merged File Count:" << output.keyCount << endl;

	fout.close();
	return output;
}
