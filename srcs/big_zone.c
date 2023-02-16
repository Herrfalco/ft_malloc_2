/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_zone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 14:53:21 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/16 11:36:07 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/big_zone.h"

void		*big_zone_alloc(big_zone_t *zone, uint64_t size) {
	big_cell_hdr_t		*cell;

	if (!(cell = mmap(NULL, size + sizeof(big_cell_hdr_t),
		PROT_READ | PROT_WRITE | PROT_EXEC,
		MAP_PRIVATE | MAP_ANONYMOUS,
		-1, 0)))
		return (NULL);
	cell->sz = size;
	if (*zone)
		(*zone)->prev = cell;
	cell->next = *zone;
	*zone = cell;
	return (cell + 1);
}

void		big_zone_free(big_zone_t *zone, void *ptr) {
	big_cell_hdr_t		*cell = ((big_cell_hdr_t *)ptr) - 1;

	if (cell->prev)
		cell->prev->next = cell->next;
	else
		*zone = cell->next;
	if (cell->next)
		cell->next->prev = cell->prev;
	munmap(cell, cell->sz + sizeof(big_cell_hdr_t));
}

static void *next_cell(big_zone_t *zone, big_cell_hdr_t *lst_cell) {
	big_cell_hdr_t		*cell, *min;

	for (min = NULL, cell = *zone; cell; cell = cell->next)
		if (cell > lst_cell && (!min || cell < min))
			min = cell;
	return (min);
}

void		*big_zone_start(big_zone_t *zone) {
	return (next_cell(zone, NULL));
}

void		big_zone_print(big_zone_t *zone) {
	big_cell_hdr_t		*cell = NULL;

	while ((cell = next_cell(zone, cell))) {
			print_str("  ");
			print_nb_base((uint64_t)(cell + 1), HEX);
			print_str(" - ");
			print_nb_base((uint64_t)(cell + 1) + cell->sz, HEX);
			print_str(" : ");
			print_nb_base(cell->sz, DEC);
			print_str(" bytes\n");
	}
}
