#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <set>
#include <vector>
#include <string>
class myBor {
	struct Node {
		int parent = -1;
		int isEnded = 1;
		std::vector<int> rfr;
		std::vector<int> lefter;
		Node() {
			rfr.resize(26, -1);
			lefter.resize(26, 0);
		}

	};
	std::vector<Node> bigbor;
public:
	myBor() {
		bigbor.clear();
		Node root;
		root.isEnded = 0;
		bigbor.push_back(root);
	}
	~myBor() {}
	void add(std::string S) {
		int m = S.length();
		int nowNode = 0;
		for (int i = 0; i < m; i++) {
			if (bigbor[nowNode].rfr[S[i] - 'a'] == -1) {
				Node W;
				if (i != m - 1) {
					W.isEnded = 0;
				}
				W.parent = nowNode;
				bigbor[nowNode].rfr[(int)S[i] - 'a'] = bigbor.size();
				for (int j = (S[i] - 'a'); j < 26; j++) {
					bigbor[nowNode].lefter[j]++;
				}
				nowNode = bigbor.size();
				bigbor.push_back(W);
			}
			else {
				for (int j = (S[i] - 'a'); j < 26; j++) {
					bigbor[nowNode].lefter[j]++;
				}
				nowNode = bigbor[nowNode].rfr[(int)S[i] - 'a'];
				if (i == m - 1) {
					bigbor[nowNode].isEnded = 1;
				}
			}
		}
	}
	void printString(int n) {
		int pos = n;
		int nowNode = 0;
		while (pos != 0) {
			for (int i = 0; i < 26; i++) {
				if (pos - bigbor[nowNode].lefter[i] <= 0) {
					std::cout << (char)(i + 'a');
					if (i > 0) {
						pos = pos - bigbor[nowNode].lefter[i - 1];
					}
					nowNode = bigbor[nowNode].rfr[i];
					break;
				}
			}
			pos -= bigbor[nowNode].isEnded;
		}
		while (bigbor[nowNode].isEnded != 1) {
			for (int i = 0; i < 26; i++) {
				if (bigbor[nowNode].rfr[i] != -1) {
					nowNode = bigbor[nowNode].rfr[i];
					std::cout << (char)(i + 'a');
					break;
				}
			}
		}
		std::cout << std::endl;
		return;
	}
};
int main()
{
	int myCount = 0;
	int command;
	int err;
	myBor newBor;
	std::string myStr;
	std::cin >> myCount;
	for (int i = 0; i < myCount; i++) {
		std::cin >> myStr;
		if ((myStr[0] >= '0') && (myStr[0] <= '9')) {
			command = std::atoi(myStr.c_str());
			newBor.printString(command);
		}
		else {
			newBor.add(myStr);
		}

	}
	return 0;
}
