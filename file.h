#ifndef file_h
#define file_h

#include <iostream>
#include <fstream>

using namespace std;

namespace file {

	void append_path(string &path, string const &append) {
		while (path.length() > 0) {
			char c = path.back();
			#ifdef _WIN32
				if (c != '\\') {
					path.pop_back();
				} else {
					break;
				}
			#else
				if (c != '/') {
					path.pop_back();
				} else {
					break;
				}
			#endif
		}

		path += append;
	}

	string prompt_file_open (string path = "") {
		ifstream file;
		string contents = "";
		do {
			string name = "";
			cout << "Enter file to open: ";
			getline(cin, name);
			append_path(path, name += ".xml");
			file.open(path, ios::in);

			string line = "";
			while (getline(file, line)) {
				contents += line + '\n';
			}
			if (contents == "") {
				cout << "Unable to open file " << name << endl;
			} else {
				break;
			}
		} while (true);
		return contents;
	}

	string file_open (string path, string const &name) {
		append_path(path, name);
		ifstream file {path};

		// file.open(name, ios::in);

		string contents = "";
		string line = "";
		while (getline(file, line)) {
			contents +=line + '\n';
		}
		return contents;
	}

} // namespace file

#endif