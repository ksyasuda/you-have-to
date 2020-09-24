#ifndef JOB_H
#define JOB_H

#include <string>

class Job {
	std::string company_name;
	std::string job_name;
	std::string date_applied;

public:

	Job();

	Job(const std::string& cname, const std::string& jname, const std::string& date);

	void set_company(const std::string& company_name);

	void set_jobname(const std::string& job_name);

	void set_date_applied(const std::string& date_applied);

	std::string get_company();

	std::string get_jobname();

	std::string get_date_applied();
};

#endif
