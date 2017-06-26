#ifndef xml_parser_h
#define xml_parser_h

#include <iostream>
#include <vector>
#include <map>
#include "stack.h"
#include "terminal.h"

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

const vector<string> COLOR_TAGS {
	"<c = blk>",
	"<c = red>",
	"<c = grn>",
	"<c = ylw>",
	"<c = blu>",
	"<c = mgn>",
	"<c = cyn>",
	"<c = wht>"
};

#define C_BLACK COLOR_TAGS[0]
#define C_RED COLOR_TAGS[1]
#define C_GREEN COLOR_TAGS[2]
#define C_YELLOW COLOR_TAGS[3]
#define C_BLUE COLOR_TAGS[4]
#define C_MAGENTA COLOR_TAGS[5]
#define C_CYAN COLOR_TAGS[6]
#define C_WHITE COLOR_TAGS[7]

const vector<string> TEXT_TAGS {
	C_BLACK,
	C_RED,
	C_GREEN,
	C_YELLOW,
	C_BLACK,
	C_WHITE,
	C_BLUE,
	C_MAGENTA,
	C_CYAN,
	C_WHITE,
	"<br/>",
	"<ln/>",
	"<u>",
	"<b>"
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

	static bool is_next_text (CharStack const &stack, string const &text) {
		return stack.size() >= text.length() && stack.peak_string(text.length()) == text;
	}

	static bool is_next_text (CharStack const &stack, vector<string> const &texts) {
		bool output = false;
		for (string s : texts) {
			output = output || is_next_text(stack, s);
		}
		return output;
	}

	static void swap_tag_for_char (CharStack &stack, string const &tag, char c) {
		for (int i = 0; i < tag.length(); i++) {
			stack.pop();
		}
		stack.push(c);
	}

	static void swap_tag_for_string (CharStack &stack, string const &tag, string const &s) {
		for (int i = 0; i < tag.length(); i++) {
			stack.pop();
		}
		for (int i = s.length() - 1; i >= 0; i--) {
			stack.push(s[i]);
		}
	}

	static bool is_text_tag (string const &tag) {
		for (string s : TEXT_TAGS) {
			if (tag == s) {
				return true;
			}
		}
		return false;
	}

	static void parse_text (CharStack &stack, string &text) {

		text += parse_till(stack, '<');
		
		if (is_next_text(stack, "<br/>")) {
			swap_tag_for_char(stack, "<br/>", PAUSE_MARKER);
			parse_text(stack, text);

		} else if (is_next_text(stack, "<ln/>")) {
			swap_tag_for_char(stack, "<ln/>", LINE_MARKER);
			parse_text(stack, text);

		} else if (is_next_text(stack, "<u>")) {
			swap_tag_for_string(stack, "<u>", terminal::esc_char("[4m"));
			parse_text(stack, text);

		} else if (is_next_text(stack, "<b>")) {
			swap_tag_for_string(stack, "<b>", terminal::esc_char("[1m"));
			parse_text(stack, text);

		} else if (is_next_text(stack, COLOR_TAGS)) {
			string tag = stack.peak_string(9);
			int i;
			for (i = 0; i < COLOR_TAGS.size(); i++) {
				if (tag == COLOR_TAGS[i]) {
					break;
				}
			}
			swap_tag_for_string(stack, "<c = --->", terminal::esc_char("[3" + to_string(i) + "m"));
			parse_text(stack, text);

		} else if (is_next_text(stack, vector<string>{"</u>", "</b>", "</c>"})) {
			swap_tag_for_string(stack, "</_>", terminal::esc_char("[0m"));
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

				// If tag is a text tag then parse as text instead of as a tag
				if (is_text_tag("<" + name + ">")) {
					raw.push_string("<" + name + ">");
					string text = "";
					parse_text(raw, text);
					tagBuilder.push(TagPiece{text, false});
					continue;
				}

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
