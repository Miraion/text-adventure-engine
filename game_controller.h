#ifndef game_controller_h
#define game_controller_h

#include "file.h"
#include "xml_parser.h"
#include "terminal.h"
#include "settings.h"
#include <map>
#include <chrono>
#include <thread>

using namespace std;

class GameController {
private:

	map<string, Tag> tags {};

	Tag currentTag {};

	vector<string> break_up_text (string const &text) {
		vector<string> output {};
		string builder = "";
		for (char c : text) {
			if (c == PAUSE_MARKER) {
				output.push_back(builder);
				builder = "";
			} else {
				builder.push_back(c);
			}
		}
		if (builder.length() > 0) {
			output.push_back(builder);
		}
		return output;
	}

	// returns the name of the selected tag
	// currentTag must have children
	string prompt () {
		cout << endl;

		vector<string> selections {};
		for (int i = 0; i < currentTag.children.size(); i++) {
			selections.push_back(
				"   [" + to_string(i + 1) + "] " + currentTag.children[i].text
			);
		}

		terminal::print_line_below(selections);
		int input = terminal::prompt_int(1, selections.size() + 1, "Selection > ");
		terminal::erase_down();

		terminal::cursor_up();
		terminal::erase_end();

		cout << "    " << currentTag.children[input - 1].text << endl;
		cout << endl;

		return currentTag.children[input - 1].name;
	}

	void print_line (string const &text) {
		if (settings::text_delay != 0) {
			chrono::nanoseconds delay_time (settings::text_delay * 10000000);
			for (int i = 0; i < text.length(); i++) {
				cout << text[i] << flush;
				this_thread::sleep_for(delay_time);
			}
		} else {
			cout << text;
		}
	}

	void print_text (string const &text) {
		vector<string> textChunks = break_up_text(text);
		if (textChunks.size() > 1) {
			for (int i = 0; i < textChunks.size(); i++) {
				print_line(textChunks[i]);
				if (i != textChunks.size() - 1) {
					terminal::prompt_enter();
				} else {
					cout << endl;
				}
			}
		} else {
			print_line(text + "\n");
		}
	}

public:

	GameController (map<string, Tag> const &tags)
	: tags(tags)
	{ }

	void start () {
		try {

			if (settings::display_synopsis && settings::synopsis != "") {
				if (settings::synopsis_text_delay) {
					print_line(settings::synopsis);
				} else {
					cout << settings::synopsis;
				}
				cout << endl << endl;
				terminal::prompt_enter("Press Enter to Start...");
				cout << endl;
			}

			currentTag = tags["start"];
			if (currentTag.text == "") {
				throw runtime_error{"Unable to find start tag."};
			}
			cout << "  --- " << tags["title"].text << " ---  " << endl << endl;
			print_text(currentTag.text);
			while (currentTag.children.size() > 0) {
				currentTag = tags[prompt()];
				print_text(currentTag.text);
			}

			cout << endl << " -- End -- \n\n";
			terminal::prompt_enter("Press Enter to Quit...");
		} catch (runtime_error e) {
			cout << "An error has occurred:\n" << "    " << e.what() << endl;
			cout << "Terminating program...\n";
		} catch (...) {
			cout << "Something went wrong.\n";
			cout << "Terminating program...\n";
		}
	}

};

#endif