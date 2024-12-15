#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstring>
#include <map>
#include <vector>
#include <thread>
#include <conio.h>
using namespace std;

const int NUMBEROFNODES = 1500000;


struct Trie {
	struct Node {
		int child[26];
		int exist, count;
	};
	Node* nodes = new Node[NUMBEROFNODES];
	int current;
	int numberOfSuggest;

	Trie(){
		numberOfSuggest = 0;
		current = 0;
		memset(nodes[0].child, -1, sizeof(nodes[0].child));
		nodes[0].count = nodes[0].exist = 0;
	}

	int creatNewNode() {
		current++;
		memset(nodes[current].child, -1, sizeof(nodes[current].child));
		nodes[current].count = nodes[current].exist = 0;
		return current;
	}

	void addWord(string word) {
		int pos = 0;
		for (char c : word) {
			int key = c - 'a';
			if (nodes[pos].child[key] == -1) {
				nodes[pos].child[key] = creatNewNode();
			}
			pos = nodes[pos].child[key];
			nodes[pos].count++;
		}
		nodes[pos].exist++;
	}

	bool findWord(string word) {
		int pos = 0;
		for (char c : word) {
			int key = c - 'a';
			if (nodes[pos].child[key] == -1) {
				return 0;
			}
			pos = nodes[pos].child[key];
		}
		return nodes[pos].exist != 0;
	}

	bool deleteProcessing(string word, int pos, int i) {
		if (i != word.size()) {
			int key = word[i] - 'a';
			bool isChildDelete = deleteProcessing(word, nodes[pos].child[key], i + 1);
			if (isChildDelete) nodes[pos].child[key] = -1;
		}
		else {
			nodes[pos].exist--;
		}

		if (pos != 0) {
			nodes[pos].count--;
			if (nodes[pos].count == 0) return 1;
		}
		return 0;
	}

	void deleteWord(string word) {
		if (!findWord(word)) return;
		deleteProcessing(word, 0, 0);
	}

	void printAllWordByLetter(Trie tree, int pos, string word = "") {
		if (this->numberOfSuggest <= 0) return;
		for (int i = 0; i <= 25; i++) {
			int back_track = pos;
			if (this->nodes[pos].child[i] != -1) {
				if (numberOfSuggest <= 0) return;
				pos = nodes[pos].child[i];
				word.push_back((char)(i + 'a'));
				if (tree.nodes[pos].exist != 0 && word != "") {
					cout << word << "\n";
					this->numberOfSuggest--;
				}
				printAllWordByLetter(*this, pos, word);
				pos = back_track;
				word.pop_back();
			}
		}
		return;
	}

	void suggestPrefixString(string word, int k = -1) {
		int pos = 0;
		for (char c : word) {
			int key = c - 'a';
			if (nodes[pos].child[key] == -1) {
				cout << "No word start with prefix: " << word << "\n";
				return;
			}
			pos = nodes[pos].child[key];
		}
		this->numberOfSuggest = k == -1 ? 10000000 : k;
		cout << "Number of suggest we need: " << numberOfSuggest << "\n";
		pos = 0;
		for (char c : word) {
			int key = c - 'a';
			pos = nodes[pos].child[key];
		}
		printAllWordByLetter(*this, pos, word);
	}
};

Trie creatDictionaryFromFile(string filename) {
	Trie tree;
	ifstream input(filename);
	string word = "";
	while (input >> word) {
		tree.addWord(word);
	}
	input.close();
	return tree;
}


struct TrieNode {

	// pointer array for child nodes of each node
	TrieNode* childNode[26];
	int wordCount;

	TrieNode()
	{
		// constructor
		// initialize the wordCnt variable with 0
		// initialize every index of childNode array with
		// NULL
		wordCount = 0;
		for (int i = 0; i < 26; i++) {
			childNode[i] = NULL;
		}
	}
};

void insert_key(TrieNode* root, string& key)
{
	// Initialize the currentNode pointer
	// with the root node
	TrieNode* currentNode = root;

	// Iterate across the length of the string
	for (auto c : key) {

		// Check if the node exist for the current
		// character in the Trie.
		if (currentNode->childNode[c - 'a'] == NULL) {

			// If node for current character does not exist
			// then make a new node
			TrieNode* newNode = new TrieNode();

			// Keep the reference for the newly created
			// node.
			currentNode->childNode[c - 'a'] = newNode;
		}

		// Now, move the current node pointer to the newly
		// created node.
		currentNode = currentNode->childNode[c - 'a'];
	}

	// Increment the wordEndCount for the last currentNode
	// pointer this implies that there is a string ending at
	// currentNode.
	currentNode->wordCount++;
}

bool search_key(TrieNode* root, string& key)
{
	// Initialize the currentNode pointer
	// with the root node
	TrieNode* currentNode = root;

	// Iterate across the length of the string
	for (auto c : key) {

		// Check if the node exist for the current
		// character in the Trie.
		if (currentNode->childNode[c - 'a'] == NULL) {

			// Given word does not exist in Trie
			return false;
		}

		// Move the currentNode pointer to the already
		// existing node for current character.
		currentNode = currentNode->childNode[c - 'a'];
	}

	return (currentNode->wordCount > 0);
}

bool delete_key(TrieNode* root, string& word)
{
	TrieNode* currentNode = root;
	TrieNode* lastBranchNode = NULL;
	char lastBrachChar = 'a';

	for (auto c : word) {
		if (currentNode->childNode[c - 'a'] == NULL) {
			return false;
		}
		else {
			int count = 0;
			for (int i = 0; i < 26; i++) {
				if (currentNode->childNode[i] != NULL)
					count++;
			}

			if (count > 1) {
				lastBranchNode = currentNode;
				lastBrachChar = c;
			}
			currentNode = currentNode->childNode[c - 'a'];
		}
	}

	int count = 0;
	for (int i = 0; i < 26; i++) {
		if (currentNode->childNode[i] != NULL)
			count++;
	}

	// Case 1: The deleted word is a prefix of other words
	// in Trie.
	if (count > 0) {
		currentNode->wordCount--;
		return true;
	}

	// Case 2: The deleted word shares a common prefix with
	// other words in Trie.
	if (lastBranchNode != NULL) {
		lastBranchNode->childNode[lastBrachChar] = NULL;
		return true;
	}
	// Case 3: The deleted word does not share any common
	// prefix with other words in Trie.
	else {
		root->childNode[word[0]] = NULL;
		return true;
	}
}
const int HASH_SIZE = 500000;
struct Dictionary {
	string* hashTable;
	int size;
	int collision;
	int numberOfSuggestion;

	Dictionary() {
		this->hashTable = NULL;
		this->size = 0;
		this->collision = 0;
		numberOfSuggestion = 0;
	}

	vector<string> readFile(string filename) {
		ifstream input(filename);
		vector<string> res;
		string token = "";
		while (input >> token) {
			res.push_back(token);
		}
		input.close();
		return res;
	}

	long long hashString(string word) {
		long long key = 0;
		long long mod = 1e9 + 9;
		if (word.size() <= 20) {
			long long factor = 1;
			for (int i = 0; i < word.size(); i++) {
				key = key + (((word[i] % mod) * factor % mod) % mod);
				factor = (factor * 31);
				factor %= mod;
				key %= mod;
			}
		}
		else {
			word = word.substr(word.size() - 20);
			long long factor = 1;
			for (int i = 0; i < word.size(); i++) {
				key = key + (((word[i] % mod) * factor % mod) % mod);
				factor = (factor * 31);
				factor %= mod;
				key %= mod;
			}
		}
		return key % this->size;
	}

	void Insert(string* hashTable, string word) {
		int key = hashString(word);
		int cnt = 0;
		int i = 0;
		while (this->hashTable[key] != "" && cnt <= this->size) {
			this->collision++;
			key = key++;
			key %= this->size;
			cnt++;
		}
		if (cnt <= this->size) {
			hashTable[key] = word;
		}
	}

	void creatHashTable(string filename) {
		vector<string> list = readFile(filename);
		this->size = list.size() + 1000;
		this->hashTable = new string[this->size];
		for (int i = 0; i < this->size; i++) {
			hashTable[i] = "";
		}
		for (string token : list) {
			Insert(this->hashTable, token);
		}
	}

	void printAllWord() {
		for (int i = 0; i < this->size; i++) {
			if(this->hashTable[i]!= "")cout << this->hashTable[i] << endl;
		}
	}

	int searchWord(string word) {
		int key = hashString(word);
		int cnt = 0;
		while (this->hashTable[key] != word && cnt <= this->size) {
			key++;
			key %= this->size;
			cnt++;
		}

		if (cnt <= this->size) {
			return key;
		}
		return -1;
	}

	void deleteWord(string word) {
		int key = hashString(word);
		int cnt = 0;
		while (this->hashTable[key] != word && cnt <= this->size) {
			key++;
			key %= this->size;
			cnt++;
		}
		if (cnt <= this->size) {
			this->hashTable[key] = "";
			return;
		}
		return;
	}

	string common_prefix(string s1, string s2) {
		int leng = min(s1.length(), s2.length());
		for (int i = 0; i < leng; i++) {
			if (s1[i] != s2[i]) {
				return s1.substr(0, i);
			}
		}
		return s1.substr(0, leng);
	}

	void suggestWord(string prefix, int k) {
		this->numberOfSuggestion = k;
		for (int i = 0; i < this->size; i++) {
			if (numberOfSuggestion <= 0) return;
			string check = common_prefix(prefix, this->hashTable[i]);
			if (check.size() == prefix.size() && this->hashTable[i] != prefix) {
				cout << this->hashTable[i] <<"\n";
				this->numberOfSuggestion--;
			}
		}
		if (this->numberOfSuggestion == k) {
			cout << "No word start with prefix: " << prefix << "\n";
		}
	}
};

void measureRuntimeFileByTrie(Trie tree) {
	system("cls");
	cout << "What size of prefix do you want to test, please choose...\n";
	cout << "1. From 1-2 character\n";
	cout << "2. From 3-5 character\n";
	cout << "3. From 1-5 character\n";
	cout << "Your choice: ";
	string cmd = "";
	cin >> cmd;
	cout << "How many words maximum do you want to suggest in every prefix ? ";
	cout << "Your answer: ";
	int k;
	cin >> k;
	string start = "";
	string end = "";
	if (cmd == "1") {
		start = "1";
		end = "2";
	}
	else if (cmd == "2") {
		start = "3";
		end = "5";
	}
	else if (cmd == "3") {
		start = "1";
		end = "5";
	}
	ifstream input("prefix_sample_" + start + "_to_" + end + ".txt");
	string token = "";
	auto current2 = std::chrono::steady_clock::now();
	while (input >> token) {
		cout << "All word start with " << token << "\n";
		tree.suggestPrefixString(token, k);
		cout << "____________________________________________________________\n";
	}
	auto end2 = std::chrono::steady_clock::now();
	auto elapsed_time2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - current2).count();
	cout << "It takes " << elapsed_time2 << " microseconds with Trie\n";
	input.close();
}

void measureRuntimeFileByHashTable(Dictionary dictionary) {
	system("cls");
	cout << "What size of prefix do you want to test, please choose...\n";
	cout << "1. From 1-2 character\n";
	cout << "2. From 3-5 character\n";
	cout << "3. From 1-5 character\n";
	cout << "Your choice: ";
	string cmd = "";
	cin >> cmd;
	cout << "How many words maximum do you want to suggest in every prefix ? ";
	cout << "Your answer: ";
	int k;
	cin >> k;
	string start = "";
	string end = "";
	if (cmd == "1") {
		start = "1";
		end = "2";
	}
	else if (cmd == "2") {
		start = "3";
		end = "5";
	}
	else if (cmd == "3") {
		start = "1";
		end = "5";
	}
	ifstream input("prefix_sample_" + start + "_to_" + end + ".txt");
	string token = "";
	auto current2 = std::chrono::steady_clock::now();
	while (input >> token) {
		cout << "All word start with " << token << "\n";
		dictionary.suggestWord(token, k);
		cout << "____________________________________________________________\n";
	}
	auto end2 = std::chrono::steady_clock::now();
	auto elapsed_time2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - current2).count();
	cout << "It takes " << elapsed_time2 << " microseconds with HashTable\n";
	input.close();
}


