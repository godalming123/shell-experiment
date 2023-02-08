#include <iostream> // for cout (print to console)
#include <unistd.h> // to sleep (and usleep)
#include <sstream>  // for string streams

// --- a list of ANSI keycodes to help us ---
// text styles
char* TEXT_UNDERLINE   = "\033[4m";
char* TEXT_BOLD        = "\033[1m";
char* RESET            = "\033[0m";
// keycodes to move the cursor
char* CURSOR_UP        = "\E[A";
char* CURSOR_DOWN      = "\E[B";
char* CURSOR_LEFT      = "\E[D";
char* CURSOR_RIGHT     = "\E[C";
char* CURSOR_START     = "\r";
// colors (fg)
char* FG_BLACK         = "\033[30m";
char* FG_RED           = "\033[31m";
char* FG_GREEN         = "\033[32m";
char* FG_YELLOW        = "\033[33m";
char* FG_BLUE          = "\033[34m";
char* FG_MAGENTA       = "\033[35m";
char* FG_CYAN          = "\033[36m";
char* FG_WHITE         = "\033[37m";
char* FG_LIGHT_BLACK   = "\033[90m";
char* FG_LIGHT_RED     = "\033[91m";
char* FG_LIGHT_GREEN   = "\033[92m";
char* FG_LIGHT_YELLOW  = "\033[93m";
char* FG_LIGHT_BLUE    = "\033[94m";
char* FG_LIGHT_MAGENTA = "\033[95m";
char* FG_LIGHT_CYAN    = "\033[96m";
char* FG_LIGHT_WHITE   = "\033[97m";
// colors (bg)
char* BG_BLACK          = "\033[40m";
char* BG_RED            = "\033[41m";
char* BG_GREEN          = "\033[42m";
char* BG_YELLOW         = "\033[43m";
char* BG_BLUE           = "\033[44m";
char* BG_MAGENTA        = "\033[45m";
char* BG_CYAN           = "\033[46m";
char* BG_WHITE          = "\033[47m";
char* BG_LIGHT_BLACK   = "\033[100m";
char* BG_LIGHT_RED     = "\033[101m";
char* BG_LIGHT_GREEN   = "\033[102m";
char* BG_LIGHT_YELLOW  = "\033[103m";
char* BG_LIGHT_BLUE    = "\033[104m";
char* BG_LIGHT_MAGENTA = "\033[105m";
char* BG_LIGHT_CYAN    = "\033[106m";
char* BG_LIGHT_WHITE   = "\033[107m";

// print a simple prompt for the user
void printPrompt(std::string cmdTxt) {
	std::cout << FG_LIGHT_GREEN << TEXT_BOLD << " currentDir " << RESET << FG_LIGHT_CYAN << "> " << RESET << cmdTxt;
}

// repeat a string
std::string repeat(std::string string, int n) {
    std::ostringstream os;
    for(int i = 0; i < n; i++)
        os << string;
    return os.str();
}

// the main code
int main() {
	// initialise
	system("stty -icanon"); // enable special charecter reading
	system("stty -echo"); // stop keypresses from appearing on screen
	
	// variables for main loop
	std::string cmdTxt = "";
	std::string currentDirectory = "";

	// start prompt
	printPrompt(cmdTxt);

	// main loop
	bool running = true;
	while(running) {
		// get a user keypress and process it
		switch (char charecter = getchar()) {
			case '\n': //  if they press newline
				// run the command
				if (cmdTxt == "exit") {
					// exit
					running = false;
				}
				else {
					// print a banner for the command output
					std::cout << CURSOR_START << "──── " << FG_LIGHT_CYAN << TEXT_BOLD << cmdTxt << RESET << " " << repeat("─", 50 - cmdTxt.length()) << "\n";
					system(cmdTxt.data()); // run the command
					cmdTxt = ""; // reset the command text
					printPrompt(cmdTxt); // print a prompt for the user
				}
			break;
			case '\177': // if they press backspace
				// remove the last charecter from the command to be ran
				if (cmdTxt != "") {
					cmdTxt.pop_back();
					std::cout << CURSOR_LEFT << " " << CURSOR_LEFT;
				}
			break;
			case '\E': // this means they will press an arrow key
				if (getchar() == '[') {
					switch (char charecter = getchar()) {
						case 'A': // up arrow
							cmdTxt = "up";
						break;
						case 'B': // down arrow
							cmdTxt = "down";
						break;
						case 'D': // left arrow
							cmdTxt = "left";
						break;
						case 'C': // right arrow
							cmdTxt = "right";
						break;
					}
					std::cout << CURSOR_START;
					printPrompt(cmdTxt);
				}
			break;
			default:
				// add typed charecter to string
				cmdTxt += charecter;
				std::cout << charecter;
			break;
		}
	}

	// return success code
	return 0;
}
