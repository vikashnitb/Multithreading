#include <bits/stdc++.h>
using namespace std;

deque<int> q;
mutex m;

void producer() {
    for(int i=0; i<10; ++i) {
        unique_lock<mutex> lock(m);
        q.push_front(i);
        cout << "Produced " << i << endl;
        lock.unlock();
    }
}

void consumer() {
    while(true) {
        unique_lock<mutex> lock(m);
        if (!q.empty()) {
            int x = q.back();
            q.pop_back();
            lock.unlock();
            cout << "Consumed " << x << endl;
        } else {
            lock.unlock();
        }
    }
}


int main() {
    thread t1(producer);
    thread t2(consumer);
    
    t1.join();
    t2.join();
    return 0;
}