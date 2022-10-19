#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <bitset>
#include "permutations.h"
#include "constants.h"
#include <chrono>
#include <thread>

using namespace std;

// ������� ��������� ��������� �������� E(r - 1) � ������ �� 8 6 ������� ������
unsigned char* slice_blocks(unsigned char ER_xor_key[6]) {
	unsigned char* B = new unsigned char[8];	// ������ ���� ����������� 6 ������,
	memset(B, 0, 8);							// �� ������ ����������� ����� ������
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
		row = (B[i] & 0b00100000) >> 4 | (B[i] & 0b00000001);	// ���������� ������
		column = (B[i] & 0b00011110) >> 1;	// � �������
		temp[i] = S[i][row][column];	// ��������� ����� � ���������� ���
	}
	B_converted[0] = temp[0] << 4 | temp[1];	// ���������� ����� � ���� ����
	B_converted[1] = temp[2] << 4 | temp[3];
	B_converted[2] = temp[4] << 4 | temp[5];
	B_converted[3] = temp[6] << 4 | temp[7];

	return B_converted;
}

// ������� ������������ �������� �������� C � D
unsigned char* shift_CD(unsigned char CD[7], int iter) {
	unsigned char* shifted_CD = new unsigned char[7];
	unsigned char C[4];
	unsigned char D[4];
	// ��������� �������� ������ �� 28 ������ �����
	memcpy(C, CD, 4);
	C[3] &= 0b11110000;
	memcpy(D, CD + 3, 4);
	D[0] &= 0b00001111;
	// ����������� ����� ����� ��� C
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
	// ����������� ����� ����� ��� D
	if (shift_CD_table[iter] == 1) {
		int save_bits[4] = { 0, 0, 0, 0 };
		for (int i = 1; i < 4; i++)
			save_bits[i] = D[i] & 0b10000000;
		save_bits[0] = D[0] & 0b00001000;	// ��� ��� ���� ��������, ���������� ������ ���

		D[0] = D[0] << 1; D[0] |= (save_bits[1] >> 7);
		D[1] = D[1] << 1; D[1] |= (save_bits[2] >> 7);
		D[2] = D[2] << 1; D[2] |= (save_bits[3] >> 7);
		D[3] = D[3] << 1; D[3] |= (save_bits[0] >> 3);
	}
	else {
		int save_bits[4] = { 0, 0, 0, 0 };
		for (int i = 0; i < 4; i++)
			save_bits[i] = D[i] & 0b11000000;
		save_bits[0] = D[0] & 0b00001100;	// ��� ��� ���� ��������, ���������� ������ ���

		D[0] = D[0] << 2; D[0] |= (save_bits[1] >> 6);
		D[1] = D[1] << 2; D[1] |= (save_bits[2] >> 6);
		D[2] = D[2] << 2; D[2] |= (save_bits[3] >> 6);
		D[3] = D[3] << 2; D[3] |= (save_bits[0] >> 2);
	}
	
	// �������� ���������� ����
	char div_byte = 0;
	div_byte = (C[3] & 0b11110000) | (D[0] & 0b00001111);
	// ��������� ������
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
	auto expanded_R = permutate_E(R);	// ��������� R �� ������� E
	for (int i = 0; i < 6; i++) {
		expanded_R[i] = expanded_R[i] ^ key[i];	// ����������� ��� � ������
	}

	auto B = slice_blocks(expanded_R);	// ����������� � ����� �� 6 ���
	auto converted_B = convert_S(B);	// �������� ������ 32 ��� ����� �������������� ����� S

	converted_B = permutate_P_small(converted_B);	// ��������� �������� ������������

	delete expanded_R;
	delete B;

	return converted_B;
}

unsigned char* encrypt_block(unsigned char data[8], unsigned char key[8]) {
	auto permutate_data = permutate_P(data);	// ������ �������������� P
	auto CD = permutate_key(key);		// ������ CD

	// ��������� ���� �� L � R
	unsigned char L[4];
	unsigned char R[4];
	memcpy(L, permutate_data, 4);
	memcpy(R, permutate_data + 4, 4);
	unsigned char temp_R[4];	// ����� ��� ����������� �������� ����� L � R
	memcpy(temp_R, R, 4);

	for (int i = 0; i < 16; i++) {
		CD = shift_CD(CD, i);					// �������� CD
		auto key_iter = generate_key_iter(CD);	// ��������� ����� ��������

		auto func_res = Feistel_func(R, key_iter);	// ���������� ������� ��������
		for (int j = 0; j < 4; j++) {
			R[j] = L[j] ^ func_res[j];	// ����������� ��� � �������� ��������
		}
		memcpy(L, temp_R, 4);	// ����������� L R-1
		memcpy(temp_R, R, 4);	// ���������� ������� R

		delete key_iter;
		delete func_res;
	}

	unsigned char RL[8];		// ��������� R � L
	memcpy(RL, R, 4);
	memcpy(RL + 4, L, 4);

	// �������� ������������ C - ������������� ���������
	auto encrypted_data = permutate_P_reverse(RL);

	delete permutate_data;
	delete CD;

	return encrypted_data;
}

unsigned char* decrypt_block(unsigned char data[8], unsigned char key[8]) {
	auto permutate_data = permutate_P(data);	// ������ �������������� P
	auto CD = permutate_key(key);		// ������ CD

	// ��������� ���� �� L � R
	unsigned char L[4];
	unsigned char R[4];
	memcpy(L, permutate_data, 4);
	memcpy(R, permutate_data + 4, 4);
	unsigned char temp_R[4];	// ����� ��� ����������� �������� ����� L � R
	memcpy(temp_R, R, 4);

	// �������������� ���������� ����� �� 0 �� 16
	unsigned char* keys[16];
	for (int i = 0; i < 16; i++) {
		CD = shift_CD(CD, i);
		keys[i] = new unsigned char[6];
		memset(keys[i], 0, 6);
		keys[i] = generate_key_iter(CD);
	}

	for (int i = 15; i >= 0; i--) {
		// ���������� ����� ��������������� �����
		auto func_res = Feistel_func(R, keys[i]);	// ���������� ������� ��������
		for (int j = 0; j < 4; j++) {
			R[j] = L[j] ^ func_res[j];	// ����������� ��� � �������� ��������
		}
		memcpy(L, temp_R, 4);	// ����������� L R-1
		memcpy(temp_R, R, 4);	// ���������� ������� R

		delete func_res;
	}

	unsigned char RL[8];		// ��������� R � L
	memcpy(RL, R, 4);
	memcpy(RL + 4, L, 4);

	// �������� ������������ C - �������������� ���������
	auto decrypted_data = permutate_P_reverse(RL);

	for (int i = 0; i < 16; i++)
		delete keys[i];
	delete permutate_data;
	delete CD;

	return decrypted_data;
}

void encrypt_file(std::string file_name, std::string key_file_name) {
	unsigned char key[8];
	std::ifstream file(file_name, std::ios::binary);
	std::ifstream key_file(key_file_name);
	size_t added_bytes = 0;
	size_t file_length = 0;
	char* bytes;

	// �������� ���� � ����������� ������
	for (int i = 0; i < 8; i++) {
		key_file >> key[i];
	}

	// �������� ����� �����
	file.seekg(0, std::ios::end);
	file_length = file.tellg();
	file.seekg(0, std::ios::beg);

	// �������� ����� � ������
	if (file_length % 8 == 0) {		// ���� ������ 8, �� ��������� �� �����
		added_bytes = 0;
		bytes = new char[file_length];
	}
	else {	// (8 - (length % 8)) - ����� ��������� 64 ������� �����
		added_bytes = (8 - (file_length % 8));
		bytes = new char[file_length + added_bytes];
		memset(bytes + file_length, 0, added_bytes);	// ��������� ������ ������������ ������
	}
	// ������ ����
	file.read(bytes, file_length);

	std::ofstream encrypted_file("encrypted_" + file_name, std::ios::binary);

	// �������� ���������� �������
	for (int i = 0; i < (file_length + added_bytes) / 8; i++) {
		unsigned char block[8];
		int byte_offset = 0;
		// �������� ��������� ���� � ������������ ����� � ����� ��� �����
		for (int j = i * 8; j < (i + 1) * 8; j++) {
			block[byte_offset] = (unsigned char)bytes[j];
			byte_offset++;
		}
		// �������� ����
		auto encrypted_block = encrypt_block(block, key);

		// ���������� ��� � ����
		for (int j = 0; j < 8; j++) {
			encrypted_file << encrypted_block[j];
		}

		delete encrypted_block;
	}

	std::cout << "file encryped" << std::endl;
}

void decrypt_file(std::string file_name, std::string key_file_name) {
	unsigned char key[8];
	std::ifstream file(file_name, std::ios::binary);
	std::ifstream key_file(key_file_name);
	size_t added_bytes = 0;
	size_t file_length = 0;
	char* bytes;

	// �������� ���� � ����������� ������
	for (int i = 0; i < 8; i++) {
		key_file >> key[i];
	}

	// �������� ����� �����
	file.seekg(0, std::ios::end);
	file_length = file.tellg();
	file.seekg(0, std::ios::beg);

	// �������� ����� � ������
	if (file_length % 8 == 0) {		// ���� ������ 8, �� ��������� �� �����
		added_bytes = 0;
		bytes = new char[file_length];
	}
	else {	// (8 - (length % 8)) - ����� ��������� 64 ������� �����
		added_bytes = (8 - (file_length % 8));
		bytes = new char[file_length + added_bytes];
		memset(bytes + file_length, 0, added_bytes);	// ��������� ������ ������������ ������
	}
	// ������ ����
	file.read(bytes, file_length);

	std::ofstream dectyped_file("decrypted_" + file_name, std::ios::binary);

	// �������� ���������� �������
	for (int i = 0; i < (file_length + added_bytes) / 8; i++) {
		unsigned char block[8];
		int byte_offset = 0;
		// �������� ��������� ���� � ������������ ����� � ����� ��� �����
		for (int j = i * 8; j < (i + 1) * 8; j++) {
			block[byte_offset] = (unsigned char)bytes[j];
			byte_offset++;
		}
		// �������� ����
		auto encrypted_block = decrypt_block(block, key);
		// ���������� ��� � ����
		for (int j = 0; j < 8; j++)
			dectyped_file << encrypted_block[j];

		delete encrypted_block;
	}

	std::cout << "file decrypted" << std::endl;
}


int main() {
	while (true) {
		std::cout << "enter 'e' to encrypt file, 'd' to decrypt, 'q' to exit" << std::endl;
		std::string file_name;
		std::string key;
		char cmd = 0;
		std::cin >> cmd;
		if (cmd == 'e') {
			std::cout << "which file to encrypt?: ";
			std::cin >> file_name;
			std::cout << "enter name of file key: ";
			std::cin >> key;
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			encrypt_file(file_name, key);
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			std::cout << "Encryption time: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << " seconds" << std::endl;
		}
		else if (cmd == 'd') {
			std::cout << "which file to decrypt?: ";
			std::cin >> file_name;
			std::cout << "enter name of file key: ";
			std::cin >> key;
			decrypt_file(file_name, key);
		}
		else if (cmd == 'q') {
			std::cout << "exit" << std::endl;
			break;
		}
		else {
			std::cout << "wrong command" << std::endl;
		}
	}

	return 0;
}