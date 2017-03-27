#include <mutex>
#include <condition_variable>

class barrier {
public:
    explicit barrier(size_t num_threads) {
         n = num_threads;
         virtualGate1 = 0;
         virtualGate2 = 0;
    }

    void enter() {
        std::unique_lock<std::mutex> lock(m);

        if (virtualGate1 != n - 1) {
            ++virtualGate1;
            cv.wait(lock, [this](){ return virtualGate1 == 0; });
        }
        else {
            virtualGate1 = 0;
            cv.notify_all();
        }

        if (virtualGate2 != n - 1) {
            ++virtualGate2;
            cv.wait(lock, [this](){ return virtualGate2 == 0; });
        }
        else {
            virtualGate2 = 0;
            cv.notify_all();
        }

    }

private:
    std::mutex m;
    std::condition_variable cv;
    size_t n, virtualGate1, virtualGate2;
};
