#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <math.h>
#include <algorithm>

#include "utils.h"

using namespace std;

class Table
{

private:
    std::string name = "";

    std::map <std::string, int> titles;
    std::vector <std::vector<std::string>> rows;
    vector <int> ids;

    void drawBorder (int colsN, vector <int> colWidths) {
        for (int i = 0; i < colsN; i++) {
            cout << "+";
            for (int j = 0; j < colWidths[i] + 3; j++)
                cout << "-";
        }
        cout << "+" << endl;
    }
    void drawRow (vector <string> row, vector <int> colWidths) {
        cout << "|";
        for (int i = 0; i < row.size(); i++) {
            cout << setw(colWidths[i] + 2) << row[i] << " |";
        }
        cout << endl;
        // cout << "+" << endl;
    }


    void drawTable (std::vector <std::string> columns, std::vector <std::vector <std::string>> values);
    vector <string> getColumn (int colID);

public:
    Table(std::string n = "", std::vector <std::string> columns = {});

    std::string getName() {
        return this->name;
    }
    void setName (std::string n) {
        this->name = n;
    }

    bool clear () {
        rows = {};
        ids = {};

        return true;
    };

    bool addRow (std::vector <std::string> row);
    bool removeRow (int id);
    bool showRows (std::vector <std::string> columns, int id = 0);

    int execute (std::string command);
};

#endif // TABLE_H
