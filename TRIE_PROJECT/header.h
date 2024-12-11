#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
using namespace std;

const int NUMBEROFNODES = 1500000;


struct Trie {
	struct Node {
		int child[26];
		int exist, count;
	};
	Node* nodes = new Node[NUMBEROFNODES];
	int current;

	Trie(){
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
		for (int i = 0; i <= 25; i++) {
			if (this->nodes[pos].child[i] != -1) {
				word.push_back((char)(i + 'a'));
				if (tree.nodes[pos].exist && word != "") cout << word << endl;
				printAllWordByLetter(*this, this->nodes[pos].child[i], word);
				word.pop_back();
			}
		}
		return;
	}

	void suggestPrefixString(string word) {
		if (findWord(word) == -1) {
			cout << "No prefix word availiable..." << endl;
			return;
		}
		int pos = 0;
		for (char c : word) {
			int key = c - 'a';
			pos = nodes[pos].child[key];
		}
		cout << word << "\n";
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






//struct Trie {
//    struct Node {
//        Node* child[26];
//        int exist, cnt;
//
//        Node() {
//            for (int i = 0; i < 26; i++) child[i] = NULL;
//            exist = cnt = 0;
//        }
//    };
//
//    int cur;
//    Node* root;
//    Trie() : cur(0) {
//        root = new Node();
//    };
//
//    void add_string(string s) {
//        Node* p = root;
//        for (auto f : s) {
//            int c = f - 'a';
//            if (p->child[c] == NULL) p->child[c] = new Node();
//
//            p = p->child[c];
//            p->cnt++;
//        }
//        p->exist++;
//    }
//
//    bool delete_string_recursive(Node* p, string& s, int i) {
//        if (i != (int)s.size()) {
//            int c = s[i] - 'a';
//            bool isChildDeleted = delete_string_recursive(p->child[c], s, i + 1);
//            if (isChildDeleted) p->child[c] = NULL;
//        }
//        else p->exist--;
//
//        if (p != root) {
//            p->cnt--;
//            if (p->cnt == 0) {
//                delete(p); // Khác với cài đặt bằng mảng,
//                // ta có thể thực sự xóa đỉnh này đi
//                return true;
//            }
//        }
//        return false;
//    }
//
//    void delete_string(string s) {
//        if (find_string(s) == false) return;
//
//        delete_string_recursive(root, s, 0);
//    }
//
//    bool find_string(string s) {
//        Node* p = root;
//        for (auto f : s) {
//            int c = f - 'a';
//            if (p->child[c] == NULL) return false;
//            p = p->child[c];
//        }
//        return (p->exist != 0);
//    }
//};