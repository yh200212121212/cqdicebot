// dicebot_ConsoleApplication.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <diceSet.h>
#include <iostream>
#include <string>
using namespace std;

int main()
{
	diceSet ds1;
	string rollstring = "5d100k4+1d200+12";
	
	while (rollstring.length() > 0) {
		ds1.getDice(&rollstring);
		cout << ds1.display() << endl;
	}


	getchar();
	return 0;
}

