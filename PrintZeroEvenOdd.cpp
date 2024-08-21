// https://leetcode.com/problems/print-zero-even-odd/

#include <bits/stdc++.h>
using namespace std;

class ZeroEvenOdd {
private:
    int n;
    condition_variable cv;
    mutex m;
    int turn;

public:
    ZeroEvenOdd(int n) : turn{0} {
        this->n = n;
    }

    // printNumber(x) outputs "x", where x is an integer.
    void zero(function<void(int)> printNumber) {
        for(int i=1; i<=n; ++i) {
            unique_lock<mutex> lock(m);
            while (turn!=0) cv.wait(lock);
            printNumber(0);
            if(i&1) turn=1;
            else turn=2;
            cv.notify_all();
        }
    }

    void even(function<void(int)> printNumber) {
        for(int i=2; i<=n; i+=2) {
            unique_lock<mutex> lock(m);
            while(turn!=2) cv.wait(lock);
            printNumber(i);
            turn = 0;
            cv.notify_all();
        }
    }

    void odd(function<void(int)> printNumber) {
        for(int i=1; i<=n; i+=2) {
            unique_lock<mutex> lock(m);
            while(turn!=1) cv.wait(lock);
            printNumber(i);
            turn = 0;
            cv.notify_all();
        }
    }
};

void printNumber(int x) {
    std::cout << x << " ";
}

int main() {
    int n = 5; // Change this value as needed

    ZeroEvenOdd zeroEvenOdd(n);

    std::thread t1(&ZeroEvenOdd::zero, &zeroEvenOdd, printNumber);
    std::thread t2(&ZeroEvenOdd::even, &zeroEvenOdd, printNumber);
    std::thread t3(&ZeroEvenOdd::odd, &zeroEvenOdd, printNumber);

    t1.join();
    t2.join();
    t3.join();

    std::cout << std::endl;

    return 0;
}