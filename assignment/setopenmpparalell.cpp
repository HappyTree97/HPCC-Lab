#include <iostream>
#include <vector>
#include <set>
#include <time.h>
#include <omp.h>
using namespace std;

int main(){
    vector<int> mSet;
    int N =50000000;

    clock_t t1,t2;
    for(int i = 1; i< N; i++){
        mSet.push_back(i+1);
    }
    int temp;
    vector<int>::iterator it;
    double start = omp_get_wtime( );  
    cout<<"Start :" << start << "\n";

    #pragma omp parallel for
    for (it = mSet.begin(); it!= mSet.end(); it++){
	 

        if(*it%2==0) temp+=(*it) + 2;
        else temp++;
    }

    double end = omp_get_wtime( );  
    cout<< "End : " << end <<endl;
    double diff = (end- start);
    double seconds = diff ;
    cout << "Execution time: ";
    cout << seconds <<endl;

}
