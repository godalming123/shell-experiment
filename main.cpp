#include <iostream> // for cout (print to console)
#include <unistd.h> // to sleep (and usleep)
#include <sstream>  // for string streams

// --- a list of ANSI keycodes to help us ---
// text styles
std::string TEXT_UNDERLINE      = "\033[4m";
std::string TEXT_BOLD           = "\033[1m";
std::string RESET               = "\033[0m";
// keycodes to move the cursor
std::string CURSOR_UP           = "\E[A";
std::string CURSOR_DOWN         = "\E[B";
std::string CURSOR_LEFT         = "\E[D";
std::string CURSOR_RIGHT        = "\E[C";
std::string CURSOR_START        = "\r";
std::string CURSOR_ERASE_TO_END = "\033[K";
std::string CURSOR_SAVE_POS     = "\033[s";
std::string CURSOR_RESTORE_POS  = "\033[u";
// colors (fg)
std::string FG_BLACK            = "\033[30m";
std::string FG_RED              = "\033[31m";
std::string FG_GREEN            = "\033[32m";
std::string FG_YELLOW           = "\033[33m";
std::string FG_BLUE             = "\033[34m";
std::string FG_MAGENTA          = "\033[35m";
std::string FG_CYAN             = "\033[36m";
std::string FG_WHITE            = "\033[37m";
std::string FG_LIGHT_BLACK      = "\033[90m";
std::string FG_LIGHT_RED        = "\033[91m";
std::string FG_LIGHT_GREEN      = "\033[92m";
std::string FG_LIGHT_YELLOW     = "\033[93m";
std::string FG_LIGHT_BLUE       = "\033[94m";
std::string FG_LIGHT_MAGENTA    = "\033[95m";
std::string FG_LIGHT_CYAN       = "\033[96m";
std::string FG_LIGHT_WHITE      = "\033[97m";
// colors (bg)
std::string BG_BLACK            = "\033[40m";
std::string BG_RED              = "\033[41m";
std::string BG_GREEN            = "\033[42m";
std::string BG_YELLOW           = "\033[43m";
std::string BG_BLUE             = "\033[44m";
std::string BG_MAGENTA          = "\033[45m";
std::string BG_CYAN             = "\033[46m";
std::string BG_WHITE            = "\033[47m";
std::string BG_LIGHT_BLACK      = "\033[100m";
std::string BG_LIGHT_RED        = "\033[101m";
std::string BG_LIGHT_GREEN      = "\033[102m";
std::string BG_LIGHT_YELLOW     = "\033[103m";
std::string BG_LIGHT_BLUE       = "\033[104m";
std::string BG_LIGHT_MAGENTA    = "\033[105m";
std::string BG_LIGHT_CYAN       = "\033[106m";
std::string BG_LIGHT_WHITE      = "\033[107m";
// combinations of ansi codes
std::string CLEAR_LINE          = CURSOR_START + CURSOR_ERASE_TO_END + CURSOR_START;

// move the cursor with ansi functions
void cursorMove(int x, int y) {
	std::cout << "\033[" << y << x << "H";
}
void cursorChangeX(int relativeX) {
	if (relativeX > 0) {
		std::cout << "\033[" << relativeX << "C";
	}
	else if (relativeX < 0) {
		std::cout << "\033[" << -relativeX << "D";
	}
}
void cursorChangeY(int relativeY) {
	if (relativeY > 0) {
		std::cout << "\033[" << relativeY << "B";
	}
	else if (relativeY < 0) {
		std::cout << "\033[" << -relativeY << "A";
	}
}

// print a simple prompt for the user
void printPrompt(std::string cmdTxt) {
	std::cout << FG_LIGHT_GREEN << TEXT_BOLD << "   currentDir " << RESET << "on " << TEXT_BOLD << FG_LIGHT_BLUE << " github branch [+] " << RESET << FG_LIGHT_CYAN << "\n > " << RESET << cmdTxt;
}

// print the command text again
void reprintCmdTxt(std::string cmdTxt, int cursorPos) {
	std::cout << CURSOR_SAVE_POS;  // save the cursor position
	cursorChangeX(-cursorPos);     // move the cursor to the start of the line
	std::cout
		<< cmdTxt              // print the command text again
		<< " "                 // print a space so any charecters from the preious command text get overidden
		<< CURSOR_RESTORE_POS; // restore the cursor postion
}

// print a portion of text styled like a header
void printHeader(std::string text) {
	std::cout << FG_CYAN << "  " << RESET << BG_CYAN << TEXT_BOLD << text << RESET << FG_CYAN << ""<< RESET << "\n";
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
	int cursorPos = 0;

	// print some fancy colors
	std::cout << BG_BLACK       << "   " << BG_RED       << "   " << BG_GREEN       << "   " << BG_YELLOW       << "   " << BG_BLUE       << "   " << BG_MAGENTA       << "   " << BG_CYAN << "   " << RESET << "\n";
	std::cout << BG_LIGHT_BLACK << "   " << BG_LIGHT_RED << "   " << BG_LIGHT_GREEN << "   " << BG_LIGHT_YELLOW << "   " << BG_LIGHT_BLUE << "   " << BG_LIGHT_MAGENTA << "   " << BG_LIGHT_CYAN << "   " << RESET << "\n";

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
					std::cout << CLEAR_LINE << CURSOR_UP << CLEAR_LINE; // clear the line for a banner
					printHeader(cmdTxt); // print a banner for the command output
					system(cmdTxt.data()); // run the command
					cmdTxt = ""; // reset the command text
					cursorPos = 0; // reset the cursor position
					printPrompt(cmdTxt); // print a prompt for the user
				}
			break;
			case '\177': // if they press backspace
				// remove the last charecter from the command to be ran
				if (cmdTxt != "" and cursorPos > 0) {
					cmdTxt.erase(cursorPos-1, 1);     // erase the charecter before our cursor
					reprintCmdTxt(cmdTxt, cursorPos); // print the command text again
					std::cout << CURSOR_LEFT;         // move the cursor left
					cursorPos--;                      // change the variable that stores cursor position as it has changed in the console
				}
			break;
			case '^':
				std::cout << "here";
			break;
			case '\E': // this means they pressed an arrow key or delete
				if (getchar() == '[') {
					switch (char charecter = getchar()) {
						case 'A': // up arrow
							std::cout << "↑" << CURSOR_LEFT;
						break;
						case 'B': // down arrow
							std::cout << "↓" << CURSOR_LEFT;
						break;
						case 'D': // left arrow
							if (cursorPos > 0) {
								cursorPos--;
								std::cout << CURSOR_LEFT;
							}
						break;
						case 'C': // right arrow
							if (cursorPos < cmdTxt.length()) {
								cursorPos++;
								std::cout << CURSOR_RIGHT;
							}
						break;
						case 'H': // home key
							cursorChangeX(-cursorPos);
							cursorPos = 0;
						break;
						case 'F': // end key
							cursorChangeX(cmdTxt.length() - cursorPos);
							cursorPos = cmdTxt.length();
						case '3': // delete key
							if (getchar() == '~' and cmdTxt != "" and cursorPos < cmdTxt.length()) {
								cmdTxt.erase(cursorPos, 1);     // erase the charecter on our cursor
								reprintCmdTxt(cmdTxt, cursorPos); // print the command text again
							}
						break;
					}
				}
			break;
			default:
				// add typed charecter to string
				cmdTxt.insert(cursorPos, 1, charecter);
				reprintCmdTxt(cmdTxt, cursorPos);
				std::cout << CURSOR_RIGHT;
				cursorPos++;
			break;
		}
	}

	// return success code
	return 0;
}
