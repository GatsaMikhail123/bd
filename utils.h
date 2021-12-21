#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

const static char * ws = " \t\n\r\f\v";

class Utils
{

private:
    // trim from end of string (right)
    static inline std::string& rtrim(std::string& s, const char* t = ws)
    {
        s.erase(s.find_last_not_of(t) + 1);
        return s;
    }

    // trim from beginning of string (left)
    static inline std::string& ltrim(std::string& s, const char* t = ws)
    {
        s.erase(0, s.find_first_not_of(t));
        return s;
    }
public:    
    Utils() {};

    static bool isNumber (const std::string& s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it)) ++it;
        return !s.empty() && it == s.end();
    }

    static int maxLength (std::vector <std::string> vec) {
        if (vec.size() == 0) return 0;

        int max = vec[0].length();

        for (int i = 1; i < vec.size(); i++)
            if (max < vec[i].length()) max = vec[i].length();

        return max;
    }

    static std::vector <std::string> split (std::string s, const char splitter = ' ', bool ignoreQuotes = false);

    // trim from both ends of string (right then left)
    static inline std::string& trim(std::string& s, const char* t = ws)
    {
        return ltrim(rtrim(s, t), t);
    }

    static std::pair<int, std::string> getLeftPart (std::string s, const char splitter = ' ', bool ignoreQuotes = false);
    static std::pair<int, std::string> getRightPart (std::string s, const char splitter = ' ', bool ignoreQuotes = false);
    static std::string toLower (std::string s) {
        std::for_each(s.begin(), s.end(), [](char & c){
            c = ::tolower(c);
        });

        return s;
    }

    static bool replace(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = str.find(from);
        if(start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }
};

#endif // UTILS_H
