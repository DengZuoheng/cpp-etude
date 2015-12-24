#ifndef SLOT_H_
#define SLOT_H_
#include <string>
#include <boost/function.hpp>
#include <boost/regex.hpp>
using std::string;
namespace pigeonhole{
	struct slot{
		typedef boost::function<void(int, string, boost::smatch)> handler_type;
		boost::regex pattern;
		handler_type handler;
	};

	slot make_slot(boost::regex& pt, slot::handler_type& hdlr);
	bool operator<(const slot& lhs, const slot& rhs);
}
#endif