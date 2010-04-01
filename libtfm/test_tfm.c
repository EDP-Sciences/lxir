#include <kpathsea/kpathsea.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "libtfm.h"
#include "tfm_internal.h"

static
char const * get_tag_info(struct tfm_internal_file_s * f, int c) {
static char buffer[32];
	int tag = f->char_info[c].tag;
	switch(tag) {
		case 0: return "<no tag>";
		case 1: sprintf(buffer, "<ligature:%d>", f->char_info[c].remainder); break;
		case 2: return "<chain>";
		case 3: return "<extensible>";
		default: sprintf(buffer, "<UNKNOWN TAG:%d>", tag); break;
	}
	return buffer;
}

static
char const * get_lig_kern_info(struct tfm_internal_file_s * f, int c, struct tfm_internal_lig_kern_s * inst) {
static char buffer[256];
	if (inst->op_byte < 128) {
		int i;
		int keep_curr, keep_next, skip;
		static char str_before[256], str_after[4], str_temp[2];
		keep_curr = inst->op_byte & 2;
		keep_next = inst->op_byte & 1;
		skip = inst->op_byte >> 2;
		sprintf(str_before, "%c%c", c, inst->next_char);
		for (i = 0; i < skip; i++) {
			strcat(str_before, "?");
		}
		str_after[0] = 0;
		if (keep_curr) {
			sprintf(str_temp, "%c", c); strcat(str_after, str_temp);
		}
		sprintf(str_temp, "%c", inst->remainder); strcat(str_after, str_temp);
		if (keep_next) {
			sprintf(str_temp, "%c", inst->next_char); strcat(str_after, str_temp);
		}

		sprintf(buffer, "ligature: %d (%c) keep_curr = %s, keep_next = %s, skip = %d, %s => %s\n",
			inst->remainder, inst->remainder,
			keep_curr ? "true" : "false", keep_next ? "true" : "false", skip,
			str_before, str_after);
	} else {
		sprintf(buffer, "kern : %.2f", fix_word_to_float(f->kern[inst->op_byte - 128 + inst->remainder]));
	}
	return buffer;
}

static
void print_internal_tfm_info(const char * fontname, struct tfm_internal_file_s * f) {
	int c;
	int num_chars = f->file_header.ec - f->file_header.bc + 1;
	printf("TFM info for font %s:\n", fontname);
	printf("\tFont valid for char from %d (%c) to %d (%c) (%d chars)\n",
		f->file_header.bc, f->file_header.bc,
		f->file_header.ec, f->file_header.ec, num_chars);
	printf("\tFont Design Size: %.2f\n", fix_word_to_float(f->info_header.design_size));
	printf("\tFont Encoding: %s, Font Family: %s\n",
		f->info_header.encoding, f->info_header.family);
	printf("\tFont params:\n");
	for(c = 0; c < f->file_header.np; ++c) {
		printf("\t\t%d\t%.4f\n", c, fix_word_to_float(f->param[c]));
	}
	printf("Characters info:\n");
	for(c = 0; c < num_chars; ++c) {
		printf("\t\t%d\t%c\t%.4f\t%.4f\t%.4f\t%.4f\t%s\n", c, c + f->file_header.bc,
			fix_word_to_float(f->width[f->char_info[c].width_index]),
			fix_word_to_float(f->height[f->char_info[c].height_index]),
			fix_word_to_float(f->depth[f->char_info[c].depth_index]),
			fix_word_to_float(f->italic[f->char_info[c].italic_index]),
			get_tag_info(f, c)
		);
		if (f->char_info[c].tag == 1) {
			int index = f->char_info[c].remainder;
			printf("\t\t\tLigature program:\n");
			while (1) {
				int skip_byte = f->lig_kern[index].skip_byte;
				printf("\t\t\t\t%d\t%d\t%d (%c)\t%s\n",
					index, skip_byte,
					f->lig_kern[index].next_char, f->lig_kern[index].next_char,
					get_lig_kern_info(f, c, &f->lig_kern[index]));
				if (skip_byte >= 128) break;
				index += skip_byte + 1;
			}
		}
	}
}

static
void test_internal(const char * fontname) {
	const char * kpse_path = kpse_find_file (fontname, kpse_tfm_format, true);
	struct tfm_internal_file_s * f = int_tfm_open(kpse_path);
	if(f) {
		print_internal_tfm_info(fontname, f);
		int_tfm_close(f);
	}
}

static
void print_tfm_info(tfm_file_t tfm) {
	int i;
	printf("Information for font %s (%d):\n", tfm->filename, tfm->scale);

	for(i = 0; i < tfm->character_count; ++i) {
		int c = tfm->character_offset + i;
		printf("\t%d\t%c\t%d\n", i, (isgraph(c) ? c : ' '), tfm_size(tfm, c, -1));
	}
}

static
void test_external(const char * fontname, int scale) {
	tfm_file_t tfm = tfm_open(fontname, scale);
	print_tfm_info(tfm);
	tfm_close(tfm);
}

int main(int argc, char * argv[]) {
	int i;
	kpse_set_program_name(argv[0], "latex");
	for(i = 1; i < argc; ++i) {
		test_internal(argv[i]);
		test_external(argv[i], 655360);
	}
	return 0;
}
