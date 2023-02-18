/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 10:39:16 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/18 19:04:14 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_H
#define PRINT_H

#include "incs.h"

#define BUFF_SZ		1024
#define LINE_STR	"-----------------------------------------------------\n"

typedef enum		base_e {
	BIN,
	OCT,
	DEC,
	HEX,
}					base_t;

void			print_str(int fd, const char *str);
void			print_ln(int fd);
void			print_nb_base(int fd, uint64_t nb, base_t base);

#endif // PRINT_H
