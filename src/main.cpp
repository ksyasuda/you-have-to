#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <getopt.h>
#include "../headers/job.h"

static Job parseString(std::string line)
{
	int count = 0;
	std::string cname, jname, location, date;
	std::size_t name = line.find('|', count);
	cname = line.substr(count, name);
	count = (int)name + 1;
	name = line.find('|', count);
	jname = line.substr(count, name - count);
	count = (int)name + 1;
	name = line.find('|', count);
	location = line.substr(count, name - count);
	count = (int)name + 1;
	name = line.find('|', count);
	if (name != std::string::npos)
	{
		std::cerr << "Something went wrong\n";
		exit(1);
	}
	date = line.substr(count, name - count);
	const Job job(cname, jname, location, date);
	return job;
}

void write_new_job(Job &new_job);

int main(int argc, char **argv)
{
	std::map<std::string, Job> map;
	std::ifstream in("out.txt", std::ifstream::in);
	if (!in.is_open())
	{
		std::cerr << "Something went wrong\n";
	}
	std::string line;
	while (std::getline(in, line))
	{
		// std::cout << line << "\n";
		Job job = parseString(line);
		map[job.get_company()] = job;
	}
	in.close();

	//* getopt for handling commandline
	int option_index = 0, option = 0;
	opterr = false;
	struct option longOpts[] = {{"verbose", no_argument, nullptr, 'v'},
								{"print", no_argument, nullptr, 'p'},
								{"insert", required_argument, nullptr, 'i'},
								{"help", no_argument, nullptr, 'h'},
								{"search", required_argument, nullptr, 's'},
								{nullptr, 0, nullptr, '\0'}};
	while ((option = getopt_long(argc, argv, "vpi:hs:", longOpts, &option_index)) != -1)
	{
		switch (option)
		{
		case 'h':
			std::cout << "When in doubt, factor out\n";
			exit(0);
		case 'v':
			std::cout << "VERBOSE NICE\n";
			break;
		case 'p':
		{
			for (auto i = map.begin(); i != map.end(); ++i)
			{
				i->second.print();
			}
			break;
		}
		case 'i':
		{
			std::string line = optarg;
			// std::cout << line << '\n';
			int count = 0;
			std::string cname, jname, location, date;
			std::size_t name = line.find('|', count);
			if (name == std::string::npos)
			{
				std::cerr << "SOMETHING WENT WRONG IN COMPANY NAME\n";
				exit(1);
			}
			cname = line.substr(count, name);
			count = (int)name + 1;
			name = line.find('|', count);
			if (name == std::string::npos)
			{
				std::cerr << "SOMETHING WENT WRONG IN JOB TYPE\n";
				exit(1);
			}
			jname = line.substr(count, name - count);
			count = (int)name + 1;
			name = line.find('|', count);
			if (name == std::string::npos)
			{
				std::cerr << "SOMETHING WENT WRONG IN LOCATION\n";
				exit(1);
			}
			location = line.substr(count, name - count);
			count = (int)name + 1;
			name = line.find('|', count);
			if (name != std::string::npos)
			{
				std::cerr << "SOMETHING WENT WRONG IN DATE\n";
				exit(1);
			}
			date = line.substr(count, name - count);
			Job job(cname, jname, location, date);
			map[cname] = job;
			write_new_job(job);
			break;
		}
		case 's':
		{
			std::string company = optarg;
			// std::cout << company << '\n';
			auto res = map.find(company);
			if (res != map.end())
			{
				std::cout << "Already applied to " << company << " on " << res->second.get_date_applied() << "\n";
			}
			else
			{
				std::cout << "You have not applied to " << company << " yet and as the great sage Yannick would say... Freud\n";
			}
			break;
		}
		default:
		{
			for (auto i = map.begin(); i != map.end(); ++i)
			{
				i->second.print();
			}
			break;
		}
		}
	}
	// out << name << "|" << type << "|" << date << "\n";
	//! close the file
	return 0;
}

void write_new_job(Job &new_job)
{
	//* initialize output stream
	std::ofstream out("out.txt", std::ofstream::app);
	out << new_job.get_company() << "|" << new_job.get_jobname() << "|" << new_job.get_location() << "|" << new_job.get_date_applied() << "\n";
	out.close();
}
