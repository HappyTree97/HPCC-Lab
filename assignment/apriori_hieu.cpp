#include <omp.h>
#include <ctime>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <tuple>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;
class ReadInput {
private:
	ifstream fileInput;
	vector<vector<int>> transactions;
public:
	ReadInput(string fileName) {
		fileInput.open(fileName);
		if (!fileInput) {
			cout << "Can not open file input \n";
			exit(0);
		}
		parse();
	}
	void parse() {
		string str;
		while (!getline(fileInput, str).eof()) {
			vector<int> arr;
			int count = 0;
			for (int i = 0; i < str.size(); i++) {
				if (str[i] == '\t') {
					int number = atoi(str.substr(count, i).c_str());
					arr.push_back(number);
					count = i + 1;
				}
			}
			int number = atoi(str.substr(count, str.size()).c_str());
			arr.push_back(number);

			transactions.push_back(arr);
		}
	}
	vector<vector<int>> getTransactions() {
		return transactions;
	}
};
class AssocaitionRules {
private:
	
	long double minSup;
	int stepCount;
	vector<vector<int>> transactions;
	vector<vector<int>> L , C;
	vector<vector<vector<int>>> frequentSets;
	vector<tuple<vector<int>, vector<int>, long double, long double>> associationRules;
public:
	AssocaitionRules(vector<vector<int>> _transactions, long double _minSup) {
		
		stepCount = 0;
		minSup = _minSup;
		for (auto&row : _transactions) {
			sort(row.begin(), row.end());
			transactions.push_back(row);
		}
		frequentSets.push_back({ {} });
	}

	vector<tuple<vector<int>, vector<int>, long double, long double>> getAssociationRules() {
		return associationRules;
	}

	void generateAssociationRule(vector<int> items, vector<int> X, vector<int> Y, int index) {
		if (index == items.size()) {
			if (X.size() == 0 || Y.size() == 0) return;
			long double XYsupport = calculateSupport(items);
			long double Xsupport = calculateSupport(X);

			if (Xsupport == 0) return;

			long double support = (long double)XYsupport;
			long double confidence = (long double)XYsupport / Xsupport*100.0;
			associationRules.push_back({ X, Y, support, confidence });
			return;
		}

		X.push_back(items[index]);
		generateAssociationRule(items, X, Y, index + 1);
		X.pop_back();
		Y.push_back(items[index]);
		generateAssociationRule(items, X, Y, index + 1);
	}

	vector<int> calculateElement(vector<vector<int>> itemset) {
		vector<int> element;
		set<int> s;
		for (auto&row : itemset) for (auto&col : row) s.insert(col);
		for (auto iter = s.begin(); iter != s.end(); iter++) element.push_back(*iter);
		return element;
	}

	vector<vector<int>> generate() {
		vector<vector<int>> ret;
		for (int i = 0; i < L.size(); i++) {
			for (int j = i + 1; j < L.size(); j++) {
				int k;
				for (k = 0; k < stepCount - 1; k++) {
					if (L[i][k] != L[j][k]) break;
				}
				if (k == stepCount - 1) {
					vector<int> tmp;
					for (int k = 0; k < stepCount - 1; k++) {
						tmp.push_back(L[i][k]);
					}
					int a = L[i][stepCount - 1];
					int b = L[j][stepCount - 1];
					if (a > b) swap(a, b);
					tmp.push_back(a), tmp.push_back(b);
					ret.push_back(tmp);
				}
			}
		}
		return ret;
	}

	vector<vector<int>> delete_generated(vector<vector<int>> joined) {
		vector<vector<int>> ret;

		set<vector<int>> lSet;
		for (auto&row : L) lSet.insert(row);

		for (auto&row : joined) {
			int i;
			for (i = 0; i < row.size(); i++) {
				vector<int> tmp = row;
				tmp.erase(tmp.begin() + i);
				if (lSet.find(tmp) == lSet.end()) {
					break;
				}
			}
			if (i == row.size()) {
				ret.push_back(row);
			}
		}
		return ret;
	}

	long double calculateSupport(vector<int> item) {
		int ret = 0;
		for (auto&row : transactions) {
			int i, j;
			if (row.size() < item.size()) continue;
			for (i = 0, j = 0; i < row.size(); i++) {
				if (j == item.size()) break;
				if (row[i] == item[j]) j++;
			}
			if (j == item.size()) {
				ret++;
			}
		}
		return (long double)ret / transactions.size()*100.0;
	}

	long double round(long double value, int pos) {
		long double temp;
		temp = value * pow(10, pos);
		temp = floor(temp + 0.5);
		temp *= pow(10, -pos);
		return temp;
	}

	vector<vector<int>> calculateC() {
		if (stepCount == 0) {
			vector<vector<int>> ret;
			vector<int> element = calculateElement(transactions);
			for (auto&i : element) ret.push_back(vector<int>(1, i));

			return ret;
		}
		else {
			return delete_generated(generate());
		}
	}

	vector<vector<int>> calculateL() {
		vector<vector<int>> ret;
		for (auto&row : C) {
			long double support = calculateSupport(row);
			if (round(support, 2) < minSup) continue;
			ret.push_back(row);
		}
		return ret;
	}

	void build() {
		while (true) {
			C = calculateC();
			if (C.size() == 0) break;
			stepCount++;

			L = calculateL();
			frequentSets.push_back(L);
		}

		for (auto&stepItemSet : frequentSets) {
			for (auto&items : stepItemSet) {
				generateAssociationRule(items, {}, {}, 0);
			}
		}
	}
};



class ReadOutput {
private:
	ofstream fileOutput;
	vector<tuple<vector<int>, vector<int>, long double, long double>> associationRules;
public:
	ReadOutput(string fileName, vector<tuple<vector<int>, vector<int>, long double, long double> > _associationRules) {
		fileOutput.open(fileName);
		if (!fileOutput) {
			cout << "Can not open file output\n";
			exit(0);
		}
		associationRules = _associationRules;
		buildOutput();
	}

	void buildOutput() {
		fileOutput << "[Item sets]\t[Associative item sets]\t[Supports(%)]\t[Confidence(%)]" << '\n';
		fileOutput << "-----------------------------------------------------------------------" << '\n';
		for (auto&i : associationRules) {
			fileOutput << vectorToString(get<0>(i)) << '\t' << '\t';
			fileOutput << vectorToString(get<1>(i)) << '\t' << '\t' << '\t';

			fileOutput << fixed;
			fileOutput.precision(2);
			fileOutput << get<2>(i) << '\t' << '\t';
			fileOutput << fixed;
			fileOutput.precision(2);
			fileOutput << get<3>(i);

			fileOutput << endl;
		}
	}

	string vectorToString(vector<int> arr) {
		string ret = "[";
		for (int i = 0; i<arr.size(); i++) {
			ret += to_string(arr[i]);
			if (i != arr.size() - 1) {
				ret += ",";
			}
		}
		ret += "]";
		return ret;
	}
};

int main(int argc, char ** argv) {
	if (argc != 4) {
		cout << endl;
		cout << "       namefile [min support(%)] [input file name] [output file name]" << endl;
		return 0;
	}
	clock_t start;
    double duration;
	start = clock();

	int nthreads, tid;
	string minSup(argv[1]);
	string inputName(argv[2]);
	string outputName(argv[3]);
	ReadInput readInput(inputName);
	vector<vector<int>> transactions = readInput.getTransactions();
	
	#pragma omp parallel private(nthreads, tid)
	 {

	 
  	// Obtain thread number 
	 tid = omp_get_thread_num();
	  printf("Creating  thread :  = %d\n", tid);
	  
		AssocaitionRules assocaitionRules(transactions, stold(minSup));
		
			
	
	  // Only master thread does this 
	  if(tid == 0)  {
   		 nthreads = omp_get_num_threads();
		 
		 assocaitionRules.build();
		 ReadOutput outputPrinter(outputName, assocaitionRules.getAssociationRules());
	    printf("Number of threads = %d\n", nthreads);
	    }

 	 }   //All threads join master thread and disband 
	
	duration = ( clock() - start ) / (float) CLOCKS_PER_SEC;
    cout << "Run time :  "<< duration<<"(s)"<<endl;
	

	return 0;
}
