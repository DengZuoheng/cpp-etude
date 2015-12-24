#ifndef READER_IMP_H_
#define READER_IMP_H_
#include <fstream>
#include "reader.h"

using std::ifstream;
using std::ofstream;

namespace pigeonhole{
	class file_reader :public reader{
	private:
		file_reader();
	public:
		file_reader(const string& file_name);
		bool getline(string& str);
		bool valid();
		~file_reader();
	private:
		std::ifstream* _isp;
	};
}

#endif