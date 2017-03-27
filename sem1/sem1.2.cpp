#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>


std::atomic <int> a;
int N = 10000;
std::mutex mut;


int main(){
    std::vector < std::thread > my_th;
    my_th.clear();
    for(int i=0; i < std::thread::hardware_concurrency(); i++){//maximum quantity of parallel process
        my_th.emplace_back([](){//lambda
            for ( int i = 0; i < N; i++){
                a++;
             }
        });
    }
    for(int i=0; i < std::thread::hardware_concurrency(); i++){
        my_th[i].join();//to finish process
    }
    std::cout << a << std::endl;
    my_th.clear();
    return 0;
}
