#ifndef YCSBPARSER_H
#define YCSBPARSER_H

#include <map>
#include <vector>
#include "parser.h"
#include "../sstable/sstable.h"
using namespace std;

class YCSBParser: public Parser
{
	public:
		YCSBParser(const char *filename) : mFilename(filename) {}
		virtual void dump(vector<string> sstable, long &optimalLowerBound) = 0;
		virtual void parse();
	protected:
		string parse_line(string line);
	private:
		string mFilename;
};

class NumberParser: public YCSBParser
{
	public:
		NumberParser(const char* filename): YCSBParser(filename) {}
		virtual void dump(vector<string> sstable, long &optimalLowerBound);
		vector<vector<long> > getSStables() { return mSStables; }
	private:
		vector< vector<long> > mSStables;
		map<string, long> mKeyMap;
};

class FileParser: public YCSBParser
{
	public:
		FileParser(const char *filename, long proj_elem_count): YCSBParser(filename), mNumFiles(1), mElemCount(proj_elem_count) {}
		virtual void dump(vector<string> sstable, long &optimalLowerBound);
		vector<SStable> getSStables() { return mSStables; }
		long getNumFiles() { return mNumFiles; }
	private:
		vector<SStable> mSStables;
		long mNumFiles;
		long mElemCount;
};

#endif
