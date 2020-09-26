#ifndef JOB_H
#define JOB_H

#include <string>

class Job
{
	std::string company_name;
	std::string job_name;
	std::string location;
	std::string date_applied;

public:
	Job();

	Job(const std::string &cname, const std::string &jname, const std::string &location, const std::string &date);

	void print(bool crypto);

	std::string get_company();

	std::string get_jobname();

	std::string get_date_applied();

	std::string get_location();
};

#endif
