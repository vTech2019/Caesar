#include <stdio.h>
#include <malloc.h>
typedef unsigned char uchar;
struct uchar2 {
	uchar x;
	uchar y;
};
size_t* first_freq(uchar* text, size_t length_text) {
	size_t* freq = (size_t*)calloc(256, sizeof(size_t));
	uchar* alphabet = (uchar*)malloc((256) * sizeof(uchar));
	for (size_t i = 0; i < 256; i++) {
		alphabet[i] = 0 + i;
	}
	for (size_t i = 0; i < length_text; i++) {
		for (size_t j = 0; j < 256; j++) {
			if (alphabet[j] == text[i])
				freq[j]++;
		}
	}
	free(alphabet);
	return freq;
}

size_t* first_freq(uchar* text, size_t length_text) {
	size_t* freq = (size_t*)calloc(65536, sizeof(size_t));
	//uchar2* alphabet = (uchar2*)malloc((65536) * sizeof(uchar2));
	//for (size_t i = 0; i < 65536; i++) {
	//	((unsigned short*)alphabet)[i] = i;
	//}
	for (size_t i = 0; i < length_text ; i++) {
		//for (size_t j = 0; j < 65536; j++) {
		//	if (*((unsigned short*)(alphabet + j)) == *((unsigned short*)(text + i)));
		//		freq[j]++;
		//}
		freq[text[i] || text[i-1] << 8]++;
	}
	//free(alphabet);
	return freq;
}

uchar* crypt_shift(uchar* text, size_t length_text, size_t shift) {
	uchar* crypt_text = (uchar*)malloc(length_text);
	for (size_t i = 0; i < length_text; i++) {
		crypt_text[i] = text[i] << shift;
	}
	return crypt_text;
}

uchar* print_freq(size_t* freq, size_t length) {
	uchar* table = (uchar*)malloc(51 * length * sizeof(uchar));
	for (size_t i = 0; i < 50 * length; i++)
		table[i] = 'o';
	double max = 0;
	for (size_t i = 0; i < length; i++) {
		if (max < freq[i])
			max = freq[i];
	}
	//max = 50. / max;
	for (size_t i = 1; i < length; i++) {
		table[i * 50] = i;
		for (size_t j = 1; j < 50; j++) {
			size_t length = max / (j);
			if (freq[i] < length)
				table[i * 50 + j] = '*';
			else if (j = 49)
				table[i * 50 + j] = '\n';
		}


	}
	return table;
}


size_t* crypt(uchar* text, size_t length_text, uchar** crypt_text) {
	*crypt_text = crypt_shift(text, length_text, 1);
	size_t* freq_text = first_freq(*crypt_text, length_text);
	//printf("%s \n", text);
	//printf("%s \n", crypt_text);
	for (size_t i = 0; i < 16; i++) {
		for (size_t j = 0; j < 16; j++) {
			printf("(%ld %c) ", freq_text[i * 16 + j], i * 16 + j);
		}
		printf("\n");
	}
	return freq_text;
}

size_t* freq_text(uchar* text, size_t length_text) {

	size_t* freq_text = first_freq(text, length_text);

	for (size_t i = 0; i < 16; i++) {
		for (size_t j = 0; j < 16; j++) {
			printf("(%ld %c) ", freq_text[i * 16 + j], i * 16 + j);
		}
		printf("\n");
	}

	return freq_text;
}
struct data {
	uchar symbol_0;
	uchar symbol_1;
	size_t freq;
};

uchar* decrypt(uchar* crypt_text, size_t length_crypt_text, size_t* freq_crypt_text, size_t* freq_text) {
	data* crypt_data = (data*)malloc(256 * sizeof(data));
	data* decrypt_data = (data*)malloc(256 * sizeof(data));
	uchar* decrypt_text = (uchar*)malloc(length_crypt_text * sizeof(uchar));
	for (size_t i = 0; i < 256; i++) {
		crypt_data[i].freq = freq_crypt_text[i];
		crypt_data[i].symbol_1 = i;
		decrypt_data[i].freq = freq_text[i];
		decrypt_data[i].symbol_0 = i;
	}
	for (size_t i = 0; i < 256; i++) {
		for (size_t j = i; j < 256; j++) {
			if (decrypt_data[i].freq < decrypt_data[j].freq) {
				data tmp = decrypt_data[j];
				decrypt_data[j] = decrypt_data[i];
				decrypt_data[i] = tmp;
			}
		}
	}
	for (size_t i = 0; i < 256; i++) {
		printf("%c %c %ld\n", decrypt_data[i].symbol_0, decrypt_data[i].symbol_1, decrypt_data[i].freq);
	}
	for (size_t i = 0; i < 256; i++) {
		for (size_t j = i; j < 256; j++) {
			if (crypt_data[i].freq < crypt_data[j].freq) {
				data tmp = crypt_data[j];
				crypt_data[j] = crypt_data[i];
				crypt_data[i] = tmp;
			}
		}
	}

	for (size_t i = 0; i < 256; i++) {
		printf("%c %c %ld\n", crypt_data[i].symbol_0, crypt_data[i].symbol_1, crypt_data[i].freq);
	}
	for (size_t i = 0; i < 256; i++) {
		crypt_data[i].symbol_0 = decrypt_data[i].symbol_0;
	}

	for (size_t i = 0; i < 256; i++) {
		for (size_t j = i; j < 256; j++) {
			if (crypt_data[i].symbol_1 > crypt_data[j].symbol_1) {
				data tmp = crypt_data[j];
				crypt_data[j] = crypt_data[i];
				crypt_data[i] = tmp;
			}
		}
	}


	for (size_t i = 0; i < 256; i++) {
		printf("%c %c %ld\n", crypt_data[i].symbol_0, crypt_data[i].symbol_1, crypt_data[i].freq);
	}
	for (size_t i = 0; i < length_crypt_text; i++) {
				decrypt_text[i] = crypt_data[crypt_text[i]].symbol_0;
		
	}
	free(crypt_data);
	free(decrypt_data);
	return decrypt_text;
}

int main() {
	FILE* file = fopen("text.txt", "rb");
	FILE* file_crypt = fopen("crypt.txt", "rb");
	fseek(file, 0, SEEK_END);
	fseek(file_crypt, 0, SEEK_END);
	long length = ftell(file);
	long length_crypt = ftell(file_crypt);
	rewind(file);
	rewind(file_crypt);
	uchar* text = (uchar*)malloc(length * sizeof(uchar));
	uchar* text_crypt = (uchar*)malloc(length_crypt * sizeof(uchar));
	uchar* crypt_text = NULL;

	fread(text, sizeof(uchar), length, file);
	fread(text_crypt, sizeof(uchar), length_crypt, file_crypt);

	size_t* freq_crypt = crypt(text_crypt, length_crypt, &crypt_text);
	size_t* freq_main_text = freq_text(text, length);
	uchar* decrypt_text = decrypt(crypt_text, length_crypt, freq_crypt, freq_main_text);
	//printf("%s", text);
	//printf("%s", crypt_text);
	printf("%s", decrypt_text);
	free(decrypt_text);
	free(freq_main_text);
	free(freq_crypt);
	free(text_crypt);
	free(text);
	free(crypt_text);
	fclose(file_crypt);
	fclose(file);
	return 0;
}