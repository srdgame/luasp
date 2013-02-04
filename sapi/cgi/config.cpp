#include "config.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
using namespace std;

Config::Config(string name, string parentDebugInfo) {
	debugInfo = parentDebugInfo + ", " + name;
}

Config::Config(string configFile, char** envp) {

	// Fix: by cch, should check the envp first before the *envp.
	while (envp && *envp) {
		string envEntry = *envp;
		size_t pos = envEntry.find('=');
		if (pos != string::npos) {
			string name = envEntry.substr(0, pos);
			string value = envEntry.substr(pos+1, string::npos);
			envSymbols[name] = value;
			log(LOG_DEBUG, "environment symbo: '%s' = '%s'", name.c_str(), value.c_str());
		}
		++envp;
	}


	debugInfo = configFile;
	groupStack.push_front(this);

	FILE* in = fopen(configFile.c_str(), "r");
	if (!in) {
		log(LOG_ERR, "cannot open input file '%s'", configFile.c_str());
		exit(2);
	} 

	char buff[1024];
	while (fgets(buff, 1024, in)) {

		string line=buff;
		if ( (line.length() > 2) && (line[0] != '#') && (line.find(')') == string::npos) ) {
			string name;
			string value;
			split(line, name, value, '=');

			if (value == "(") {
				log(LOG_DEBUG, "   config: new group '%s'", name.c_str());
				Config* newGroup = new Config(name, debugInfo);
				groupStack.front()->groups[name] = newGroup;
				groupStack.push_front(newGroup);
			} else {
				for (list<Config*>::reverse_iterator i = groupStack.rbegin(); i != groupStack.rend(); ++i) {
					(*i)->symbolExpand(value);
				}
				envSymbolExpand(value);
				log(LOG_DEBUG, "   config: name = '%s', value = '%s'", name.c_str(), value.c_str());
				groupStack.front()->add(name, value);
			}
		}
		if ( (line.length() > 0) && (line[0] != '#') && (line.find(')') != string::npos) ) {
			log(LOG_DEBUG, "%s", "   end of group");
			groupStack.pop_front();
		}
	}

	fclose(in);
}

Config::~Config() {
	for (map<string, Config*>::iterator i = groups.begin(); i != groups.end(); ++i) {
		delete i->second;
	}
}

void Config::add(string name, string value) {
	symbols[name] = value;
}

void Config::split(string in, string& left, string& right, char c) {
	size_t pos = in.find_first_of(c);
	if(pos == string::npos) {
		left = in;
		trim(left);
		right = "";
	} else if (pos <= 1) {
		left = "";
		right = in.substr(pos+1, string::npos);
		trim(right);
	} else {
		left = in.substr(0, pos-1);
		trim(left);
		right = in.substr(pos+1, string::npos);
		trim(right);
	}
}

void Config::trim(string& s) {
	while ( (s.length() > 1) && ( (s[0] == ' ') || (s[0] =='\t') ) ) {
		s = s.substr(1, string::npos);
	}
	while ( (s.length() > 1) &&
			( (s[s.length()-1] == ' ') ||
			  (s[s.length()-1] == '\t') || 
			  (s[s.length()-1] == '\n') || 
			  (s[s.length()-1] == '\r') ) ) {
		s = s.substr(0, s.length()-1);
	}
	if ( (s.length() > 1) && (s[0] == '"') ) {
		s = s.substr(1, string::npos);
	}
	if ( (s.length() > 1) && (s[s.length()-1] == '"') ) {
		s = s.substr(0, s.length()-1);
	}
}

void Config::symbolExpand(string& s) {
	symbolExpand(symbols, s);
}

void Config::envSymbolExpand(string& s) {
	symbolExpand(envSymbols, s);
}

void Config::symbolExpand(map<string, string>& symbols, string& s) {
	bool expanded;
	do {
		expanded = false;
		for (map<string, string>::iterator i = symbols.begin(); i != symbols.end(); ++i) {
			string search = "%" + i->first + "%";
			string replace = i->second;
			size_t pos = s.find(search);
			if (pos != string::npos) {
				expanded = true;
				s.replace(pos, search.length(), replace);
			}
		}
	} while (expanded);
}

string Config::pString(string name) {
	map<string, string>::iterator i = symbols.find(name);
	if (i == symbols.end()) {
		log(LOG_ERR, "access of missing property '%s' (%s)", name.c_str(), debugInfo.c_str());
		return string(); // do not crash here.
	}
	return i->second;
}

bool Config::pBool(string name) {
	string val = pString(name);

	if ( (val == "yes") ||
	     (val == "Yes") ||
	     (val == "YES") ||
		 (val == "true") ||
	     (val == "True") ||
	     (val == "TRUE"))
	{
		return true;
	}

	return false;
}

double Config::pDouble(string name) {
	string val = pString(name);

	return atof(val.c_str());
}

int Config::pInt(string name) {
	string val = pString(name);

	return atoi(val.c_str());
}
