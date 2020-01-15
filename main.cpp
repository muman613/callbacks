#include <iostream>
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>

using namespace std;
class ParseClass;

typedef vector<string> stringVec;

class CommandInfo {
public:
    stringVec       args;
    string          response = "OK";
};

typedef bool (ParseClass::*action)(CommandInfo & info);

typedef struct __commandEntry {
    string command;
    int minOptions;
    int maxOptions;
    action callback;
} CMDENTRY;


class ParseClass {
public:
    ParseClass();
    ~ParseClass();

    bool                parse(const string & command);

protected:
    bool                StartCmd(CommandInfo & info);
    bool                StopCmd(CommandInfo & info);

    vector<CMDENTRY>    commandVec;
};

ParseClass::ParseClass() {
//    cout << __FUNCTION__ << "\n";

    commandVec.push_back({ "START",
                           2, 2,
                           &ParseClass::StartCmd });
    commandVec.push_back({ "STOP",
                           2, 2,
                           &ParseClass::StopCmd });
}

ParseClass::~ParseClass() {
//    cout << __FUNCTION__ << "\n";
}

bool ParseClass::StartCmd(CommandInfo & info) {
    cout << __FUNCTION__ << "\n";

    cout << info.args[0] << " " << info.args[1] << "\n";
    return true;
}

bool ParseClass::StopCmd(CommandInfo & info) {
    cout << __FUNCTION__ << "\n";

    info.response = "FAIL";

    return true;
}

bool ParseClass::parse(const string & cmd) {
    stringVec args;

    boost::split(args, cmd, [](char c) { return c == ':'; });
    string command = args[0];
//  cout << "command = " << command << "\n";

    // Remove root command from list, leaving only the arguments...
    args.erase(args.begin());

    for (auto n : commandVec) {
        if (n.command == command) {
            // Check argument count...
            if ((n.minOptions >= args.size()) && (n.maxOptions <= args.size())) {
                CommandInfo info;
                info.args = args;
                bool result = (this->*(n.callback))(info);
                cout << info.response << "\n";

                return result;
            } else {
                cout << "Invalid # of arguments" << "\n";
            }
        }
    }
}

int main() {
    ParseClass m;

    m.parse("START:1:2");
    m.parse("STOP:1:2");

    return 0;
}
