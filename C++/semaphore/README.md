# 使用 C++ 并发库封装信号量（semaphore）

C++11 和 Boost.Thread 都没有提供信号量。对此 Boost 是这样解释的（[Why has class semaphore disappeared?](http://www.boost.org/doc/libs/1_31_0/libs/thread/doc/faq.html)）：

> Semaphore was removed as too error prone. The same effect can be achieved with greater safety by the combination of a mutex and a condition variable. Dijkstra (the semaphore's inventor), Hoare, and Brinch Hansen all depreciated semaphores and advocated more structured alternatives. In a 1969 letter to Brinch Hansen, Wirth said "semaphores ... are not suitable for higher level languages." [Andrews-83] summarizes typical errors as "omitting a **P** or a **V**, or accidentally coding a **P** on one semaphore and a **V** on on another", forgetting to include all references to shared objects in critical sections, and confusion caused by using the same primitive for "both condition synchronization and mutual exclusion".

简单来说，就是信号量太容易出错了（too error prone），通过组合互斥锁（mutex）和条件变量（condition variable）可以达到相同的效果，且更加安全。

## 简单的实现

```c++
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
```

## 完整实现

```c++
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
```

