#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstring>
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

	void printAllWordByLetter(Trie& tree, int& pos, string word = "") {
		if (this->numberOfSuggest <= 0) return;
		for (int i = 0; i <= 25; i++) {
			if (this->nodes[pos].child[i] != -1) {
				if (numberOfSuggest <= 0) return;
				word.push_back((char)(i + 'a'));
				if (tree.nodes[pos].exist && word != "") {
					cout << numberOfSuggest << "th: " << word<<"\n";
					this->numberOfSuggest--;

				}
				printAllWordByLetter(*this, this->nodes[pos].child[i], word);
				word.pop_back();
			}
		}
		return;
	}

	void suggestPrefixString(string word, int k) {
		if (findWord(word) == -1) {
			cout << "No prefix word availiable...\n";
			return;
		}
		this->numberOfSuggest = k;
		cout << "Number of suggest we need: " << numberOfSuggest << "\n";
		int pos = 0;
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
	//auto current = std::chrono::steady_clock::now();
	while (input >> word) {
		tree.addWord(word);
	}
	//auto end = std::chrono::steady_clock::now();
	//auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - current).count();
	//cout << elapsed_time;
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
