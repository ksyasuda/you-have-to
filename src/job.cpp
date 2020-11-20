#include "../headers/job.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <termios.h>
#include <unistd.h>

using namespace std;

// trim from start (in place)
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

static std::string encrypt(std::string &part) {
  char key = 'F';
  std::string word = part;
  // cout << "part " << part << endl;
  auto j = part.begin();
  for (auto i = word.begin(); i != word.end(); ++i) {
    *j = *i ^ key;
    ++j;
  }
  trim(part);
  // cout << "PART " << part << endl;
  // cout << "LEN " << part.length() << endl;
  return part;
}

Job::Job() {
  company_name = "";
  job_name = "";
  date_applied = "";
  character = "";
  result = "";
}

Job::Job(const std::string &cname, const std::string &jname,
         const std::string &location, const std::string &date,
         const std::string &character)
    : company_name{cname}, job_name{jname}, location{location},
      date_applied{date}, character{character} {
  result = false;
}

std::string Job::get_company() { return company_name; }

std::string Job::get_jobname() { return job_name; }

std::string Job::get_date_applied() { return date_applied; }

std::string Job::get_location() { return location; }

std::string Job::get_result() { return result; }

std::string Job::get_character() { return character; }

void Job::set_character(std::string c) { character = c; }

void Job::set_result(std::string res) { result = res; }

constexpr auto RED = "\033[41m";
constexpr auto GREEN = "\033[42m";
constexpr auto RESET = "\033[0m";

void Job::print(bool crypto, bool is_authenticated) {
  if (!crypto) {
    if (character == "+") {
      std::cout << GREEN << "| " << company_name << " | " << job_name << " | "
                << location << " | " << date_applied << " |" << RESET << "\n";
    } else if (character == "x") {
      std::cout << RED << "| " << company_name << " | " << job_name << " | "
                << location << " | " << date_applied << " |" << RESET << "\n";
    } else if (character == "-") {
      std::cout << RESET << "| " << company_name << " | " << job_name << " | "
                << location << " | " << date_applied << " |" << RESET << "\n";
    }
  } else {
    if (is_authenticated) {
      std::string cname = company_name;
      std::string jname = job_name;
      std::string loc = location;
      std::string date = date_applied;
      encrypt(cname);
      encrypt(jname);
      encrypt(loc);
      encrypt(date);
      if (character == "+") {
        std::cout << GREEN << "| " << cname << " | " << jname << " | " << loc
                  << " | " << date << " |" << RESET << "\n";
      } else if (character == "x") {
        std::cout << RED << "| " << cname << " | " << jname << " | " << loc
                  << " | " << date << " |" << RESET << "\n";
      } else {
        std::cout << "| " << cname << " | " << jname << " | " << loc << " | "
                  << date << " |" << RESET << "\n";
      }
    }
  }
}
