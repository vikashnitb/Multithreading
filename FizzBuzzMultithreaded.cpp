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
    

/*  Parameters Explained
    
    &FizzBuzz::fizz: This is a pointer to the member function fizz of the FizzBuzz class. It tells the thread to execute the fizz method.

    &fb: This is a pointer to the FizzBuzz instance (fb). It is passed to the thread so that the member function fizz will be called on this specific instance of FizzBuzz.

    printFizz: This is a function pointer (or more accurately, a callable object) that is passed as an argument to the fizz method. In this case, printFizz is a function that prints "fizz". It will be used by the fizz method to output the required string when appropriate.
*/

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
