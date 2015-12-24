#include "slot.h"

namespace pigeonhole{
	slot make_slot(boost::regex& pt, slot::handler_type& hdlr){
		slot ret;
		ret.pattern = pt;
		ret.handler = hdlr;
		return ret;
	}

	bool operator<(const slot& lhs, const slot& rhs){
		return &lhs < &rhs;
	}
}