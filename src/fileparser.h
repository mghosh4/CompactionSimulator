#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <map>
#include <vector>
using namespace std;

class YCSBParser
{
	public:
		YCSBParser(const char *filename) : mFilename(filename) {}
		virtual void dump(vector<string> sstable) = 0;
		void parse();
	protected:
		string parse_line(string line);
	private:
		string mFilename;
};

class NumberParser: public YCSBParser
{
	public:
		NumberParser(const char* filename): YCSBParser(filename) {}
		virtual void dump(vector<string> sstable);
		vector<vector<long> > getSStables() { return mSStables; }
	private:
		vector< vector<long> > mSStables;
		map<string, long> mKeyMap;
};

class FileParser: public YCSBParser
{
	public:
		FileParser(const char *filename): YCSBParser(filename), mNumFiles(0) {}
		virtual void dump(vector<string> sstable);
		long getNumFiles() { return mNumFiles; }
	private:
		long mNumFiles;
};

#endif
