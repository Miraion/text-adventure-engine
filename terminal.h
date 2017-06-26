#ifndef terminal_h
#define terminal_h

#include <iostream>
#include <vector>

using namespace std;

namespace terminal {

	void esc (string const &cmd) {
		cout << "\u001B" + cmd;
	}

	string esc_char (string const &cmd) {
		return "\u001B" + cmd;
	}

	void cursor_pos (int row, int col) {
		esc("[" + to_string(row) + ";" + to_string(col) + "H");
	}

	void curosr_home () {
		esc("[H");
	}

	void cursor_up (int n = 1) {
		esc("[" + to_string(n) + "A");
	}

	void clear (int height = 50) {
		for (int i = 0; i < height; i++) {
			cout << '\n';
		}
		esc("[r");
	}

	void erase () {
		curosr_home();
		esc("[J");
	}

	void erase_down () {
		esc("[J");
	}

	void erase_end () {
		esc("[K");
	}

	void print_line_below (string const &line) {
		esc("7");
		cout << '\n';
		cout << line;
		esc("8");
	}

	void print_line_below (vector<string> const &lines) {
		esc("7");
		for (string line : lines) {
			cout << '\n';
			cout << line;
		}
		esc("8");
	}

	string prompt_string (string const &text = " > ") {
		cout << text;
		string input = "";
		getline(cin, input);
		return input;
	}

	int prompt_int (int lower_bound, int upper_bound, string text = " > ") {
		do {
			esc("7");
			try {
				string input = prompt_string(text);
				int i = stoi(input);
				if (i < lower_bound || i >= upper_bound) {
					throw out_of_range("input out of bound");
				}
				return i;
			} catch (...) {
				text = "Try again > ";
				esc("8");
				esc("[K");
			}
		} while (true);
	}

	void prompt_enter (string const &text = "...") {
		cout << text;
		string input = "";
		getline(cin, input);
	}

}

#endif