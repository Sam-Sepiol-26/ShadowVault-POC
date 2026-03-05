#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>


uint8_t msg[] = { 0x49, 0x50, 0x3A, 0x31, 0x37, 0x32, 0x2E, 0x31, 0x36, 0x38, 0x2E, 0x32, 0x37, 0x38, 0x2E, 0x39, 0x30, 0x00};

uint8_t shadow_vault[sizeof(msg)];

uint32_t master_key = 0x11223344;


void XOR_data(uint32_t* c) {
	for (uint32_t i = 0; i < sizeof(shadow_vault); i++) {
		shadow_vault[i] = msg[i] ^ (*c + i);
	}
}

void decrypt_data(uint32_t* d) {
	for (uint32_t i = 0; i < sizeof(shadow_vault); i++) {
		shadow_vault[i] ^= *d + i;
	}
}

void modify_value(uint32_t *a) {
	*a = *a << 1;
}

void input_func(uint32_t *b, uint32_t inp) {
	printf("Enter your code > ");
	scanf_s("%x", &inp);
	if (inp == master_key) {
		decrypt_data(b);
		printf("\nYour secret message: %s", shadow_vault);
	}

	else {
		printf("\nGarbage: %s", shadow_vault);
	}
}


int main() {
	uint32_t input = 0x00000000;
	uint32_t master_key = 0xDEADBEEF;

	uint32_t* ptr = &master_key;
	modify_value(ptr);
	XOR_data(ptr);
	input_func(ptr, input);

	return 0;
}
