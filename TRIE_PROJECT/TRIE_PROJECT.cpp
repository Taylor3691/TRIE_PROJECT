#include "header.h"

int main()
{
    Trie tree;
    //cout << numberOfNode("words_alpha.txt");
    tree = creatDictionaryFromFile("words_alpha.txt");
    //cout << tree.findWord("oversufficiency");
    //cout << tree.current << endl;
    //tree.printAllWordByLetter(tree, 0);
    auto current = std::chrono::steady_clock::now();
    tree.suggestPrefixString("a");
    auto end = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - current).count();
    cout << elapsed_time << endl;

    //ifstream input("words_alpha.txt");
    //string word = "";
    //auto current = std::chrono::steady_clock::now();
    //while (input >> word) {
    //    tree.add_string(word);
    //}
    //auto end = std::chrono::steady_clock::now();
    //auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - current).count();
    //cout << elapsed_time << endl;
    //input.close();
}
