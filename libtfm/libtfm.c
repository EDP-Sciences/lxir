#include <kpathsea/kpathsea.h>
#define USE_KPSE 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "libtfm.h"
#include "tfm_internal.h"

static
int make_tfm_kerning_info(struct tfm_file_s * tfm, struct tfm_internal_file_s * f, int i) {
	int c = tfm->character_offset + i;
	int kern_count = 0;
	int index = f->char_info[c].remainder;
	while (1) {
		int skip_byte = f->lig_kern[index].skip_byte;
		if (f->lig_kern[index].op_byte >= 128)	{
			++kern_count;
		}
		if (skip_byte >= 128) break;
		index += skip_byte + 1;
	}
	tfm->characters[i].kerning_count = kern_count;
	if (kern_count > 0) {
		tfm->characters[i].kernings =
			(struct tfm_kerning_s *) malloc(kern_count
			* sizeof(struct tfm_kerning_s));
		if (!tfm->characters[i].kernings) {
			return -1;
		}

		kern_count = 0;
		index = f->char_info[c].remainder;
		while (1) {
			float value;
			int skip_byte = f->lig_kern[index].skip_byte;
			if (f->lig_kern[index].op_byte >= 128)	{
				tfm->characters[i].kernings[kern_count].next_char = f->lig_kern[index].next_char;
				value = fix_word_to_float(f->kern[f->lig_kern[index].op_byte - 128 + f->lig_kern[index].remainder]);
				tfm->characters[i].kernings[kern_count].kerning = (int) floor(value * tfm->scale + .5f);
				++kern_count;
			}
			if (skip_byte >= 128) break;
			index += skip_byte + 1;
		}
	}
	return 0;
}

tfm_file_t tfm_open(char const * fontname, int scale) {
	struct tfm_file_s * tfm;
	struct tfm_internal_file_s * f;
	int i;

	const char * kpse_path = kpse_find_file (fontname, kpse_tfm_format, true);
	if(!kpse_path) {
		fprintf(stderr, "libtfm error: unable to find tfm file for font %s\n",
			fontname);
		return NULL;
	}

	f = int_tfm_open(kpse_path);
	if (!f) {
		fprintf(stderr, "libtfm error: invalid tfm file %s\n", kpse_path);
		return NULL;
	}

	tfm = (struct tfm_file_s *) malloc(sizeof(struct tfm_file_s));
	if (!tfm) {
		fprintf(stderr, "libtfm error: allocation error\n");
		int_tfm_close(f);
		return NULL;
	}

	tfm->filename = strdup(kpse_path);
	tfm->scale = scale;

	tfm->character_offset = f->file_header.bc;
	tfm->character_count = f->file_header.ec - tfm->character_offset + 1;

	tfm->characters = (struct tfm_character_s *) malloc(tfm->character_count
		* sizeof(struct tfm_character_s));

	if (!tfm->characters) {
		fprintf(stderr, "libtfm error: allocation error\n");
		int_tfm_close(f);
		tfm_close(tfm);
		return NULL;
	}
	for (i = 0; i < tfm->character_count; ++i) {
		int c;
		float value;

		c = tfm->character_offset + i;

		value = fix_word_to_float(f->width[f->char_info[c].width_index]);
		tfm->characters[i].width = (int) floor(value * scale + .5f);
		value = fix_word_to_float(f->height[f->char_info[c].height_index]);
		tfm->characters[i].height = (int) floor(value * scale + .5f);
		value = fix_word_to_float(f->depth[f->char_info[c].depth_index]);
		tfm->characters[i].depth = (int) floor(value * scale + .5f);
		value = fix_word_to_float(f->italic[f->char_info[c].italic_index]);
		tfm->characters[i].italic = (int) floor(value * scale + .5f);

		if (f->char_info[c].tag == 1) {
			if (make_tfm_kerning_info(tfm, f, i) < 0) {
				fprintf(stderr, "libtfm error: allocation error\n");
				int_tfm_close(f);
				tfm_close(tfm);
				return NULL;
			}
		}
	}

	int_tfm_close(f);
	return tfm;
}

void tfm_close(tfm_file_t tfm) {
	if (!tfm) return;

	if (tfm->characters) {
		int i;
		for (i = 0; i < tfm->character_count; ++i) {
			if (tfm->characters[i].kerning_count) {
				free(tfm->characters[i].kernings);
			}
		}
		free(tfm->characters);
	}
	free(tfm);
}

int tfm_size(tfm_file_t tfm, int first_char, int next_char) {
	int char_index = first_char - tfm->character_offset;
	if (char_index < 0 || char_index >= tfm->character_count) {
		fprintf(stderr, "libtfm error: invalid char requested\n");
		return 0;
	}
	if (next_char > 0 && tfm->characters[char_index].kerning_count) {
		int i;
		for (i = 0; i < tfm->characters[char_index].kerning_count; ++i) {
			if (tfm->characters[char_index].kernings[i].next_char == next_char) {
				return tfm->characters[char_index].width +
					tfm->characters[char_index].kernings[i].kerning;
			}
		}
	}
	return tfm->characters[char_index].width;
}
