#include "table.h"

Table::Table(std::string n, std::vector <std::string> columns) {
    this->name = n;

    for (int i = 0; i < columns.size(); i++)
        titles[columns[i]] = i;
}

int Table::execute (std::string command) {
    command = Utils::toLower(Utils::trim(command));

    auto firstPart = Utils::getLeftPart(command, ' ', true);

    if (firstPart.second == "show") {
        command = command.substr(firstPart.first + 1);
        command = Utils::trim(command);

        if (command.length() == 0 || command[0] != '(' || command[command.length() - 1] != ')') {
            cerr << "ERROR: Missing arguments in command SHOW.\n\tIN [table name] SHOW (col1, col2, col3 ...)" << endl;
            return -1;
        }

        command = Utils::trim(command, "()");

        std::vector <std::string> columns = Utils::split(command, ',');

        if (columns.size() == 0)
        {
            cerr << "ERROR: Missing arguments in command CREATE.\n\tCREATE [table name] (col1, col2, col3 ...)" << endl;
            return -1;
        }

        for (auto & col : columns) {
            if (col == "")
            {
                cerr << "ERROR: Missing arguments in command CREATE.\n\tCREATE [table name] (col1, col2, col3 ...)" << endl;
                return -1;
            }
            col = Utils::trim(col, "\"");
        }

        if (!showRows(columns)) {
            return -1;
        }
    }
    else if (firstPart.second == "delete") {
        command = command.substr(firstPart.first + 1);
        command = Utils::trim(command);

        if (Utils::isNumber(command) == false)
        {
            cerr << "ERROR: Incorrect arguments in command DELETE.\n\tIN [table name] DELETE [row id]" << endl;
            return -1;
        }

        int row_id = stoi(command);
        auto ids_iter = find(ids.begin(), ids.end(), row_id);

        if (ids_iter == ids.end()) {
            cerr << "ERROR: Row with id=" << row_id << " doesn't exists!" << endl;
            return -1;
        }

        rows.erase(rows.begin() + (ids_iter - ids.begin()));
        ids.erase(ids_iter);

        cout << "SUCCESS: Row with id \"" << row_id << "\" deleted!" << endl;
    }
    else if (firstPart.second == "add") {
        command = command.substr(firstPart.first + 1);
        command = Utils::trim(command);

        command = Utils::trim(command, "()");

        if (command == "")
        {
            cerr << "ERROR: Incorrect arguments in command ADD.\n\tIN [table name] ADD (value1, value2, ...)" << endl;
            return -1;
        }

        if (ids.empty()) ids.push_back(1);
        else             ids.push_back(ids[ids.size() - 1] + 1);

        vector <string> values = Utils::split(command, ',');

        if (values.size() != titles.size())
        {
            cerr << "ERROR: " << values.size() << " values given, " << titles.size() << " expected" << endl;
            return -1;
        }

        for (auto & val : values) {
            val = Utils::trim(val, "\"");
        }

        addRow (values);

        cout << "SUCCESS: Row added to table!" << endl;
    }
    else if (firstPart.second == "edit") {
        command = command.substr(firstPart.first + 1);
        command = Utils::trim(command);

        cout << command << endl;

        vector <string> args = Utils::split(command, ' ');
        if (args.size() < 3)
        {
            cerr << "ERROR: Incorrect arguments in command EDIT.\n\tIN [table name] EDIT [column] [value] [id]" << endl;
            return -1;
        }

        for (auto & arg : args) {
            arg = Utils::trim(arg, "\"");
        }

        if (Utils::isNumber(args[2]) == false)
        {
            cerr << "ERROR: Incorrect arguments in command EDIT.\n\tIN [table name] EDIT [column] [value] [id]" << endl;
            return -1;
        }

        int id = stoi(args[2]);
        auto ids_iter = find(ids.begin(), ids.end(), id);

        if (ids_iter == ids.end()) {
            cerr << "ERROR: Row with id=" << id << " doesn't exists!" << endl;
            return -1;
        }

        int index = (ids_iter - ids.begin());

        if (titles.find(args[0]) == titles.end())
        {
            cerr << "ERROR: Column \"" << args[0] << "\" doesn't exists!" << endl;
            return -1;
        }

        rows[index][titles[args[0]]] = args[1];

        cout << "SUCCESS: Value in row \"" << id << "\" and column \"" << args[0] << "\" changed to \"" << args[1] << "\"!" << endl;
    }

    return 1;
}

bool Table::showRows (std::vector <std::string> columns, int id) {
    std::vector <int> colIndexes;
    std::vector <vector<string>> usedRows;

    for (auto col : columns) {

        if (titles.find(col) != titles.end())
            colIndexes.push_back(titles[col]);

        else {
            cerr << "ERROR: Column \"" << col << "\" not found!" << endl;
            return false;
        }
    }

    columns.insert(columns.begin(), "id");

    for (int i = 0; i < rows.size(); i++) {
        usedRows.push_back(vector<string>());

        usedRows[i].push_back(to_string(ids[i]));

        for (int col : colIndexes)
            usedRows[i].push_back(rows[i][col]);
    }

    drawTable(columns, usedRows);

    return true;
}

bool Table::addRow (std::vector <std::string> row) {
    rows.push_back(row);
    return true;
}

vector <string> Table::getColumn (int colID) {
    vector <string> col;

    for (int i = 0; i < (int)rows.size(); i++)
        col.push_back(rows[i][colID]);

    return col;
}

void Table::drawTable (std::vector <std::string> columns, std::vector <std::vector <std::string>> values) {
    bool isEmpty = !values.size();

    int colsN = columns.size();

    vector <int> colWidths;

    if (isEmpty) {
        for (int i = 0; i < columns.size(); i++)
            colWidths.push_back(columns[i].length());
    }
    else {
        for (int i = 0; i < columns.size(); i++) {
            if (columns[i] == "id") {
                colWidths.push_back(max<int> (to_string(ids[ids.size() - 1]).length(), 2));
                continue;
            }

            int len = max<int>(Utils::maxLength(getColumn(titles[columns[i]])), columns[i].length());
            colWidths.push_back(len);
        }
    }

    drawBorder(colsN, colWidths);
    drawRow(columns, colWidths);
    drawBorder(colsN, colWidths);

    for (int i = 0; i < values.size(); i++) {
        drawRow(values[i], colWidths);
        drawBorder (colsN, colWidths);
    }
}
