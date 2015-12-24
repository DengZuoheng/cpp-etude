#include "log.h"

std::ostream& operator<<(std::ostream& os, const log_t& l){
	os << l.raw;
	return os;
}

bool operator<(const log_t& lhs, const log_t& rhs){
	if (lhs.timestamp == rhs.timestamp) return lhs.line < rhs.line;
	return lhs.timestamp < rhs.timestamp;
}