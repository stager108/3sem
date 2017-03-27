#include <mutex>
#include <condition_variable>
#include <queue>
#include <exception>
#include <thread>
#include <limits>
#include <future>
#include <atomic>
#include <algorithm>
#include <functional>
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

    bool pop_if_can(Value& v){
        std::unique_lock<std::mutex> lock(m);
        if(is_closed_flag == 1){
            if(now_size == 0){
                throw std::exception();
            }
        }
        else{
            if(now_size == 0){
                    return false;
            }
        }
        v = std::move(general_queue.front());
        general_queue.pop_front();
        now_size--;
        cv1.notify_all();
        return true;
    }

    void shutdown(){
        is_closed_flag = 1;
        cv2.notify_all();
        cv1.notify_all();
    }
    bool empty(){
        return general_queue.empty();
    }
private:
    std::mutex m;
    std::condition_variable cv1, cv2;
    std::atomic <int> is_closed_flag;
    size_t max_size, now_size;
    Container general_queue;
};

template <class Value>
class thread_pool {
public:

    explicit thread_pool(size_t num_workers){
        for(size_t i = 0; i < num_workers; i++){
            threads_vector.emplace_back( std::thread([this](){
                                    std::pair <std::packaged_task<Value()>, bool> task;
                                        while(true)
                                        {
                                            local_queue.pop(task);
                                            if(task.second == true){
                                                break;
                                            }
                                            task.first();
                                        }
                                    }
            ));
        }
    }

    std::future<Value> submit(std::function<Value()> newFunction){
        std::packaged_task<Value()> task( newFunction );
        std::future<Value> package_future = task.get_future();
        local_queue.enqueue(std::make_pair(std::move(task), false));
        return package_future;
    }

    void shutdown(){
        for(size_t i = 0; i < threads_vector.size(); i++ ){
            local_queue.enqueue(std::make_pair(std::packaged_task<Value()> (),true));
        }
        local_queue.shutdown();
        for(size_t i = 0; i < threads_vector.size(); i++ ){
            threads_vector[i].join();
        }

    }

    void wait(std::future<Value> &async_result){
        while (async_result.wait_for(std::chrono::duration<int>(0)) != std::future_status::ready) {
            std::pair <std::packaged_task<Value()>, bool> task;
            if(local_queue.pop_if_can(task)){
                task.first();
            }
            else{
                break;
            }
        }
        async_result.wait();
    }

private:
    thread_safe_queue <std::pair <std::packaged_task<Value()>, bool>, std::deque<std::pair <std::packaged_task<Value()>, bool>> > local_queue;
    std::vector < std::thread > threads_vector;
};


class parallel_sort {
    const long int maximum_sorted_size = 1500;
public:
    parallel_sort(size_t num_workers):task_queue(num_workers) {}
    ~parallel_sort() { task_queue.shutdown(); }

template < class RandomAccessIterator, class Compare>
void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
        std::vector<typename RandomAccessIterator::value_type> buffer(last-first);
        inner_sort(first, last, comp, buffer);
    }

private:
    std::mutex mut;

template < class RandomAccessIterator, class Compare>
void inner_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp, std::vector<typename RandomAccessIterator::value_type>& buffer) {

        if (last-first < maximum_sorted_size) {
            std::sort(first,last, comp);
            return;
        }
        RandomAccessIterator middle = first + (last - first)/2;
        auto sort_left_part = std::bind(&parallel_sort::inner_sort<RandomAccessIterator,Compare> , this, first, middle, comp, std::ref(buffer)) ;
        auto sort_right_part = std::bind(&parallel_sort::inner_sort<RandomAccessIterator,Compare> , this, middle,last, comp, std::ref(buffer));

        auto left_part_sorted = task_queue.submit(sort_left_part);
        sort_right_part();
        task_queue.wait(left_part_sorted);
        mut.lock();
        std::merge(first, middle, middle, last, buffer.begin() );
        std::swap_ranges(first, last, buffer.begin());
        mut.unlock();
    }
    thread_pool<void> task_queue;
};
