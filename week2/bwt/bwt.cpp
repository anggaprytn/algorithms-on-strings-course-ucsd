#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::cin;
using std::istringstream;
using std::map;
using std::string;
using std::vector;

// Preprocess the Burrows-Wheeler Transform bwt of some text
void PreprocessBWT(const string& bwt, 
                   map<char, int>& starts, 
                   map<char, vector<int>>& occ_count_before) {
	int n = bwt.size();
	map<char, int> char_count;

	// Initialize occ_count_before map with 0's
	for (char ch : bwt) {
		if (occ_count_before.find(ch) == occ_count_before.end()) {
			occ_count_before[ch] = vector<int>(n + 1, 0);
		}
	}

	// Fill the occ_count_before map
	for (int i = 0; i < n; ++i) {
		char_count[bwt[i]]++;
		for (auto& entry : occ_count_before) {
			entry.second[i + 1] = entry.second[i] + (entry.first == bwt[i] ? 1 : 0);
		}
	}

	// Compute the starts map
	int total_chars = 0;
	for (auto& entry : char_count) {
		starts[entry.first] = total_chars;
		total_chars += entry.second;
	}
}

// Compute the number of occurrences of string pattern in the text
int CountOccurrences(const string& pattern, 
                     const string& bwt, 
                     const map<char, int>& starts, 
                     const map<char, vector<int>>& occ_count_before) {
	int top = 0;
	int bottom = bwt.size() - 1;

	int pattern_len = pattern.size();

	while (top <= bottom) {
		if (pattern_len > 0) {
			char symbol = pattern[--pattern_len];
			if (occ_count_before.find(symbol) != occ_count_before.end()) {
				top = starts.at(symbol) + occ_count_before.at(symbol)[top];
				bottom = starts.at(symbol) + occ_count_before.at(symbol)[bottom + 1] - 1;
			} else {
				return 0;
			}
		} else {
			return bottom - top + 1;
		}
	}

	return 0;
}

int main() {
	string bwt;
	cin >> bwt;
	int pattern_count;
	cin >> pattern_count;

	// Start of each character in the sorted list of characters of bwt,
	map<char, int> starts;
	// Occurrence counts for each character and each position in bwt,
	map<char, vector<int>> occ_count_before;

	// Preprocess the BWT once to get starts and occ_count_before
	PreprocessBWT(bwt, starts, occ_count_before);

	for (int pi = 0; pi < pattern_count; ++pi) {
		string pattern;
		cin >> pattern;
		int occ_count = CountOccurrences(pattern, bwt, starts, occ_count_before);
		printf("%d ", occ_count);
	}
	printf("\n");
	return 0;
}
