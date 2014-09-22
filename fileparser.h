#ifndef FILEPARSER_H
#define FILEPARSER_H

#include<vector>
using namespace std;

class FileParser
{
	public:
		FileParser() {}
		virtual void parse() = 0;
};

class YCSBParser : public FileParser
{
	public:
		YCSBParser(const char *filename) : mFilename(filename) {}
		virtual void parse();
		vector<vector<long> > getSStables() { return mSStables; }
	private:
		string parse_line(string line);
		string mFilename;
		vector< vector<long> > mSStables;
};

#endif
