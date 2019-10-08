#include <alsa/asoundlib.h>

#include "../util.h"


const char *
get_alsa_vol(long volume)
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

	if (snd_mixer_open(&handle, 0) < 0)  // Get handler and open connection
		return -1;
	if (snd_mixer_attach(handle, card) < 0) {
		snd_mixer_close(handle);
		return -2;
	}

	if (snd_mixer_selem_register(handle, NULL, NULL) < 0) {
		snd_mixer_close(handle);
		return -3;
	}
	if (snd_mixer_load(handle) < 0) {
		snd_mixer_close(handle);
		return -4;
	}


	elem = snd_mixer_find_selem(handle, sid);
	if (!elem) {
		snd_mixer_close(handle);
		return -5;
	}

	/* vars that will store current 'min' and 'max' volume values
	 * and current volume percentage (relative to 'min' and 'max')
	 */
	long min, max, outvol;

	/* get volume range */
	snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
	warn("Volume range <%lf, %lf>", min, max);

	/* get current volume and calculate it's percentage */
	if (snd_mixer_selem_get_playback_volume(elem, 0, &outvol) < 0) {
		snd_mixer_close(handle);
		return -6;
	}
	warn("Current vol %ld", outvol);
	snd_mixer_close(handle);

	return bprintf("%d", outvol ? 0 : (outvol - min) * 100 / (max - min));
}

