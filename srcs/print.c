/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 10:37:15 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/18 19:43:56 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/print.h"

static uint64_t	str_len(const char *str) {
	uint64_t	len;

	for (len = 0; str[len]; ++len);
	return (len);
}

void			print_str(int fd, const char *str) {
	write(fd, str, str_len(str));
}

void			print_ln(int fd) {
	write(fd, "\n", 1);
}

static void		put_nb_base(int fd, uint64_t nb, const char *hdr, const char *set, uint64_t min_w) {
	uint64_t	base = str_len(set);
	char		c;

	if (hdr)
		print_str(fd, hdr);
	if (nb / base || min_w > 1)
		put_nb_base(fd, nb / base, NULL, set, min_w ? min_w - 1 : 0);
	c = set[nb % base];
	write(fd, &c, 1);
}

void			print_nb_base(int fd, uint64_t nb, base_t base, uint64_t min_w) {
	const char	*base_hdr[] = { "0b", "0o", "", "0x" };
	const char	*base_set[] = {
		"01", "01234567",
		"0123456789", "0123456789ABCDEF",
	};

	if (!nb && base == HEX)
		print_str(fd, "NULL");
	else
		put_nb_base(fd, nb, base_hdr[base], base_set[base], min_w);
}
