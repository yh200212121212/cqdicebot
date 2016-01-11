#pragma once
#include <random>
#include <string>
using namespace std;

random_device rdd;
mt19937 mtrand(rdd());

#define MAX_DICE 100
#define MAX_FACE 1000

/*
A single dice roll, for example +4d6, -8d6k3.
*/
class diceSet
{
private:
	int sign;
	int numOfDice;
	int numOfFaces;
	int numOfKeep;
public:
	const static int GETDICE_DONE = 0;
	const static int GETDICE_FAIL = 1;

	diceSet() {
		sign = 1;
		numOfDice = 0;
		numOfFaces = 1;
		numOfKeep = -1;
	}
	diceSet(int sig, int dice, int face = 1, int keep = -1) {
		sign = sig < 0 ? -1 : 1;
		numOfDice = dice > MAX_FACE ? MAX_FACE : dice;
		numOfFaces = face > MAX_FACE ? MAX_FACE : face;
		numOfKeep = keep;
	}
	void reset(int sig, int dice, int face = 1, int keep = -1) {
		sign = sig < 0 ? -1 : 1;
		numOfDice = dice;
		numOfFaces = face;
		numOfKeep = keep;
	}
	int throwDice() {
		if (numOfDice > MAX_DICE) return -1;//too many dice
		if (numOfFaces > MAX_FACE) return -1;//too many faces
		if (numOfDice <= 0) return 0;//no dice rolled
		if (numOfFaces <= 1) return numOfDice;//just a value, not dice
		if (numOfKeep <= 0)return 0;//nothing left if you keep nothing

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
				else { out[i] = (mtrand() % numOfFaces + 1); }
			}
			for (int i = 0; i < numOfKeep; i++)
				ret += out[i];
		}
		return (ret * sign);
	}

	int getDice(string str) {
		size_t split = str.find_first_of("+-",1);
		int endOfRoll;
		if (split == string::npos) endOfRoll = str.length() - 1;
		else endOfRoll = split - 1;

		bool flag_no_dice;
		bool flag_no_keep;
		bool flag_has_sign;
		int sign = 1, dice, face, keep = -1;
		//1d100+2d200
		//split = 5
		//
		flag_has_sign = (str[0] == '+' || str[0] == '-');
		string roll_string = str.substr(flag_has_sign ? 1 : 0, split);
		if (flag_has_sign) sign = (str[0] == '+' ? 1 : -1);

		size_t seq_d = roll_string.find_first_of("Dd");
		if (seq_d == string::npos) {
			//+6
			flag_no_dice = true;
			dice = atoi(roll_string.c_str());
			if (dice <= 0 || dice == INT32_MAX || dice == INT32_MIN) return GETDICE_FAIL;
			reset(sign, dice, 1, keep);
			return GETDICE_DONE;
		}

		size_t seq_k = roll_string.find_first_of("Kk");
		flag_no_keep = (seq_k == string::npos);
		
		string dice_string = roll_string.substr(0, seq_d);
		dice = atoi(dice_string.c_str());
		if (dice <= 0 || dice == INT32_MAX || dice == INT32_MIN) return GETDICE_FAIL;

		if (flag_no_keep) {
			string face_string = roll_string.substr(seq_d + 1);
			face = atoi(face_string.c_str());
			if (face <= 0 || face == INT32_MAX || face == INT32_MIN) return GETDICE_FAIL;
		}
		else {
			//3d100k2
			//seq_d = 1
			//seq_k = 5
			//length = 7
			string face_string = roll_string.substr(seq_d, roll_string.length() - seq_k - 1);
			face = atoi(face_string.c_str());
			if (face <= 0 || face == INT32_MAX || face == INT32_MIN) return GETDICE_FAIL;
			string keep_string = roll_string.substr(seq_k + 1);
		}

		reset(sign, dice, face, keep);

		return GETDICE_DONE;
	}
};