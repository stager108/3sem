

#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <set>
#include <vector>
#include <string>
#include <stdio.h>
class mySuffmas {

	int max_len;
	int n;
	int m;
    long long int sum = 0;
	std::string myStr;
	std::vector <int> suff;
	std::vector <int> lcp;
	std::vector <int> rang;

	void sortCycleShifts() {

	    n++;
	    myStr[n-1] = 0;
		std::vector <int> new_suff(n);
		std::vector<int> cnt(n);
		std::vector <int> new_cnt(n);
		std::vector <int> h;
		int kolc = 256;
		h.assign(256, 0);
		for (int i = 0; i < n; ++i) {
			cnt[i] = myStr[i];
			h[myStr[i]]++;
		}
		for (int i = 1; i < h.size(); ++i) {
			h[i] += h[i - 1];
		}
		for (int i = n - 1; i >= 0; --i) {
			new_suff[--h[cnt[i]]] = i;
		}
		suff.swap(new_suff);
		for (int l = 1; l <= n; l += l) {
			h.assign(kolc, 0);
			for (int i = 0; i < n; ++i) {
				h[cnt[i]]++;
			}
			for (int i = 1; i < kolc; ++i) {
				h[i] += h[i - 1];
			}
			for (int i = n - 1; i >= 0; --i) {
				int v = (suff[i] - l + n) % n;
				new_suff[--h[cnt[v]]] = v;
			}
			suff.swap(new_suff);
			kolc = 1;
			new_cnt[suff[0]] = 0;
			for (int i = 1; i < n; ++i) {
				if ((cnt[suff[i]] == cnt[suff[i - 1]]) && (cnt[(suff[i] + l) % n] == cnt[(suff[i - 1] + l) % n])) {
					new_cnt[suff[i]] = kolc - 1;
				}
				else {
					new_cnt[suff[i]] = kolc++;
				}
			}
			cnt.swap(new_cnt);
		}
		n--;
		for(int i = 0; i < n; i++){
            suff[i] = suff[i+1];
		}
		suff[n] = -1;
	}

	void Kasaua() {
		for (int i = 0; i < n; ++i)
			rang[suff[i]] = i;
		int k = 0;
		for (int i = 0; i < n; ++i)
		{
			if (k > 0) k--;
			if (rang[i] == n - 1){
				lcp[n - 1] = -1;
				k = 0;
			}
			else {
				int j = suff[rang[i] + 1];
				while ((i + k < n) && (j + k < n) && (myStr[i + k] == myStr[j + k]))
					k++;
				lcp[rang[i]] = k;
			}
		}
	}
	void getAnswer() {
		lcp[n-1] = 0;
		std::vector <int> my_stack;
		std::vector <int> my_counts;
        int min_num;
        int max_num;
        int prev = 0;
        int e;
        int i = 0;
		for (int i = 1; i < n; i++){
            int j  = 0;
            //std::cout << " begin solve "<< i << std::endl;
            while( j < n-1){
                while((j < n -1)&&(lcp[j] < i)) j++;
                if(j < n-1) {
                    min_num = suff[j];
                    max_num = suff[j+1];
                    while(lcp[j] >= i){
                        if(min_num > suff[j]){
                            min_num = suff[j];
                        }
                        if(min_num > suff[j+1]){
                            min_num = suff[j+1];
                        }
                        if(max_num < suff[j]){
                            max_num = suff[j];
                        }
                        if(max_num < suff[j+1]){
                            max_num = suff[j+1];
                        }
                        j++;
                    }
                   // std::cout << "abacaba " << max_num << " "<< min_num <<std::endl;
                    e = max_num - min_num;
                    if( e >= i){
                        sum ++;
                    }
                }
            }
        }
	}
	void printAnswer() {
		printf("%lli\n", sum);
	}

public:
	mySuffmas() {

		int tmp;
        myStr.resize(3001);
        scanf("%s", &myStr[0]);
        n = strlen(myStr.c_str());
		suff.resize(n+1, 0);
		lcp.resize(n+1, 0);
		rang.resize(n+1, 0);
		myStr.resize(n+1);

	}
	~mySuffmas() {}

	void solve() {
		sortCycleShifts();
		Kasaua();
		getAnswer();
		printAnswer();
	}
};
int main() {
	mySuffmas newS;
	newS.solve();
	return 0;
}




