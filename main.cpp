#include <iostream>
#include "database.h"
using namespace std;

int main(){
    Db * db = new Db;

    string command;
    int r = 1;

    while (r != 0){
        cout << ">>> ";
        getline(cin, command);

        r = db->execute(command);
    }
}
