#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <bitset>
#include "permutations.h"
#include "constants.h"

using namespace std;

// ������� ��������� ��������� �������� E(r - 1) � ������ �� 8 6 ������� ������
unsigned char* slice_blocks(unsigned char ER_xor_key[6]) {
	unsigned char* B = new unsigned char[8];	// ������ ���� ����������� 6 ������,
	memset(B, 0, 8);							// �� ��� ��� �������� � ���� ����
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

int count_set_bits(unsigned char byte) {
	int set = 0;
	for (int i = 0; i < 8; i++) {
		set += byte % 2;
		byte /= 2;
	}
	return set;
}

// ������� ��������� ���� �������� � ��������� �����
unsigned char* add_key_bits(unsigned char key[7]) {
	unsigned char* key_with_bits = new unsigned char[8];
	memcpy(key_with_bits, key, 7);
	memset(key_with_bits + 7, 0, 1);
	for (int i = 0; i < 8; i++) {
		char byte = key_with_bits[i] & 0b11111110;
		int save_bit = key_with_bits[i] & 0b00000001;
		if (count_set_bits(byte) % 2 != 0) {	// ���� ����� ������ ������� ->
			byte |= 0b00000001;					// ��������� �� ��������
		}
		key_with_bits[i] = byte;	// ��������� ����
		// �������� ��������� ������
		for (int j = i + 1; j < 8; j++) {
			int temp_bit = key_with_bits[j] & 0b00000001;
			key_with_bits[j] = key_with_bits[j] >> 1;
			key_with_bits[j] |= (save_bit << 7);
			save_bit = temp_bit;
		}
	}
	return key_with_bits;
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

int main() {
	unsigned char data[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
	unsigned char key[7] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g' };

	auto permutate_data = permutate_P(data);	// ������ �������������� P
	auto key_with_bits = add_key_bits(key);		// ���� � ������ ��������
	auto CD = permutate_key(key_with_bits);		// ������ CD

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

	// �������� ������������ permutated_C - ������������� ���������
	auto C = permutate_P_reverse(RL);

	for (int i = 0; i < 8; i++) {
		std::cout << C[i] << " ";
	}

	delete permutate_data;
	delete key_with_bits;
	delete CD;
	delete C;


	return 0;
}

/*
// ���������� ����� �������� � �������� ����
void add_key_bits(std::string& key) {
	for (int i = 0; i < 8; i++) {
		auto substr = key.substr(i * 8, 7);
		if (std::count(substr.begin(), substr.end(), '1') % 2 != 1)
			key.insert(key.begin() + (i * 8) + 7, '1');
		else
			key.insert(key.begin() + (i * 8) + 7, '0');
	}
}

// ������ ������������ ��������� ����� ������
std::string permutate_P(std::string bits) {
	std::string perm_bits(block_size, '0');
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 16; j++) {
			perm_bits[i * 16 + j] = bits[P[i][j]];
		}
	}
	return perm_bits;
}

std::string permutate_key(std::string key) {
	std::string perm_key(start_key_size, '0');
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 14; j++) {
			perm_key[i * 14 + j] = key[key_perm_table[i][j]];
		}
	}
	return perm_key;
}


*/

/*	// read file
std::ifstream infile("test.txt");

	//get length of file
	infile.seekg(0, std::ios::end);
	size_t length = infile.tellg();
	infile.seekg(0, std::ios::beg);

	char* buffer = new char[length];

	//read file
	infile.read(buffer, length);

	std::cout << length << std::endl;

	for (int i = 0; i < length; i++) {
		if (i == 16) std::cout << std::endl;
		auto bits = byte_bin(buffer[i]);
		std::cout << bin_hex(bits.substr(0, 4)) << bin_hex(bits.substr(4, 4)) << " ";
	}

*/

/*char bin_hex(std::string bits) {
	static std::map<std::string, char> hex_bits = {	// for better perfomance static
		{"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'},
		{"0100", '4'}, {"0101", '5'}, {"0110", '6'}, {"0111", '7'},
		{"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'},
		{"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'},
	};
	return hex_bits[bits];
}

std::string byte_bin(unsigned char ch) {
	std::string bits = "";
	for (int i = 0; i < 8; i++) {
		char bit = ((bool)(ch << i & 0b10000000) + '0');
		bits.push_back(bit);
	}
	return bits;
}*/