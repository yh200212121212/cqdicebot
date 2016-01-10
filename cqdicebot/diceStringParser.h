#include <string>
using namespace std;

/*
String parser specified for .r command and so on.
*/
class diceStringParser
{
private:
	string ret;
	int status = PARSE_STANDBY;
public:
	const int PARSE_STANDBY = 1;//0001 No string is parsed right now.
	const int PARSE_FINISHED_SUCCESSFULLY = 0;//0000 A command string is parsed successfully.
	const int PARSE_FINISHED_ERROR = 4;//0100 Something bad happens in parsing.
	const int PARSE_ERROR_ILL_INPUT = 4;//0100 The input is not written properly.
	const int PARSE_ERROR_DICE_EXCEED_MAXIMUM = 6;//0110 Too much dice or a die has too many faces.
	diceStringParser() {

	};
	~diceStringParser() {
		
	};
	/*
	Input target string. This method will update the inner status of object.
	*/
	void Parse(const char * msg) {
		if (msg[0] != '.') { status = PARSE_ERROR_ILL_INPUT; return; }
		string input = msg;

		//fuck this std::string without trim()
		int i = input.length() - 1;
		while (i > 0) {
			if (input[i] == ' '
				|| input[i] == '\n'
				|| input[i] == '\r'
				) {
				input.erase(i);
			}
			i--;
		}

		if (input.length() < 3) { status = PARSE_ERROR_ILL_INPUT; return; }
		
		int command_end = input.find_first_of("1234567890+-") - 1;
		string command = input.substr(0, command_end);
		if (command.compare(".r") == 0
			|| command.compare(".R") == 0) {
		}
		else if (command.compare(".h") == 0
			|| command.compare(".H") == 0) {

		}
		else if (command.compare(".d") == 0
			|| command.compare(".D") == 0) {
		}
		else if (command.compare(".n") == 0
			|| command.compare(".N") == 0) {
		}
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
	string getResult() {
		if (status == PARSE_STANDBY) return NULL;
		if (status&PARSE_FINISHED_ERROR) return NULL;
		return ret;
	};
};