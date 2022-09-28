#include <iostream>
#include "constants.h"

unsigned char* permutate_P(unsigned char block[8]) {
	unsigned char* perm_block = new unsigned char[8];
	memset(perm_block, 0, 8);
	for (int i = 0; i < 64; i++) {	//P[i / 16][i % 16] -1 -> � ������� ���������� �� 1, ������� -1
		int offset_byte = (P[i / 16][i % 16] - 1) / 8;	// �������� �� �����
		int offset_bit = (P[i / 16][i % 16] - 1) % 8;	// �������� �� ����
		int bit = ((block[offset_byte] << offset_bit) & 0b10000000);	// ��������� ���
		perm_block[i / 8] |= ((0b10000000 & bit) >> (i % 8));	// ������������� ��� � ������� �������
	}
	return perm_block;
}

unsigned char* permutate_key(unsigned char key_with_bits[8]) {
	unsigned char* perm_key = new unsigned char[7];
	memset(perm_key, 0, 7);
	for (int i = 0; i < 56; i++) {
		int offset_byte = (key_perm_table[i / 14][i % 14] - 1) / 8;	// �������� �� �����
		int offset_bit = (key_perm_table[i / 14][i % 14] - 1) % 8;	// �������� �� ����
		int bit = ((key_with_bits[offset_byte] << offset_bit) & 0b10000000);	// ��������� ���
		perm_key[i / 8] |= ((0b10000000 & bit) >> (i % 8));	// ������������� ��� � ������� �������
	}
	return perm_key;
}

unsigned char* generate_key_iter(unsigned char CD[7]) {
	unsigned char* key_iter = new unsigned char[6];
	memset(key_iter, 0, 6);
	for (int i = 0; i < 48; i++) {
		int offset_byte = (key_generate_table[i / 16][i % 16] - 1) / 8;	// �������� �� �����
		int offset_bit = (key_generate_table[i / 16][i % 16] - 1) % 8;	// �������� �� ����
		int bit = ((CD[offset_byte] << offset_bit) & 0b10000000);	// ��������� ���
		key_iter[i / 8] |= ((0b10000000 & bit) >> (i % 8));	// ������������� ��� � ������� �������
	}
	return key_iter;
}

unsigned char* permutate_E(unsigned char R[4]) {
	unsigned char* ER = new unsigned char[6];
	memset(ER, 0, 6);
	for (int i = 0; i < 48; i++) {
		int offset_byte = (E[i / 6][i % 6] - 1) / 8;	// �������� �� �����
		int offset_bit = (E[i / 6][i % 6] - 1) % 8;	// �������� �� ����
		int bit = ((R[offset_byte] << offset_bit) & 0b10000000);	// ��������� ���
		ER[i / 8] |= ((0b10000000 & bit) >> (i % 8));	// ������������� ��� � ������� �������
	}
	return ER;
}

unsigned char* permutate_P_small(unsigned char B[4]) {
	unsigned char* BP = new unsigned char[4];
	memset(BP, 0, 4);
	for (int i = 0; i < 32; i++) {
		int offset_byte = (P_small[i / 8][i % 8] - 1) / 8;	// �������� �� �����
		int offset_bit = (P_small[i / 8][i % 8] - 1) % 8;	// �������� �� ����
		int bit = ((B[offset_byte] << offset_bit) & 0b10000000);	// ��������� ���
		BP[i / 8] |= ((0b10000000 & bit) >> (i % 8));	// ������������� ��� � ������� �������
	}
	return BP;
}

unsigned char* permutate_P_reverse(unsigned char RL[8]) {
	unsigned char* C = new unsigned char[8];
	memset(C, 0, 8);
	for (int i = 0; i < 64; i++) {
		int offset_byte = (P_small[i / 16][i % 16] - 1) / 8;	// �������� �� �����
		int offset_bit = (P_small[i / 16][i % 16] - 1) % 8;	// �������� �� ����
		int bit = ((RL[offset_byte] << offset_bit) & 0b10000000);	// ��������� ���
		C[i / 8] |= ((0b10000000 & bit) >> (i % 8));	// ������������� ��� � ������� �������
	}
	return C;
}