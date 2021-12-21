#include <iostream>
#include "database.h"
using namespace std;

int main(){
    Database * db = new Database;

    string command;
    int retval = 1;

    while (retval != 0){
        cout << ">>> ";
        getline(cin, command);

        retval = db->execute(command);
    }
}
