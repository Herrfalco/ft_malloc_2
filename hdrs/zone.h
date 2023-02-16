/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 12:26:57 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/16 17:11:10 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZONE_H
#define ZONE_H

#include "print.h"

typedef struct		zone_s {
	uint64_t		cell_cap;
	uint64_t		cell_sz[CELL_NB];
	uint64_t		sz;
	void			*mem;
}					zone_t;

int		zone_inited(zone_t *zone);
int		zone_init(zone_t *zone, uint64_t cell_cap);
int		zone_ptr_in(zone_t *zone, void *ptr);
int		zone_full(zone_t *zone);
void	*zone_alloc(zone_t *zone, uint64_t size);
void	zone_free(zone_t *zone, void *ptr);
void	*zone_start(zone_t *zone);
void	zone_print(zone_t *zone);
void	zone_dest(zone_t *zone);

#endif // ZONE_H
