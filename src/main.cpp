#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <getopt.h>
#include <set>
#include <algorithm>
// #include <cctype>
#include <locale>
#include <cstring>
#include <termios.h>
#include <unistd.h>
#include "../headers/job.h"

using namespace std;


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

static string encrypt(string& part) {
	char key = 'F';
	string word = part;
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

Job parseString(std::string line);

void write_new_job(Job &new_job, bool crypto);

bool checkUser();

set<string> encrypt_decrypt(string type);

const string PASSWORD = "%.'4*/#";

int main(int argc, char **argv)
{
	std::map<std::string, Job> map;
	std::ifstream in("out.txt", std::ifstream::in);
	if (!in.is_open())
	{
		std::cerr << "Something went wrong in main\n";
	}
	std::string line;
	string enc;
	bool need_decrypt = false, is_verbose = false;
	getline(in, enc);
	if(enc == "e") {
		need_decrypt = true;
	}
	getline(in, enc);
	int cnt = 0;
	while (std::getline(in, line))
	{
		// std::cout << line << "\n";
		Job job = parseString(line);
		map[job.get_company()] = job;
		++cnt;
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
			if(is_verbose) {
				cout << "Encrypting file content\n";
			}
			set = encrypt_decrypt("e");
			if(is_verbose) {
				cout << "Encryption complete... Writing to the output file\n";
			}
			ofstream out ("out.txt");
			out << "e\n";
			out << PASSWORD << "\n";
			for(auto i = set.begin(); i != set.end(); ++i) {
				out << *i << "\n";
			}
			out.close();
			if(is_verbose) {
				cout << "Write complete... Closing file\n";
			}
			break;
		}
		case 'd':
		{
			set<string> set;
			if(is_verbose) {
				cout << "Checking user permissions\n";
			}
			bool is_user = checkUser();
			if(!is_user) {
				cerr << "You can't do that\n";
				exit(1);
			}
			if(is_verbose) {
				cout << "Access granted\n";
				cout << "Decrpyting file content\n";
			}
			set = encrypt_decrypt("d");
			if(is_verbose) {
				cout << "Decryption complete... Writing to output file\n";
			}
			ofstream out ("out.txt");
			out << "d\n";
			out << PASSWORD << "\n";
			for(auto i = set.begin(); i != set.end(); ++i) {
				out << *i << "\n";
			}
			out.close();
			if(is_verbose) {
				cout << "File write complete... Closing file\n";
			}
			break;
		}
		case 'h':
			std::cout << "When in doubt, factor out\n";
			exit(0);
		case 'v':
			std::cout << "VERBOSE NICE\n";
			is_verbose = true;
			break;
		case 'p':
		{
			for (auto i = map.begin(); i != map.end(); ++i)
			{
				if(i->first.length() > 0) {
					i->second.print(need_decrypt);
				}
			}
			break;
		}
		case 'i':
		{
			//* cast optarg to a string
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
			//* parse out each part of the string delimited by a '|'
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
			locale loc;
			//* convert input string to lower case
			for(auto i = cname.begin(); i != cname.end(); ++i) {
				*i = tolower(*i, loc);
			}
			for(auto i = jname.begin(); i != jname.end(); ++i) {
				*i = tolower(*i, loc);
			}
			for(auto i = location.begin(); i != location.end(); ++i) {
				*i = tolower(*i, loc);
			}
			for(auto i = date.begin(); i != date.end(); ++i) {
				*i = tolower(*i, loc);
			}
			Job job(cname, jname, location, date);
			job.print(need_decrypt);
			map[cname] = job;
			write_new_job(job, need_decrypt);
			if(is_verbose) {
				cout << "Successfully wrote the job to the output file\n";
			}
			break;
		}
		case 's':
		{
			std::string company = optarg;
			// std::cout << company << '\n';
			if(need_decrypt) encrypt(company);
			if(is_verbose) {
				cout << "Searching for " << company << " in the map\n";
			}
			auto res = map.find(company);
			if(is_verbose) {
				cout << "Search complete for " << company << "\n";
			}
			if (res != map.end() && !need_decrypt)
			{
				std::cout << "Already applied to " << company << " on " << res->second.get_date_applied() << "\n";
			}
			else if (res != map.end() && need_decrypt)
			{
				string c = company;
				encrypt(c);
				string d = res->second.get_date_applied();
				encrypt(d);
				std::cout << "Already applied to " << c << " on " << d << "\n";
			}
			else if(res == map.end() && !need_decrypt)
			{
				std::cout << "You have not applied to " << company << " yet and as the great sage Yannick would say... Freud\n";
			}
			else
			{
				string cname = company;
				encrypt(cname);
				std::cout << "You have not applied to " << cname << " yet and as the great sage Yannick would say... Freud\n";
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
					i->second.print(need_decrypt);
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

bool checkUser() {
	ifstream in("out.txt");
	string line, password;
	bool is_authenticated = false;
	getline(in, line);
	// getline(in, password);
	if(strcmp(line.c_str(), "e") == 0) {
		cout << "Enter your password: ";
		termios oldt;
		tcgetattr(STDIN_FILENO, &oldt);
		termios newt = oldt;
		newt.c_lflag &= ~ECHO;
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
		string entered_password;
		cin >> entered_password;
		cout << "\n";
		encrypt(entered_password);
		in >> password;
		if(strcmp(entered_password.c_str(), password.c_str()) != 0) {
			cerr << "Incorrect password goodbye\n";
			exit(1);
		}
		is_authenticated = true;
	}
	return is_authenticated;
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
	getline(in, enc);
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
		// cout << "PART " << part << endl;
		encrypt(part);
		// cout << "ENCPART " << part << endl;
		nline += '|' + part;
		count = pos + 1;
		pos = line.find('|', count);
		part = line.substr(count, pos - count);
		encrypt(part);
		nline += '|' + part;
		// cout << nline << endl;
		set.insert(nline);
	}
	return set;
}

void write_new_job(Job &new_job, bool crypto)
{
	//* initialize output stream
	std::ofstream out("out.txt", std::ofstream::app);
	if(!crypto)
	{
		out << new_job.get_company() << "|" << new_job.get_jobname() << "|" << new_job.get_location() << "|" << new_job.get_date_applied() << "\n";
	}
	else 
	{
		string cname = new_job.get_company();
		encrypt(cname);
		// trim(cname);
		string jname = new_job.get_jobname();
		encrypt(jname);
		// trim(jname);
		string loc = new_job.get_location();
		encrypt(loc);
		// trim(loc);
		string date = new_job.get_date_applied();
		encrypt(date);
		// trim(date);
		out << cname << "|" << jname << "|" << loc << "|" << date << "\n";
	}
	out.close();
}

Job parseString(std::string line)
{
	int count = 0;
	std::string cname, jname, location, date;
	std::size_t name = line.find('|', count);
	cname = line.substr(count, name);
	// if(need_decrypt) encrypt(cname);
	count = (int)name + 1;
	name = line.find('|', count);
	jname = line.substr(count, name - count);
	// if(need_decrypt) encrypt(jname);
	count = (int)name + 1;
	name = line.find('|', count);
	location = line.substr(count, name - count);
	// if(need_decrypt) encrypt(location);
	count = (int)name + 1;
	name = line.find('|', count);
	if (name != std::string::npos)
	{
		std::cerr << "Something went wrong in map setup\n";
		exit(1);
	}
	date = line.substr(count, name - count);
	// if(need_decrypt) encrypt(date);
	const Job job(cname, jname, location, date);
	return job;
}
