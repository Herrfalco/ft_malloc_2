/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 17:59:23 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/16 21:55:46 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/core.h"

static g_zones_t		g_zones = { 0 };

void	*malloc(uint64_t size) {
	if (!size)
		return (NULL);
	else if (size <= TINY_CELL_SZ && !zone_full(&g_zones.tiny))
		return (!zone_inited(&g_zones.tiny)
			&& zone_init(&g_zones.tiny, TINY_CELL_SZ)
			? NULL : zone_alloc(&g_zones.tiny, size));
	else if (size <= SMALL_CELL_SZ && !zone_full(&g_zones.small))
		return (!zone_inited(&g_zones.small)
			&& zone_init(&g_zones.small, SMALL_CELL_SZ)
			? NULL : zone_alloc(&g_zones.small, size));
	return (big_zone_alloc(&g_zones.big, size));
}

void	free(void *ptr) {
	if (!ptr)
		return;
	if (zone_ptr_in(&g_zones.tiny, ptr))
		zone_free(&g_zones.tiny, ptr);
	else if (zone_ptr_in(&g_zones.small, ptr))
		zone_free(&g_zones.small, ptr);
	else if (big_zone_ptr_in(&g_zones.big, ptr))
		big_zone_free(&g_zones.big, ptr);
}

static void		mem_cpy(void *dst, void *src, uint64_t size) {
	uint64_t		i;

	for (i = 0; i < size; ++i)
		((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
}

int			main(void) {
	char	src[] = "bonjour";
	char	dst[10];

	mem_cpy(dst, src, 8);
	write(1, dst, 7);
}

void	*realloc(void *ptr, uint64_t size) {
	void		*new = NULL;
	uint64_t	o_sz;

	if (!ptr && !size);
	else if (!ptr)
		new = malloc(size);
	else if (!size)
		free(ptr);
	else {
		if (zone_ptr_in(&g_zones.tiny, ptr))
			o_sz = zone_csize(&g_zones.tiny, ptr);
		else if (zone_ptr_in(&g_zones.small, ptr))
			o_sz = zone_csize(&g_zones.small, ptr);
		else if (big_zone_ptr_in(&g_zones.big, ptr))
			o_sz = big_zone_csize(ptr);
		else
			return (new);
		if (!(new = malloc(size)))
			return (new);
		mem_cpy(new, ptr, o_sz < size ? o_sz : size);
		free(ptr);
	}
	return (new);
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
	zone_typ_t		zones[] = { TINY, SMALL, BIG };
	void			*starts[] = {
		zone_start(&g_zones.tiny),
		zone_start(&g_zones.small),
		big_zone_start(&g_zones.big),
	};

	sort_zones(zones, starts);
	for (i = 0; i < ZONE_NB; ++i) {
		if (!starts[zones[i]])
			continue;
		switch (zones[i]) {
			case TINY:
				print_str("TINY : ");
				print_nb_base((uint64_t)starts[TINY], HEX);
				print_ln();
				zone_print(&g_zones.tiny);
				break;
			case SMALL:
				print_str("SMALL : ");
				print_nb_base((uint64_t)starts[SMALL], HEX);
				print_ln();
				zone_print(&g_zones.small);
				break;
			default:
				print_str("BIG : ");
				print_nb_base((uint64_t)starts[BIG], HEX);
				print_ln();
				big_zone_print(&g_zones.big);
		}
	}
}

void	zones_dtor(void) {
	if (zone_inited(&g_zones.tiny))
		zone_dest(&g_zones.tiny);
	if (zone_inited(&g_zones.small))
		zone_dest(&g_zones.small);
}
