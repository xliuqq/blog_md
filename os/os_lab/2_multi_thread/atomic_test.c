#include "thread.h"

#define N 100000000
long sum = 0;

void Tsum() { 
  for (int i = 0; i < N; i++) {
    // 汇编语句的结果也是错的
    asm volatile("add $1, %0": "+m"(sum));
    // sum++; 
  }
}

int main() {
  create(Tsum);
  create(Tsum);
  join();
  printf("sum = %ld\n", sum);
}