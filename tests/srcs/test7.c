#include <stdlib.h>
#include "../../lib.h"

int main() {
	int		i;
	uint8_t	*ptr[] = {
		malloc(1024),
		malloc(1024 * 2),
		malloc(1024 * 32),
		malloc(1024 * 1024),
		malloc(1024 * 1024 * 16),
		malloc(1024 * 1024 * 128),
	};

    show_alloc_mem(); 
	for (i = 0; i < 6; ++i)
		ptr[i] = realloc(ptr[i], 4096 / (i + 1));
    show_alloc_mem(); 
	for (i = 0; i < 120; ++i)
		ptr[5][i] = i;
	show_alloc_dump(ptr[5], 10);
	show_alloc_dump(ptr[5], 45);
	show_alloc_dump(ptr[5], 0);
	for (i = 0; i < 6; ++i)
		free(ptr[i]);
	show_alloc_hist();
    return (0); 
}
