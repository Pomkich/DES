#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

char hex(std::string bits) {
	std::map<std::string, char> hex_bits = {
		{"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'},
		{"0100", '4'}, {"0101", '5'}, {"0110", '6'}, {"0111", '7'},
		{"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'},
		{"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'},
	};
	return hex_bits[bits];
}

std::string to_bits(unsigned char ch) {
	std::string bits = "";
	for (int i = 0; i < 8; i++) {
		char bit = ((bool)(ch << i & 0b10000000) + '0');
		bits.push_back(bit);
	}
	return bits;
}

int main() {
	char buffer[32];
	std::ifstream infile("test3.txt");

	//get length of file
	infile.seekg(0, std::ios::end);
	size_t length = infile.tellg();
	infile.seekg(0, std::ios::beg);

	// don't overflow the buffer!
	if (length > sizeof(buffer))
	{
		length = sizeof(buffer);
	}

	//read file
	infile.read(buffer, length);

	std::cout << length << std::endl;

	for (int i = 0; i < 32; i++) {
		if (i == 16) std::cout << std::endl;
		auto bits = to_bits(buffer[i]);
		std::cout << hex(bits.substr(0, 4)) << hex(bits.substr(4, 4)) << " ";
	}

	return 0;
}