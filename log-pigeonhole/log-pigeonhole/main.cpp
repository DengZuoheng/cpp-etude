#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "pigeonhole.h"
#include "log_writer.h"
#include "reader_imp.h"

using std::string;
using namespace boost::program_options;

int main(int argc, char* argv[])
{
	options_description opts("log pigeonhole");
	opts.add_options()
		("help,h", "help")
		("filename,f", value<string>(), "input log file")
		("debug,d", value<string>()->implicit_value("debug.log"), "output debug log file")
		("info,i", value<string>()->implicit_value("info.log"), "output info log file")
		("error,e", value<string>()->implicit_value("error.log"), "output error log file");
	variables_map vm;
	store(parse_command_line(argc, argv, opts), vm);
	pigeonhole::pigeonhole ph;
	if (vm.count("help") || vm.empty()){
		std::cout << opts << std::endl;
	}
	if (vm.count("filename")){
		try{
			string input_file = vm["filename"].as<string>();
			pigeonhole::reader_ptr frdr = boost::make_shared<pigeonhole::file_reader>(input_file);
			if (frdr->valid()){
				ph.set_reader(frdr);
			}
			else{
				std::cerr << "Can't open input log file: " << input_file << std::endl;
				return 1;
			}
		}
		catch (...){}
	}
	else{
		std::cerr << "Input log file is necessary!" << std::endl;
	}
	if (vm.count("debug")){
		try{
			string debug_log_file = vm["debug"].as<string>();
			boost::regex debug_log_pattern = boost::regex("(\\d{8}-\\d{6})<DEBUG:0x([0-9a-fA-F]{8})>: (.*)");
			log_writer writer = make_log_writer(debug_log_file);
			pigeonhole::slot::handler_type debug_log_handler(
				boost::bind(&log_writer::on_pt_match, writer,_1,_2,_3));
			ph.add_slot(debug_log_pattern, debug_log_handler); 
		}
		catch (...){}
	}
	if (vm.count("info")){
		try{
			string info_log_file = vm["info"].as<string>();
			boost::regex info_log_pattern = boost::regex("(\\d{8}-\\d{6})<INFO\\s*:0x([0-9a-fA-F]{8})>: (.*)");
			log_writer writer = make_log_writer(info_log_file);
			pigeonhole::slot::handler_type info_log_handler(
				boost::bind(&log_writer::on_pt_match, writer, _1, _2, _3));
			ph.add_slot(info_log_pattern, info_log_handler);
		}
		catch (...){}
	}
	if (vm.count("error")) {
		try{
			string error_log_file = vm["error"].as<string>();
			boost::regex error_log_pattern = boost::regex("(\\d{8}-\\d{6})<ERROR:0x([0-9a-fA-F]{8})>: (.*)");
			log_writer writer = make_log_writer(error_log_file);
			pigeonhole::slot::handler_type error_log_handler(
				boost::bind(&log_writer::on_pt_match, writer, _1, _2, _3));
			ph.add_slot(error_log_pattern, error_log_handler);
		}
		catch (...){}
	}
	pigeonhole::pigeonhole::default_handler_type default_handler;
	ph.set_default(default_handler);
	ph.run();
	return 0;
}
