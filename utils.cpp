#include "utils.h"

std::vector <std::string> Utils::split (std::string s, const char splitter, bool ignoreQuotes) {
    if (s == "")
        return std::vector<std::string>();

    bool inQuotes = false;
    int start = 0;
    std::vector <std::string> parts;

    for (unsigned int i = 0; i <= s.length(); i++) {
        if (s[i] == splitter || i == s.length()) {
            if (inQuotes && !ignoreQuotes) continue;

            std::string piece = s.substr(start, i - start);
            parts.push_back(trim(piece));
            start = i + 1;
        }
        if (s[i] == '\"')
            inQuotes = !inQuotes;
    }

    return parts;
}

std::pair<int, std::string> Utils::getLeftPart (std::string str, const char splitter, bool ignoreQ) {
    bool inQ = false;

    if (str.length() == 0)
        return std::make_pair(0, "");

    for (unsigned int i = 0; i <= str.length(); i++) {
        if (str[i] == splitter || i == str.length()) {
            if (inQ && !ignoreQ) continue;

            std::string piece = str.substr(0, i);

            return std::make_pair(i - 1, piece);
        }
        if (str[i] == '\"')
            inQ = !inQ;
    }

    return std::make_pair(0, "");
}
std::pair<int, std::string> Utils::getRightPart (std::string str, const char splitter, bool ignoreQ) {
    bool inQ = false;

    if (str.length() == 0)
        return std::make_pair(0, "");

    for (unsigned int i = str.length() - 1; i >= 0; i--) {
        if (str[i] == splitter || i == 0) {
            if (inQ && !ignoreQ) continue;

            std::string piece = str.substr(i + 1);

            return std::make_pair(i - 1, piece);
        }
        if (str[i] == '\"')
            inQ = !inQ;
    }

    return std::make_pair(0, "");
}
