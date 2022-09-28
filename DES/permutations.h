#pragma once
unsigned char* permutate_P(unsigned char block[8]);

unsigned char* permutate_key(unsigned char key_with_bits[8]);

unsigned char* generate_key_iter(unsigned char CD[7]);

unsigned char* permutate_E(unsigned char R[4]);

unsigned char* permutate_P_small(unsigned char B[4]);

unsigned char* permutate_P_reverse(unsigned char RL[8]);