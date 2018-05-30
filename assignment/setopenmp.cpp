#include <iostream>
#include <vector>
#include <set>
#include <time.h>
using namespace std;

int main(){
    set<int> mSet;
    int N =50000000;

    clock_t t1,t2;
    for(int i = 1; i< N; i++){
        mSet.insert(i+1);
    }
    int temp;
    t1 = clock();
    
    for (auto it = mSet.begin(); it!= mSet.end(); it++2){
        if(*it%2==0) temp+=2;
        else temp++;
    }

    t2= clock();
    double diff = ((double)t2 - (double)t1);
    double seconds = diff / CLOCKS_PER_SEC;
    cout << "Execution time: ";
    cout << seconds <<endl ;

}
