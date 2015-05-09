#include "task.hpp"

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

Task::Task(std::string id)
{
	this->id = id;
}

Task::Task(zmq::message_t* message)
{
	std::string rpl = std::string(static_cast<char*>(message->data()),
	                              message->size());
	rpl = rpl.substr(rpl.find("{"));
	ptree pt;
	std::istringstream is (rpl);
	read_json (is, pt);

	this->id = pt.get<std::string> ("id");
	if (this->id != "@" && this->id != "#")
	{
		this->hash = pt.get<std::string> ("hash");
		this->algorithm = pt.get<std::string> ("algorithm");
		this->method = this->parseMathod(pt.get<std::string> ("password_cracking_algorithm"));
	}
}

std::string Task::getJson()
{
	ptree pt;
	pt.put ("id", this->id);
	if (this->id != "#" && this->id != "@")
	{
		pt.put ("password", this->password);
		pt.put ("start_time", this->start_time);
		pt.put ("end_time", this->end_time);
	}
	std::ostringstream buf;
	write_json (buf, pt, false);

	return buf.str();
}
std::string Task::getProgressJson()
{
	ptree pt;
	pt.put ("id", this->id);

	std::ostringstream buf;
	write_json (buf, pt, false);

	return buf.str();
}

std::string Task::getId()
{
	return this->id;
}

// Returns hash with leading method char
std::string Task::getHash()
{
	return this->method + this->hash;
}

void Task::setPassword(std::string password)
{
	this->password = password;
}

void Task::start()
{
	this->start_time = time(0);
}

void Task::stop()
{
	this->end_time = time(0);
}

/**
	* R - Rainbow tables
	* B - Brute force
	* D - Dictionary attack
**/
char Task::parseMathod(std::string password_cracking_algorithm)
{
	char result;
	if (password_cracking_algorithm.compare("Metoda Slownikowa") == 0) {
		result = 'D';
	} else if (password_cracking_algorithm.compare("Tablice Teczowe") == 0) {
		result = 'R';
	} else {
		result = 'B';
	}

	return result;
}
