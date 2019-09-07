#include "Source.h"
void print_info(size_t* data, size_t length_data) {
	size_t length = sqrt(length_data);
	for (size_t i = 0; i < length; i++) {
		for (size_t j = 0; j < length; j++) {
			printf("(%zd %zd) ", data[i * length + j], i * length + j);
		}
		printf("\n");
	}
}

size_t* freq(uchar* text, size_t length_text, size_t round_dictionary) {
	size_t length_dictionary = 1 << round_dictionary * 8;
	size_t* freq = (size_t*)calloc(length_dictionary, sizeof(size_t));
	for (size_t i = 0; i < length_text - round_dictionary + 1; i += round_dictionary) {
		size_t index = 0;
		for (size_t j = round_dictionary; j != 0; j--) {
			index |= (size_t)text[i + j - 1] << (8 * (j - 1));
		}
		index = index > length_dictionary ? index & length_dictionary : index;
		freq[index]++;
	}
	//free(freq);
	return freq;
}

uchar* crypt_shift(uchar* text, size_t length_text, size_t shift) {
	uchar* crypt_text = (uchar*)malloc(length_text);
	for (size_t i = 0; i < length_text; i++) {
		crypt_text[i] = ROR(text[i], shift);
	}
	return crypt_text;
}

void print_freq(size_t* freq, size_t length) {
	uchar* table = (uchar*)malloc(51 * length * sizeof(uchar));
	for (size_t i = 0; i < 50 * length; i++)
		table[i] = 'o';
	double max = 0;
	for (size_t i = 0; i < length; i++) {
		if (max < freq[i])
			max = freq[i];
	}
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
	printf("%s \n", table);
	free(table);
}

size_t* crypt(uchar* text, size_t length_text, uchar** crypt_text, size_t round_dictionary) {
	*crypt_text = crypt_shift(text, length_text, 1);
	size_t* freq_text = freq(*crypt_text, length_text, round_dictionary);
	return freq_text;
}

size_t* freq_text(uchar* text, size_t length_text, size_t round_dictionary) {
	size_t* freq_text = freq(text, length_text, round_dictionary);
	return freq_text;
}

uchar* decrypt(uchar* crypt_text, size_t length_crypt_text, size_t* freq_crypt_text, size_t* freq_text, size_t round_dictionary) {
	size_t length_dictionary = 1ull << round_dictionary * 8ull;
	uchar* decrypt_text = (uchar*)calloc(length_crypt_text, sizeof(uchar));
	uchar* memory = (uchar*)_mm_malloc(round_dictionary * 2 * length_dictionary * sizeof(uchar), 4096);
	size_t* index_crypt_text = freq_crypt_text;
	size_t* index_text = freq_text;
	uchar* code_crypt_text = memory;
	uchar* code_text = memory + length_dictionary * round_dictionary;

	for (size_t i = 0; i < round_dictionary * length_dictionary; ) {
		size_t code = i / round_dictionary;
		for (size_t j = 0; j < round_dictionary; j++) {
			code_crypt_text[i] = (uchar)code;
			code_text[i++] = (uchar)code;
			code >>= 8;
		}
	}
	size_t time = __rdtsc();
	for (ptrdiff_t i = 0, memory = 0; i < length_dictionary - 1;) {
		if (index_crypt_text[i] <= index_crypt_text[i + 1])
			memory++,
			i = memory;
		else {
			for (size_t k = 0; k < round_dictionary; k++) {
				uchar tmp = code_crypt_text[i * round_dictionary + k];
				code_crypt_text[i * round_dictionary + k] = code_crypt_text[(i + 1) * round_dictionary + k];
				code_crypt_text[(i + 1) * round_dictionary + k] = tmp;
			}
			size_t index = index_crypt_text[i];
			index_crypt_text[i] = index_crypt_text[i + 1];
			index_crypt_text[i + 1] = index;
			i -= i > 0 ? 1 : 0;
		}
	}
	//for (size_t i = 0; i < length_dictionary; i++)
	//	printf("%d\n", index_crypt_text[i]);
	for (size_t j = 0; j < length_dictionary; j++) {
		for (size_t i = j; i < length_dictionary; i++) {
			if (index_text[i] < index_text[j]) {
				for (size_t k = 0; k < round_dictionary; k++) {
					uchar tmp = code_text[i * round_dictionary + k];
					code_text[i * round_dictionary + k] = code_text[(j)* round_dictionary + k];
					code_text[(j)* round_dictionary + k] = tmp;
				}
				size_t index = index_text[i];
				index_text[i] = index_text[j];
				index_text[j] = index;
			}
		}
	}
	time = __rdtsc() - time;
	printf("%d\n", time);
	for (size_t i = 0; i < length_dictionary; i++) {
		for (size_t j = i; j < length_dictionary; j++) {
			if (code_crypt_text[i] > code_crypt_text[j]) {
				for (size_t k = 0; k < round_dictionary; k++) {
					uchar tmp = code_crypt_text[i * round_dictionary + k];
					code_crypt_text[i * round_dictionary + k] = code_crypt_text[j * round_dictionary + k];
					code_crypt_text[j * round_dictionary + k] = tmp;
					tmp = code_text[i * round_dictionary + k];
					code_text[i * round_dictionary + k] = code_text[j * round_dictionary + k];
					code_text[j * round_dictionary + k] = tmp;
				}
				size_t tmp_2 = index_crypt_text[i];
				index_crypt_text[i] = index_crypt_text[j];
				index_crypt_text[j] = tmp_2;
			}
		}
	}
	for (size_t i = 0; i < length_crypt_text - round_dictionary;) {
		size_t index = 0;
		for (size_t j = round_dictionary + i; j != i; j--) {
			index <<= 8;
			index |= (uchar)index + crypt_text[j - 1];
		}
		for (size_t j = 0; j < round_dictionary; j++)
			decrypt_text[i++] = code_text[index * round_dictionary + j];
	}
	_mm_free(memory);
	return decrypt_text;
}

int main() {
	//setlocale(LC_ALL, "ru_RU.UTF-8");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	FILE* file = fopen("white.txt", "rb");
	FILE* file_crypt = fopen("war.txt", "rb");

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
	fclose(file_crypt);
	fclose(file);
	size_t* freq_crypt = crypt(text_crypt, length_crypt, &crypt_text, 1);
	size_t* freq_main_text = freq_text(text, length, 1);
	uchar* decrypt_text = decrypt(crypt_text, length_crypt, freq_crypt, freq_main_text, 1);

	//printf("%s", text);
	//printf("%s", crypt_text);
	printf("%s", decrypt_text);
	free(decrypt_text);
	free(freq_main_text);
	free(freq_crypt);
	free(text_crypt);
	free(text);
	free(crypt_text);
	return 0;
}