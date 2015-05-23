#include "task.hpp"

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>
#include <limits>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

int Task::dict_lines = 306613;

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
		this->algorithm = this->parseAlgorithm(pt.get<std::string> ("algorithm"));
		this->method = this->parseMethod(pt.get<std::string> ("password_cracking_algorithm"));
		this->graininess = std::stoi(pt.get<std::string> ("graininess"));
		this->subtask_size = this->parseIntervalSize("");
		this->progress = 0;

		std::cout << "MASTER: New task created. Hash [" << this->hash << "], algorithm ["
				  << this->algorithm << "], method [" << this->method << "], subtask size ["
				  << this->subtask_size << "], graininess [" << this->graininess <<"]\n";
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

int Task::getGraininess()
{
	return this->graininess;
}

std::string Task::getProgressJson()
{
	ptree pt;
	pt.put ("id", this->id);
	pt.put ("status", getStatus());

	std::ostringstream buf;
	write_json (buf, pt, false);

	return buf.str();
}

std::string Task::getId()
{
	return this->id;
}

// Returns hash with leading method char and subtask size char
std::string Task::getHash()
{
	return std::string(1, this->method) + std::string(1, this->algorithm) + this->hash;
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
char Task::parseMethod(std::string password_cracking_algorithm)
{
	char result;
	if (password_cracking_algorithm.compare("MS") == 0) {
		result = 'D';
	} else if (password_cracking_algorithm.compare("TT") == 0) {
		result = 'R';
	} else {
		result = 'B';
	}

	return result;
}

char Task::parseAlgorithm(std::string algorithm)
{
	char result;
	if (algorithm.compare("md5") == 0) {
		result = 'M';
	} else {
		result = 'S';
	}

	return result;
}

char Task::parseIntervalSize(std::string interval_size)
{
	if (graininess == 1)
		return 'B';
	else if (graininess == 2)
		return 'M';
	else if (graininess == 3)
		return 'S';
}

SubtaskQueue<Subtask> Task::getSubtaskQueue()
{
	if (method == 'R')
	{
		if (subtask_size == 'B')
			return SubtaskQueue<Subtask> { {0, 1000, 1}, {0, 1000, 2},
				{0, 1000, 3}, {0, 1000, 4}, {0, 1000, 5}, {0, 1000, 6},
				{0, 1000, 7}, {0, 1000, 8}, {0, 1000, 9}, {0, 1000, 10} };
		if (subtask_size == 'M')
			return SubtaskQueue<Subtask> { {0, 500, 1}, {501, 1000, 1},
				{0, 500, 2}, {501, 1000, 2}, {0, 500, 3}, {501, 1000, 3},
				{0, 500, 4}, {501, 1000, 4}, {0, 500, 5}, {501, 1000, 5},
				{0, 500, 6}, {501, 1000, 6}, {0, 500, 7}, {501, 1000, 7},
				{0, 500, 8}, {501, 1000, 8}, {0, 500, 9}, {501, 1000, 9},
				{0, 500, 10}, {501, 1000, 10} };
		if (subtask_size == 'S')
			return SubtaskQueue<Subtask>
				{ {0, 250, 1}, {251, 500, 1}, {501, 750, 1}, {751, 1000, 1},
				{0, 250, 2}, {251, 500, 2}, {501, 750, 2}, {751, 1000, 2},
				{0, 250, 3}, {251, 500, 3}, {501, 750, 3}, {751, 1000, 3},
				{0, 250, 4}, {251, 500, 4}, {501, 750, 4}, {751, 1000, 4},
				{0, 250, 5}, {251, 500, 5}, {501, 750, 5}, {751, 1000, 5},
				{0, 250, 6}, {251, 500, 6}, {501, 750, 6}, {751, 1000, 6},
				{0, 250, 7}, {251, 500, 7}, {501, 750, 7}, {751, 1000, 7},
				{0, 250, 8}, {251, 500, 8}, {501, 750, 8}, {751, 1000, 8},
				{0, 250, 9}, {251, 500, 9}, {501, 750, 9}, {751, 1000, 9},
				{0, 250, 10}, {251, 500, 10}, {501, 750, 10}, {751, 1000, 10} };
	}
	else if (method == 'B')
	{
		int perTask;
		if (subtask_size == 'S')
		{
			perTask = 5;
		}
		else if (subtask_size == 'M')
		{
			perTask = 10;
		}
		else if (subtask_size == 'B')
		{
			perTask = 25;
		}
		SubtaskQueue<Subtask> sq;
		// 4 znakowe hasla
		int start_i = 1;
		for (int i=1; i<=94; i++)
		{
			if (i % perTask == 0)
			{
				sq.push_back({start_i+32, i+32, 4});
				start_i= i+1;
			}
			if (i == 94 && start_i < 94)
			{
				sq.push_back({start_i+32, i+32, 4});
			}
		}
		// 5 znakowe hasla
	    start_i = 1;
	    int start_j = 1;
	    for (int i=1; i<=94; i++)
	    {
	        for (int j=1; j<=94; j++)
	        {
	            if (((i-1)*94 + j) % perTask == 0)
	            {
					sq.push_back({(start_j+32)*1000 + (start_i+32), (j+32)*1000 + (i+32), 5});
	                start_j = j+1;
	                start_i = i;
	                if (start_j > 94)
	                {
	                    start_j = 1;
	                    start_i = i+1;
	                }
	            }
	            if (i == 94 && j == 94 && start_j < 94)
	            {
					sq.push_back({(start_j+32)*1000 + (start_i+32), (j+32)*1000 + (i+32), 5});
	            }
	        }
	    }
		return sq;
	}
	// 5 przedzialow - mala ziarnistosc
	// 50 przedzialow - srednia ziarnistosc
	// 200 przedzialow - duza ziarnistosc
	else if (method == 'D')
	{
		int bins;
		int max = dict_lines;
		if (subtask_size == 'S')
		{
			bins = 5;
		}
		else if (subtask_size == 'M')
		{
			bins = 50;
		}
		else if (subtask_size == 'B')
		{
			bins = 200;
		}
		SubtaskQueue<Subtask> sq;
		int bin_size = max / bins;
		for (int i=0; i < bins; i++)
		{
			int range_start = i * bin_size + 1;
			int range_end = (i == bins - 1) ? max : (i + 1) * bin_size;
			std::cout << "MASTER: New subtask, range start " << range_start << ", range end " << range_end << std::endl;
			sq.push_back({range_start, range_end, 0});
		}
		return sq;
	}
	return SubtaskQueue<Subtask> { };
}

int Task::getStatus()
{
	return (100 * progress / interval_count);
}

void Task::setProgress(int new_progress)
{
	progress = new_progress;
}

void Task::setIntervalCount(int count)
{
	interval_count = count;
}

void Task::incrementProgress()
{
	++progress;
}

int Task::getIntervalCount()
{
	return interval_count;
}
