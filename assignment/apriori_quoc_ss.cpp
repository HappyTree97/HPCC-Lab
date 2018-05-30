#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <map>
#include <time.h>
#include <omp.h>
using namespace std;


struct largeItemSet {
    map<set<int>,int> count;
    int size;
};

struct rule{
    set<int> left;
    set<int> right;
};

class Apriori {
    private:
        // largeItemSet counter;
        rule rules;
        double suportThreshold;
        double confidenceThreshold;
        int supportCount;
        vector<set<int>> transactions;
        vector<largeItemSet> listL;
        
        largeItemSet init();
        largeItemSet generateCandidates(largeItemSet &preL);
        bool isSubSet(set<int> parent,set<int> child);
        void keepFrequentCandidates(largeItemSet &C);
        void insert(vector<int> &vect, int value);
        set<int> convertVectorToSet(const vector<int> & vect);
        bool find (const vector<int> &vect, int value);
        bool find (const vector<vector<int>> &vect, vector<int> value);
    public:
        Apriori(double suportThreshold, double confidenceThreshold);
        void readFile(string path);
        void doApriori();
        void generateARule();
        void printListL();
        
};

Apriori::Apriori(double suportThreshold, double confidenceThreshold)
{
    this->suportThreshold = suportThreshold;
    this->confidenceThreshold = confidenceThreshold;
}

void Apriori::readFile(string path){
    cout<<path.c_str()<<endl;
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
        set<int> transaction;
        string temp;
        //for csv
        // while (getline(ss, temp, ',')) {
        //     if (temp[0] == ' ') {
        //         transaction.insert(atoi(temp.c_str()));
        //     }
        // }
        // for txt
        while (getline(ss, temp, ' ')) {
            transaction.insert(atoi(temp.c_str()));
        }
        transactions.push_back(transaction);
        numTransaction++;
    }
    inFile.close();
    this->supportCount = round(numTransaction*this->suportThreshold); 
}

largeItemSet Apriori::init(){
    largeItemSet C1;
    C1.size = 1;
    set<int> itemset;
    for(int i=0;i < transactions.size(); i++){
        for(auto itr = transactions[i].begin(); itr != transactions[i].end(); ++itr){
            itemset.insert(*itr);
            if(C1.count.count(itemset) == 0){
                C1.count.insert(pair<set<int>,int>(itemset,1));
            }
            else{
                C1.count[itemset]++;
            }
            itemset.clear();
        }
    }
    // cout<<"C1"<<endl;
    // for(auto it1 = C1.count.begin();it1 != C1.count.end();++it1){
    //     set<int> mySet = it1->first;
    //     for(set<int>::iterator it2 = mySet.begin();it2 != mySet.end(); ++it2){
    //         cout << *(it2) << " " << it1->second << endl;
    //     }
    // }
    
    largeItemSet L1;
    L1.size = 1;
    int count;
    for(auto i = C1.count.begin(); i!= C1.count.end(); i++){
        itemset = i->first;
        count = i->second;
        if(count >= supportCount){
            L1.count.insert(pair<set<int>,int>(itemset,count));
        }
    }

    C1.count.clear();
    this->listL.push_back(L1);
    // cout<<"L1"<<endl;
    // cout<<this->supportCount<<endl;
    // for(auto it1 = L1.count.begin();it1 != L1.count.end();++it1){
    //     set<int> mySet = it1->first;
    //     for(set<int>::iterator it2 = mySet.begin();it2 != mySet.end(); ++it2){
    //         cout << *(it2) << " " << it1->second << endl;
    //     }
    // }
    return L1;
}


typedef set<set<int>> newclist;
newclist add_newc(newclist out, newclist in){
    out.insert(in.begin(),in.end());
    return out;
}

#pragma omp declare reduction(add: newclist: omp_out=add_newc(omp_out,omp_in))



largeItemSet Apriori::generateCandidates(largeItemSet &preL){
    largeItemSet newC;
    newC.size = preL.size + 1;

    set<int> list_item;
    vector<int> list_item_vec;
    vector<int> itemset_vec;

    set<int> itemset;
    vector<int> temp;
    int item;
    bool to_gennerate;

    vector<vector<int>> preLvec;
    set<vector<int>> newCset;


    for(auto i = preL.count.begin(); i != preL.count.end(); ++i){
        itemset = i->first;    
        list_item.insert(itemset.begin(), itemset.end()); 
        vector<int >itemsetVec   ;
        for(auto j = itemset.begin();j!= itemset.end(); j++){
            itemsetVec.push_back(*j);
        }
        preLvec.push_back(itemsetVec);
    }

    for(auto i = list_item.begin(); i != list_item.end(); ++i){
        list_item_vec.push_back(*i);
    }
    

    // #pragma omp parallel private(itemset,temp,to_gennerate,item) reduction(add: newCset)
    
    double t1 = omp_get_wtime();
    // int num_thread = omp_get_num_threads(); 
    // int thread_id = omp_get_thread_num();
    #pragma omp parallel for  private(itemset_vec, item, to_gennerate, temp )
    for(int i=0; i<preLvec.size(); i++){
        itemset_vec = preLvec[i];
        for(int j = 0; j <list_item_vec.size(); ++j){
            item = list_item_vec[j];
            if(!find(itemset_vec, item) ){
                insert(itemset_vec, item);
                to_gennerate = true;
                if(newCset.count(itemset_vec)==0){
                    temp = itemset_vec;
                    for(int k = 0; k != itemset_vec.size() ; ++k){
                        temp.erase(temp.begin()+k);
                        if(!find(preLvec, temp)){
                            to_gennerate = false;
                            break;
                        }
                        insert(temp, itemset_vec[k]);
                    }
                }
                if(to_gennerate){
                    {
                        #pragma omp critical
                        {
                            newCset.insert(itemset_vec);
                        }
                        
                    }   
                }
                itemset.erase(item);
            }
        }
    }

    for(auto i = newCset.begin(); i!=newCset.end();i++ ){
        newC.count.insert(pair<set<int>,int>(convertVectorToSet(*i),0));
    } 
    return newC;
}

void Apriori::insert(vector<int> &vect, int value) {
    vector<int>::iterator it = upper_bound(vect.begin(), vect.end(), value);
    vect.insert(it, value);
}
set<int> Apriori::convertVectorToSet(const vector<int> & vect){
    set<int> result;
    for(int i = 0; i<vect.size(); i++){
        result.insert(vect[i]);
    }
    return result;
}
bool Apriori::find (const vector<int> &vect, int value){
    for(int i=0; i<vect.size(); i++){
        if(vect[i] == value) return true;
    }
    return false;
}
bool Apriori::find (const vector<vector<int>> &vect, vector<int> value){
    for(auto i=0; i< vect.size(); i++){
        if(vect[i].size()== value.size()){
            bool isEqual = true;
            for(int j=0 ; j< vect[i].size(); j++){
                if(vect[i][j] != value[j] ) isEqual=false;
            }
            if(isEqual) return true;
        }
    }
    return false;
}
bool Apriori::isSubSet(set<int> parent,set<int> child){
    for(set<int>::iterator it = child.begin(); it!= child.end();++it){
        if(parent.find(*it) == parent.end()){
            return false;
        }
    }
    return true;
}

// void Apriori::keepFrequentCandidates(largeItemSet &C){
//     for(auto it = C.count.begin(); it != C.count.end(); ++it){
//         set<int> itemset = it->first;
//         int count = 0;
//         bool flag = false;
//         for(int i =0; i<transactions.size(); i++){//sua
//             if(isSubSet(transactions[i], itemset)){
//                 count++;
//             }
//             if(count >= this->supportCount){//trinh sua lai
//                 flag = true;
//             }
//         }
//         //cout << count << endl;
//         if(flag == true){
//             it->second = count;
//         }
//         else{
//             C.count.erase(itemset);
//         }
//     }
//     listL.push_back(C);
// }

int combinations(int d, int k){

    if(k==1){
        return d;
    }
    if(k==d){
        return 1;
    }

    if(k > d){
        return 0;
    }
    return combinations(d-1,k) + combinations(d-1,k-1);
}


// find subsets of one transaction (transaction is stored in sset) with size "size" and increment their counters in candidate itemsets "c" if exist.
void subset(set<int> &sset, int size, int left_size, set<int>::iterator index, set<int> &v, largeItemSet &c){
    if(left_size==0){
        if(c.count.count(v)>0){
            c.count[v] +=1;
        }
        return;
    }
    for(set<int>::iterator it=index; it!=sset.end(); ++it){
        v.insert(*it);
        subset(sset,size,left_size-1,++index,v,c);
        v.erase(*it);
    }
    return;}


// Go to each tracsaction and find subsets of size k and increment their counter in c. This algorithm is implemented in function subset().
// If the number of candidates in c is less than the number of subsets of size k of one trasaction, then loop over each candidate itemset and check whether it is inside the transaction or not, then increment its counter if exists.
void Apriori::keepFrequentCandidates(largeItemSet &c){

    bool to_count;

    set<int> temp_set;
    set<int> t_set;
    set<int> help_set;

    // when all transactions are in main memory.
    // vector<set<int>> transactions = this->transactions;
    // int i;


#pragma omp parallel private(t_set,temp_set,to_count,help_set)
{
    int num_thread = omp_get_num_threads(); 
    int thread_id = omp_get_thread_num(); 
    
    for (int i=thread_id; i<transactions.size(); i+=num_thread){
        t_set=transactions[i];
        
        if(t_set.size() >= c.size){
            if(c.count.size() < combinations(t_set.size(),c.size)){
            
                for (auto it = c.count.begin(); it!= c.count.end(); ++it){
                    temp_set=it->first;
                    to_count=true;        
                    for (auto itt = temp_set.begin(); itt != temp_set.end(); ++itt){
                        if(t_set.count(*itt)==0){
                            to_count=false;
                            break;
                        }
                    }

                    if(to_count){
                        c.count[temp_set] +=1;
                    }
                }
                
            }
            else{
                subset(t_set,t_set.size(),c.size,t_set.begin(),help_set,c);
            }
        }
    }
}

    largeItemSet l_new;
    l_new.size = c.size;

    // Copy frequent items into l_new.
    int count;
    for (auto it = c.count.begin(); it != c.count.end(); ++it){
        help_set=it->first;
        count=it->second;
        if (count >= supportCount){
            l_new.count.insert(pair<set<int>,int>(help_set,count));
        }
    }

    c.count.clear();

    listL.push_back(l_new);

    return;
}



void Apriori::doApriori(){
    largeItemSet newC;
    largeItemSet preL;
    double t1,t2;
    t1 = omp_get_wtime();
    preL = init();
    cout<<"Itemset "<<preL.size<<": "<< preL.count.size() <<" - "<<omp_get_wtime()-t1<<endl;
    while (preL.count.size() > 0){
        t1 = omp_get_wtime();
        newC = generateCandidates(preL);
        t2 = omp_get_wtime();
        keepFrequentCandidates(newC);
        preL = listL.back();
        cout<<"Itemset "<<preL.size<<": "<< preL.count.size() <<" - "<<"genC:"<<t2-t1<<" - keepC :"<<omp_get_wtime()-t2<<endl;
    }
    listL.pop_back();
}

void Apriori::printListL(){
    for(int i=0; i<listL.size(); i++){        
        cout<<"L_"<<listL[i].size<<": "<<listL[i].count.size()<<endl;
    }
}
int main(int argc, char **argv){
    clock_t t1,t2,t3;
    string path = argv[1];
    double sp  = atof(argv[2]);
    double cf  = atof(argv[3]);
    Apriori myAripori(sp, cf);
    t3=omp_get_wtime();
    myAripori.readFile(path);
    t1=omp_get_wtime();
    cout<<"readFile: "<<t1-t3<<"s"<<endl;
    t1=omp_get_wtime();
    myAripori.doApriori();
    t2=omp_get_wtime();
    
    myAripori.printListL();



    cout<<"doApriori: "<<t2-t1<<"s"<<endl;  
}



