#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

int main()
{
    std::string myStr;
    std::cin >> myStr;
    int n = myStr.length();

    std::reverse(myStr.begin(), myStr.end());

 	std::vector<int> zet(n);
	int l = 0;
	int r = 0;
	for (int i = 1; i < n; ++i) {
		if (i <= r)
			zet[i] = std::min(r - i + 1, zet[i - l]);
		while ((i + zet[i] < n) && (myStr[zet[i]] == myStr[i + zet[i]])){
			zet[i]++;
		}
		if (i+zet[i]-1 > r){
			l = i;
            r = i + zet[i] - 1;
		}
	}

    std::reverse(myStr.begin(), myStr.end());
    int ans = 0;
    int min_lenght = n;
    int tek_min;
    int start = 0;
	int q = 1;
	while(( q < n - 2)&&(zet[q] == 0)){
        q++;
	}
	if( q != n - 2){
        ans = q;
      //  std::cout << " ans = " << ans <<std::endl;
        min_lenght = n - zet[q];
       // std::cout << " min_l = " << min_lenght <<std::endl;
        start = n - zet[q] - ans;
       // std::cout << " start = " << start <<std::endl;
        q = q++;
       // std::cout << " q = " << q <<std::endl;
	}
	//pervoe priblizenie k periody
	while( q < n - 2){
        while(( q < n - 2)&&(zet[q] == 0)){
            q++;
        }
        if( q == n-2) break;
        tek_min = n - zet[q];
        if( tek_min < min_lenght){
            ans = q;
           // std::cout << " ans = " << ans <<std::endl;
            min_lenght = n - zet[q];
         //   std::cout << " min_l = " << min_lenght <<std::endl;
            start = n - zet[q] - ans;
          //  std::cout << " start = " << start <<std::endl;
        }
        q++;
	}
//	std::cout << "abacaba123\n";
	if(ans == 0){
        std::cout << myStr[0] << ".(";
        for(int i = 2; i < n; i++){
            std::cout<< myStr[i];
        }
        std::cout << ")";
	}
	else{
        for(int i = 0; i < start; i++){
            std::cout << myStr[i];
        }
        std::cout << "(";
        for(int i = 0; i < ans; i++){
            std::cout << myStr[start + i];
        }
        std::cout << ")";
	}


    return 0;
}
