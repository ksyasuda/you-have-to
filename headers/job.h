#ifndef JOB_H
#define JOB_H

#include <string>

class Job {
  std::string company_name;
  std::string job_name;
  std::string location;
  std::string date_applied;
  std::string result;
  std::string character;

public:
  Job();

  Job(const std::string &cname, const std::string &jname,
      const std::string &location, const std::string &date,
      const std::string &character);

  void print(bool crypto, bool is_authenticated);

  std::string get_company();

  std::string get_jobname();

  std::string get_date_applied();

  std::string get_location();

  std::string get_character();

  void set_character(std::string character);

  void set_result(std::string res);

  std::string get_result();
};

#endif
