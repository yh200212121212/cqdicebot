// dicebot_ConsoleApplication.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <diceSet.h>
#include <iostream>
using namespace std;

int main()
{
	diceSet ds1 = diceSet(1, 1, 20);
	diceSet ds2 = diceSet(1, 2, 6);
	diceSet ds3 = diceSet(1, 3, 6);
	cout << "1d20s:\n";
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			cout << ds1.throwDice() << "\t";
		}
		cout << endl;
	}

	getchar();
	return 0;
}

