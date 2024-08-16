#include <bits/stdc++.h>
using namespace std;

class H2O {
private:
    mutex m;
    condition_variable cv;
    int hydrogen_count;

public:
    H2O() : hydrogen_count(0) {}

    void hydrogen(function<void()> releaseHydrogen) {
        unique_lock<mutex> lock(m);
        while (hydrogen_count == 2) {
            cv.wait(lock);
        }
        // releaseHydrogen() outputs "H". Do not change or remove this line.
        releaseHydrogen();
        hydrogen_count++;
        if (hydrogen_count == 2) {
            cv.notify_all();
        }
    }

    void oxygen(function<void()> releaseOxygen) {
        unique_lock<mutex> lock(m);
        while (hydrogen_count < 2) {
            cv.wait(lock);
        }
        // releaseOxygen() outputs "O". Do not change or remove this line.
        releaseOxygen();
        hydrogen_count = 0;  // Reset hydrogen count for the next molecule
        cv.notify_all();
    }
};

int main() {
    H2O h2;

    auto releaseHydrogen = [] () {cout << "H";};
    auto releaseOxygen = [] () {cout << "O";};

    vector<thread> threads;

    for (int i = 0; i < 2; ++i) {
        threads.push_back(thread(&H2O::hydrogen, &h2, releaseHydrogen));
    }
    threads.push_back(thread(&H2O::oxygen, &h2, releaseOxygen));

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
