
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <set>
#include <vector>
#include <string>
#include <stdio.h>

class mySuffmas {

	int max_number_end;
	long long int max_count;
	int max_len;
	int n;
	int m;
	std::vector <int> myStr;
	std::vector <int> suff;
	std::vector <int> lcp;
	std::vector <int> rang;

	void remember(int a, long long int num, int q) {
		if (num > max_count) {
			max_number_end = a;
			//std::cout << " end " << a << std:: endl;
			max_count = num;
			//std::cout << " num " <<  num << std::endl;
			max_len = q;
			//std::cout << " len " << q << std:: endl;
		}
	}

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
		int last_el = 0;
		int count_last_el = 0;
		max_number_end = 0;
		max_len = n;
		max_count = n;

		for (int i = 0; i < n; i++){
			if (lcp[i] > last_el){
                last_el = lcp[i];
				my_stack.push_back(lcp[i]);
				my_counts.push_back(1);
				remember(suff[i], (long long int)last_el*2, last_el);
			}
			else {
                last_el = lcp[i];
                count_last_el = 0;
                while ((my_stack.size()> 0) && (my_stack.back() >= last_el)) {
                    count_last_el += my_counts.back();
                    remember(suff[i - 1], (long long int)(count_last_el + 1) * my_stack.back(), my_stack.back());
                    my_stack.pop_back();
                    my_counts.pop_back();
                }
                count_last_el++;
                my_stack.push_back(last_el);
                my_counts.push_back(count_last_el);
                remember(suff[i], (long long int)(count_last_el + 1)* last_el, last_el);
                if(last_el == 0) {
                    count_last_el = 0;
                    my_stack.clear();
                    my_counts.clear();
                }
			}
        }
	}
	void printAnswer() {
		printf("%lli\n", max_count);
		printf("%i\n", max_len);
		//printf("%i\n", max_number_end);
		for (int i = 0; i < max_len; i++) {
		    int r = max_number_end + i;
            //printf("%i\n",r );
			printf("%i ", myStr[r]);
		}
	}

public:
	mySuffmas() {

		int tmp;
		scanf("%i %i", &n, &m);
		suff.resize(n+1, 0);
		lcp.resize(n+1, 0);
		rang.resize(n+1, 0);
		myStr.resize(n+1);
		for (int i = 0; i < n; i++) {
			scanf("%i", &tmp);
			myStr[i] = tmp;
		}
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




