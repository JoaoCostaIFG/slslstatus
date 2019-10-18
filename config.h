/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
const unsigned int interval = 60000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "N/A";

/* maximum output string length */
#define MAXLEN 50

/*
 *
 * battery_perc		battery percentage		battery name
 *							(BAT0)
 * battery_state	battery charging state		battery name
 *							(BAT0)
 * datetime		date and time			format string
 *							(%F %T)
 * ipv4			IPv4 address			interface name
 *							(eth0)
 * ipv6			IPv6 address			interface name
 *							(eth0)
 * run_command		custom shell command		command
 *							(echo foo)
 * temp			temperature in degree celsius	sensor file
 *							(/sys/class/thermal/...)
 * get_alsa_vol		ALSA volume in percent		NULL
 * wifi_perc		WiFi signal in percent		interface name
 *							(wlan0)
 * wifi_essid		WiFi ESSID			interface name
 *							(wlan0)
 */

static const struct arg args[] = {
	/* function	format		argument */
	//{ battery_perc,	"%s%%",		"BAT0"					},
	//{ temp,		"%sC ",		"/sys/class/thermal/thermal_zone0/temp"	},
	{ get_alsa_vol,	"%s ",		NULL					},
	{ datetime,	"%s",		"%a %d-%m-%Y %H:%M"			},
};
