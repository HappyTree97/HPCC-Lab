#include <iostream>
#include <vector>
#include <set>
#include <time.h>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

map<vector<int>, int> transactions;
int readFile(string path);


int readFile(string path){
    int numTransaction = 0;
    ifstream inFile;
    inFile.open(path.c_str());
    if (inFile.is_open()) {
        cout << "File has been open" << endl;
    } else {
        cout << "No file has been open" << endl;
    }  
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        vector<int> transaction;
        string temp;
        //for csv
        // while (getline(ss, temp, ',')) {
        //     if (temp[0] == ' ') {
        //         transaction.insert(atoi(temp.c_str()));
        //     }
        // }
        // for txt
        while (getline(ss, temp, ' ')) {
            transaction.push_back(atoi(temp.c_str()));
        }
        if(transactions.find(transaction) == transactions.end()){
            transactions.insert(pair<vector<int>, int>(transaction, 1));
        }
        else{
            transactions[transaction] ++;
        }
        
        numTransaction++;
    }
    inFile.close();
}

int main(int argc, char **argv){
    map<vector<int>, int>::iterator it;
    clock_t t1,t2;
    map<int, int> frequent ;
    string path = argv[1];
    readFile(path);
    t1=clock();
    for(it = transactions.begin(); it!=transactions.end(); it++){
        if(frequent.find(it->second) == frequent.end()){
            frequent[it->second] = 1;
        }else{
            frequent[it->second] ++;
        }
    }
    t2=clock();


    for(auto it = frequent.begin(); it!=frequent.end(); it++){
        cout << "Frequent : "<< it->first << " appear " << it->second <<" items\n";
    }
    double diff=((double)t2-(double)t1);
    double seconds = diff / CLOCKS_PER_SEC;
    cout<<"Execution time: ";
    cout<<seconds;
    cout<<" seconds"<<endl;
    



    

}

