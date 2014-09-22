#include "kwaymerge.h"
#include <map>

vector< vector<long> > unique(vector< vector<long> > allTables)
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
		uniqSStables.push_back(final);
	}

	return uniqSStables;
}

vector<long> KWayNumberMerge::merge(vector< vector<long> > sets)
{
	map<long, vector<long>::iterator> numberMap;
	vector< vector<long> > sstables = unique(sets);
	vector<long> output;

	for (int i = 0; i < sstables.size(); i++)
	{
		vector<long>::iterator it = sstables[i].begin();
		numberMap.insert(pair<long, vector<long>::iterator>(*it, it));
	}

	while(numberMap.size())
	{
		map<long, vector<long>::iterator>::iterator mit = numberMap.begin();
		output.push_back(mit->first);
		vector<long>::iterator incrIt = mit->second + 1;
		numberMap.erase(mit);
		if (incrIt != output.end())
			numberMap.insert(pair<long, vector<long>::iterator>(*incrIt, incrIt));
	}

	return output;
}
