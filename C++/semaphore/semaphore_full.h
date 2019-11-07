#include <mutex>
#include <condition_variable>
#include <chrono>
using namespace std;

template <class Mutex, class CondVar>
class basic_semaphore {
private:
    Mutex mutex_;
    CondVar cond_;
    size_t count_;

public:
    using native_handle_type = typename CondVar::native_handle_type;

    explicit basic_semaphore(int count = 0) : count_(count) {}

    basic_semaphore(const basic_semaphore&) = delete;
    basic_semaphore(basic_semaphore&&) = delete;
    basic_semaphore& operator=(const basic_semaphore&) = delete;
    basic_semaphore& operator=(basic_semaphore&&) = delete;

    void notify() {
        lock_guard<mutex> lock(mutex_);
        ++count_;
        cond_.notify_one();
    }

    void wait() {
        lock_guard<mutex> lock(mutex_);
        cond_.wait(lock, [&]{
            return count_ > 0;
        });

        --count_;
    }

    bool try_wait() {
        lock_guard<mutex> lock(mutex_);
        if (count_ > 0) {
            --count_;
            return true;
        }

        return false;
    }

    template <class Rep, class Period>
    bool wait_for(const chrono::duration<Rep, Period>& d) {
        unique_lock<Mutex> lock(mutex_);
        auto finished = cond_.wait_for(lock, d, [&]{
            return count_ > 0;
        });

        if (finished) {
            --count_;
        }

        return finished;
    }

    template <class Clock, class Duration>
    bool wait_until(const chrono::time_point<Clock, Duration>& t) {
        unique_lock<Mutex> lock(mutex_);
        auto finished = cond_.wait_until(lock, t, [&]{
            return count_ > 0;
        });

        if (finished) {
            --count_;
        }

        return finished;
    }

    native_handle_type native_handle() {
        return cond_.native_handle();
    }
};

using semaphore = basic_semaphore<mutex, condition_variable>;
