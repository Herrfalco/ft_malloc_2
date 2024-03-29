/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 10:39:16 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/18 21:22:01 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_H
#define PRINT_H

#include "incs.h"

#define BUFF_SZ		1024
#define LINE_STR	"-----------------------------------------------------\n"

typedef enum		base_e {
	B_BIN,
	B_OCT,
	B_DEC,
	B_HEX,
	B_DMP,
}					base_t;

void			print_str(int fd, const char *str);
void			print_ln(int fd);
void			print_nb_base(int fd, uint64_t nb, base_t base, uint64_t min_w);

#endif // PRINT_H
