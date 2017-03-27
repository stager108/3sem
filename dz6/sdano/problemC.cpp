#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <queue>
#include <limits.h>
#include <stdio.h>
int main()
{

    std::string myStr;
    myStr.resize(100000);
    scanf("%s", &myStr[0]);
    int m;
    int a1, a2, b1, b2;
    int n = myStr.length();
    std::vector <int> myHash(n);
    int p = 179;
    std::vector <int> stp (n);
    stp[0] = 1;
    for(int i = 1; i < n; i++){
        stp[i] = stp[i - 1] * p;
    }

    myHash[0] = stp[0] * myStr[0];
    for (int i = 1; i < n; i++) {
        myHash[i] = myHash[i - 1] + stp[i] * myStr[i];
    }

    std::cin >> m;
    for(int i = 0; i < m; i++){
        scanf( "%i %i %i %i",&a1, &a2, &b1, &b2);
        a1--;
        a2--;
        b1--;
        b2--;
        int ans1 = myHash[a2];
        if( a1 > 0) ans1 -= myHash[a1 - 1];
        int ans2 = myHash[b2];
        if( b1 > 0 ) ans2 -= myHash[b1 - 1];
        ans1 *= stp[b1];
        ans2 *= stp[a1];
        if( ans1 == ans2){
            printf("%s\n", "Yes");
        }
        else{
            printf("%s\n", "No");
        }
    }
    return 0;
}
