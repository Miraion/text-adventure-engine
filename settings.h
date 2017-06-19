#ifndef settings_h
#define settings_h

#include <iostream>
#include <map>
#include "xml_parser.h"

using namespace std;

namespace settings {

	string synopsis = "";

	int text_delay = 0;
	bool display_synopsis = true;
	bool synopsis_text_delay = true;

	void load (map<string, Tag> &tags) {
		string settingsTag = "settings";
		Tag settings = tags[settingsTag];
		
		if (settings.name != "" && settings.children.size() > 0) {
			for (Tag config : settings.children) {
				
				if (config.name == "text_delay") {
					try {
						int delay = stoi(config.text);
						if (delay < 0) {
							throw out_of_range("delay must be positve");
						}
						text_delay = delay;
					} catch (...) { }
				}

				else if (config.name == "display_synopsis") {
					display_synopsis = config.text != "false";
					if (display_synopsis) {
						synopsis = tags["synopsis"].text;
					}
				}

				else if (config.name == "synopsis_text_delay") {
					synopsis_text_delay = config.text != "false";
				}

			}
		}
	}

}

#endif