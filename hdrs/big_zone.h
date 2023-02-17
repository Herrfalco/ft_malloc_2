/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_zone.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 13:13:00 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/16 21:52:53 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIG_ZONE
#define BIG_ZONE

#include "print.h"

typedef struct __attribute__((packed))	big_cell_hdr_s {
	struct big_cell_hdr_s				*prev;
	struct big_cell_hdr_s				*next;
	uint64_t							sz;
	uint8_t								_pad[8];
}										big_cell_hdr_t;

typedef big_cell_hdr_t		*big_zone_t;

void		*big_zone_alloc(big_zone_t *zone, uint64_t size);
uint64_t	big_zone_csize(void *ptr);
int			big_zone_free(big_zone_t *zone, void *ptr);
int			big_zone_ptr_in(big_zone_t *zone, void *ptr);
void		*big_zone_start(big_zone_t *zone);
void		big_zone_print(big_zone_t *zone);

#endif // BIG_ZONE
