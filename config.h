/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "n/a";

/* maximum output string length */
#define MAXLEN 50

/*
 *
 * battery_perc		battery percentage		battery name (BAT0)
 *							NULL on OpenBSD/FreeBSD
 * battery_state	battery charging state		battery name (BAT0)
 *							NULL on OpenBSD/FreeBSD
 * datetime		date and time			format string (%F %T)
 * ipv4			IPv4 address			interface name (eth0)
 * ipv6			IPv6 address			interface name (eth0)
 * run_command		custom shell command		command (echo foo)
 * temp			temperature in degree celsius	sensor file
 *							(/sys/class/thermal/...)
 *							NULL on OpenBSD
 *							thermal zone on FreeBSD
 *							(tz0, tz1, etc.)
 * get_alsa_vol		ALSA volume in percent		mixer file (/dev/mixer)
 * wifi_perc		WiFi signal in percent		interface name (wlan0)
 * wifi_essid		WiFi ESSID			interface name (wlan0)
 */

static const struct arg args[] = {
	/* function	format		argument */
	{ temp,		"%sC ",		"/sys/class/thermal/thermal_zone0/temp"	},
	{ get_alsa_vol,	"%s ",	NULL					},
	{ datetime,	"%s",		"%a %d-%m-%Y %H:%M"			},
};
