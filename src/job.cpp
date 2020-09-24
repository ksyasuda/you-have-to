#include <iostream>
#include <string>
#include "../headers/job.h"

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

void Job::print()
{
	std::cout << "| " << company_name << " | " << job_name << " | " << location << " | " << date_applied << " |\n";
}
