#include "lock_free_queue.h"

int main(){
    lock_free_queue<int> lfq;
    lfq.enqueue(1);
    int a;
    lfq.dequeue(a);

    return 0;
}
