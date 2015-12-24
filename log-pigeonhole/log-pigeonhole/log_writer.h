#ifndef LOG_WRITER_H
#define LOG_WRITER_H
#include <string>
#include <boost/regex.hpp>
#include "log_writer_inner.h"

using std::string;

class log_writer{
public:
	log_writer(log_writer_inner_ptr inner) :_inner(inner){}
	void on_pt_match(int line, string raw, boost::smatch match){
		_inner->on_pt_match(line, raw, match);
	}
private:
	log_writer_inner_ptr _inner;
};

log_writer make_log_writer(string file_name){
	log_writer_inner_ptr inner_writer(new log_writer_inner(file_name));
	log_writer writer = log_writer(inner_writer);
	return writer;
}

#endif