all:
		gcc allocator2.c tests.c -o tests
clean:
		rm tests
