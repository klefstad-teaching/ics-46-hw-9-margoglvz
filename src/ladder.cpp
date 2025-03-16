#include "ladder.h"

bool edit_distance_within(const string& str1, const string& str2, int d) {
    size_t len1 = str1.size();
    size_t len2 = str2.size();
    
    // If the length difference exceeds d, return false immediately.
    if (abs(static_cast<int>(len1 - len2)) > d) return false;

    // Convert both words to lowercase for case-insensitive comparison.
    string lower1 = to_lower(str1);
    string lower2 = to_lower(str2);

    // If d == 1, check for one-letter modification, insertion, or deletion.
    if (d == 1) {
        if (lower1 == lower2) return true;  // Exact match

        if (len1 == len2) {  
            int differences = 0;
            for (size_t i = 0; i < len1; ++i) {
                if (lower1[i] != lower2[i]) {
                    differences++;
                    if (differences > 1) return false;
                }
            }
            return true;
        }

        // Check for one-letter insertion or deletion
        const string &shorter = (len1 < len2) ? lower1 : lower2;
        const string &longer = (len1 < len2) ? lower2 : lower1;
        size_t i = 0, j = 0;
        bool found_difference = false;

        while (i < shorter.size() && j < longer.size()) {
            if (shorter[i] != longer[j]) {
                if (found_difference) return false;
                found_difference = true;
                j++;  
            } else {
                i++;
                j++;
            }
        }
        return true;
    }

    // If d > 1, use dynamic programming to compute the edit distance.
    int n = lower1.size(), m = lower2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 0; i <= n; i++) dp[i][0] = i;
    for (int j = 0; j <= m; j++) dp[0][j] = j;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (lower1[i - 1] == lower2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        }
    }

    return dp[n][m] <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    string start = to_lower(begin_word);
    string end = to_lower(end_word);
    
    if (word_list.find(end) == word_list.end()) {
        error(begin_word, end_word, "End word not in dictionary");
        return vector<string>();
    }
    
    queue<vector<string>> ladder_queue;
    vector<string> initial = { start };
    ladder_queue.push(initial);
    
    set<string> visited;
    visited.insert(start);
    
    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();
        
        for (const string& candidate : word_list) {
            if (abs(static_cast<int>(candidate.size()) - static_cast<int>(last_word.size())) > 1)
                continue;
            if (visited.find(candidate) == visited.end() && is_adjacent(last_word, candidate)) {
                visited.insert(candidate);
                vector<string> new_ladder = ladder;
                new_ladder.push_back(candidate);
                if (candidate == end)
                    return new_ladder;
                ladder_queue.push(new_ladder);
            }
        }
    }


    error(begin_word, end_word, "No ladder found");
    return vector<string>();
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream infile(file_name);
    if (!infile) {
        cerr << "Cannot open file: " << file_name << endl;
        return;
    }
    string word;
    while (getline(infile, word)) {
        if (!word.empty())
            word_list.insert(to_lower(word));
    }
    infile.close();
}

void verify_word_ladder() {
    #define my_assert(e) { cout << #e << ((e) ? " passed" : " failed") << endl; }
    set<string> word_list;
    load_words(word_list, "src/words.txt");
    
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}

// Helper
string to_lower(const string& s) {
    string new_str = s;
    transform(new_str.begin(), new_str.end(), new_str.begin(), ::tolower);
    return new_str;
}

// Error
void error(string word1, string word2, string msg) {
    cout << msg << word1 << word2 << endl;
}

// Printing
void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << "\n";
        return;
    }
    cout << "Word ladder found: ";
    for (const auto& word : ladder) {
        cout << word << " ";
    }
    cout << "\n";
}
