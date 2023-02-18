/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:17:02 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/18 19:23:44 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOB_H
#define GLOB_H

#include "zone.h"
#include "big_zone.h"

#define ZONE_NB		3

typedef enum		fn_typ_e {
	FT_MLC,
	FT_FRE,
	FT_RLC,
	FT_SHW,
	FT_HST,
}					fn_typ_t;

typedef struct		g_zones_s {
	zone_t			tiny;
	zone_t			small;
	big_zone_t		big;
	pthread_mutex_t	mut;
	int				log;
}					g_zones_t;

typedef enum		zone_typ_e {
	ZT_TINY,
	ZT_SMALL,
	ZT_BIG,
}					zone_typ_t;

extern g_zones_t	g_zones;

void				glob_lock(fn_typ_t fn, void *dat1, void *dat2);
void				*glob_unlock(fn_typ_t fn, void *dat);

#endif // GLOB_H
