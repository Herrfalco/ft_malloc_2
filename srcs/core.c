/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 17:59:23 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/18 19:05:51 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/core.h"

static g_zones_t		g_zones = {
	.mut = PTHREAD_MUTEX_INITIALIZER,
};

static void		glob_lock(fn_typ_t fn, void *dat1, void *dat2) {
	pthread_mutex_lock(&g_zones.mut);
	if (!g_zones.log)
		g_zones.log = open(getenv(ENV_FLAG),
			O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (g_zones.log > 0) {
		switch (fn) {
			case FT_MLC:
				print_str(g_zones.log, "  malloc(");
				print_nb_base(g_zones.log, *((uint64_t *)dat1), DEC);
				print_str(g_zones.log, ") = ");
				break;
			case FT_FRE:
				print_str(g_zones.log, "  free(");
				print_nb_base(g_zones.log, (uint64_t)dat1, HEX);
				print_str(g_zones.log, ")\n");
				break;
			case FT_RLC:
				print_str(g_zones.log, "  realloc(");
				print_nb_base(g_zones.log, (uint64_t)dat1, HEX);
				print_str(g_zones.log, ", ");
				print_nb_base(g_zones.log, *((uint64_t *)dat2), DEC);
				print_str(g_zones.log, ") = ");
				break;
			default:
				break;
		}
	}
}

static void		*glob_unlock(fn_typ_t fn, void *dat) {
	if (g_zones.log > 0) {
		switch (fn) {
			case FT_MLC:
				print_nb_base(g_zones.log, (uint64_t)dat, HEX);
				print_ln(g_zones.log);
				break;
			case FT_FRE:
				break;
			case FT_RLC:
				print_nb_base(g_zones.log, (uint64_t)dat, HEX);
				print_ln(g_zones.log);
				break;
			default:
				break;
		}
	}
	pthread_mutex_unlock(&g_zones.mut);
	return (dat);
}

static void		*malloc_no_lock(uint64_t size) {
	if (!size)
		return (NULL);
	zone_init(&g_zones.tiny, TINY_CELL_SZ);
	zone_init(&g_zones.small, SMALL_CELL_SZ);
	if (size <= TINY_CELL_SZ
			&& zone_inited(&g_zones.tiny)
			&& !zone_full(&g_zones.tiny))
		return (zone_alloc(&g_zones.tiny, size));
	if (size <= SMALL_CELL_SZ
			&& zone_inited(&g_zones.small)
			&& !zone_full(&g_zones.small))
		return (zone_alloc(&g_zones.small, size));
	return (big_zone_alloc(&g_zones.big, size));
}

void	*malloc(uint64_t size) {
	glob_lock(FT_MLC, &size, NULL);
	return (glob_unlock(FT_MLC, malloc_no_lock(size)));
}

static void		free_no_lock(void *ptr) {
	if (!ptr)
		return;
	if (zone_ptr_in(&g_zones.tiny, ptr))
		zone_free(&g_zones.tiny, ptr);
	else if (zone_ptr_in(&g_zones.small, ptr))
		zone_free(&g_zones.small, ptr);
	else if (big_zone_ptr_in(&g_zones.big, ptr))
		big_zone_free(&g_zones.big, ptr);
}

void	free(void *ptr) {
	glob_lock(FT_FRE, ptr, NULL);
	free_no_lock(ptr);
	glob_unlock(FT_FRE, NULL);
}

static void		mem_cpy(void *dst, void *src, uint64_t size) {
	uint64_t		i;

	for (i = 0; i < size; ++i)
		((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
}

void	*realloc(void *ptr, uint64_t size) {
	void		*new = NULL;
	uint64_t	o_sz;

	glob_lock(FT_RLC, ptr, &size);
	if (!size) {
		if (ptr)
			free_no_lock(ptr);
		return (glob_unlock(FT_RLC, NULL));
	} else if (!ptr)
		return (glob_unlock(FT_RLC, malloc_no_lock(size)));
	if (zone_ptr_in(&g_zones.tiny, ptr))
		o_sz = zone_csize(&g_zones.tiny, ptr);
	else if (zone_ptr_in(&g_zones.small, ptr))
		o_sz = zone_csize(&g_zones.small, ptr);
	else if (big_zone_ptr_in(&g_zones.big, ptr))
		o_sz = big_zone_csize(ptr);
	else
		return (glob_unlock(FT_RLC, NULL));
	if (!(new = malloc_no_lock(size)))
		return (glob_unlock(FT_RLC, NULL));
	mem_cpy(new, ptr, o_sz < size ? o_sz : size);
	free_no_lock(ptr);
	return (glob_unlock(FT_RLC, new));
}

static void		sort_zones(zone_typ_t *zones, void **starts) {
	int				i, j;

	for (j = ZONE_NB - 1; j; --j) {
		for (i = 0; i < j; ++i) {
			if (starts[zones[i]] > starts[zones[i + 1]]) {
				zones[i] += zones[i + 1];
				zones[i + 1] = zones[i] - zones[i + 1];
				zones[i] -= zones[i + 1];
			}
		}
	}
}

void	show_alloc_mem(void) {
	int				i;
	zone_typ_t		zones[] = { ZT_TINY, ZT_SMALL, ZT_BIG };
	void			*starts[ZONE_NB];


	glob_lock(FT_SHW, NULL, NULL);
	print_str(1, LINE_STR);
	starts[0] = zone_start(&g_zones.tiny);
	starts[1] = zone_start(&g_zones.small);
	starts[2] = big_zone_start(&g_zones.big);
	sort_zones(zones, starts);
	for (i = 0; i < ZONE_NB; ++i) {
		if (!starts[zones[i]])
			continue;
		switch (zones[i]) {
			case ZT_TINY:
				print_str(1, "TINY : ");
				print_nb_base(1, (uint64_t)starts[ZT_TINY], HEX);
				print_ln(1);
				zone_print(&g_zones.tiny);
				break;
			case ZT_SMALL:
				print_str(1, "SMALL : ");
				print_nb_base(1, (uint64_t)starts[ZT_SMALL], HEX);
				print_ln(1);
				zone_print(&g_zones.small);
				break;
			default:
				print_str(1, "BIG : ");
				print_nb_base(1, (uint64_t)starts[ZT_BIG], HEX);
				print_ln(1);
				big_zone_print(&g_zones.big);
		}
	}
	glob_unlock(FT_SHW, NULL);
}

void	show_alloc_hist(void) {
	char		buff[BUFF_SZ];
	ssize_t		ret;

	glob_lock(FT_HST, NULL, NULL);
	print_str(1, LINE_STR);
	if (g_zones.log > 0) {
		print_str(1, "HIST:\n");
		if (lseek(g_zones.log, 0, SEEK_SET) < 0)
			print_str(2, "Error: Can't acces history file\n");
		else {
			while ((ret = read(g_zones.log, buff, BUFF_SZ)) > 0)
				write(1, buff, ret);
			if (ret < 0)
				print_str(2, "Error: Can't read history file\n");
			if (lseek(g_zones.log, 0, SEEK_END) < 0)
				print_str(2, "Error: Can't acces history file\n");
		}
	} else
		print_str(2, "Error: No log file (please export LOG_FILE variable)\n");
	glob_unlock(FT_HST, NULL);
}

void	zones_dtor(void) {
	zone_dest(&g_zones.tiny);
	zone_dest(&g_zones.small);
	if (g_zones.log > 0)
		close(g_zones.log);
}
