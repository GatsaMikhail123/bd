#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>

#include "table.h"

using namespace std;

class Db
{
private:
    vector <Table *> tables;

    Table * findTableByName (string name);
public:
    Db();
    int execute (string command);
};

#endif // DATABASE_H
