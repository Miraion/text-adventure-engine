#ifndef xml_parser_h
#define xml_parser_h

#include <iostream>
#include <vector>
#include <map>
#include "stack.h"

using namespace std;

const char PAUSE_MARKER = 1;
const char LINE_MARKER = '\n';
const char COMMENT_MARKER = '#';

struct Tag {
	string name;
	string text;
	vector<Tag> children {};

	Tag ()
	: name(""), text("")
	{ }

	Tag (string const &name, string const &text = "")
	: name(name), text(text)
	{ }
};

class XML {
private:

	struct TagPiece {
		string text;
		bool isTag;

		TagPiece (string const &text, bool isTag)
		: text(text), isTag(isTag)
		{ }
	};

	// Creates string by poping elements off of stack
	// Stops when sees keyChar.
	static string parse_till (CharStack &stack, char keyChar) {

		string str = "";
		while (!stack.empty() && stack.peak() != keyChar) {
			str.push_back(stack.pop());
		}
		return str;
	}

	static void parse_text (CharStack &stack, string &text) {

		text += parse_till(stack, '<');
		if (stack.size() >= 5 && stack.peak_string(5) == "<br/>") {
			for (int i = 0; i < 5; i++) {
				stack.pop();
			}
			stack.push(PAUSE_MARKER);
			parse_text(stack, text);
		} else if (stack.size() >= 5 && stack.peak_string(5) == "<ln/>") {
			for (int i = 0; i < 5; i++) {
				stack.pop();
			}
			stack.push(LINE_MARKER);
			parse_text(stack, text);
		}
	}

	static void clean (CharStack &stack) {

		CharStack cleanStack {};
		while (!stack.empty()) {
			char c = stack.pop();
			if (c == COMMENT_MARKER) {
				parse_till(stack, '\n');
			} else if (c != '\n' && c != '\t') {
				cleanStack.push(c);
			}
		}

		cleanStack.invert();
		stack = cleanStack;
	}

	static bool oppositeStrings (string const &open, string const &close) {
		return close == "/" + open;
	}

	static Tag make_tag (Stack<TagPiece> &tagBuilder) {

		string name = "";
		string text = "";
		vector<Tag> children {};

		while (!tagBuilder.empty()) {
			TagPiece item = tagBuilder.pop();
			if (item.isTag && item.text[0] != '/') {
				name = item.text;

				if (!tagBuilder.empty()) {
					children.push_back(Tag{name, text});
				} else {
					Tag t {name, text};
					reverse(children.begin(), children.end());
					t.children = children;
					return t;
				}
			} else if (!item.isTag) {
				text = item.text;
			}
		}
		// will not reach here (only to remove warning)
		return Tag {}; 
	}

public:
		
	static map<string, Tag> parse (string const &str) {

		CharStack raw = CharStack{str};
		Stack<TagPiece> tagBuilder = Stack<TagPiece>{};
		map<string, Tag> tagList;

		clean(raw);

		while (!raw.empty()) {
			char c = raw.pop();

			if (c == '<') {
				string name = parse_till(raw, '>');
				raw.pop(); // remove '>'

				if (tagBuilder.size() > 0) {

					if (oppositeStrings(tagBuilder.peak_bottom().text, name)) {
						Tag t = make_tag(tagBuilder);
						tagList[t.name] = t;
					} else {
						tagBuilder.push(TagPiece{name, true});
					}
				} else {
					tagBuilder.push(TagPiece{name, true});
				}
			} else {
				raw.push(c);
				string text = "";
				parse_text(raw, text);
				tagBuilder.push(TagPiece{text, false});
			}
		}

		return tagList;
	}

};

#endif