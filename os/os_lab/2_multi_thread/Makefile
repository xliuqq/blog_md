
all: test_shm test_stack test_atomic test_mem_ordering test_peterson

test_shm:
	gcc shm_test.c -lpthread -o test_shm.out

test_stack:
	gcc stack_test.c -lpthread -o test_stack.out

test_atomic:
	gcc atomic_test.c -lpthread -o test_atomic.out

test_mem_ordering:
	gcc mem_ordering_test.c -lpthread -o test_mem_ordering.out

test_peterson:
	gcc peterson_test.c -lpthread -o test_peterson.out
	
clean:
	rm -rf *.out