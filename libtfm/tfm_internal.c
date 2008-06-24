#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tfm_internal.h"

static
int get_byte(FILE * fp) {
	unsigned char temp;
	size_t r = fread(&temp, 1, 1, fp);
	if (r != 1) return -1;
	return (int) temp;
}

static
int get_short(FILE * fp) {
	unsigned char temp[2];
	size_t r = fread(temp, 1, 2, fp);
	if (r != 2) return -1;
	return (int) temp[0] << 8 | temp[1];
}

static
int get_word(FILE * fp) {
	unsigned char temp[4];
	size_t r = fread(temp, 1, 4, fp);
	if (r != 4) return -1;
	return (int) temp[0] << 24 | temp[1] << 16 | temp[2] << 8 | temp[3];
}

static
fix_word get_fix_word(FILE * fp) {
	unsigned char temp[4];
	size_t r = fread(temp, 1, 4, fp);
	if (r != 4) return -1;
	return (fix_word) temp[0] << 24 | temp[1] << 16 | temp[2] << 8 | temp[3];
}

static
char * get_bcpl_string(FILE * fp, int max_len) {
	char * output;
	size_t result;
	int len = get_byte(fp);
	if (len < 0 || len > max_len - 1) return NULL;
	output = malloc(len + 1);
	if (!output) return NULL;
	result = fread(output, 1, len, fp);
	if (result != len) return NULL;
	output[len] = 0;
	fseek(fp, max_len - 1 - len, SEEK_CUR);

	return output;
}

static
int int_tfm_read_file_header(struct tfm_internal_file_s * f, FILE * fp) {
#define GET(x) { (x) = get_short(fp); if ((x) == -1) return -1; }
	GET(f->file_header.lf);
	GET(f->file_header.lh);
	GET(f->file_header.bc);
	GET(f->file_header.ec);
	GET(f->file_header.nw);
	GET(f->file_header.nh);
	GET(f->file_header.nd);
	GET(f->file_header.ni);
	GET(f->file_header.nl);
	GET(f->file_header.nk);
	GET(f->file_header.ne);
	GET(f->file_header.np);
#undef GET
	return 0;
}

static
int int_tfm_read_info_header(struct tfm_internal_file_s * f, FILE * fp) {
	long int fpos;
	int size = f->file_header.lh;
	if (size < 2) return -1;
	fpos = ftell(fp) + size * 4;
	f->info_header.checksum = get_word(fp);
	if (f->info_header.checksum == -1) return -1;
	f->info_header.design_size = get_fix_word(fp);
	if (f->info_header.design_size == -1) return -1;

	if (size >= 12) {
		f->info_header.encoding = get_bcpl_string(fp, 40);
		if (!f->info_header.encoding) return -1;
		if (size >= 17) {
			f->info_header.family = get_bcpl_string(fp, 20);
			if (!f->info_header.family) return -1;
			if (size >= 18) {
				f->info_header.seven_bit_safe_flag = get_byte(fp);
				fseek(fp, 2, SEEK_CUR);
				f->info_header.face = get_byte(fp);
			}
		}
	}
	fseek(fp, fpos, SEEK_SET);
	return 0;
}

static
int int_tfm_read_char_info(struct tfm_internal_file_s * f, FILE * fp) {
	int i;
	int num_chars = f->file_header.ec - f->file_header.bc + 1;
	if (num_chars < 0 || num_chars >= 256) return -1;

	f->char_info = (struct tfm_internal_char_info_s *) malloc(num_chars * sizeof(struct tfm_internal_char_info_s));
	if(!f->char_info) return -1;

	for(i = 0; i < num_chars; ++i) {
		int temp;
		temp = get_byte(fp); if(temp < 0) return -1;
		f->char_info[i].width_index = temp;
		temp = get_byte(fp); if(temp < 0) return -1;
		f->char_info[i].height_index = temp >> 4;
		f->char_info[i].depth_index = temp & 15;
		temp = get_byte(fp); if(temp < 0) return -1;
		f->char_info[i].italic_index = temp >> 2;
		f->char_info[i].tag = temp & 3;
		temp = get_byte(fp); if(temp < 0) return -1;
		f->char_info[i].remainder = temp;
	}
	return 0;
}

static
int int_tfm_read_lig_kern(struct tfm_internal_file_s * f, FILE * fp) {
	int i, b;
	int num_lig_kern = f->file_header.nl;
	f->lig_kern = (struct tfm_internal_lig_kern_s *)
		malloc(num_lig_kern * sizeof(struct tfm_internal_lig_kern_s));
	if (!f->lig_kern) return -1;
	for (i = 0; i < num_lig_kern; ++i) {
		b = get_byte(fp); if (b < 0) return -1;
		f->lig_kern[i].skip_byte = b;
		b = get_byte(fp); if (b < 0) return -1;
		f->lig_kern[i].next_char = b;
		b = get_byte(fp); if (b < 0) return -1;
		f->lig_kern[i].op_byte = b;
		b = get_byte(fp); if (b < 0) return -1;
		f->lig_kern[i].remainder = b;
	}
	return 0;
}

static
int int_tfm_read_exten(struct tfm_internal_file_s * f, FILE * fp) {
	int i, b;
	int num_exten = f->file_header.ne;
	f->extensible = (struct tfm_internal_extensible_s *)
		malloc(num_exten * sizeof(struct tfm_internal_extensible_s));
	if (!f->extensible) return -1;
	for (i = 0; i < num_exten; ++i) {
		b = get_byte(fp); if (b < 0) return -1;
		f->extensible[i].top = b;
		b = get_byte(fp); if (b < 0) return -1;
		f->extensible[i].middle = b;
		b = get_byte(fp); if (b < 0) return -1;
		f->extensible[i].bottom = b;
		b = get_byte(fp); if (b < 0) return -1;
		f->extensible[i].repeat = b;
	}
	return 0;
}

static
int get_fix_words(FILE * fp, fix_word ** poutput, int size) {
	int i;
	fix_word * output;
	if (size == 0) {
		*poutput = NULL;
		return 0;
	}
	if (size < 0) return -1;

	output = malloc(sizeof(fix_word) * size);
	if (!output) return -1;

	for(i = 0; i < size; ++i) {
		output[i] = get_fix_word(fp);
	}

	*poutput = output;
	return 0;
}

struct tfm_internal_file_s * int_tfm_open(char const * filename) {
	struct tfm_internal_file_s * f;
	FILE * fp;

	fp = fopen(filename, "rb");
	if(!fp) return NULL;

	f = (struct tfm_internal_file_s *) malloc(sizeof(struct tfm_internal_file_s));
	if(!f) return NULL;

	memset(f, 0, sizeof(struct tfm_internal_file_s));
	if (int_tfm_read_file_header(f, fp) < 0) return NULL;
	if (int_tfm_read_info_header(f, fp) < 0) return NULL;
	if (int_tfm_read_char_info(f, fp) < 0) return NULL;
	if (get_fix_words(fp, &f->width, f->file_header.nw) < 0) return NULL;
	if (get_fix_words(fp, &f->height, f->file_header.nh) < 0) return NULL;
	if (get_fix_words(fp, &f->depth, f->file_header.nd) < 0) return NULL;
	if (get_fix_words(fp, &f->italic, f->file_header.ni) < 0) return NULL;
	if (int_tfm_read_lig_kern(f, fp) < 0) return NULL;
	if (get_fix_words(fp, &f->kern, f->file_header.nk) < 0) return NULL;
	if (int_tfm_read_exten(f, fp) < 0) return NULL;
	if (get_fix_words(fp, &f->param, f->file_header.np) < 0) return NULL;

	fclose(fp);
	return f;
}

void int_tfm_close(struct tfm_internal_file_s * f) {
	if (f->info_header.encoding) free(f->info_header.encoding);
	if (f->info_header.family) free(f->info_header.family);
	if (f->width) free(f->width);
	if (f->height) free(f->height);
	if (f->depth) free(f->depth);
	if (f->italic) free(f->italic);
	free(f);
}

