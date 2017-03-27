
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iostream>
#include <stdio.h>
const int MAX_SIZE = 26;
class SuffixAutomat {

private:
	char cmd[3], myString[1000000];
	int n;
	struct Node {
		int suffix = -1;
		int link[MAX_SIZE];
		int len;
		bool is_End;
		Node(int l) {
			len = l;
			suffix = 0;
			is_End = 0;
			for (int i = 0; i < MAX_SIZE; i++) {
				link[i] = 0;
			}
		}
	};
	int last;
	std::vector <Node> myTree;
	void add_char(int C) {
		Node newl(myTree[last].len + 1);
		int v = last;
		int number = myTree.size();
		myTree.push_back(newl);
		if (last == 0) {
			myTree[v].link[C] = number;
			v = myTree[v].suffix;
		}
		else {
			while ((v != 0) && (!(myTree[v].link[C] == 0))) {
				myTree[v].link[C] = number;
				v = myTree[v].suffix;
			}

			if (v != 0) {
				int q = myTree[v].link[C];
				if (myTree[q].len == myTree[v].len + 1)
					myTree[number].suffix = q;
				else {
					Node Clone(myTree[v].len + 1);
					int clone_number = myTree.size();
					myTree.push_back(Clone);
					for (int i = 0; i < MAX_SIZE; i++) {
						myTree[clone_number].link[i] = myTree[q].link[i];
					}
					myTree[clone_number].suffix = myTree[q].suffix;
					while ((v != -1) && (myTree[v].link[C] == q)) {
						myTree[v].link[C] = myTree.size() - 1;
						v = myTree[v].suffix;
					}
					myTree[q].suffix = myTree[number].suffix = clone_number;
				}
			}
			else myTree[number].suffix = 0;
		}
		last = number;
	}

	int convert(char c) {
		if (c < 'a')
			return c - 'A';
		return c - 'a';
	}

	void add_string(char *s, int len) {
		for (int i = 0; i < len; i++)
			add_char(convert(s[i]));
	}

	void mark() {
		int v = last;
		while (v == 1) {
			myTree[v].is_End = 1;
			v = myTree[v].suffix;
		}
	}
	int isInTree() {
		int ans = 1;
		n = strlen(myString);
		int v = 0;
		for (int i = 0; (i < n) && ans; i++) {
			if (!myTree[v].link[convert(myString[i])])
				ans = 0;
			else v = myTree[v].link[convert(myString[i])];
		}
		return ans;
	}
public:
	SuffixAutomat () {
		Node Root(0);
		myTree.push_back(Root);
		last = 0;
	}
	~SuffixAutomat () {
		myTree.clear();
	}
	void solve() {
		while (scanf("%s%s", cmd, myString) == 2) {
			if (cmd[0] == 'A')
				add_string(myString, strlen(myString));
			else {
				if (isInTree()) {
					puts("YES");
				}
				else {
					puts("NO");
				}
			}
		}
	}

};

int main() {

	SuffixAutomat newU;
	newU.solve();
	return 0;
}
