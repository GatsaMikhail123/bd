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

std::pair<int, std::string> Utils::getLeftPart (std::string s, const char splitter, bool ignoreQuotes) {
    bool inQuotes = false;

    if (s.length() == 0)
        return std::make_pair(0, "");

    for (unsigned int i = 0; i <= s.length(); i++) {
        if (s[i] == splitter || i == s.length()) {
            if (inQuotes && !ignoreQuotes) continue;

            std::string piece = s.substr(0, i);

            return std::make_pair(i - 1, piece);
        }
        if (s[i] == '\"')
            inQuotes = !inQuotes;
    }

    return std::make_pair(0, "");
}
std::pair<int, std::string> Utils::getRightPart (std::string s, const char splitter, bool ignoreQuotes) {
    bool inQuotes = false;

    if (s.length() == 0)
        return std::make_pair(0, "");

    for (unsigned int i = s.length() - 1; i >= 0; i--) {
        if (s[i] == splitter || i == 0) {
            if (inQuotes && !ignoreQuotes) continue;

            std::string piece = s.substr(i + 1);

            return std::make_pair(i - 1, piece);
        }
        if (s[i] == '\"')
            inQuotes = !inQuotes;
    }

    return std::make_pair(0, "");
}
