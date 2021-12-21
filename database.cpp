#include "database.h"

Database::Database(){
}

int Database::execute (string command) {
    command = Utils::toLower(Utils::trim(command));

    if (command[command.length() - 1] == ';')
        command = command.substr(0, command.length() - 1);

    auto firstPart = Utils::getLeftPart(command, ' ', true);

    if(firstPart.second == "create"){
        command = command.substr(firstPart.first + 1);
        command = Utils::trim(command);

        if(command.length() == 0){
            cerr << "ERROR: Missing arguments in command CREATE.\n\tCREATE [table name] (col1, col2, col3 ...)" << endl;
            return -1;
        }

        auto namePart = Utils::getLeftPart(command, ' ');

        string name = namePart.second;
        name = Utils::trim(name, "\"");

        if (findTableByName(name) != nullptr) {
            cerr << "таблица \"" << name << "\" уже существует" << endl;
            return -1;
        }

        command = command.substr(namePart.first + 1);
        command = Utils::trim(command);

        if (command.length() == 0 || command[0] != '(' || command[command.length() - 1] != ')') {
            cerr << "ERROR: Missing arguments in command CREATE.\n\tCREATE [table name] (col1, col2, col3 ...)" << endl;
            return -1;
        }

        command = Utils::trim(command, "()");

        vector <string> columns = Utils::split(command, ',');

        if (columns.size() == 0)
        {
            cerr << "ERROR: Missing arguments in command CREATE.\n\tCREATE [table name] (col1, col2, col3 ...)" << endl;
            return -1;
        }

        for (auto & col : columns) {
            if (count(columns.begin(), columns.end(), col) > 1) {
                cerr << "Имя уже существует \"" << col << "\"" << endl;
                return -1;
            }
            if (col == "")
            {
                cerr << "ERROR: Missing arguments in command CREATE.\n\tCREATE [table name] (col1, col2, col3 ...)" << endl;
                return -1;
            }

            col = Utils::trim(col, "\"");
        }

        tables.push_back(new Table(name, columns));

        cout << "таблица \"" << name << "\" создана с " << columns.size() << "столбцами" << endl;
        return 1;

    }
    else if (firstPart.second == "delete") {
        command = command.substr(firstPart.first + 1);
        command = Utils::trim(command);

        if (command.length() == 0) {
            cerr << "ERROR: Missing arguments in command DELETE.\n\tDELETE [table]" << endl;
            return -1;
        }

        string name = command;
        name = Utils::trim(name);
        name = Utils::trim(name, "\"");

        if (name == "")
        {
            cerr << "ERROR: Missing arguments in command DELETE.\n\tDELETE [table]" << endl;
            return -1;
        }

        Table * table = findTableByName(name);

        if (table == nullptr)
        {
            cerr << "Таблица \"" << name << "\" не найдена!" << endl;
            return -1;
        }

         tables.erase(std::find(tables.begin(), tables.end(), table));
         delete table;
         cout << "Таблица \"" << name << "\" удалена" << endl;
         return 1;
    }
    else if (firstPart.second == "clear") {
        command = command.substr(firstPart.first + 1);
        command = Utils::trim(command);

        if (command.length() == 0) {
            cerr << "ERROR: Missing arguments in command CLEAR.\n\tCLEAR [table]" << endl;
            return -1;
        }

        string name = command;
        name = Utils::trim(name);
        name = Utils::trim(name, "\"");

        if (name == "")
        {
            cerr << "ERROR: Missing arguments in command CLEAR.\n\tCLEAR [table]" << endl;
            return -1;
        }

        Table * table = findTableByName(name);

        if (table == nullptr)
        {
            cerr << "Таблица \"" << name << "\"не найдена" << endl;
            return -1;
        }

        table->clear();
        cout << "Таблица\"" << name << "\" очищена" << endl;
    }
    else if (firstPart.second == "rename") {
        command = command.substr(firstPart.first + 1);
        command = Utils::trim(command);

        auto names = Utils::split(command, ' ');
        if (names.size() < 2) {
            cerr << "ERROR: Missing arguments in command RENAME.\n\tRENAME [old name] [new name]" << endl;
            return -1;
        }

        string oldName = names[0], newName = names[1];
        oldName = Utils::trim(Utils::trim(oldName), "\"");
        newName = Utils::trim(Utils::trim(newName), "\"");

        // cout << "Rename " << oldName << " to " << newName << endl;

        Table * table = findTableByName(oldName);

        if (table == nullptr)
        {
            cerr << "Таблица \"" << oldName << "\" не найдена" << endl;
            return -1;
        }

        if (findTableByName(newName) != nullptr) {
            cerr << "Таблица \"" << newName << "\"уже существует" << endl;
            return -1;
        }

        table->setName(newName);
        cout << "Таблица \"" << oldName << "\"переименована в \"" << newName << endl;
        return 1;
    }
    else if (firstPart.second == "exit") {
        return 0;
    }
    else if (firstPart.second == "list") {
        cout << "Tables: " << endl;

        for (int i = 0; i < tables.size(); i++) {
            cout << "\t" << i + 1 << ". " << tables[i]->getName() << endl;
        }
        if (tables.size() == 0)
            cout << "\t Таблиц нет" << endl;
    }

    else if (firstPart.second == "in") {
        command = command.substr(firstPart.first + 1);
        command = Utils::trim(command);

        auto namePart = Utils::getLeftPart(command, ' ');
        string name = namePart.second;
        name = Utils::trim(name);
        name = Utils::trim(name, "\"");

        if (name == "")
        {
            cerr << "ERROR: Missing arguments in command IN.\n\tIN [table] ADD/EDIT/DELETE/SHOW" << endl;
            return -1;
        }

        Table * table = findTableByName(name);

        if (table == nullptr) {
            cerr << "Таблица \"" << name << "\" не найдена" << endl;
            return -1;
        }

        command = command = command.substr(namePart.first + 1);
        command = Utils::trim(command);

        return table->execute(command);
    }

    else if (command == "") return -1;

    else {
        cerr << "ERROR: Unknown command \"" << firstPart.second << "\"" << endl;
    }

    return 1;
}

Table * Database::findTableByName (string name) {
    for (int i = 0; i < tables.size(); i++)
        if (tables[i]->getName() == name) return tables[i];

    return nullptr;
}
