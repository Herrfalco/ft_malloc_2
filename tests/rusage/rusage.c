/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rusage.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 12:15:25 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/16 14:42:38 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE

#include <sys/resource.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int			main(int argc, char **argv, char **env) {
	int					pid, status;
	struct rusage		usage;

	if (argc < 2) {
		fprintf(stderr, "Error: need an executable as parameter\n");
		return (1);
	}
	if (!(pid = fork())) {
		if (execvpe(argv[1], argv + 1, env))
			perror("Error: ");
		return (1);
	}
	while (42) {
		if (waitpid(pid, &status, 0) < 0)
			return (1);
		if (WIFEXITED(status) || WIFSIGNALED(status)) {
			if (WEXITSTATUS(status)
					|| getrusage(RUSAGE_CHILDREN, &usage) < 0)
				return (1);
			printf("%ld maximum resident set size\n", usage.ru_maxrss);
			printf("%ld integral shared memory size\n", usage.ru_ixrss);
			printf("%ld integral unshared data size\n", usage.ru_idrss);
			printf("%ld integral unshared stack size\n", usage.ru_isrss);
			printf("%ld page reclaims (soft page faults)\n", usage.ru_minflt);
			printf("%ld page faults (hard page faults)\n", usage.ru_majflt);
			printf("%ld swaps\n", usage.ru_nswap);
			printf("%ld block input operations\n", usage.ru_inblock);
			printf("%ld block output operations\n", usage.ru_oublock);
			printf("%ld IPC messages sent\n", usage.ru_msgsnd);
			printf("%ld IPC messages received\n", usage.ru_msgrcv);
			printf("%ld signals received\n", usage.ru_nsignals);
			printf("%ld voluntary context switches\n", usage.ru_nvcsw);
			printf("%ld involuntary context switches\n", usage.ru_nivcsw);
			return (0);
		}
	}
	return (0);
}
