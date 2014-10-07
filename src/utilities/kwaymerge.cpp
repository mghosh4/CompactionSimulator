#include "kwaymerge.h"
#include <map>
#include <iostream>

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

vector< vector<long> > unique(vector< vector<long> > allTables, long &cost)
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
}

void printMap(map<long, Iterator> numberMap)
{
	for (map<long, Iterator>::iterator it = numberMap.begin(); it != numberMap.end(); it++)
		cout << it->first << " ";
	cout << "\n";
}

vector<long> KWayNumberMerge::merge(vector< vector<long> > sets, long &cost)
{
	map<long, Iterator> numberMap;
	vector< vector<long> > sstables = unique(sets, cost);
	vector<long> output;
	//cout << "Cost:" << cost << " " << sstables.size() << endl;

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
	//cout << "Cost:" << cost << endl;

	return output;
}
