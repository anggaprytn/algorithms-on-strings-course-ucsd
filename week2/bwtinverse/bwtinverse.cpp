#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

string InverseBWT(const string& bwt) {
	int n = bwt.size();
	vector<string> table(n);

	// Initialize the table with empty strings
	for (int i = 0; i < n; ++i) {
		table[i] = "";
	}

	// Reconstruct the original text
	for (int i = 0; i < n; ++i) {
		// Prepend BWT to each row
		for (int j = 0; j < n; ++j) {
			table[j] = bwt[j] + table[j];
		}
		// Sort the table
		std::sort(table.begin(), table.end());
	}

	// Find the row that ends with the special end character '$'
	for (int i = 0; i < n; ++i) {
		if (table[i][n - 1] == '$') {
			return table[i];
		}
	}
	return "";
}

int main() {
	string bwt;
	cin >> bwt;
	cout << InverseBWT(bwt) << endl;
	return 0;
}
