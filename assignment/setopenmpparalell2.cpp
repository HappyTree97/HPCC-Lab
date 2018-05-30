#include <iostream>
#include <vector>
#include <set>
#include <time.h>
#include <omp.h>
using namespace std;

int main(){
    set<int> mSet;
    int N =50000000;

    clock_t t1,t2;
    for(int i = 1; i< N; i++){
        mSet.insert(i+1);
    }
    int temp;
    double start = omp_get_wtime( );  
    cout<<"Start :" << start << ", ";
    #pragma omp parallel for     
    for (auto it = mSet.begin(); it!= mSet.end(); it++){
        if(temp%2==0) temp+=2;
        else temp++;
    }

    double end = omp_get_wtime( );  
cout<< "End : " << end <<endl;
    double diff = (end- start);
    double seconds = diff ;
    cout << "Execution time: ";
    cout << seconds <<endl;

}
