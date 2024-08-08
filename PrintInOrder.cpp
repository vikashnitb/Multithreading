#include <bits/stdc++.h>
using namespace std;

class Foo {

    private:
        mutex m;
        condition_variable cv; // To sync the execution sequence of the threads
        int turn; // Shared resource

    public:
        
        Foo() : turn(0) {}

        void first(function<void()> printFirst) {
            unique_lock<mutex> lock(m);
            turn = 1;
            printFirst();
            cv.notify_all();
        }

        void second(function<void()> printSecond) {
            unique_lock<mutex> lock(m);
            cv.wait(lock, [this]() { return turn == 1; });
            printSecond();
            turn = 2;
            cv.notify_all();
        }

        void third(function<void()> printThird) {
            unique_lock<mutex> lock(m);
            cv.wait(lock, [this]() { return turn == 2; });
            printThird();
        }
        void print(const std::string& s) {
            std::cout << s << std::endl;
        }
};

int main() {
    Foo foo;

    // Define lambda functions to pass as arguments
    auto printFirst = [&foo]() { foo.print("first"); };
    auto printSecond = [&foo]() { foo.print("second"); };
    auto printThird = [&foo]() { foo.print("third"); };

    // Create threads and pass the lambda functions
    std::thread threadA([&foo, printFirst]() { foo.first(printFirst); });
    std::thread threadB([&foo, printSecond]() { foo.second(printSecond); });
    std::thread threadC([&foo, printThird]() { foo.third(printThird); });

    threadA.join();
    threadB.join();
    threadC.join();

    return 0;
}
