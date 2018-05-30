/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: ASUS
 *
 * Created on April 12, 2018, 2:22 PM
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <algorithm>
using namespace std;

/*
 * 
 */
struct itemset
{
    vector<int> items;
    int count;
};

struct associationRule
{
    vector<int> left;
    vector<int> total;
    float confidence;
};

class Aripori
{
  private:
    unordered_map<string, int> supports;
    vector<itemset> records;
    vector<associationRule> associations;
    vector<itemset> largeItemset;
    vector<int> itemCategorize;
    double suportThreshold;
    double confidenceThreshold;
    int countThreshold = 1;

    bool isSubset(const vector<int> &v1, const vector<int> &v2);
    bool isCommonAtLeastTwoElement(const vector<int> &v1, const vector<int> &v2);
    vector<int> unionTwoVector(const vector<int> &v1, const vector<int> &v2);

    vector<itemset> generateVectorC(const vector<itemset> &vectL);

  public:
    Aripori(double suportThreshold, double confidenceThreshold);
    bool readFile(string path);
    void generateARule();

    void printTransaction();
    void printAssociationRule();
    void printLargeItemset();
    void printItem();
    void doAripori();

    int getSupport(vector<int> &transaction);
};

//Association rule generation
Aripori::Aripori(double suportThreshold, double confidenceThreshold)
{
    this->suportThreshold = suportThreshold;
    this->confidenceThreshold = confidenceThreshold;
}

/*
**  Function : Generate all association rule (confidence >= confident_threshold)
**  Input : Vector of Itemset
**  Output : void
**  All result save in this->associations
*/
void Aripori::generateARule()
{

    unordered_map<string, int>::const_iterator got;
    for (int i = 0; i < this->largeItemset.size(); i++)
    {
        itemset *tempItemset = &(this->largeItemset.at(i));
        int numItem = tempItemset->items.size();
        int sup1 = this->getSupport(tempItemset->items);

        for (int k = 1; k < numItem; k++) // Traverse on large itemset
        {
            int *combination = new int[k];
            for (int i = 0; i < k; i++)
                combination[i] = i;
            // Generate combination and check whether is association rule compatible
            while (combination[k - 1] < numItem)
            {
                // Implement generate association rule
                // Get key
                vector<int> transaction;
                for (int i = 0; i < k; i++)
                    transaction.push_back(tempItemset->items.at(combination[i]));

                //Get left-support
                int sup2 = this->getSupport(transaction);

                //Calculate confidence
                // cout << "Rule ";
                // for (int i = 0; i < transaction.size(); i++)
                //     cout << transaction.at(i) << " ";
                // cout  << "("<< (double)sup2 << " - " << (double)sup1 << ") | ";
                double confidence = (double)sup1 / (double)sup2;
                // cout << "Calculate Confidence : " << confidence << endl;
                if (confidence >= this->confidenceThreshold)
                { // Exist association rule
                    // cout<<"New Rule\n";
                    associationRule newRule;
                    newRule.confidence = confidence;
                    newRule.left = transaction;
                    newRule.total = tempItemset->items;
                    //Insert new association rule
                    this->associations.push_back(newRule);
                }

                //Create combination
                int t = k - 1;
                while (t != 0 && combination[t] == numItem - k + t)
                    t--; // Reach last element of ItemSet
                //Shift 1 all element from t
                combination[t]++;
                for (int i = t + 1; i < k; i++)
                    combination[i] = combination[i - 1] + 1;
            }
            delete combination;
        }
    }
};

void Aripori::printTransaction()
{
    cout << this->records.size() << "Transactions "
         << " : " << endl;
    for (int i = 0; i < this->records.size(); i++)
    {
        vector<int> *temp = &(this->records.at(i).items);
        for (int j = 0; j < temp->size(); j++)
        {
            cout << temp->at(j) << " ";
        }
        cout << " : " << this->records.at(i).count << endl;
    }
}
void Aripori::printLargeItemset()
{
    cout << this->largeItemset.size() << " Final large itemset : " << endl;
    for (int i = 0; i < this->largeItemset.size(); i++)
    {
        vector<int> *temp = &(this->largeItemset.at(i).items);
        for (int j = 0; j < temp->size(); j++)
        {
            cout << temp->at(j) << " ";
        }
        cout << endl;
    }
}

void Aripori::printItem()
{
    cout << this->itemCategorize.size() << "Item Cagetorize : "
         << " : " << endl;
    for (int i = 0; i < this->itemCategorize.size(); i++)
    {
        cout << this->itemCategorize.at(i) << " ";
    }
    cout << endl;
}

void Aripori::printAssociationRule()
{
    cout << "Generate " << this->associations.size() << " Association Rule(s) : \n";
    for (int i = 0; i < this->associations.size(); i++)
    {
        associationRule temp = this->associations.at(i);
        cout << "Left : ";
        for (int j = 0; j < temp.left.size(); j++)
        {
            cout << temp.left.at(j) << " ";
        }
        cout << "   | Total : ";
        for (int j = 0; j < temp.total.size(); j++)
        {
            cout << temp.total.at(j) << " ";
        }
        cout << "   | " << temp.confidence << endl;
    }
}

bool Aripori::readFile(string path)
{
    int numTransaction = 0;
    ifstream inFile;
    inFile.open(path.c_str());
    if (inFile)
    {
        cout << "File has been Open" << endl;
    }
    else
    {
        cout << "NO FILE HAS BEEN OPENED" << endl;
    }

    // Clear old data
    this->records.clear();
    this->itemCategorize.clear();

    string line;
    while (getline(inFile, line))
    {

        stringstream ss(line);
        string temp;
        vector<int> transaction;
        while (getline(ss, temp, ' '))
        {
            int newValue = atoi(temp.substr(1, temp.length() - 1).c_str());
            if (find(this->itemCategorize.begin(), this->itemCategorize.end(), newValue) == this->itemCategorize.end())
            {
                vector<int>::iterator it = upper_bound(this->itemCategorize.begin(), this->itemCategorize.end(), newValue);
                this->itemCategorize.insert(it, newValue);
            }
            transaction.push_back(newValue); //hard code
        }
        numTransaction++;

        bool isExist = false;
        bool isMatch;
        itemset *traverseItem;
        // cout<<this->records.size();
        // this->printTransaction();
        for (int i = 0; i < this->records.size(); i++)
        {

            traverseItem = &(this->records.at(i));
            if (traverseItem->items.size() == transaction.size())
            {
                isMatch = true;
                for (int j = 0; j < traverseItem->items.size(); j++)
                {
                    // cout<<transaction.at(j)<< "vs" <<traverseItem->items.at(j)<<endl;

                    if (transaction.at(j) != traverseItem->items.at(j))
                    {
                        // cout<< "New Element \n";
                        isMatch = false;

                        break;
                    }
                }
                if (isMatch)
                {
                    isExist = true;
                    traverseItem->count++;
                    // cout << " ~ " + traverseItem->count << endl;
                    break;
                }
            }
            if (isExist)
            {
                break;
            }
        }
        if (!isExist)
        {
            itemset newItemset;
            newItemset.items = transaction;
            // for (int i = 0; i < newItemset.items.size(); i++)
            // {
            //     cout << newItemset.items.at(i) << " ";
            // }
            // cout << " : 1" << endl;

            newItemset.count = 1;
            this->records.push_back(newItemset);
        }
    }

    this->countThreshold = (int)(this->suportThreshold * numTransaction) + 1;
    // for(int i=0; i<this->records.at(1).items.size(); i++) cout<<this->records.at(1).items.at(i) << " ";
    // cout << " : " << records.at(1).count<< endl;
    inFile.close();
    // this->printTransaction();
}

int Aripori::getSupport(vector<int> &transaction)
{
    int result = 0;
    string strKey = "";
    for (int i = 0; i < transaction.size(); i++)
    {
        strKey += to_string(transaction.at(i)) + "_";
    }
    // Get support
    if (supports.find(strKey) == supports.end())
    { // The key does not exist in map
        //Calculate support for strKey
        // cout <<"New Support\n";
        for (int i = 0; i < this->records.size(); i++)
        {
            itemset record = this->records.at(i);

            //Accumulate support
            if (includes(record.items.begin(), record.items.end(), transaction.begin(), transaction.end()))
            {
                result += record.count;
            }
        }
        if (result > 0)
            this->supports[strKey] = result;
    }
    else
    { // The key exists in map

        result = this->supports[strKey];
    }

    // cout<< strKey << " ~~~~~ " << endl;
    return result;
}

void Aripori::doAripori()
{
    int k = 0;
    int count = 0;
    vector<itemset> vectC;
    vector<itemset> vectL;
    vector<int> temp;
    //Init Vector C
    // itemset initItemset ;
    // initItemset.items = this->itemCategorize;
    // vectC.push_back(initItemset);

    cout << "Count Threshold : " << this->countThreshold << endl;
    for (int i = 0; i < this->itemCategorize.size(); i++)
    {
        itemset newItem;
        newItem.items.push_back(this->itemCategorize.at(i));
        vectC.push_back(newItem);
    }
    // cout<< "List of Item :\n";
    // for(int i=0; i< vectC.size(); i++){
    //     for(int j=0; j< vectC.at(i).items.size(); j++){
    //         cout<<vectC.at(i).items.at(j)<<", ";
    //     }
    // }
    // cout<<endl;
    k = 1;
    int countvalid = 0;
    do
    {
        //Choose itemset with support larger than threshold
        vectL.clear();
        for (int i = 0; i < vectC.size(); i++)
        {

            if (this->getSupport(vectC.at(i).items) >= this->countThreshold)
            {
                // cout<<++countvalid<< ") ";
                // for (int j = 0; j < vectC.at(i).items.size(); j++)
                // {
                //     cout << vectC.at(i).items.at(j) << ", ";
                // }
                // cout << " : " << this->getSupport(vectC.at(i).items) << endl;
                vectL.push_back(vectC.at(i));
                this->largeItemset.push_back(vectC.at(i));
            }
        }

        //Generate aripori
        vectC.clear();
        temp.clear();
        bool flag;
        for (int i = 0; i < vectL.size(); i++)
        {
            for (int j = i + 1; j < vectL.size(); j++)
            {
                if (this->isCommonAtLeastTwoElement(vectL.at(i).items, vectL.at(j).items))
                {
                    //                vectC.push_back(unionTwoVector(vectL.at(i),vectL.at(j)));
                    temp = this->unionTwoVector(vectL.at(i).items, vectL.at(j).items);
                    flag = true;
                    for (int k = 0; k < vectC.size(); k++)
                    {
                        if (equal(vectC.at(k).items.begin(), vectC.at(k).items.end(), temp.begin()))
                        {
                            flag = false;
                            break;
                        }
                    }
                    if (flag == true)
                    {
                        itemset newItem;
                        newItem.items = temp;
                        vectC.push_back(newItem);
                    }
                    else
                    {
                        temp.clear();
                    }
                }
            }
        }
    } while (vectC.size() > 0);
}

vector<itemset> Aripori::generateVectorC(const vector<itemset> &vectL)
{
    vector<itemset> vectC;
    vector<int> temp;
    bool flag;
    for (int i = 0; i < vectL.size(); i++)
    {
        for (int j = i + 1; j < vectL.size(); j++)
        {
            if (this->isCommonAtLeastTwoElement(vectL.at(i).items, vectL.at(j).items))
            {
                //                vectC.push_back(unionTwoVector(vectL.at(i),vectL.at(j)));
                temp = unionTwoVector(vectL.at(i).items, vectL.at(j).items);
                flag = true;
                for (int k = 0; k < vectC.size(); k++)
                {
                    if (equal(vectC.at(k).items.begin(), vectC.at(k).items.end(), temp.begin()))
                    {
                        flag = false;
                        break;
                    }
                }
                if (flag == true)
                {
                    itemset newItem;
                    newItem.items = temp;
                    vectC.push_back(newItem);
                }
                else
                {
                    temp.clear();
                }
            }
        }
    }
    return vectC;
}

bool Aripori::isCommonAtLeastTwoElement(const vector<int> &v1, const vector<int> &v2)
{
    vector<int> vi;
    set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(vi));
    if ((int)vi.size() >= (int)v1.size() - 2)
    {
        return true;
    }
    return false;
}

vector<int> Aripori::unionTwoVector(const vector<int> &v1, const vector<int> &v2)
{
    vector<int> vu;
    set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(vu));
    return vu;
}
bool isSubset(const vector<int> &A, const vector<int> &B)
{ //check B is a sub of A
    return includes(A.begin(), A.end(), B.begin(), B.end());
}

int main(int argc, char **argv)
{
    string nameFile = argv[1];
    cout << nameFile << endl;
    Aripori *aripori = new Aripori(0.01, 0.01);
    aripori->readFile(nameFile);

    int arr[] = {1, 2};
    vector<int> a(arr, arr + 2);
    itemset item;
    item.items = a;
    // cout<<"\n Test : "<<aripori->getSupport(a)<< endl;

    aripori->doAripori();

    // cout<<"\n Test : "<<aripori->getSupport(a);
    aripori->printLargeItemset();
    aripori->generateARule();

    aripori->printAssociationRule();

    // aripori->printTransaction();

    return 0;
}
