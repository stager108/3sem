
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <set>
#include <vector>
#include <string>
#include <cstring>
#include <stdio.h>
#include <deque>
const int MAX_SIZE = 256;
    int Count = 0;
    std::vector <tNode> bigTree;
	std::vector<Node> bigbor;
	std::vector <int> isVisited;
	std::vector <int> answer;

    struct tNode {
		int rfr[MAX_SIZE];
		Node() {
			for (int i = 0; i < 2; i++) {
				rfr[i] = -1;
			}
		}
	};

    struct Node {
		int parent = -1;
		int isLeaf = 0;
		int link;
		char parent_char = '0';
		int rfr[2];
		int go[2];
		Node(){
			for (int i = 0; i < MAX_SIZE; i++) {
				rfr[i] = -1;
				go[i] = -1;
			}
		}
	};

    void readTree(){
        scanf("%i",&Count);
        bigTree.resize(Count);
        for(int i = 0; i < Count; i++){
            int m;
            int next_vert;
            char next_sym;
            scanf("%i",&m);
            for(int j = 0; j < m; j++){
                scanf("%i %c", &next_vert, &next_sym);
                bigTree[i].rfr[(int)next_sym] = next_vert;
            }
        }
    }

	void initMyBor() {
		bigbor.clear();
		Node root;
		root.link = -1;
		bigbor.push_back(root);
	}

	void add(std::string S) {
		int m = strlen(S.c_str());
		int nowNode = 0;
		for (int i = 0; i < m; i++) {
			if (bigbor[nowNode].rfr[S[i] - '0'] == -1) {
				Node W;
				W.parent = nowNode;
                W.parent_char = S[i];
				W.link = -1;
				bigbor[nowNode].rfr[(int)S[i] - '0'] = bigbor.size();
				nowNode = bigbor.size();
				bigbor.push_back(W);
			}
			else {
				nowNode = bigbor[nowNode].rfr[(int)S[i] - '0'];
			}
		}
        bigbor[nowNode].isLeaf = 1;
	}

	int get_link(int v) {
		if (bigbor[v].link == -1){
			if (v == 0 || bigbor[v].parent == 0){
				bigbor[v].link = 0;
			}
			else{
				bigbor[v].link = go(get_link(bigbor[v].parent), bigbor[v].parent_char);
			}
		}
		return bigbor[v].link;
	}

	int go(int v, char c) {
		if (bigbor[v].go[(int)c - '0'] == -1){
			if (bigbor[v].rfr[(int)c - '0'] != -1){
				bigbor[v].go[(int)c - '0'] = bigbor[v].rfr[(int)c - '0'];
			}
			else{
				if (v == 0) {
					bigbor[v].go[(int)c - '0'] = 0;
				}
				else {
					bigbor[v].go[(int)c - '0'] = go(get_link(v), c);
				}
			}
		}
		return bigbor[v].go[(int)c - '0'];
	}

	int dfs(int v) {
        if (v < 0) return 0;
		int ans = 0;
		int res = 0;
        if (bigbor[v].isLeaf) { isVisited[v] = 2; return 0; }
        if (isVisited[v] == 1) { return 1; }
        if (isVisited[v] == 2) { return answer[v]; }

        isVisited[v] = 1;
        for (int j = 0; j< 2; j++) {
            res = dfs(bigbor[v].go[j]);
            if (res == 1) { return 1; }
		}
		answer[v] = ans;
		isVisited[v] = 2;
		return ans;
	}

	void initialize() {
		isVisited.resize(bigbor.size(), 0);
		answer.resize(bigbor.size(), 0);
	}

	void readBor(){
        int myCount;
        scanf("%i", &myCount);
        std::string myStr;
        myStr.resize(300000);
        for (int i = 0; i < myCount; i++) {
            scanf("%s", &myStr[0]);
            add(myStr);
        }
        initialize();
	}

int main()
{
	initMyBor();
    readTree();
    readBor();
    dfs(0);
	return 0;
}
