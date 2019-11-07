#include <mutex>
#include <condition_variable>
using namespace std;

class semaphore {
private:
    mutex mutex_;
    condition_variable cond_;
    int count_;

public:
    semaphore(int count = 0) : count_(count) {}

    inline void notify() {
        lock_guard<mutex> lock(mutex_);
        ++count_;
        cond_.notify_one();
    }

    inline void wait() {
        unique_lock<mutex> lock(mutex_);
        while (count_ == 0) {
            cond_.wait(lock);
        }
        --count_;
    }
};
