/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   incs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 12:28:15 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/18 18:11:40 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCS_H
#define INCS_H

#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>

#define CELL_NB			1024
#define SMALL_CELL_SZ	(uint64_t)getpagesize()
#define TINY_CELL_SZ	(SMALL_CELL_SZ / 4)
#define ENV_FLAG		"LOG_FILE"

#endif // INCS_H
