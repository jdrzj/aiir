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
    SubtaskQueue() { }
    void pop() { data.pop_front(); }
    void push_back (T&& val) { return data.push_back(val); }
    T front() const { return data.front(); }
    bool empty() const { return data.empty();}
    size_t size() const noexcept { return data.size(); }
};

class Task
{
private:
	std::string id;
	std::string hash;
	std::string password;
	char subtask_size;
	char method;
	char algorithm;
	time_t start_time;
	time_t end_time;
	int progress;
	int interval_count;

	char parseMethod(std::string password_cracking_algorithm);
	char parseIntervalSize(std::string interval_size);
	char parseAlgorithm(std::string algorithm);

public:
	Task(std::string id);
	Task(zmq::message_t* message);
	std::string getId();
	std::string getJson();
	std::string getProgressJson();
	std::string getHash();
	int getStatus();
	int getIntervalCount();
	SubtaskQueue<Subtask> getSubtaskQueue();
	void setPassword(std::string password);
	void setProgress(int progress);
	void setIntervalCount(int count);
	void incrementProgress();
	void start();
	void stop();
};


#endif
