#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>


int a;
int N = 10000;
std::mutex mut;
std:: atomic_int atomic_global;

//solution: put useful operations into the destructor
/*class MyClass{
    std::mutex* my_mut;
public:
    MyClass( std::mutex &mut ){
        my_mut = &mut;
        my_mut->lock();
    }
    ~MyClass(){
        my_mut->unlock();
    }
};*/

class MyClass{
    std::mutex &my_mut;
public:
    MyClass( std::mutex &mut ):  my_mut(mut)// why does it work???
    {
        my_mut.lock();
    }

    ~MyClass(){
        my_mut.unlock();
    }
};

void incr(){
    for(int i = 0; i < N; i++){
        mut.lock();
        a++;//what if fail? no unlock; horrible situation with other processes
        atomic_global.load();//zagruzit!
        //atomic - that's mean compilation can't optimize code, and change operation's places
        //operation is a whole block
        mut.unlock();
    }
}

int main(){
    std::vector < std::thread > my_th;
    my_th.clear();
    for(int i=0; i < std::thread::hardware_concurrency(); i++){//maximum quantity of parallel process
        my_th.emplace_back([](){//lambda
            MyClass MC(mut);
            for ( int i = 0; i < N; i++){
                //mut.lock();
                a++;
                //mut.unlock();
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
