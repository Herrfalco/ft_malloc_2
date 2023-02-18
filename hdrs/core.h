/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 12:54:33 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/18 14:18:14 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
#define CORE_H

#include "zone.h"
#include "big_zone.h"
#include "../lib.h"

#define ZONE_NB		3

typedef enum		bool_e {
	FALSE,
	TRUE,
}					bool_t;

typedef enum		zone_typ_e {
	ZT_TINY,
	ZT_SMALL,
	ZT_BIG,
}					zone_typ_t;

typedef enum		mut_op_e {
	MO_NO,
	MO_LOCK,
	MO_UNLOCK,
}					mut_op_t;

typedef struct		g_zones_s {
	zone_t			tiny;
	zone_t			small;
	big_zone_t		big;
	pthread_mutex_t	mut;
}					g_zones_t;

void __attribute__ ((destructor))		zones_dtor(void);

#endif // CORE_H
