#include "header.h"

int main()
{
    Trie tree;
    //cout << numberOfNode("words_alpha.txt");
    auto current2 = std::chrono::steady_clock::now();
    tree = creatDictionaryFromFile("words_alpha.txt");
    auto end2 = std::chrono::steady_clock::now();
    auto elapsed_time2 = std::chrono::duration_cast<std::chrono::nanosecondsư>(end2- current2).count();
    cout << elapsed_time2 << endl;
    //cout << tree.findWord("oversufficiency");
    //cout << tree.current << endl;
    //tree.printAllWordByLetter(tree, 0);
    //auto current = std::chrono::steady_clock::now();
    //tree.suggestPrefixString("aa",15);
    //auto end = std::chrono::steady_clock::now();
    //auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - current).count();
    //cout << elapsed_time << endl;
    //auto elapsed_time2 = std::chrono::duration_cast<std::chrono::microseconds>(end - current).count();
    //cout << elapsed_time2 << endl;
    TrieNode* root = new TrieNode();
    ifstream input("words_alpha.txt");
    string key = "";
    auto current = std::chrono::steady_clock::now();
    while (input >> key) {
        insert_key(root, key);
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono:: nanoseconds>(end - current).count();
    cout << elapsed_time << endl;
    int x; 
    cin >> x;
}
