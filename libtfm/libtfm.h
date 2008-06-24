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
	int checksum;

	int character_offset;
	int character_count;
	struct tfm_character_s * characters;
};

typedef struct tfm_file_s tfmfile_t;

tfmfile_t * tfm_open(char const * fontname, int scale);
void tfm_close(tfmfile_t *);

/*
	Renvoie la taille du caractère 'first_char' selon la police donnée.
	si pnext_char n'est pas NULL, il pointe vers le caractère suivant
	et, apres l'execution de la fonction, il pointe vers la distance de kerning
	nécéssaire (0 s'il n'y a pas de kerning possible).
*/
int tfm_size(tfmfile_t *, int first_char, int *pnext_char);

#endif
