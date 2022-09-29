#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <bitset>
#include "permutations.h"
#include "constants.h"

using namespace std;

// функция разделяет результат сложения E(r - 1) с ключём на 8 6 битовых блоков
unsigned char* slice_blocks(unsigned char ER_xor_key[6]) {
	unsigned char* B = new unsigned char[8];	// каждый блок представлен 6 битами,
	memset(B, 0, 8);							// но каждый представлен одним байтом
	B[0] = (ER_xor_key[0] >> 2) & 0b00111111;
	B[1] = ((ER_xor_key[0] & 0b00000011) << 4) | ((ER_xor_key[1] & 0b11110000) >> 4);
	B[2] = ((ER_xor_key[1] & 0b00001111) << 2) | ((ER_xor_key[2] & 0b11000000) >> 6);
	B[3] = (ER_xor_key[2] & 0b00111111);

	B[4] = (ER_xor_key[3] >> 2) & 0b00111111;
	B[5] = ((ER_xor_key[3] & 0b00000011) << 4) | ((ER_xor_key[4] & 0b11110000) >> 4);
	B[6] = ((ER_xor_key[4] & 0b00001111) << 2) | ((ER_xor_key[5] & 0b11000000) >> 6);
	B[7] = (ER_xor_key[5] & 0b00111111);

	return B;
}

unsigned char* convert_S(unsigned char B[8]) {
	unsigned char* B_converted = new unsigned char[4];
	memset(B_converted, 0, 4);
	
	unsigned char temp[8];
	memset(temp, 0, 8);
	for (int i = 0; i < 8; i++) {
		int row = 0, column = 0;
		row = (B[i] & 0b00100000) >> 4 | (B[i] & 0b00000001);	// определяем строку
		column = (B[i] & 0b00011110) >> 1;	// и столбец
		temp[i] = S[i][row][column];	// вычисляем число и записываем его
	}
	B_converted[0] = temp[0] << 4 | temp[1];	// объединяем числа в один байт
	B_converted[1] = temp[2] << 4 | temp[3];
	B_converted[2] = temp[4] << 4 | temp[5];
	B_converted[3] = temp[6] << 4 | temp[7];

	return B_converted;
}

// функция циклического смещения векторов C и D
unsigned char* shift_CD(unsigned char CD[7], int iter) {
	unsigned char* shifted_CD = new unsigned char[7];
	unsigned char C[4];
	unsigned char D[4];
	// разбиваем исходный вектор на 28 битные блоки
	memcpy(C, CD, 4);
	C[3] &= 0b11110000;
	memcpy(D, CD + 3, 4);
	D[0] &= 0b00001111;
	// циклический сдвиг влево для C
	if (shift_CD_table[iter] == 1) {
		int save_bits[4] = { 0, 0, 0, 0 };
		for (int i = 0; i < 4; i++)
			save_bits[i] = C[i] & 0b10000000;
		C[0] = C[0] << 1; C[0] |= (save_bits[1] >> 7);
		C[1] = C[1] << 1; C[1] |= (save_bits[2] >> 7);
		C[2] = C[2] << 1; C[2] |= (save_bits[3] >> 7);
		C[3] = C[3] << 1; C[3] |= (save_bits[0] >> 3);
	}
	else {
		int save_bits[4] = { 0, 0, 0, 0 };
		for (int i = 0; i < 4; i++)
			save_bits[i] = C[i] & 0b11000000;
		C[0] = C[0] << 2; C[0] |= (save_bits[1] >> 6);
		C[1] = C[1] << 2; C[1] |= (save_bits[2] >> 6);
		C[2] = C[2] << 2; C[2] |= (save_bits[3] >> 6);
		C[3] = C[3] << 2; C[3] |= (save_bits[0] >> 2);
	}
	// циклический сдвиг влево для D
	if (shift_CD_table[iter] == 1) {
		int save_bits[4] = { 0, 0, 0, 0 };
		for (int i = 1; i < 4; i++)
			save_bits[i] = D[i] & 0b10000000;
		save_bits[0] = D[0] & 0b00001000;	// так как байт неровный, приходится делать так

		D[0] = D[0] << 1; D[0] |= (save_bits[1] >> 7);
		D[1] = D[1] << 1; D[1] |= (save_bits[2] >> 7);
		D[2] = D[2] << 1; D[2] |= (save_bits[3] >> 7);
		D[3] = D[3] << 1; D[3] |= (save_bits[0] >> 3);
	}
	else {
		int save_bits[4] = { 0, 0, 0, 0 };
		for (int i = 0; i < 4; i++)
			save_bits[i] = D[i] & 0b11000000;
		save_bits[0] = D[0] & 0b00001100;	// так как байт неровный, приходится делать так

		D[0] = D[0] << 2; D[0] |= (save_bits[1] >> 6);
		D[1] = D[1] << 2; D[1] |= (save_bits[2] >> 6);
		D[2] = D[2] << 2; D[2] |= (save_bits[3] >> 6);
		D[3] = D[3] << 2; D[3] |= (save_bits[0] >> 2);
	}
	
	// собираем разделённый байт
	char div_byte = 0;
	div_byte = (C[3] & 0b11110000) | (D[0] & 0b00001111);
	// склеиваем вектор
	shifted_CD[0] = C[0];
	shifted_CD[1] = C[1];
	shifted_CD[2] = C[2];
	shifted_CD[3] = div_byte;
	shifted_CD[4] = D[1];
	shifted_CD[5] = D[2];
	shifted_CD[6] = D[3];

	return shifted_CD;
}

unsigned char* Feistel_func(unsigned char R[4], unsigned char key[6]) {
	auto expanded_R = permutate_E(R);	// расширяем R по матрице E
	for (int i = 0; i < 6; i++) {
		expanded_R[i] = expanded_R[i] ^ key[i];	// исключающее или с ключём
	}

	auto B = slice_blocks(expanded_R);	// преобразуем в блоки по 6 бит
	auto converted_B = convert_S(B);	// получаем вектор 32 бит после преобразования через S

	converted_B = permutate_P_small(converted_B);	// выполняем конечную перестановку

	delete expanded_R;
	delete B;

	return converted_B;
}

unsigned char* encrypt_block(unsigned char data[8], unsigned char key[8]) {
	auto permutate_data = permutate_P(data);	// первое преобразование P
	auto CD = permutate_key(key);		// вектор CD

	// разбиваем блок на L и R
	unsigned char L[4];
	unsigned char R[4];
	memcpy(L, permutate_data, 4);
	memcpy(R, permutate_data + 4, 4);
	unsigned char temp_R[4];	// буфер для перемещения значений между L и R
	memcpy(temp_R, R, 4);

	for (int i = 0; i < 16; i++) {
		CD = shift_CD(CD, i);					// смещение CD
		auto key_iter = generate_key_iter(CD);	// генерация ключа итерации

		auto func_res = Feistel_func(R, key_iter);	// вычисление функции Фейстеля
		for (int j = 0; j < 4; j++) {
			R[j] = L[j] ^ func_res[j];	// исключающее или с функцией Фейстеля
		}
		memcpy(L, temp_R, 4);	// присваиваем L R-1
		memcpy(temp_R, R, 4);	// запоминаем текущее R

		delete key_iter;
		delete func_res;
	}

	unsigned char RL[8];		// склеиваем R и L
	memcpy(RL, R, 4);
	memcpy(RL + 4, L, 4);

	// конечная перестановка C - зашифрованное сообщение
	auto encrypted_data = permutate_P_reverse(RL);

	delete permutate_data;
	delete CD;

	return encrypted_data;
}

unsigned char* decrypt_block(unsigned char data[8], unsigned char key[8]) {
	auto permutate_data = permutate_P(data);	// первое преобразование P
	auto CD = permutate_key(key);		// вектор CD

	// разбиваем блок на L и R
	unsigned char L[4];
	unsigned char R[4];
	memcpy(L, permutate_data, 4);
	memcpy(R, permutate_data + 4, 4);
	unsigned char temp_R[4];	// буфер для перемещения значений между L и R
	memcpy(temp_R, R, 4);

	// предварительно генерируем ключи от 0 до 16
	unsigned char* keys[16];
	for (int i = 0; i < 16; i++) {
		CD = shift_CD(CD, i);
		keys[i] = new unsigned char[6];
		memset(keys[i], 0, 6);
		keys[i] = generate_key_iter(CD);
	}

	for (int i = 15; i >= 0; i--) {
		// используем ранее сгенерированные ключи
		auto func_res = Feistel_func(R, keys[i]);	// вычисление функции Фейстеля
		for (int j = 0; j < 4; j++) {
			R[j] = L[j] ^ func_res[j];	// исключающее или с функцией Фейстеля
		}
		memcpy(L, temp_R, 4);	// присваиваем L R-1
		memcpy(temp_R, R, 4);	// запоминаем текущее R

		delete func_res;
	}

	unsigned char RL[8];		// склеиваем R и L
	memcpy(RL, R, 4);
	memcpy(RL + 4, L, 4);

	// конечная перестановка C - расшифрованное сообщение
	auto decrypted_data = permutate_P_reverse(RL);

	for (int i = 0; i < 16; i++)
		delete keys[i];
	delete permutate_data;
	delete CD;

	return decrypted_data;
}

void encrypt_file(std::string file_name, std::string str_key) {
	unsigned char key[8];
	std::ifstream file(file_name);
	size_t added_bytes = 0;
	size_t file_length = 0;
	char* bytes;

	// копируем ключ в специальный массив
	memset(key, 0, 8);
	for (int i = 0; i < 8; i++)
		key[i] = str_key[i];

	// получаем длину файла
	file.seekg(0, std::ios::end);
	file_length = file.tellg();
	file.seekg(0, std::ios::beg);

	// собираем байты в массив
	if (file_length % 8 == 0) {		// если кратно 8, то дополнять не нужно
		added_bytes = 0;
		bytes = new char[file_length];
	}
	else {	// (8 - (length % 8)) - чтобы дополнить 64 битного блока
		added_bytes = (8 - (file_length % 8));
		bytes = new char[file_length + added_bytes];
		memset(bytes + file_length, 0, added_bytes);	// заполняем лишнее пространство нулями
	}
	// читаем файл
	file.read(bytes, file_length);

	std::ofstream encrypted_file("encrypted_" + file_name);

	// начинаем кодировать блоками
	for (int i = 0; i < (file_length + added_bytes) / 8; i++) {
		unsigned char block[8];
		int byte_offset = 0;
		// копируем следующий блок и конвертируем байты в байты без знака
		for (int j = i * 8; j < (i + 1) * 8; j++) {
			block[byte_offset] = (unsigned char)bytes[j];
			byte_offset++;
		}
		// кодируем блок
		auto encrypted_block = encrypt_block(block, key);
		// записываем его в файл
		for (int j = 0; j < 8; j++)
			encrypted_file << encrypted_block[j];

		delete encrypted_block;
	}

	std::cout << "file encryped" << std::endl;
}

void decrypt_file(std::string file_name, std::string str_key) {
	unsigned char key[8];
	std::ifstream file(file_name);
	size_t added_bytes = 0;
	size_t file_length = 0;
	char* bytes;

	// копируем ключ в специальный массив
	memset(key, 0, 8);
	for (int i = 0; i < 8; i++)
		key[i] = str_key[i];

	// получаем длину файла
	file.seekg(0, std::ios::end);
	file_length = file.tellg();
	file.seekg(0, std::ios::beg);

	// собираем байты в массив
	if (file_length % 8 == 0) {		// если кратно 8, то дополнять не нужно
		added_bytes = 0;
		bytes = new char[file_length];
	}
	else {	// (8 - (length % 8)) - чтобы дополнить 64 битного блока
		added_bytes = (8 - (file_length % 8));
		bytes = new char[file_length + added_bytes];
		memset(bytes + file_length, 0, added_bytes);	// заполняем лишнее пространство нулями
	}
	// читаем файл
	file.read(bytes, file_length);

	std::ofstream dectyped_file("decrypted_" + file_name);

	// начинаем кодировать блоками
	for (int i = 0; i < (file_length + added_bytes) / 8; i++) {
		unsigned char block[8];
		int byte_offset = 0;
		// копируем следующий блок и конвертируем байты в байты без знака
		for (int j = i * 8; j < (i + 1) * 8; j++) {
			block[byte_offset] = (unsigned char)bytes[j];
			byte_offset++;
		}
		// кодируем блок
		auto encrypted_block = decrypt_block(block, key);
		// записываем его в файл
		for (int j = 0; j < 8; j++)
			dectyped_file << encrypted_block[j];

		delete encrypted_block;
	}

	std::cout << "file decrypted" << std::endl;
}


int main() {
	unsigned char data[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
	unsigned char key[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };

	encrypt_file("test.txt", "abcdefgh");
	decrypt_file("encrypted_test.txt", "abcdefgh");

	return 0;
}

/*int count_set_bits(unsigned char byte) {
	int set = 0;
	for (int i = 0; i < 8; i++) {
		set += byte % 2;
		byte /= 2;
	}
	return set;
}

// функция добавляет биты чётности к исходному ключу
unsigned char* add_key_bits(unsigned char key[7]) {
	unsigned char* key_with_bits = new unsigned char[8];
	memcpy(key_with_bits, key, 7);
	memset(key_with_bits + 7, 0, 1);
	for (int i = 0; i < 8; i++) {
		char byte = key_with_bits[i] & 0b11111110;
		int save_bit = key_with_bits[i] & 0b00000001;
		if (count_set_bits(byte) % 2 != 0) {	// если число единиц нечётно ->
			byte |= 0b00000001;					// добавляем до чётности
		}
		key_with_bits[i] = byte;	// сохраняем байт
		// смещение остальных байтов
		for (int j = i + 1; j < 8; j++) {
			int temp_bit = key_with_bits[j] & 0b00000001;
			key_with_bits[j] = key_with_bits[j] >> 1;
			key_with_bits[j] |= (save_bit << 7);
			save_bit = temp_bit;
		}
	}
	return key_with_bits;
}*/