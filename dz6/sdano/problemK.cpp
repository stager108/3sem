#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <set>
#include <vector>
#include <string>
#include <stdio.h>
int main()
{
    int n;
    int tmp;
    scanf("%i", &n);
    std::vector <int> suff(n);
    std::vector <int> lcp(n);
    std::vector <int> rang(n);
    std::string myStr;
    myStr.resize(n);
    scanf("%s", &myStr[0]);
    for(int i = 0; i < n; i++){
        scanf("%i", &tmp);
        suff[i] = tmp - 1;
    }
	for (int i = 0; i < n; ++i)
        rang[suff[i]] = i;
    int k = 0;
	for (int i = 0; i < n; ++i)
	{
		if (k > 0) k--;
		if (rang[i] == n - 1)
            {
                lcp[n - 1] = -1;
                k = 0;
            }
        else{
            int j = suff[rang[i] + 1];
            while ((i + k < n )&&(j + k < n)&& (myStr[i + k] == myStr[j + k]))
                k++;
            lcp[rang[i]] = k;
        }
	}
    for(int i = 0; i < n - 1; i++)
        printf("%i ", lcp[i]);
    return 0;
}
