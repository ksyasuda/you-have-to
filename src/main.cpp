#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <getopt.h>
#include <set>
#include <cstring>
#include "../headers/job.h"

using namespace std;

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
		std::cerr << "Something went wrong in map setup\n";
		exit(1);
	}
	date = line.substr(count, name - count);
	const Job job(cname, jname, location, date);
	return job;
}

void write_new_job(Job &new_job);

set<string> encrypt_decrypt(string type);

int main(int argc, char **argv)
{
	std::map<std::string, Job> map;
	std::ifstream in("out.txt", std::ifstream::in);
	if (!in.is_open())
	{
		std::cerr << "Something went wrong in main\n";
	}
	std::string line;
	char enc;
	in >> enc;
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
								{"count", no_argument, nullptr, 'c'},
								{"encrypt", no_argument, nullptr, 'e'},
								{"decrpyt", no_argument, nullptr, 'd'},
								{nullptr, 0, nullptr, '\0'}};
	while ((option = getopt_long(argc, argv, "vpi:hs:ced", longOpts, &option_index)) != -1)
	{
		switch (option)
		{
		case 'e': 
		{
			set<string> set;
			set = encrypt_decrypt("e");
			ofstream out ("out.txt");
			out << "e\n";
			for(auto i = set.begin(); i != set.end(); ++i) {
				out << *i << "\n";
			}
			break;
		}
		case 'd':
		{
			set<string> set;
			set = encrypt_decrypt("d");
			ofstream out ("out.txt");
			out << "d\n";
			for(auto i = set.begin(); i != set.end(); ++i) {
				out << *i << "\n";
			}
			break;
		}
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
				if(i->first.length() > 0) {
					i->second.print();
				}
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
		case 'c': 
		{
			std::cout << map.size() << " apps sent\n";	
			break;
		}
		default:
		{
			for (auto i = map.begin(); i != map.end(); ++i)
			{
				if(i->first.length() > 0)
					i->second.print();
			}
			std::cout << map.size() << " aps sent\n";
			break;
		}
		}
	}
	// out << name << "|" << type << "|" << date << "\n";
	//! close the file
	return 0;
}

static string encrypt(string& part) {
	char key = 'F';
	string word = part;
	auto j = word.begin();
	for(auto i = part.begin(); i != part.end(); ++i) {
		if(*j != ' ' && *j != ',') {
			*i = *j ^ key;	
		}
		++j;
	}
	return part;
}

set<string> encrypt_decrypt(string type) {
	set<string> set;
	ifstream in ("out.txt");
	if(!in.is_open()) {
		cerr << "Something went wrong opening output file\n";
		exit(1);
	}
	string line;
	string nline;
	string enc;
	//in >> enc;
	getline(in, enc);
	if(strcmp(enc.c_str(), type.c_str()) == 0) {
		cerr << "You can't do that\n";
		exit(1);
	}
	while(getline(in, line)) {
		nline = "";
		size_t count = 0;
		size_t pos = line.find('|', count);
		string part = line.substr(count, pos);
		encrypt(part);
		nline += part;
		count = pos + 1;
		pos = line.find('|', count);
		part = line.substr(count, pos - count);
		encrypt(part);
		nline += '|' + part;
		count = pos + 1;
		pos = line.find('|', count);
		part = line.substr(count, pos - count);
		encrypt(part);
		nline += '|' + part;
		count = pos + 1;
		pos = line.find('|', count);
		part = line.substr(count, pos - count);
		encrypt(part);
		nline += '|' + part;
		set.insert(nline);
	}
	return set;
}

void write_new_job(Job &new_job)
{
	//* initialize output stream
	std::ofstream out("out.txt", std::ofstream::app);
	out << new_job.get_company() << "|" << new_job.get_jobname() << "|" << new_job.get_location() << "|" << new_job.get_date_applied() << "\n";
	out.close();
}
