#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <ctime>
#include <zmq.hpp>

class Task
{
private:
	std::string id;
	std::string hash;
	std::string password;
	char method;
	time_t start_time;
	time_t end_time;
	std::string algorithm;

	char parseMathod(std::string password_cracking_algorithm);

public:
	Task(std::string id);
	Task(zmq::message_t* message);
	std::string getId();
	std::string getJson();
	std::string getProgressJson();
	std::string getHash();
	void setPassword(std::string password);
	void start();
	void stop();
};
#endif
