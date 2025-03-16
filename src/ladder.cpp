#include "ladder.h"

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

bool edit_distance_within(const string& str1, const string& str2, int d) {
    if(d > 1) {
        int n = str1.size(), m = str2.size();
        if (abs(n - m) > d)
            return false;
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        for (int i = 0; i <= n; i++)
            dp[i][0] = i;
        for (int j = 0; j <= m; j++)
            dp[0][j] = j;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (tolower(str1[i - 1]) == tolower(str2[j - 1]))
                    dp[i][j] = dp[i - 1][j - 1];
                else
                    dp[i][j] = 1 + min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
            }
        }
        return dp[n][m] <= d;
    }
    
    // d == 1
    int len1 = str1.size();
    int len2 = str2.size();
    if (abs(len1 - len2) > 1)
        return false;
    
    // equal strings
    if (to_lower(str1) == to_lower(str2))
        return true;
    
    if (len1 == len2) {
        int diffCount = 0;
        for (int i = 0; i < len1; i++) {
            if (tolower(str1[i]) != tolower(str2[i]))
                diffCount++;
            if (diffCount > 1)
                return false;
        }
        return true;
    } else {
        const string &shorter = (len1 < len2) ? str1 : str2;
        const string &longer  = (len1 < len2) ? str2 : str1;
        int i = 0, j = 0;
        bool foundDifference = false;
        while (i < shorter.size() && j < longer.size()) {
            if (tolower(shorter[i]) == tolower(longer[j])) {
                i++;
                j++;
            } else {
                if (foundDifference)
                    return false;
                foundDifference = true;
                j++; // skip one letter in the longer string
            }
        }
        return true;
    }
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    queue<vector<string>> ladder_queue;
    set<string> visited;

    string start = to_lower(begin_word);
    string end = to_lower(end_word);

    // Input validation
    if (start == end) {
        error(start, end, "Start and end words must be different");
        return {};
    }
    if (word_list.find(end) == word_list.end()) {
        error(start, end, "End word must be in the dictionary");
        return {};
    }

    ladder_queue.push({start});
    visited.insert(start);

    while (!ladder_queue.empty()) {
        int level_size = ladder_queue.size();  
        set<string> words_this_level;  

        for (int i = 0; i < level_size; i++) {
            vector<string> ladder = ladder_queue.front();
            ladder_queue.pop();
            string last_word = ladder.back();

            for (const string& word : word_list) {
                // only check similar length words
                if (abs(static_cast<int>(word.size()) - static_cast<int>(last_word.size())) > 1)
                    continue;

                if (is_adjacent(last_word, word) && visited.find(word) == visited.end()) {
                    vector<string> new_ladder = ladder;
                    new_ladder.push_back(word);
                    words_this_level.insert(word);

                    if (word == end) return new_ladder;

                    ladder_queue.push(new_ladder);
                }
            }
        }

        visited.insert(words_this_level.begin(), words_this_level.end());
    }
    return {}; 
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
    set<string> word_list;
    load_words(word_list, "words.txt");

    cout << "Testing word ladder correctness...\n";

    // Standard transformations
    cout << (generate_word_ladder("cold", "warm", word_list).size() == 5 ? "Passed\n" : "Failed\n"); // Example: cold -> cord -> word -> ward -> warm
    cout << (generate_word_ladder("stone", "money", word_list).size() == 5 ? "Passed\n" : "Failed\n"); // stone -> stony -> story -> store -> money

    // Case sensitivity test
    cout << (generate_word_ladder("COLD", "WARM", word_list).size() == 5 ? "Passed\n" : "Failed\n"); // Should work the same as lowercase

    // Length variation transformations (insertion/deletion allowed)
    cout << (generate_word_ladder("cat", "cheat", word_list).size() == 4 ? "Passed\n" : "Failed\n"); // cat -> chat -> cheat
    cout << (generate_word_ladder("data", "database", word_list).size() == 5 ? "Passed\n" : "Failed\n"); // data -> date -> database

    // Short word transformations
    cout << (generate_word_ladder("at", "it", word_list).size() == 2 ? "Passed\n" : "Failed\n"); // at -> it (single letter change)
    cout << (generate_word_ladder("a", "to", word_list).size() == 3 ? "Passed\n" : "Failed\n"); // a -> at -> to

    // No valid word ladder (impossible transformation)
    cout << (generate_word_ladder("hello", "world", word_list).empty() ? "Passed\n" : "Failed\n"); // "hello" to "world" is unlikely

    // Start word not in dictionary but valid path
    cout << (generate_word_ladder("marty", "curls", word_list).size() == 6 ? "Passed\n" : "Failed\n"); // marty -> party -> parts -> carts -> cards -> curds -> curls

    // Edge case: Start and end words are the same (should return empty or error)
    cout << (generate_word_ladder("code", "code", word_list).empty() ? "Passed\n" : "Failed\n"); // Should return empty

    // Edge case: End word not in dictionary
    cout << (generate_word_ladder("work", "xyzabc", word_list).empty() ? "Passed\n" : "Failed\n"); // "xyzabc" should not be found in dictionary

    // Large ladder test (complex word transformation)
    cout << (generate_word_ladder("butter", "better", word_list).size() == 2 ? "Passed\n" : "Failed\n"); // butter -> better (one letter change)

    cout << "All test cases executed.\n";
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
