/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:09:59 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/18 21:32:04 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/show.h"

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
	zone_typ_t		zones[] = { ZT_TINY, ZT_SMALL, ZT_BIG };
	void			*starts[ZONE_NB];


	glob_lock(FT_SHW, NULL, NULL);
	print_str(1, LINE_STR);
	starts[0] = zone_start(&g_zones.tiny);
	starts[1] = zone_start(&g_zones.small);
	starts[2] = big_zone_start(&g_zones.big);
	sort_zones(zones, starts);
	for (i = 0; i < ZONE_NB; ++i) {
		if (!starts[zones[i]])
			continue;
		switch (zones[i]) {
			case ZT_TINY:
				print_str(1, "TINY : ");
				print_nb_base(1, (uint64_t)starts[ZT_TINY], B_HEX, 0);
				print_ln(1);
				zone_print(&g_zones.tiny);
				break;
			case ZT_SMALL:
				print_str(1, "SMALL : ");
				print_nb_base(1, (uint64_t)starts[ZT_SMALL], B_HEX, 0);
				print_ln(1);
				zone_print(&g_zones.small);
				break;
			default:
				print_str(1, "BIG : ");
				print_nb_base(1, (uint64_t)starts[ZT_BIG], B_HEX, 0);
				print_ln(1);
				big_zone_print(&g_zones.big);
		}
	}
	glob_unlock(FT_SHW, NULL);
}

void	show_alloc_hist(void) {
	char		buff[BUFF_SZ];
	ssize_t		ret;

	glob_lock(FT_HST, NULL, NULL);
	print_str(1, LINE_STR);
	if (g_zones.log > 0) {
		print_str(1, "HIST:\n");
		if (lseek(g_zones.log, 0, SEEK_SET) < 0)
			print_str(2, "Error: Can't acces history file\n");
		else {
			while ((ret = read(g_zones.log, buff, BUFF_SZ)) > 0)
				write(1, buff, ret);
			if (ret < 0)
				print_str(2, "Error: Can't read history file\n");
			if (lseek(g_zones.log, 0, SEEK_END) < 0)
				print_str(2, "Error: Can't acces history file\n");
		}
	} else
		print_str(2, "Error: No log file (please export LOG_FILE variable)\n");
	glob_unlock(FT_HST, NULL);
}

void	show_alloc_dump(void *ptr, uint64_t sz) {
	uint64_t		r_sz, i;

	glob_lock(FT_DMP, NULL, NULL);
	print_str(1, LINE_STR);
	print_str(1, "DUMP at ");
	print_nb_base(1, (uint64_t)ptr, B_HEX, 0);
	print_str(1, ":");
	if (zone_ptr_in(&g_zones.tiny, ptr))
		r_sz = zone_csize(&g_zones.tiny, ptr);
	else if (zone_ptr_in(&g_zones.small, ptr))
		r_sz = zone_csize(&g_zones.small, ptr);
	else if (big_zone_ptr_in(&g_zones.big, ptr))
		r_sz = big_zone_csize(ptr);
	else
		return (print_str(2, "Error: Memory location never allocated\n"));
	for (i = 0; (!sz || i < sz) && i < r_sz; ++i) {
		print_str(1, !(i % DUMP_LINE_SZ) ? "\n  " : " ");
		print_nb_base(1, (uint64_t)((uint8_t *)ptr)[i], B_DMP, 2);
	}
	if (sz && sz < r_sz) {
		print_str(1, " ... ");
		print_nb_base(1, r_sz - sz, B_DEC, 0);
		print_str(1, " byte(s)");
	}
	print_ln(1);
	glob_unlock(FT_DMP, NULL);
}
