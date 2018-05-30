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
using namespace std;

struct largeItemSet
{
    map<set<int>, int> count;
    int size;
};

struct rule
{
    set<int> left;
    set<int> right;
};

class Apriori
{
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
    bool isSubSet(set<int> parent, set<int> child);
    void keepFrequentCandidates(largeItemSet &maybeC);
    void countSubsetSupport(set<int> &transaction, int transactionSize, int subsetSize, set<int>::iterator index, set<int> &helper, largeItemSet &maybeC);
    int generateRuleSubset(const set<int> &sset, int left_size, set<int>::iterator index, set<int> &result, int count);

  public:
    Apriori(double suportThreshold, double confidenceThreshold);
    void readFile(string path);
    void doApriori();
    void generateStrongRule();
    void printListL();
};

Apriori::Apriori(double suportThreshold, double confidenceThreshold)
{
    this->suportThreshold = suportThreshold;
    this->confidenceThreshold = confidenceThreshold;
}

void Apriori::readFile(string path)
{
    int numTransaction = 0;
    ifstream inFile;
    inFile.open(path.c_str());
    if (inFile.is_open())
    {
        cout << "File has been open" << endl;
    }
    else
    {
        cout << "No file has been open" << endl;
    }
    string line;
    while (getline(inFile, line))
    {
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
        while (getline(ss, temp, ' '))
        {
            transaction.insert(atoi(temp.c_str()));
        }
        transactions.push_back(transaction);
        numTransaction++;
    }
    inFile.close();
    this->supportCount = round(numTransaction * this->suportThreshold);
}

largeItemSet Apriori::init()
{
    largeItemSet C1;
    C1.size = 1;
    set<int> itemset;
    for (int i = 0; i < transactions.size(); i++)
    {
        for (auto itr = transactions[i].begin(); itr != transactions[i].end(); ++itr)
        {
            itemset.insert(*itr);
            if (C1.count.count(itemset) == 0)
            {
                C1.count.insert(pair<set<int>, int>(itemset, 1));
            }
            else
            {
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
    for (auto i = C1.count.begin(); i != C1.count.end(); i++)
    {
        itemset = i->first;
        count = i->second;
        if (count >= supportCount)
        {
            L1.count.insert(pair<set<int>, int>(itemset, count));
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

largeItemSet Apriori::generateCandidates(largeItemSet &preL)
{
    largeItemSet newC;
    newC.size = preL.size + 1;

    set<int> list_item;
    set<int> itemset;
    set<int> temp;
    int item;
    bool to_gennerate;

    for (auto i = preL.count.begin(); i != preL.count.end(); ++i)
    {
        itemset = i->first;
        list_item.insert(itemset.begin(), itemset.end());
    }

    for (auto i = preL.count.begin(); i != preL.count.end(); ++i)
    {
        itemset = i->first;
        for (auto j = list_item.begin(); j != list_item.end(); ++j)
        {
            item = *j;
            if (itemset.count(item) == 0)
            {
                itemset.insert(item);
                to_gennerate = true;
                if (newC.count.count(itemset) == 0)
                {
                    temp = itemset;
                    for (auto k = itemset.begin(); k != itemset.end(); ++k)
                    {
                        temp.erase(*k);
                        if (preL.count.count(temp) == 0)
                        {
                            to_gennerate = false;
                            break;
                        }
                        temp.insert(*k);
                    }
                }
                if (to_gennerate)
                {
                    newC.count.insert(pair<set<int>, int>(itemset, 0));
                }
                itemset.erase(item);
            }
        }
    }
    return newC;
}

bool Apriori::isSubSet(set<int> parent, set<int> child)
{
    for (set<int>::iterator it = child.begin(); it != child.end(); ++it)
    {
        if (parent.find(*it) == parent.end())
        {
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

int countCommbinations(int d, int k)
{

    if (k == 1)
    {
        return d;
    }
    if (k == d)
    {
        return 1;
    }

    if (k > d)
    {
        return 0;
    }
    return countCommbinations(d - 1, k) + countCommbinations(d - 1, k - 1);
}

// find subsets of one transaction (transaction is stored in sset) with size "size" and increment their counters in candidate itemsets "maybeC" if exist.
void Apriori::countSubsetSupport(set<int> &transaction, int transactionSize, int subsetSize, set<int>::iterator index, set<int> &helper, largeItemSet &maybeC)
{
    if (transactionSize == 0)
    {
        if (maybeC.count.count(helper) > 0)
        {
            maybeC.count[helper] += 1;
        }
        return;
    }
    for (set<int>::iterator it = index; it != transaction.end(); ++it)
    {
        helper.insert(*it);
        countSubsetSupport(transaction, transactionSize, subsetSize - 1, ++index, helper, maybeC);
        helper.erase(*it);
    }
    return;
}

// Go to each tracsaction and find subsets of size k and increment their counter in c. This algorithm is implemented in function subset().
// If the number of candidates in c is less than the number of subsets of size k of one trasaction, then loop over each candidate itemset and check whether it is inside the transaction or not, then increment its counter if exists.
void Apriori::keepFrequentCandidates(largeItemSet &maybeC)
{

    int temp_count;
    bool to_count;

    set<int> temp_set;
    set<int> t_set;
    set<int> help_set;

    // when all transactions are in main memory.
    for (int i = 0; i < transactions.size(); i++)
    {
        t_set = transactions[i];

        if (t_set.size() >= maybeC.size)
        {
            if (maybeC.count.size() < countCommbinations(t_set.size(), maybeC.size))
            {

                for (auto it = maybeC.count.begin(); it != maybeC.count.end(); ++it)
                {
                    temp_set = it->first;

                    to_count = true;

                    for (auto itt = temp_set.begin(); itt != temp_set.end(); ++itt)
                    {
                        if (t_set.count(*itt) == 0)
                        {
                            to_count = false;
                            break;
                        }
                    }

                    if (to_count)
                    {
                        maybeC.count[temp_set] += 1;
                    }
                }
            }
            else
            {
                countSubsetSupport(t_set, t_set.size(), maybeC.size, t_set.begin(), help_set, maybeC);
            }
        }
    }

    largeItemSet l_new;
    l_new.size = maybeC.size;

    // Copy frequent items into l_new.
    int count;
    for (auto it = maybeC.count.begin(); it != maybeC.count.end(); ++it)
    {
        help_set = it->first;
        count = it->second;
        if (count >= supportCount)
        {
            l_new.count.insert(pair<set<int>, int>(help_set, count));
        }
    }

    maybeC.count.clear();

    listL.push_back(l_new);

    return;
}

void Apriori::doApriori()
{
    largeItemSet newC;
    largeItemSet preL;
    preL = init();
    while (preL.count.size() > 0)
    {
        newC = generateCandidates(preL);
        keepFrequentCandidates(newC);
        preL = listL.back();
    }
    listL.pop_back();
}

void Apriori::generateStrongRule(){
    int ithItemset = 0;
    for (auto largeItemsetIt = this->listL.begin(); largeItemsetIt != this->listL.end(); largeItemsetIt++)
    {
        int count =0;
        ++ ithItemset;
        // Sinh Strong Association rule cho largeitem set thứ i
        for (auto itemIt = largeItemsetIt->count.begin(); itemIt != largeItemsetIt->count.end(); itemIt++)
        {
            for (int subsetSize = 1; subsetSize < itemIt->first.size(); subsetSize++)
            {
                set<int> result;
                count += generateRuleSubset(itemIt->first, subsetSize, itemIt->first.begin(), result, itemIt->second);
            }
            
        }
        cout<< "Number rule of " << ithItemset << " : " << count <<endl;
    }
}

int Apriori::generateRuleSubset(const set<int> &sset, int left_size, set<int>::iterator index, set<int> &result, int count)
{
    // Sinh ra được một tổ hợp mới
    if (left_size == 0)
    {
        int leftSupport = listL[result.size()-1].count[result];
        double conf = count/ leftSupport;
        if (conf >= this->confidenceThreshold)
        {
            rule newRule;
            newRule.left = result;
            for (auto it = sset.begin(); it != sset.end(); it++)
            {
                if (newRule.left.find(*it) == newRule.left.end())
                {
                    newRule.right.insert(*it);
                }
            }
        }
        return 1;
    }
    int countSubsetRule=0;
    for (set<int>::iterator it = index; it != sset.end(); ++it)
    {
        result.insert(*it);
        countSubsetRule += generateRuleSubset(sset, left_size - 1, ++index, result, count);
        result.erase(*it);
    }
    return countSubsetRule;
}


void Apriori::printListL()
{
    cout<<"Number of large itemset : " << listL.size() <<endl;
    for (int i = 0; i < listL.size(); i++)
    {
        cout << "L_" << listL[i].size << ": " << listL[i].count.size() << endl;
    }
}
int main(int argc, char **argv)
{
    clock_t t1, t2;
    string path = argv[1];
    double sp = atof(argv[2]);
    double cf = atof(argv[3]);
    Apriori myAripori(sp, cf);
    myAripori.readFile(path);
    t1 = clock();
    myAripori.doApriori();
//    myAripori.generateStrongRule();
    t2 = clock();

    myAripori.printListL();

    double diff = ((double)t2 - (double)t1);
    double seconds = diff / CLOCKS_PER_SEC;
    cout << "Execution time: ";
    cout << round(seconds);
    cout << " seconds" << endl;
}
