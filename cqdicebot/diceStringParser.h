#include <string>
#include <list>
#include "diceSet.h"
#include "mystring.h"
using namespace std;

#define PARSE_STANDBY 1//0001 No string is parsed right now.
#define PARSE_FINISHED_SUCCESSFULLY 0//0000 A command string is parsed successfully.
#define PARSE_FINISHED_ERROR 4//0100 Something bad happens in parsing.
#define PARSE_ERROR_ILL_INPUT 4//0100 The input is not written properly.
#define PARSE_ERROR_DICE_EXCEED_MAXIMUM 6//0110 Too much dice or a die has too many faces.
#define PARSE_OUTPUT_GENERATED 8//1000

#define PARSE_MODE_ROLL 0//00 command for rolling some dice
#define PARSE_MODE_HIDE 1//01 command for rolling some dice behind screen
#define PARSE_MODE_NAME 2//10 command for changing name

/*
String parser specified for .r command and so on.
*/
class diceStringParser
{
private:
	int status = PARSE_STANDBY;
	int mode = PARSE_MODE_ROLL;
	string roll_name;
	string dice_roll;
	list<diceSet> diceseries;
	string output;
public:
	diceStringParser() {

	};
	diceStringParser(const char * msg) {
		Parse(msg);
	};
	/*
	Input target string. This method will update the inner status of object.
	*/
	int Parse(const char * msg) {
		if (msg[0] != '.') { status = PARSE_ERROR_ILL_INPUT; return status; }
		string input_ori = msg;
		size_t input_ori_length = input_ori.length();
		string input_trim = msg;
		
		my_trim(&input_trim);
		size_t input_trim_length = input_trim.length();
		if (input_trim.length() < 3) { status = PARSE_ERROR_ILL_INPUT; return status; }
		
		if (getCommand(&input_ori) == PARSE_ERROR_ILL_INPUT) return status;

		diceSet a;

		switch (status)
		{
		case PARSE_MODE_ROLL:
			my_trim(&input_ori);
			a.getDice(&input_ori,true);
		case PARSE_MODE_NAME:
			//TODO: write a name class, make it possible for player to change their name temporarily
		case PARSE_MODE_HIDE:
			//TODO: .h1d6 means roll 1d6 and not to display it in current window but through a private message
		default:
			return status;
		}

		size_t command_end = input_trim.find_first_of("1234567890+-") - 1;
		if(command_end == string::npos) { status = PARSE_ERROR_ILL_INPUT; return status; }
		string command = input_trim.substr(0, command_end + 1);

		if (command.compare(".r") == 0
			|| command.compare(".R") == 0) {
			string roll_list;
			mode = PARSE_MODE_ROLL;
			size_t roll_name_loc = input_ori.find_last_of("1234567890") + 1;
			if (roll_name_loc < input_ori_length) { roll_name = input_trim.substr(roll_name_loc, input_ori_length - roll_name_loc - 1); }
			else roll_name = "";
			roll_list = input_trim.substr(command_end + 1, roll_name_loc - command_end);

			diceSet ds;
			while (roll_list.length() > 0)
			{
				int lap = ds.getDice_old(&roll_list);
				if (lap == GETDICE_FAIL) continue;
				else diceseries.push_back(ds);
			};
			status = PARSE_FINISHED_SUCCESSFULLY;
		}
		else if (command.compare(".h") == 0
			|| command.compare(".H") == 0) {
			mode = PARSE_MODE_HIDE;

		}
		else if (command.compare(".n") == 0
			|| command.compare(".N") == 0) {
			mode = PARSE_MODE_NAME;

		}
		else {
			status = PARSE_ERROR_ILL_INPUT; 
		}
		return status;
	};

	int getCommand(string * cmd) {
		my_trim(cmd);
		if(cmd->at(0)!='.') return status = PARSE_ERROR_ILL_INPUT;
		my_trim(cmd);
		switch (cmd->at(1))
		{
		case 'r':
		case 'R':
			mode = PARSE_MODE_ROLL;
			break;
		case 'h':
		case 'H':
			mode = PARSE_MODE_HIDE;
			break;
		case 'n':
		case 'N':
			mode = PARSE_MODE_NAME;
			break;
		default:
			//TODO: .4r1d6+1d8 mean roll 1d6+1d8 for 4 times and display results separately.
			return status = PARSE_ERROR_ILL_INPUT;
		}
		cmd->erase(0, 2);
		return PARSE_STANDBY;
	}

	/*
	Get parse status.
	*/
	int getStatus() {
		return status;
	};
	/*
	Generate dice roll output.
	*/
	void rollDice() {
		if (status != PARSE_FINISHED_SUCCESSFULLY) return;
		list<diceSet>::iterator itor;
		char buffer[256];
		dice_roll.clear();
		itor = diceseries.begin();
		if (itor != diceseries.end()) {
			_itoa_s(itor->throwDice(), buffer, 10);
			dice_roll += buffer;
		}
		itor++;
		while (itor != diceseries.end()) {
			int result = itor->throwDice();
			if (result > 0) dice_roll += "+";
			_itoa_s(result, buffer, 10);
			dice_roll += buffer;
			itor++;
		}
		status = PARSE_OUTPUT_GENERATED;
	}
	/*
	Get parse result.
	*/
	string getRoll() {
		return dice_roll;
	}
	/*
	Get parse result.
	*/
	list<diceSet> getDice() {
		return diceseries;
	}
	/*
	Get the name of roll.
	*/
	string getName() {
		return roll_name;
	}
};