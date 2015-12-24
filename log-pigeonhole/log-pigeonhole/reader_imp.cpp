#include "reader_imp.h"
namespace pigeonhole{

	file_reader::file_reader() :_isp(0){}

	file_reader::file_reader(const string& file_name) : _isp(0){
		_isp = new ifstream(file_name, ifstream::in);
	}

	bool file_reader::getline(string& str){
		if (!valid()) return false;
		if (std::getline(*_isp, str)) return true;
		return false;
	}

	bool file_reader::valid(){
		if (_isp&&*_isp) return true;
		return false;
	}

	file_reader::~file_reader(){
		if (!_isp) return;
		_isp->close();
		delete _isp;
	}
}
