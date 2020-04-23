#include <alsa/asoundlib.h>

#include "../util.h"

const char *
alsa_vol(void)
{
	const char *card = "default";
	const char *mixer_name = "Master";
	const int min_index = 0;

	snd_mixer_t *handle;
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

	/* sets simple-mixer index and name */
	snd_mixer_selem_id_t *sid;
	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, min_index);
	snd_mixer_selem_id_set_name(sid, mixer_name);

	snd_mixer_elem_t* elem;
	if (!(elem = snd_mixer_find_selem(handle, sid))) {
		warn("snd_mixer_find_selem failed");
		snd_mixer_close(handle);
		return NULL;
	}

	/* check if muted */
	int is_unmuted;
	if (snd_mixer_selem_has_playback_switch(elem)) {
		if (snd_mixer_selem_get_playback_switch(elem, 0, &is_unmuted) < 0) {
			warn("snd_mixer_selem_get_playback_switch failed");
			is_unmuted = 0;
		}
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

	if (is_unmuted)
		return bprintf("♬-%ld%%", outvol);
	else
		return bprintf("%ld%%", outvol);
}

const char *
mic_state(void)
{
  const char *card = "default";
  const char *selem_name = "Capture";

  snd_mixer_t *handle;
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

	/* sets simple-mixer index and name */
  snd_mixer_selem_id_t *sid;
  snd_mixer_selem_id_alloca(&sid);
  snd_mixer_selem_id_set_index(sid, 0);
  snd_mixer_selem_id_set_name(sid, selem_name);

  snd_mixer_elem_t *elem;
	if (!(elem = snd_mixer_find_selem(handle, sid))) {
		warn("snd_mixer_find_selem failed");
		snd_mixer_close(handle);
		return NULL;
	}

  int mic_state;
  if (snd_mixer_selem_get_capture_switch(elem, SND_MIXER_SCHN_UNKNOWN, &mic_state) < 0) {
		warn("snd_mixer_selem_get_capture_switch failed");
		snd_mixer_close(handle);
		return NULL;
  }

	snd_mixer_close(handle);
  return mic_state ? bprintf("🎤 ") : bprintf("");
}

