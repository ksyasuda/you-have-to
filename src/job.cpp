#include <iostream>
#include <string>
#include <algorithm>
#include "../headers/job.h"

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

static std::string encrypt(std::string& part) {
	char key = 'F';
	std::string word = part;
	// cout << "part " << part << endl;
	auto j = part.begin();
	for(auto i = word.begin(); i != word.end(); ++i) {
		*j = *i ^ key;	
		++j;
	}
	trim(part);
	// cout << "PART " << part << endl;
	// cout << "LEN " << part.length() << endl;
	return part;
}

Job::Job()
{
	company_name = "";
	job_name = "";
	date_applied = "";
}

Job::Job(const std::string &cname, const std::string &jname, const std::string &location, const std::string &date) : company_name{cname}, job_name{jname}, location{location}, date_applied{date} {}

std::string Job::get_company()
{
	return company_name;
}

std::string Job::get_jobname()
{
	return job_name;
}

std::string Job::get_date_applied()
{
	return date_applied;
}

std::string Job::get_location()
{
	return location;
}

void Job::print(bool crypto)
{
	if(!crypto)
	{
		std::cout << "| " << company_name << " | " << job_name << " | " << location << " | " << date_applied << " |\n";
	}
	else
	{
		std::string cname = company_name;
		std::string jname = job_name;
		std::string loc = location;
		std::string date = date_applied;
		encrypt(cname);
		encrypt(jname);
		encrypt(loc);
		encrypt(date);
		std::cout << "| " << cname << " | " << jname << " | " << loc << " | " << date << " |\n";
	}
}
