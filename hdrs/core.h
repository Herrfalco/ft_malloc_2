/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 12:54:33 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/18 17:37:11 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
#define CORE_H

#include "zone.h"
#include "big_zone.h"
#include "../lib.h"

#define ZONE_NB		3

typedef enum		fn_typ_e {
	FT_MLC,
	FT_FRE,
	FT_RLC,
	FT_SHW,
}					fn_typ_t;

typedef enum		zone_typ_e {
	ZT_TINY,
	ZT_SMALL,
	ZT_BIG,
}					zone_typ_t;

typedef struct		g_zones_s {
	zone_t			tiny;
	zone_t			small;
	big_zone_t		big;
	pthread_mutex_t	mut;
	int				log;
}					g_zones_t;

void __attribute__ ((destructor))		zones_dtor(void);

#endif // CORE_H
