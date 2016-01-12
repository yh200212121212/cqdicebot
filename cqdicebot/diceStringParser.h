#include <string>
#include <list>
#include "diceSet.h"
using namespace std;

const int PARSE_STANDBY = 1;//0001 No string is parsed right now.
const int PARSE_FINISHED_SUCCESSFULLY = 0;//0000 A command string is parsed successfully.
const int PARSE_FINISHED_ERROR = 4;//0100 Something bad happens in parsing.
const int PARSE_ERROR_ILL_INPUT = 4;//0100 The input is not written properly.
const int PARSE_ERROR_DICE_EXCEED_MAXIMUM = 6;//0110 Too much dice or a die has too many faces.

const int PARSE_MODE_ROLL = 0;//00 command for rolling some dice
const int PARSE_MODE_HIDE = 1;//01 command for rolling some dice behind screen
const int PARSE_MODE_NAME = 2;//10 command for changing name

/*
String parser specified for .r command and so on.
*/
class diceStringParser
{
private:
	int status = PARSE_STANDBY;
	int mode = PARSE_MODE_ROLL;
	string roll_name;
	list<diceSet> diceseries;
public:
	diceStringParser() {

	};
	~diceStringParser() {
		
	};
	/*
	Input target string. This method will update the inner status of object.
	*/
	int Parse(const char * msg) {
		if (msg[0] != '.') { status = PARSE_ERROR_ILL_INPUT; return status; }
		string input_ori = msg;
		int input_ori_length = input_ori.length();
		string input_trim = msg;
		//fuck this std::string without trim()
		int i = input_ori_length - 1;
		while (i > 0) {
			if (input_trim[i] == ' '
				|| input_trim[i] == '\n'
				|| input_trim[i] == '\r'
				) {
				input_trim.erase(i);
			}
			i--;
		}
		int input_trim_length = input_trim.length();

		if (input_trim.length() < 3) { status = PARSE_ERROR_ILL_INPUT; return status; }
		
		size_t command_end = input_trim.find_first_of("1234567890+-") - 1;
		if(command_end == string::npos) { status = PARSE_ERROR_ILL_INPUT; return status; }
		string command = input_trim.substr(0, command_end + 1);

		if (command.compare(".r") == 0
			|| command.compare(".R") == 0) {
			string roll_list;
			mode = PARSE_MODE_ROLL;
			int roll_name_loc = input_ori.find_last_of("1234567890") + 1;
			if (roll_name_loc < input_ori_length) { roll_name = input_trim.substr(roll_name_loc, input_ori_length - roll_name_loc - 1); }
			else roll_name = "";
			roll_list = input_trim.substr(command_end + 1, roll_name_loc - command_end);

			diceSet ds;
			while (roll_list.length() > 0)
			{
				int lap = ds.getDice(&roll_list);
				if (lap == diceSet::GETDICE_FAIL) continue;
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
			mode = PARSE_MODE_ROLL;
		}
		else {
			status = PARSE_ERROR_ILL_INPUT; 
		}
		return status;
	};
	/*
	Get parse status.
	*/
	int getStatus() {
		return status;
	};
	/*
	Get parse result.
	*/
	list<diceSet> getDice() {
		return diceseries;
	}
	string getName() {
		return roll_name;
	}
};