#include "log_writer_inner.h"

log_writer_inner::log_writer_inner() :_ofsp(0){}

log_writer_inner::log_writer_inner(const string& file_name) : _ofsp(0){
	_ofsp = new ofstream(file_name);
}

void log_writer_inner::on_pt_match(int line, string raw, boost::smatch match){
	log_t log;
	log.line = line;
	log.type = match[1];
	log.content = match[2];
	log.timestamp = match[0];
	log.raw = raw;
	_logs.push_back(log);
}

void log_writer_inner::output(){
	if (!_ofsp) return;
	std::ostream& os = *_ofsp;
	if (!os) return;
	_logs.sort();
	for (std::list<log_t>::iterator it = _logs.begin();
		it != _logs.end();
		++it)
	{
		os << *it << std::endl;
	}
	_ofsp->close();
}

log_writer_inner::~log_writer_inner(){
	output();
	delete _ofsp;
}
