/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 10:37:15 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/16 11:26:13 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/print.h"

static uint64_t	str_len(const char *str) {
	uint64_t	len;

	for (len = 0; str[len]; ++len);
	return (len);
}

void			print_str(const char *str) {
	write(1, str, str_len(str));
}

void			print_ln(void) {
	write(1, "\n", 1);
}

static void		put_nb_base(uint64_t nb, const char *hdr, const char *set) {
	uint64_t	base = str_len(set);
	char		c;

	if (hdr)
		print_str(hdr);
	if (nb / base)
		put_nb_base(nb / base, NULL, set);
	c = set[nb % base];
	write(1, &c, 1);
}

void			print_nb_base(uint64_t nb, base_t base) {
	const char	*base_hdr[] = { "0b", "0", "", "0x" };
	const char	*base_set[] = {
		"01", "01234567",
		"0123456789", "0123456789ABCDEF",
	};

	put_nb_base(nb, base_hdr[base], base_set[base]);
}
