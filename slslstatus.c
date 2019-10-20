/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <X11/Xlib.h>

#include "arg.h"
#include "slslstatus.h"
#include "util.h"

struct arg {
	const char *(*func)();
	const char *fmt;
	const char *args;
};

char buf[1024];
static int done;
static Display *dpy;

#include "config.h"

static void
terminate(const int signo)
{
	(void)signo;

	done = 1;
}

static void
difftimespec(struct timespec *res, struct timespec *a, struct timespec *b)
{
	res->tv_sec = a->tv_sec - b->tv_sec - (a->tv_nsec < b->tv_nsec);
	res->tv_nsec = a->tv_nsec - b->tv_nsec +
	               (a->tv_nsec < b->tv_nsec) * 1E9;
}

static void
usage(void)
{
	die("usage: %s [-su]", argv0);
}

int
wait_next_min(void)
{
	/* get current real time */
	struct timespec waittime;
	clock_gettime(CLOCK_REALTIME, &waittime);

	/* calculate time needed to wait until next minute, rounded up */
	waittime.tv_sec = 61 - (waittime.tv_sec % 60);
	waittime.tv_nsec = 0;

	if (nanosleep(&waittime, NULL) < 0)
		die("nanosleep:");

	return 0;
}

int
single_update(unsigned short int sflag)
{
	size_t i, len;
	int ret;
	char status[MAXLEN];
	const char *res;

	if (!sflag && !(dpy = XOpenDisplay(NULL))) {
		die("XOpenDisplay: Failed to open display");
	}

	status[0] = '\0';
	for (i = len = 0; i < LEN(args); i++) {
		if (!(res = args[i].func(args[i].args))) {
			res = unknown_str;
		}
		if ((ret = esnprintf(status + len, sizeof(status) - len,
				    args[i].fmt, res)) < 0) {
			break;
		}
		len += ret;
	}

	if (sflag) {
		/* print to stdout */
		puts(status);
		fflush(stdout);
		if (ferror(stdout))
			die("puts:");
	} else {
		/* print to WN_NAME */
		if (XStoreName(dpy, DefaultRootWindow(dpy), status)
		    < 0) {
			die("XStoreName: Allocation failed");
		}
		XFlush(dpy);
	}

	if (XCloseDisplay(dpy) < 0) {
		die("XCloseDisplay: Failed to close display");
	}

	return 0;
}

int
main(int argc, char *argv[])
{
	/* if 'sflag' is true, print to stdout */
	/* if 'uflag' is true, just update bar 1 time */
	unsigned short int sflag = 0;
	unsigned short int uflag = 0;
	ARGBEGIN {
		case 's':
			sflag = 1;
			break;
		case 'u':
			uflag = 1;
			break;
		default:
			usage();
	} ARGEND
	if (argc) {
		usage();
	}

	/* update bar 1 time only */
	single_update(sflag);
	if (uflag) {
		return 0;
	}

	wait_next_min();

	struct sigaction act;
	struct timespec start, current, diff, intspec, wait;
	size_t i, len;
	int ret;
	char status[MAXLEN];
	const char *res;

	/* trap SIGINT and SIGTERM to set the 'done' flag terminating the while loop this way */
	memset(&act, 0, sizeof(act));
	act.sa_handler = terminate;
	sigaction(SIGINT,  &act, NULL);
	sigaction(SIGTERM, &act, NULL);

	if (!sflag && !(dpy = XOpenDisplay(NULL))) {
		die("XOpenDisplay: Failed to open display");
	}

	while (!done) {
		if (clock_gettime(CLOCK_MONOTONIC, &start) < 0) {
			die("clock_gettime:");
		}

		status[0] = '\0';
		for (i = len = 0; i < LEN(args); i++) {
			if (!(res = args[i].func(args[i].args))) {
				res = unknown_str;
			}
			if ((ret = esnprintf(status + len, sizeof(status) - len,
			                    args[i].fmt, res)) < 0) {
				break;
			}
			len += ret;
		}

		if (sflag) {
			/* print to stdout */
			puts(status);
			fflush(stdout);
			if (ferror(stdout))
				die("puts:");
		} else {
			/* print to WN_NAME */
			if (XStoreName(dpy, DefaultRootWindow(dpy), status)
                            < 0) {
				die("XStoreName: Allocation failed");
			}
			XFlush(dpy);
		}

		if (!done) {
			if (clock_gettime(CLOCK_MONOTONIC, &current) < 0) {
				die("clock_gettime:");
			}
			difftimespec(&diff, &current, &start);

			intspec.tv_sec = interval / 1000;  // whole secs
			intspec.tv_nsec = (interval % 1000) * 1E6;  // nano secs

			/* get the extra time needed to wait to make the total waiting interval */
			difftimespec(&wait, &intspec, &diff);

			if (wait.tv_sec >= 0) {
				if (nanosleep(&wait, NULL) < 0 &&
				    errno != EINTR) {
					die("nanosleep:");
				}
			}
		}
	}

	/* clear WN_NAME on exit */
	if (!sflag) {
		XStoreName(dpy, DefaultRootWindow(dpy), NULL);
		if (XCloseDisplay(dpy) < 0) {
			die("XCloseDisplay: Failed to close display");
		}
	}

	return 0;
}
