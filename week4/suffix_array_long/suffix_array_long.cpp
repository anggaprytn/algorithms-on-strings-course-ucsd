#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using std::cin;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;

// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.
vector<int> BuildSuffixArray(const string& text) {
    int n = text.size();
    vector<int> suffix_array(n), classes(n), cnt(std::max(256, n), 0), c_new(n), p_new(n);

    // Initial counting sort based on the first character
    for (int i = 0; i < n; ++i) {
        cnt[text[i]]++;
    }
    for (int i = 1; i < 256; ++i) {
        cnt[i] += cnt[i - 1];
    }
    for (int i = n - 1; i >= 0; --i) {
        suffix_array[--cnt[text[i]]] = i;
    }
    classes[suffix_array[0]] = 0;
    int num_classes = 1;
    for (int i = 1; i < n; ++i) {
        if (text[suffix_array[i]] != text[suffix_array[i - 1]]) {
            num_classes++;
        }
        classes[suffix_array[i]] = num_classes - 1;
    }

    // Iterative sorting by 2^k characters
    for (int h = 0; (1 << h) < n; ++h) {
        for (int i = 0; i < n; ++i) {
            p_new[i] = suffix_array[i] - (1 << h);
            if (p_new[i] < 0) p_new[i] += n;
        }

        fill(cnt.begin(), cnt.begin() + num_classes, 0);
        for (int i = 0; i < n; ++i) {
            cnt[classes[p_new[i]]]++;
        }
        for (int i = 1; i < num_classes; ++i) {
            cnt[i] += cnt[i - 1];
        }
        for (int i = n - 1; i >= 0; --i) {
            suffix_array[--cnt[classes[p_new[i]]]] = p_new[i];
        }
        c_new[suffix_array[0]] = 0;
        num_classes = 1;
        for (int i = 1; i < n; ++i) {
            pair<int, int> curr = {classes[suffix_array[i]], classes[(suffix_array[i] + (1 << h)) % n]};
            pair<int, int> prev = {classes[suffix_array[i - 1]], classes[(suffix_array[i - 1] + (1 << h)) % n]};
            if (curr != prev) {
                num_classes++;
            }
            c_new[suffix_array[i]] = num_classes - 1;
        }
        classes = c_new;
    }
    return suffix_array;
}

int main() {
    string text;
    cin >> text;
    vector<int> suffix_array = BuildSuffixArray(text);
    for (int i = 0; i < suffix_array.size(); ++i) {
        cout << suffix_array[i] << ' ';
    }
    cout << endl;
    return 0;
}
