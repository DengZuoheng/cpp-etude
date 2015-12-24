#include "pigeonhole.h"

namespace pigeonhole{
	
	void pigeonhole::add_slot(boost::regex pt, slot::handler_type h){
		_slots.insert(make_slot(pt, h));
	}

	void pigeonhole::set_default(default_handler_type h){
		_default_handler = h;
	}

	void pigeonhole::set_reader(boost::shared_ptr<reader> rd){
		_reader = rd;
	}

	void pigeonhole::run(){
		if (!_reader) return;
		if (_reader->valid() == false) return;
		string raw;
		int line = 1;
		while (_reader->getline(raw)){
			_pige_line(line, raw);
			line++;
		}
	}

	inline void pigeonhole::_pige_line(int line, string& raw){
		bool flag_no_match = true;
		for (slot_set_type::iterator it = _slots.begin();
			it != _slots.end();
			++it)
		{// begin for
			boost::smatch match;
			if (regex_match(raw, match, it->pattern)){
				flag_no_match = false;
				try{
					if (it->handler){
						it->handler(line, raw, match);
						continue;
					}
				}
				catch (...){

				}
			}//end if
		}// end for
		if (flag_no_match&&raw.size()){
			try{
				if (_default_handler) _default_handler(line, raw);
			}
			catch (...){

			}
		}
	}
}