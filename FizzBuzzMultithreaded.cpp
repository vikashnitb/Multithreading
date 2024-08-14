#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

using namespace std;

class FizzBuzz {
private:
    int n;
    mutex m;
    condition_variable c;
    int i;

public:
    FizzBuzz(int n) : n(n), i(1) {}

    // printFizz() outputs "fizz".
    void fizz(function<void()> printFizz) {
        while (i <= n) {
            unique_lock<mutex> lock(m);
            while (i <= n && !((i % 3 == 0) && (i % 5 != 0))) {
                c.wait(lock);
            }
            if (i <= n) {
                printFizz();
                ++i;
            }
            c.notify_all();
        }
    }

    // printBuzz() outputs "buzz".
    void buzz(function<void()> printBuzz) {
        while (i <= n) {
            unique_lock<mutex> lock(m);
            while (i <= n && !((i % 5 == 0) && (i % 3 != 0))) {
                c.wait(lock);
            }
            if (i <= n) {
                printBuzz();
                ++i;
            }
            c.notify_all();
        }
    }

    // printFizzBuzz() outputs "fizzbuzz".
    void fizzbuzz(function<void()> printFizzBuzz) {
        while (i <= n) {
            unique_lock<mutex> lock(m);
            while (i <= n && !((i % 3 == 0) && (i % 5 == 0))) {
                c.wait(lock);
            }
            if (i <= n) {
                printFizzBuzz();
                ++i;
            }
            c.notify_all();
        }
    }

    // printNumber(x) outputs "x", where x is an integer.
    void number(function<void(int)> printNumber) {
        while (i <= n) {
            unique_lock<mutex> lock(m);
            while (i <= n && !((i % 3 != 0) && (i % 5 != 0))) {
                c.wait(lock);
            }
            if (i <= n) {
                printNumber(i);
                ++i;
            }
            c.notify_all();
        }
    }
};

void printFizz() {
    cout << "fizz" << endl;
}

void printBuzz() {
    cout << "buzz" << endl;
}

void printFizzBuzz() {
    cout << "fizzbuzz" << endl;
}

void printNumber(int x) {
    cout << x << endl;
}

int main() {
    int n = 15;  // You can change this value to test different ranges

    FizzBuzz fb(n);

    thread t1(&FizzBuzz::fizz, &fb, printFizz);
    thread t2(&FizzBuzz::buzz, &fb, printBuzz);
    thread t3(&FizzBuzz::fizzbuzz, &fb, printFizzBuzz);
    thread t4(&FizzBuzz::number, &fb, printNumber);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
