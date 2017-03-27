#include <mutex>
#include <condition_variable>
#include <queue>
#include <exception>
#include <thread>
#include <limits>
#include <future>
template <class Value, class Container >
class thread_safe_queue {
public:
    explicit thread_safe_queue(){
        max_size = std::numeric_limits<int>::max() / 2;
        now_size = 0;
        is_closed_flag = 0;
    }
    explicit thread_safe_queue(size_t capacity){
        max_size = capacity;
        now_size = 0;
        is_closed_flag = 0;
    }
    void enqueue(Value v){
        std::unique_lock<std::mutex> lock(m);
        if(is_closed_flag == 1){
            throw std::exception();
        }
        else{
            if(now_size == max_size){
                cv1.wait(lock, [this](){ return ((now_size < max_size)||(is_closed_flag)); });
            }
        }
        general_queue.push_back(std::move(v));
        now_size++;
        cv2.notify_all();
    }

    void pop(Value& v){
        std::unique_lock<std::mutex> lock(m);
        if(is_closed_flag == 1){
            if(now_size == 0){
                throw std::exception();
            }
        }
        else{
            if(now_size == 0){
                cv2.wait(lock, [this](){ return ((now_size > 0)||(is_closed_flag)); });
            }
        }
        v = std::move(general_queue.front());
        general_queue.pop_front();
        now_size--;
        cv1.notify_all();
    }

    void shutdown(){
        is_closed_flag = 1;
        cv2.notify_all();
        cv1.notify_all();
    }

private:
    std::mutex m;
    std::condition_variable cv1, cv2;
    int is_closed_flag;
    size_t max_size, now_size;
    Container general_queue;
};
