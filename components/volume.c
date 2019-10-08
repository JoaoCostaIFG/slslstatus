#include <alsa/asoundlib.h>

#include "../util.h"


const char *
get_alsa_vol(void)
{

	snd_mixer_t *handle;
	snd_mixer_selem_id_t *sid;
	snd_mixer_elem_t* elem;
	const char *card = "default";
	const char *mixer_name = "Master";
	const int min_index = 0;

	snd_mixer_selem_id_alloca(&sid);

	/* sets simple-mixer index and name */
	snd_mixer_selem_id_set_index(sid, min_index);
	snd_mixer_selem_id_set_name(sid, mixer_name);

	if (snd_mixer_open(&handle, 0) < 0) {  // Get handler and open connection
		warn("snd_mixer_open failed");
		return NULL;
	}
	if (snd_mixer_attach(handle, card) < 0) {
		warn("snd_mixer_attach failed");
		snd_mixer_close(handle);
		return NULL;
	}

	if (snd_mixer_selem_register(handle, NULL, NULL) < 0) {
		warn("snd_mixer_selem_register failed");
		snd_mixer_close(handle);
		return NULL;
	}
	if (snd_mixer_load(handle) < 0) {
		warn("snd_mixer_load failed");
		snd_mixer_close(handle);
		return NULL;
	}


	elem = snd_mixer_find_selem(handle, sid);
	if (!elem) {
		warn("snd_mixer_find_selem failed");
		snd_mixer_close(handle);
		return NULL;
	}

	/*
	 * vars that will store current 'min' and 'max' volume values
	 * and current volume percentage (relative to 'min' and 'max')
	 */
	long min, max, outvol;

	/* get volume range */
	snd_mixer_selem_get_playback_volume_range(elem, &min, &max);

	/* get current volume and calculate it's percentage */
	if (snd_mixer_selem_get_playback_volume(elem, 0, &outvol) < 0) {
		warn("snd_mixer_selem_get_playback_volume failed");
		snd_mixer_close(handle);
		return NULL;
	}
	snd_mixer_close(handle);

	/* normalize to [0, 100] */
	outvol -= min;
	max -= min;
	outvol = outvol * 100 / max;

	return bprintf("%ld", outvol);
}

