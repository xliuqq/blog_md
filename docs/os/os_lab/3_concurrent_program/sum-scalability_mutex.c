#include "thread.h"
#include "thread-sync.h"

#define N 10000000
mutex_t lock = MUTEX_INIT();
 
long n, sum = 0;

void Tsum() {
  for (int i = 0; i < n; i++) {
    mutex_lock(&lock);
    sum++;
    mutex_unlock(&lock);
  }
}

int main(int argc, char *argv[]) {
  assert(argc == 2);
  int nthread = atoi(argv[1]);
  n = N / nthread;
  for (int i = 0; i < nthread; i++) {
    create(Tsum);
  }
  join();
  assert(sum == n * nthread);
}
