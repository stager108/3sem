#include <vector>
#include <iostream>

int main()
{

    int n;
    std::cin >> n;
    std::vector<int> pref(n);
    std::vector<int> zet(n);
    zet.assign(n, 0);

    for(int i = 0 ; i < n ; i++){
        std::cin >> pref[i];
    }

    for(int i = 1; i < n ; i++){
        if( pref[i] > 0 ){
            zet[i - pref[i] + 1] = pref[i];
        }
    }
    zet[0] = n;

    int i = 1;
    int w;
    while( i < n){
        w = i;
        if ( zet[i] > 0){
            for(int j = 1; j < zet[i]; j++){
                if(zet[i + j] > zet[j]){
                    break;
                }
                zet[i+j] = std::min( zet[j], zet[i] - j);
                w  = i + j;
            }
        }
        i = w + 1;
    }
    for(int  j = 0; j < n; j++){
        std::cout << zet[j] << " ";
    }

    return 0;
}
