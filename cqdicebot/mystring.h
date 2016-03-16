#pragma once
#include <string>
using namespace std;

/*
fuck this std::string without trim()
remove space and newline from begining of the str
*/
void my_trim(string * str) {
	size_t rmp = str->find_first_not_of(" \n\r");
	if (rmp > 0)str->erase(0, rmp);
}