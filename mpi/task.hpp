#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <ctime>
#include <zmq.hpp>
#include <deque>
#include <initializer_list>

struct Subtask
{
	int start;
	int end;
	int pass_length;
};

template <class T, class container=std::deque<T> >
class SubtaskQueue {
    container data;
public:
    explicit SubtaskQueue(std::initializer_list<T> t) : data(t) {}
    void pop() { data.pop_front(); }
    T front() const { return data.front(); }
    bool empty() const { return data.empty();}
    size_t size() const noexcept { return data.size(); };
};

class Task
{
private:
	std::string id;
	std::string hash;
	std::string password;
	char subtask_size;
	char method;
	time_t start_time;
	time_t end_time;
	std::string algorithm;

	char parseMethod(std::string password_cracking_algorithm);
	char parseIntervalSize(std::string interval_size);

public:
	Task(std::string id);
	Task(zmq::message_t* message);
	std::string getId();
	std::string getJson();
	std::string getProgressJson();
	std::string getHash();
	SubtaskQueue<Subtask> getSubtaskQueue();
	void setPassword(std::string password);
	void start();
	void stop();
};


#endif
