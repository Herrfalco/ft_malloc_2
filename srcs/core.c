/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 17:59:23 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/18 14:21:19 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/core.h"

static g_zones_t		g_zones = {
	.mut = PTHREAD_MUTEX_INITIALIZER,
};

static void		*glob_mut_wrapper(mut_op_t mut_op, void *ret) {
	switch (mut_op) {
		case MO_LOCK:
			pthread_mutex_lock(&g_zones.mut);
			break;
		case MO_UNLOCK:
			pthread_mutex_unlock(&g_zones.mut);
			break;
		default:
			;
	}
	return (ret);
}

static void		*malloc_lock(uint64_t size, bool_t lock) {
	if (!size)
		return (NULL);
	glob_mut_wrapper(lock * MO_LOCK, NULL);
	zone_init(&g_zones.tiny, TINY_CELL_SZ);
	zone_init(&g_zones.small, SMALL_CELL_SZ);
	if (size <= TINY_CELL_SZ
			&& zone_inited(&g_zones.tiny)
			&& !zone_full(&g_zones.tiny))
		return (glob_mut_wrapper(lock * MO_UNLOCK,
			zone_alloc(&g_zones.tiny, size)));
	if (size <= SMALL_CELL_SZ
			&& zone_inited(&g_zones.small)
			&& !zone_full(&g_zones.small))
		return (glob_mut_wrapper(lock * MO_UNLOCK,
			zone_alloc(&g_zones.small, size)));
	return (glob_mut_wrapper(lock * MO_UNLOCK,
			big_zone_alloc(&g_zones.big, size)));
}

void	*malloc(uint64_t size) {
	return (malloc_lock(size, TRUE));
}

static void		free_lock(void *ptr, bool_t lock) {
	if (!ptr)
		return;
	glob_mut_wrapper(lock * MO_LOCK, NULL);
	if (zone_ptr_in(&g_zones.tiny, ptr))
		zone_free(&g_zones.tiny, ptr);
	else if (zone_ptr_in(&g_zones.small, ptr))
		zone_free(&g_zones.small, ptr);
	else if (big_zone_ptr_in(&g_zones.big, ptr))
		big_zone_free(&g_zones.big, ptr);
	glob_mut_wrapper(lock * MO_UNLOCK, NULL);
}

void	free(void *ptr) {
	free_lock(ptr, TRUE);
}

static void		mem_cpy(void *dst, void *src, uint64_t size) {
	uint64_t		i;

	for (i = 0; i < size; ++i)
		((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
}

void	*realloc(void *ptr, uint64_t size) {
	void		*new = NULL;
	uint64_t	o_sz;

	if (!ptr && !size)
		return (NULL);
	else if (!ptr)
		return (malloc(size));
	else if (!size) {
		free(ptr);
		return (NULL);
	}
	glob_mut_wrapper(MO_LOCK, NULL);
	if (zone_ptr_in(&g_zones.tiny, ptr))
		o_sz = zone_csize(&g_zones.tiny, ptr);
	else if (zone_ptr_in(&g_zones.small, ptr))
		o_sz = zone_csize(&g_zones.small, ptr);
	else if (big_zone_ptr_in(&g_zones.big, ptr))
		o_sz = big_zone_csize(ptr);
	else
		return (glob_mut_wrapper(MO_UNLOCK, NULL));
	if (!(new = malloc_lock(size, FALSE)))
		return (glob_mut_wrapper(MO_UNLOCK, NULL));
	mem_cpy(new, ptr, o_sz < size ? o_sz : size);
	free_lock(ptr, FALSE);
	return (glob_mut_wrapper(MO_UNLOCK, new));
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


	glob_mut_wrapper(MO_LOCK, NULL);
	starts[0] = zone_start(&g_zones.tiny);
	starts[1] = zone_start(&g_zones.small);
	starts[2] = big_zone_start(&g_zones.big);
	sort_zones(zones, starts);
	for (i = 0; i < ZONE_NB; ++i) {
		if (!starts[zones[i]])
			continue;
		switch (zones[i]) {
			case ZT_TINY:
				print_str("TINY : ");
				print_nb_base((uint64_t)starts[ZT_TINY], HEX);
				print_ln();
				zone_print(&g_zones.tiny);
				break;
			case ZT_SMALL:
				print_str("SMALL : ");
				print_nb_base((uint64_t)starts[ZT_SMALL], HEX);
				print_ln();
				zone_print(&g_zones.small);
				break;
			default:
				print_str("BIG : ");
				print_nb_base((uint64_t)starts[ZT_BIG], HEX);
				print_ln();
				big_zone_print(&g_zones.big);
		}
	}
	glob_mut_wrapper(MO_UNLOCK, NULL);
}

void	zones_dtor(void) {
	zone_dest(&g_zones.tiny);
	zone_dest(&g_zones.small);
}
