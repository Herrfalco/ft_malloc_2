/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 15:52:03 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/18 18:36:34 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_H
#define LIB_H

#include "hdrs/incs.h"

__attribute__ ((visibility ("default"))) void	*malloc(uint64_t size);
__attribute__ ((visibility ("default"))) void	*realloc(void *ptr, uint64_t size);
__attribute__ ((visibility ("default"))) void	free(void *ptr);
__attribute__ ((visibility ("default"))) void	show_alloc_mem(void);
__attribute__ ((visibility ("default"))) void	show_alloc_hist(void);

#endif // LIB_H
