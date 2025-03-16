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
        while (i < static_cast<int>(shorter.size()) && j < static_cast<int>(longer.size())) {
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
