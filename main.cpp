#include "file.h"
#include "stack.h"
#include "xml_parser.h"
#include "game_controller.h"
#include "terminal.h"
#include "settings.h"

using namespace std;

int main (int argc, char* argv[]) {
	terminal::clear();
	
	try {
		string data = file::prompt_file_open(argv[0]);
		map<string, Tag> tags = XML::parse(data);

		settings::load(tags);

		terminal::clear();

		GameController controller {tags};
		controller.start();
	} catch (...) {
		cout << "Something went wrong.\n";
		cout << "Ensure a proper file was selected.\n";
		cout << "\nTerminating program...\n";
	}
	
}