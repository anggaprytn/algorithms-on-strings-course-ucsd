#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using std::cin;
using std::string;
using std::vector;

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
            std::pair<int, int> curr = {classes[suffix_array[i]], classes[(suffix_array[i] + (1 << h)) % n]};
            std::pair<int, int> prev = {classes[suffix_array[i - 1]], classes[(suffix_array[i - 1] + (1 << h)) % n]}; 
            if (curr != prev) {
                num_classes++;
            }
            c_new[suffix_array[i]] = num_classes - 1;
        }
        classes = c_new;
    }
    return suffix_array;
}

vector<int> FindOccurrences(const string& pattern, const string& text, const vector<int>& suffix_array) {
    vector<int> result;
    int n = text.size();
    int m = pattern.size();

    int left = 0, right = n;
    // Find the lower bound of the pattern in the suffix array
    while (left < right) {
        int mid = (left + right) / 2;
        if (text.compare(suffix_array[mid], m, pattern) < 0) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    int start = left;
    right = n;
    // Find the upper bound of the pattern in the suffix array
    while (left < right) {
        int mid = (left + right) / 2;
        if (text.compare(suffix_array[mid], m, pattern) > 0) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    int end = right;

    for (int i = start; i < end; ++i) {
        result.push_back(suffix_array[i]);
    }

    return result;
}

int main() {
    char buffer[100001];
    scanf("%s", buffer);
    string text = buffer;
    text += '$';
    vector<int> suffix_array = BuildSuffixArray(text);
    int pattern_count;
    scanf("%d", &pattern_count);
    vector<bool> occurs(text.length(), false);
    for (int pattern_index = 0; pattern_index < pattern_count; ++pattern_index) {
        scanf("%s", buffer);
        string pattern = buffer;
        vector<int> occurrences = FindOccurrences(pattern, text, suffix_array);
        for (int j = 0; j < occurrences.size(); ++j) {
            occurs[occurrences[j]] = true;
        }
    }
    for (int i = 0; i < occurs.size(); ++i) {
        if (occurs[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
    return 0;
}
