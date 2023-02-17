/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 12:18:18 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/16 19:58:18 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/zone.h"

int		zone_inited(zone_t *zone) {
	return (!!zone->mem);
}

int		zone_init(zone_t *zone, uint64_t cell_cap) {
	zone->cell_cap = cell_cap;
	return (!(zone->mem = mmap(NULL, CELL_NB * cell_cap,
		PROT_READ | PROT_WRITE | PROT_EXEC,
		MAP_PRIVATE | MAP_ANONYMOUS,
		-1, 0)) ? -1 : 0);
}

void	zone_dest(zone_t *zone) {
	munmap(zone->mem, CELL_NB * zone->cell_cap);
}

int		zone_ptr_in(zone_t *zone, void *ptr) {
	return (ptr >= zone->mem
		&& ptr < zone->mem + CELL_NB * zone->cell_cap
		&& !((ptr - zone->mem) % zone->cell_cap));
}

int		zone_full(zone_t *zone) {
	return (zone->sz == CELL_NB);
}

void	*zone_alloc(zone_t *zone, uint64_t size) {
	int		idx;

	for (idx = 0; idx < CELL_NB; ++idx) {
		if (!zone->cell_sz[idx]) {
			zone->cell_sz[idx] = size;
			++zone->sz;
			return (zone->mem + idx * zone->cell_cap);
		}
	}
	return (NULL);
}

uint64_t	zone_csize(zone_t *zone, void *ptr) {
	return (zone->cell_sz[(ptr - zone->mem) / zone->cell_cap]);
}

void		zone_free(zone_t *zone, void *ptr) {
	zone->cell_sz[(ptr - zone->mem) / zone->cell_cap] = 0;
	--zone->sz;
}

void	*zone_start(zone_t *zone) {
	return (zone->mem);
}

void	zone_print(zone_t *zone) {
	int			idx;
	void		*start;
	
	for (idx = 0; idx < CELL_NB; ++idx) {
		if (zone->cell_sz[idx]) {
			start = zone->mem + idx * zone->cell_cap;
			print_str("  ");
			print_nb_base((uint64_t)start, HEX);
			print_str(" - ");
			print_nb_base((uint64_t)(start + zone->cell_sz[idx]), HEX);
			print_str(" : ");
			print_nb_base(zone->cell_sz[idx], DEC);
			print_str(" bytes\n");
		}
	}
}
