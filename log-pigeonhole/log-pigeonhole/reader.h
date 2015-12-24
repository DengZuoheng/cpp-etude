#ifndef READER_H_
#define READER_H_
#include <string>
#include <boost/smart_ptr.hpp>

using std::string;
namespace pigeonhole{
	class reader{
	public:
		virtual bool getline(string&) = 0;
		virtual bool valid() = 0;
		virtual ~reader(){}
	};
	typedef boost::shared_ptr<reader> reader_ptr;
}
#endif