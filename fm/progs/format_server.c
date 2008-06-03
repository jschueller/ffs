#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#undef FD_SETSIZE
#define FD_SETSIZE 1024
#include <winsock.h>
#define getpid()	_getpid()
#define getcwd(a,b)	_getcwd(a,b)
#define __ANSI_CPP__
#else
#include <unistd.h>
#include <time.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_SYS_TIMES_H
#include <sys/times.h>
#endif
#include <sys/socket.h>
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#ifdef HAVE_SYS_UIO_H
#include <sys/uio.h>
#endif
#ifdef HAVE_SYS_UN_H
#include <sys/un.h>
#endif
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#include <signal.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#include "fm.h"
#include "fm_internal.h"
#include "cercs_env.h"
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#include <string.h>
#include <assert.h>

static void
die_with_error(sig)
int sig;
{
    fprintf(stderr, "Format server not responding.  Timeout.\n");
    exit(1);
}

extern int serverAtomicWrite(void *fd, const void *buffer, int length);

int
main(argc, argv)
int argc;
char **argv;
{
    int no_fork = 0;
    int quiet = 0;
    int i;
    FMContext test;
    int fs_port = DEFAULT_FS_PORT;
    char testing_char = 'T';
    char format_server_pid[] = "/tmp/format_server_pid";
    int do_restart = 0;

/*    if (os_sockets_init_func != NULL) os_sockets_init_func();*/

    for (i = 1; i < argc; i++) {
	if (strcmp(argv[i], "-no_fork") == 0) {
	    no_fork++;
	} else if (strcmp(argv[i], "-quiet") == 0) {
	    quiet++;
	} else if (strcmp(argv[i], "-restart") == 0) {
	    do_restart++;
	} else {
	    fprintf(stderr, "Unknown argument \"%s\"\n", argv[i]);
	    fprintf(stderr, "Usage:  format_server [-no_fork] [-quiet] [-restart]\n");
	    exit(1);
	}
    }

    /* test to see if format server is running */
    test = create_FMcontext();
#ifndef HAVE_WINDOWS_H
    signal(SIGALRM, die_with_error);
    alarm(30);
#endif
    if (!quiet) {
	putenv(strdup("FORMAT_SERVER_VERBOSE=1"));
    }
    establish_server_connection(test, /* nofallback */ 0);
    if (serverAtomicWrite(test->server_fd, &testing_char, 1)
	== 1) {
	/* already running */
	if (!quiet) {
	    printf("Use FORMAT_SERVER_HOST environment variable to change format server location\n");
	    printf("Format server already running.  Exiting\n");
	}
	exit(0);
    } else {
	/* make really, really certain there's no format_server running */
	FILE *format_server_pid_file = fopen(format_server_pid, "r");
	alarm(0);
	if (format_server_pid_file != NULL) {
	    long server_pid;
	    if (fscanf(format_server_pid_file, "%lx", &server_pid) == 1) {
		if (kill(server_pid, 0) == 0) {
		    fprintf(stderr, "Format Server %lx not responding, but still running\n",
			    server_pid);
		    exit(0);
		}
	    }
	    fclose(format_server_pid_file);
	}
	if (!no_fork) {
	    if (fork() != 0) {
		/* I'm the parent, return now */
		exit(0);
	    }
	}
	format_server_pid_file = fopen(format_server_pid, "w");
	if (format_server_pid_file) {
	    fprintf(format_server_pid_file, "%lx\n", (long) getpid());
	    fclose(format_server_pid_file);
	}
	if (!no_fork) {
	    if (!quiet) {
		printf("Forking server to background\n");
	    }
	    if (fork() != 0) {
		/* I'm the parent, return now */
		exit(0);
	    }
	} else {
	    if (!quiet) {
		printf("Running...\n");
	    }
	}
    }

    {
#ifdef RLIMIT_NOFILE
	struct rlimit lim;
	if (getrlimit(RLIMIT_NOFILE, &lim) != 0) {
	    perror("Getrlimit");
	}
	lim.rlim_cur = lim.rlim_max;
	if (setrlimit(RLIMIT_NOFILE, &lim) != 0) {
	    perror("Setrlimit");
	}
	if (getrlimit(RLIMIT_CORE, &lim) != 0) {
	    perror("Setrlimit");
	}
	lim.rlim_cur = lim.rlim_max;
	if (setrlimit(RLIMIT_CORE, &lim) != 0) {
	    perror("Setrlimit");
	}
	chdir("/tmp");
#endif
    }
    if (cercs_getenv("FORMAT_SERVER_PORT") != NULL) {
	char *port_string = cercs_getenv("FORMAT_SERVER_PORT");
	int tmp_port;
	if (sscanf(port_string, "%d", &tmp_port) != 1) {
	    printf("FORMAT_SERVER_PORT spec \"%s\" not understood.\n", 
		   port_string);
	} else {
	    fs_port = tmp_port;
	}
    }
#ifndef HAVE_WINDOWS_H
    alarm(0);
#endif
    general_format_server(fs_port, do_restart, no_fork);
    return 0;
}

