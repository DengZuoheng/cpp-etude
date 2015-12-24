#ifndef LOG_H_
#define LOG_H_
#include <string>
using std::string;

struct log_t{
	int line;
	string type;
	string content;
	string timestamp;
	string raw;
};

std::ostream& operator<<(std::ostream& os, const log_t& l);

bool operator<(const log_t& lhs, const log_t& rhs);

#endif