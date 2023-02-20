/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:16:18 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/20 10:33:17 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/glob.h"

g_zones_t		g_zones = {
	.mut = PTHREAD_MUTEX_INITIALIZER,
};

void			glob_lock(fn_typ_t fn, void *dat1, void *dat2) {
	pthread_mutex_lock(&g_zones.mut);
	if (!g_zones.log)
		g_zones.log = open(getenv(ENV_FLAG),
			O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (g_zones.log > 0) {
		switch (fn) {
			case FT_MLC:
				print_str(g_zones.log, "  malloc(");
				print_nb_base(g_zones.log, *((uint64_t *)dat1), B_DEC, 0);
				print_str(g_zones.log, ") = ");
				break;
			case FT_FRE:
				print_str(g_zones.log, "  free(");
				print_nb_base(g_zones.log, (uint64_t)dat1, B_HEX, 0);
				print_str(g_zones.log, ")\n");
				break;
			case FT_RLC:
				print_str(g_zones.log, "  realloc(");
				print_nb_base(g_zones.log, (uint64_t)dat1, B_HEX, 0);
				print_str(g_zones.log, ", ");
				print_nb_base(g_zones.log, *((uint64_t *)dat2), B_DEC, 0);
				print_str(g_zones.log, ") = ");
				break;
			default:
				break;
		}
	}
}

void			*glob_unlock(fn_typ_t fn, void *dat) {
	if (g_zones.log > 0) {
		switch (fn) {
			case FT_MLC:
				print_nb_base(g_zones.log, (uint64_t)dat, B_HEX, 0);
				print_ln(g_zones.log);
				break;
			case FT_RLC:
				print_nb_base(g_zones.log, (uint64_t)dat, B_HEX, 0);
				print_ln(g_zones.log);
				break;
			default:
				break;
		}
	}
	pthread_mutex_unlock(&g_zones.mut);
	return (dat);
}
