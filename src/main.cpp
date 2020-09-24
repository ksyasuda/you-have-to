#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include "../headers/job.h"

int main() {
	std::map<std::string, Job> map;
	std::ifstream in ("out.txt", std::ifstream::in);
	if(!in.is_open()) {
		std::cerr << "Something went wrong\n";
	}
	std::string line;
	while(std::getline(in, line)) {
		std:: cout << line << "\n";
		int count = 0;
		auto first = line.find('|', count);
		while(first != line.end()) {
			std::string company = line.substr(count, first);
			count = first;
		}
	}
	in.close();
	/*
	std::ofstream out ("out.txt", std::ofstream::out);
	Job job(name, type, date);
	map[name] = job;
	for(auto x = map.begin(); x != map.end(); ++x) {
		std::cout << x->first << std::endl;
	}
	out << name << "|" << type << "|" << date << "\n";
	out.close();
	*/
	return 0;
}
