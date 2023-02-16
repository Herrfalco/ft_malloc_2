/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 17:59:23 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/16 11:29:59 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/core.h"

static g_zones_t		g_zones = { 0 };

void	*ft_malloc(uint64_t size) {
	if (size <= TINY_CELL_SZ && !zone_is(&g_zones.tiny, Z_FULL))
		return (!zone_inited(&g_zones.tiny)
			&& zone_init(&g_zones.tiny, TINY_CELL_SZ)
			? NULL : zone_alloc(&g_zones.tiny, size));
	else if (size <= SMALL_CELL_SZ && !zone_is(&g_zones.small, Z_FULL))
		return (!zone_inited(&g_zones.small)
			&& zone_init(&g_zones.small, SMALL_CELL_SZ)
			? NULL : zone_alloc(&g_zones.small, size));
	return (big_zone_alloc(&g_zones.big, size));
}

void	ft_free(void *ptr) {
	if (zone_ptr_in(&g_zones.tiny, ptr))
		zone_free(&g_zones.tiny, ptr);
	else if (zone_ptr_in(&g_zones.small, ptr))
		zone_free(&g_zones.small, ptr);
	else
		big_zone_free(&g_zones.big, ptr);
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
