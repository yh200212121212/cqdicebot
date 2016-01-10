#pragma once
#include <random>
using namespace std;

random_device rdd;
mt19937 mtrand(rdd());
/*
A single dice roll, for example +4d6, -8d6k3.
*/

const int MAX_DICE = 100;

class diceSet
{
private:
	int sign;
	int numOfDice;
	int numOfFaces;
	int numOfKeep;
public:
	diceSet(int sig, int dice, int face, int keep = -1) {
		sign = sig;
		numOfDice = dice;
		numOfFaces = face;
		numOfKeep = keep;
	}
	int throwDice() {
		if (numOfDice > MAX_DICE) return -1;
		if (numOfKeep == 0)return 0;

		int ret = 0;

		if (numOfKeep < 0 || numOfKeep>numOfDice) {
			for (int i = 0; i < numOfDice; i++) {
				ret += (mtrand() % numOfFaces + 1);
			}
		}
		else {
			int out[MAX_DICE] = { 0 };

			for (int i = 0; i < numOfDice; i++) {
				if (i >= numOfKeep) {
					int rand = (mtrand() % numOfFaces + 1);
					for (int j = 0; j < numOfKeep; i++)
						if (out[j] < rand) { out[j] = rand; break; }
				}
				else {
					out[i] = (mtrand() % numOfFaces + 1);
				}
			}

			for (int i = 0; i < numOfKeep; i++) {
				ret += out[i];
			}
		}
		return (ret * sign);
	}
};