#ifndef PIGEONHOLE_H_
#define PIGEONHOLE_H_
#include <set>
#include <string>
#include <boost/function.hpp>
#include <boost/regex.hpp>
#include "slot.h"
#include "reader.h"

namespace pigeonhole{
	class pigeonhole{
	public:
		typedef boost::function<void(int, string)> default_handler_type;
		typedef std::set<slot> slot_set_type;
		void add_slot(boost::regex pt, slot::handler_type h);
		void set_default(default_handler_type h);
		void set_reader(boost::shared_ptr<reader> rd);
		void run();
	private:
		inline void _pige_line(int line, string& raw);
	private:
		slot_set_type _slots;
		default_handler_type _default_handler;
		boost::shared_ptr<reader> _reader;
	};
}
#endif