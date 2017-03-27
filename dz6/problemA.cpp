#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
std::vector<short int> z_function (std::string str) {
	int n = s.length();
	std::vector<short int> zet(n);
	short int l = 0;
	short int r = 0;
	for (short int i = 1; i < n; ++i) {
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


int main()
{
    short int n;
    short int tmp;
    std::string myStr;
    myStr.resize(10000);
    scanf("%s", &myStr[0]);






    return 0;
}
