#include "thread.h"

int x = 0;

void sharedGlobal(int id) { 
    // prevent competition
    usleep(id * 100000);

    printf("Hello from thread $%c\n", "123456789ABCDEF"[x++]);
}

int main() {
    for (int i=0; i<10; i++) {
        create(sharedGlobal);
    }
}
