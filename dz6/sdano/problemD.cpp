
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
class solve{
public:
    solve(){}
    ~solve(){}

    void sol(){
        int max_len = read_string();
        int k = 0;
        last_ex.resize(max_len, 0);
        prefix.resize(max_len, 0);
        comp.resize(max_len, 0);
        for (int i = 1; i < max_len; i++) {
            k = prefix_function(i);
            if(k == 0){
                last_ex[i] = i;
                comp[i] = i;
            }
            else{
                if (last_ex[comp[k - 1]] + comp[k - 1] + 1 >= i){
                    comp[i] = comp[k - 1];
                    last_ex[comp[i]] = i;
                } else {
                    last_ex[i] = i;
                    comp[i] = i;
                }
            }
        }
        sz = max_len;
        write_string();
    }

private:
std::string myStr;
std::vector<long long int> comp;
std::vector <int> prefix;
std::vector <int> last_ex;
int sz;
    int read_string(){
        myStr.resize(250000);
        scanf("%s", &myStr[0]);
        int m = strlen(myStr.c_str());
        return m;
    }
    void write_string(){
        for (int i = 0; i < sz; i++) {
            comp[i]++;
            printf("%i ", comp[i]);
        }
    }
    int prefix_function(int i){
        int k = prefix[i-1];
        while ((k > 0) && (myStr[i] != myStr[k])) {
            k = prefix[k - 1];
        }
        if (myStr[i] == myStr[k]) {
            k++;
        }
        prefix[i] = k;
        return k;
    }

};
int main(){
    solve nS;
    nS.sol();
}
