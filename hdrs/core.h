/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 12:54:33 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/16 11:27:47 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
#define MALLOC_H

#include "zone.h"
#include "big_zone.h"
#include "print.h"

#define ZONE_NB		3

typedef enum		zone_typ_e {
	TINY,
	SMALL,
	BIG,
}					zone_typ_t;

typedef struct		g_zones_s {
	zone_t			tiny;
	zone_t			small;
	big_zone_t		big;
}					g_zones_t;

#endif // MALLOC_H
