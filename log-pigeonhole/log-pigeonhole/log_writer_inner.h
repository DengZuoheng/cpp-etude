#ifndef LOG_WRITER_INNER_H
#define LOG_WRITER_INNER_H
#include <iostream>
#include <string>
#include <fstream>
#include <boost/noncopyable.hpp>
#include <boost/regex.hpp>
#include "log.h"

using std::string;
using std::ofstream;

class log_writer_inner :public boost::noncopyable{
public:
	log_writer_inner();
	log_writer_inner(const string& file_name);
	void on_pt_match(int line, string raw, boost::smatch match);
	void output();
	~log_writer_inner();
private:
	std::list<log_t> _logs;
	ofstream* _ofsp;
};

typedef boost::shared_ptr<log_writer_inner> log_writer_inner_ptr;

#endif