#include "header.h"

int main()
{
    Trie tree;
    tree = creatDictionaryFromFile("words_alpha.txt");
    Dictionary dictionary;
    dictionary.creatHashTable("words_alpha.txt");
    while (true) {
        string cmd = "";
        system("cls");
        cout << "Please choose your command...\n";
        cout << "1. Insert one word to Trie\n";
        cout << "2. Delete one word frome Trie\n";
        cout << "3. Search one word in Trie\n";
        cout << "4. Suggest k words with the prefix input in Trie\n";
        cout << "5. Compare runtime k words suggestion with HashTable\n";
        cout << "6. Measure runtime with random prefix in file\n";
        cout << "7. Quit\n";
        cout << "Please enter a number: ";
        cin >> cmd;
        if (cmd == "1") {
            string word;
            cout << "Please text the word to insert: ";
            cin >> word;
            tree.addWord(word);
        }
        else if (cmd == "2") {
            string word;
            cout << "Please text the word to delete: ";
            cin >> word;
            tree.deleteWord(word);
        }
        else if (cmd == "3") {
            string word;
            cout << "Please text the word to search: ";
            cin >> word;
            int res = tree.findWord(word);
            if (res) {
                cout << "Your word are in Trie\n";
            }
            else {
                cout << "Your word are not in Trie\n";
            }
        }
        else if (cmd == "4") {
            int k = 0;
            string prefix = "";
            cout << "Please text your prefix: ";
            cin >> prefix;
            cout << "How many words maximum do you want to show: ";
            cin >> k;
            tree.suggestPrefixString(prefix, k);
        }
        else if (cmd == "5") {
            int k = 0;
            string prefix = "";
            cout << "Please text your prefix: ";
            cin >> prefix;
            cout << "How many words do you want to show: ";
            cin >> k;
            cout << "Suggestion words by Trie\n";
            auto current2 = std::chrono::steady_clock::now();
            tree.suggestPrefixString(prefix, k);
            auto end2 = std::chrono::steady_clock::now();
            auto elapsed_time2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - current2).count();
            cout << "Suggestion words by HashTable\n";
            auto current = std::chrono::steady_clock::now();
            dictionary.suggestWord(prefix, k);
            auto end = std::chrono::steady_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - current).count();
            cout << "Trie takes " << elapsed_time2 << " microseconds to suggest " << k << " words\n";
            cout << "HashTable takes " << elapsed_time << " microseconds to suggest " << k << " words\n";
        }
        else if(cmd == "6") {
            system("cls");
            cout << "Please choose the option below to measure...\n";
            cout << "1. Trie\n";
            cout << "2. HashTable\n";
            string req = "";
            cin >> req;
            if (req == "1") {
                measureRuntimeFileByTrie(tree);
            }
            else if (req == "2") {
                measureRuntimeFileByHashTable(dictionary);
            }
        }
        else if (cmd == "7") {
            cout << "Thank You\n" << endl;
            break;
        }
        cout << "Press any key to continue....\n";
        _getch();
    }
}


//int main() {
//	SuffixTree tree;
//
//	// Input string for the suffix tree
//	string text = "autocomplete";
//	tree.buildTree(text);
//	ifstream input("words_alpha.txt");
//	string token = "";
//	while (input >> token)
//	{
//		tree.buildTree(token);
//	}
//	input.close();
//	// Prefix to autocomplete
//	string prefix;
//	cout << "Enter prefix: ";
//	cin >> prefix;
//
//	// Perform autocomplete
//	vector<string> suggestions = tree.autocomplete(prefix);
//	if (suggestions.empty()) {
//		cout << "No suggestions found for prefix \"" << prefix << "\"\n";
//	}
//	else {
//		cout << "Suggestions for prefix \"" << prefix << "\":\n";
//		for (const string& suggestion : suggestions) {
//			cout << suggestion << "\n";
//		}
//	}
//
//	return 0;
//}