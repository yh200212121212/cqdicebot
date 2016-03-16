// dicebot_ConsoleApplication.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <diceSet.h>
#include <diceStringParser.h>
#include <iostream>
#include <string>
using namespace std;

int main()
{
	diceSet ds1;
	string rollstring = ".r5d100k4+1d200+12hhhhhhhhh";


	cout << atoi(" 1000 ");
	/*
	diceStringParser dsp;
	dsp.Parse(rollstring.c_str());

	if (dsp.getStatus() == PARSE_FINISHED_SUCCESSFULLY) {
		list<diceSet>::iterator itor;
		list<diceSet> dicelist = dsp.getDice();
		itor = dicelist.begin();
		while (itor != dicelist.end())
		{
			cout << (*itor).display() << endl;
			itor++;
		}
		cout << dsp.getName();
	}
	*/
	getchar();
	return 0;
}

