
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
class myBor {
	struct Node {
		int parent = -1;
		int isLeaf = 0;
		int link;
		char parent_char = '0';
		int rfr[2];
		int go[2];
		Node() {
			for (int i = 0; i < 2; i++) {
				rfr[i] = -1;
				go[i] = -1;
			}
		}
	};
	std::vector<Node> bigbor;
	std::vector <int> isVisited;
	std::vector <int> answer;
public:
	myBor() {
		bigbor.clear();
		Node root;
		root.link = -1;
		bigbor.push_back(root);
	}
	~myBor() {}
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
		//std::cout << "next v " << v << std::endl;
        if (v < 0) return 0;
		int ans = 0;
		int res = 0;
        if (bigbor[v].isLeaf) { isVisited[v] = 2; return 0; }
        if (isVisited[v] == 1) { return 1; }
        if (isVisited[v] == 2) { return answer[v]; }

        isVisited[v] = 1;
        for (int j = 0; j< 2; j++) {
            //	std::cout << "go " << go(v, (char)j + '0') << std::endl;
            res = dfs(bigbor[v].go[j]);
            //	std::cout << "rez " << res << " from " << go(v, (char)j + '0') << " to " << v << std::endl;
            if (res == 1) { return 1; }
		}
		answer[v] = ans;
		isVisited[v] = 2;
		return ans;
	}
	void zero_bfs(){
        for(int i = 0; i < bigbor.size();i++){
            for(int j = 0; j < 2; j++){
                bigbor[i].go[j] = go(i, (char)j + '0');
            }
        }
	}
	void second_bfs() {
        int v = 0;
        int r;
        std::deque <int> myQueue;
		myQueue.push_back(0);
		while(!myQueue.empty()){
            v = myQueue.front();
            myQueue.pop_front();
            for (int j = 0; j< 2; j++) {
                r = bigbor[v].link;
                if( bigbor[r].isLeaf == 1){
                    bigbor[v].isLeaf = 1;
                }
            }
            for (int j = 0; j< 2; j++) {
                if(bigbor[v].rfr[j]!= -1)
                    myQueue.push_back(bigbor[v].rfr[j]);
            }
		}
	}

	void initialize() {
		zero_bfs();
		second_bfs();
		isVisited.resize(bigbor.size(), 0);
		answer.resize(bigbor.size(), 0);
	}
};
int main()
{
	int myCount;
	myBor newBor;

	scanf("%i", &myCount);
	std::string myStr;
	myStr.resize(300000);
	for (int i = 0; i < myCount; i++) {
		scanf("%s", &myStr[0]);
		newBor.add(myStr);
	}
	newBor.initialize();
	if (newBor.dfs(0) == 1) {
		std::cout << "TAK";
	}
	else {
		std::cout << "NIE";
	}
	return 0;
}
