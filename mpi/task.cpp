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

std::string Task::getId()
{
	return this->id;
}

std::string Task::getHash()
{
	return this->hash;
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
