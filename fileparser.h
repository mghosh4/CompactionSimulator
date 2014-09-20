#include<vector>
using namespace std;

class FileParser
{
	public:
		FileParser() {}
		virtual vector<vector<long> > parse() = 0;
};

class YCSBParser : public FileParser
{
	public:
		YCSBParser(const char *filename) : mFilename(filename) {}
		virtual vector<vector<long> > parse();
	private:
		string mFilename;
};
