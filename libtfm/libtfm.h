#ifndef LIBTFM_H
#define LIBTFM_H 1

#ifndef USE_KPSE
# error libtfm requires kpathsea
#endif

struct tfm_kerning_s {
	int next_char;
	int kerning;
};

struct tfm_character_s {
	int width;
	int height;
	int depth;
	int italic;

	int kerning_count;
	struct tfm_kerning_s * kernings;
};

struct tfm_file_s {
	char * filename;
	int scale;
	int sig;

	int character_offset;
	int character_count;
	struct tfm_character_s * characters;
};

typedef struct tfm_file_s * tfm_file_t;

tfm_file_t tfm_open(char const * fontname, int scale);
void tfm_close(tfm_file_t);
int tfm_size(tfm_file_t, int first_char, int next_char);

#endif
