#include <stdio.h>     /* printf */
#include <stdint.h>    /* intptr_t */
#include <string.h>    /* memcpy */
#include <sys/mman.h>  /* mmap */
#include <fcntl.h>     /* open */

	void
print_func(void(*func_ptr)(), intptr_t func_size)
{
	for (intptr_t i = 0; i < func_size; i++) {
		printf("%02x ", ((unsigned char*)func_ptr)[i]);
	}
	printf("\n");
}

	intptr_t
funclen(void(*func_ptr)())
{
	/* only works for some functions on some machines with some compilers */
	unsigned char *p = (unsigned char *) func_ptr;
	for (intptr_t i = 0; ; i++){
		if (p[i] == 0xc3)
			return i + 1;
	}
}

	int
func(int a, int b, int c)
{
	return a * b + c;
}

	int
main()
{
	char *copy;
	int(*func_ptr)(int, int, int);
	intptr_t func_size;
	int zfd;

	func_size = funclen(&func);

	printf("sizeof func: %lu\n", func_size);

	zfd = open("/dev/zero", O_RDWR);
	copy = mmap(
			NULL,
			func_size,
			PROT_WRITE | PROT_EXEC | PROT_READ,
			MAP_PRIVATE,
			zfd,
			0
		   );

	printf("func: %p\n", func);
	printf("copy: %p\n", copy);

	memcpy(copy, &func, func_size);

	func_ptr = (int(*)(int, int, int))copy;

	print_func(&func, func_size);
	print_func(func_ptr, func_size);

	printf("func result: %d\n", func(3, 2, 3));
	printf("copy result: %d\n", func_ptr(3, 2, 3));

	munmap(copy, func_size);
}

