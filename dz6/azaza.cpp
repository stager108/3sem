
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
std::vector<int> z_function (std::string str) {
	int n = s.length();
	std::vector<int> zet(n);
	int l = 0;
	int r = 0;
	for (int i = 1; i < n; ++i) {
		if (i <= r)
			zet[i] = std::min(r - i + 1, zet[i - l]);
		while ((i + zet[i] < n) && (str[zet[i]] == str[i + zet[i]])){
			zet[i]++;
		}
		if (i+z[i]-1 > r){
			l = i;
            r = i + zet[i] - 1;
		}
	}
	return z;
}


    pref[0] = 0;
    for(int i =0; i< n; i++){
        int k = pref[i-1];
        while((k>0)&&(myStr[i]!= myStr[k])){
            k = p[k-1];
        }
        if(myStr[i] == myStr[k]){
            k++;
        }
        pref[i] = k;
    }



int main()
{
    return 0;
}
