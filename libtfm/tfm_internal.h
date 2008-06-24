#ifndef TFM_INTERNAL_H
#define TFM_INTERNAL_H 1

typedef int int32;
typedef int32 fix_word;
#define fix_word_to_float(x) ( (x) / ((float)(1 << 20)) )

struct tfm_internal_file_header_s {
	int lf;
	int lh;
	int bc;
	int ec;
	int nw;
	int nh;
	int nd;
	int ni;
	int nl;
	int nk;
	int ne;
	int np;
};

struct tfm_internal_info_header_s {
	int checksum;
	fix_word design_size;

	char * encoding;
	char * family;

	int seven_bit_safe_flag;
	int face;
};

struct tfm_internal_char_info_s {
	int width_index;
	int height_index;
	int depth_index;
	int italic_index;
	int tag;
	int remainder;
};

struct tfm_internal_lig_kern_s {
	int skip_byte;
	int next_char;
	int op_byte;
	int remainder;
};

struct tfm_internal_extensible_s {
	int top;
	int middle;
	int bottom;
	int repeat;
};

struct tfm_internal_file_s {
	struct tfm_internal_file_header_s file_header;
	struct tfm_internal_info_header_s info_header;
	struct tfm_internal_char_info_s * char_info;

	fix_word * width;
	fix_word * height;
	fix_word * depth;
	fix_word * italic;

	struct tfm_internal_lig_kern_s * lig_kern;
	fix_word * kern;

	struct tfm_internal_extensible_s * extensible;
	fix_word * param;

};

struct tfm_internal_file_s * int_tfm_open(char const * path);
void int_tfm_close(struct tfm_internal_file_s *);

#endif
