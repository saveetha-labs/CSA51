#include <stdio.h>
#include <stdint.h>
#include <string.h>
int initial_permutation[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
                                60, 52, 44, 36, 28, 20, 12, 4,
                                62, 54, 46, 38, 30, 22, 14, 6,
                                64, 56, 48, 40, 32, 24, 16, 8,
                                57, 49, 41, 33, 25, 17, 9, 1,
                                59, 51, 43, 35, 27, 19, 11, 3,
                                61, 53, 45, 37, 29, 21, 13, 5,
                                63, 55, 47, 39, 31, 23, 15, 7 };

int final_permutation[64] = { 40, 8, 48, 16, 56, 24, 64, 32,
                              39, 7, 47, 15, 55, 23, 63, 31,
                              38, 6, 46, 14, 54, 22, 62, 30,
                              37, 5, 45, 13, 53, 21, 61, 29,
                              36, 4, 44, 12, 52, 20, 60, 28,
                              35, 3, 43, 11, 51, 19, 59, 27,
                              34, 2, 42, 10, 50, 18, 58, 26,
                              33, 1, 41, 9, 49, 17, 57, 25 };
int sbox[8][4][16] = { /* S-boxes should be defined here */ };

int pbox[32] = { 16, 7, 20, 21,
                 29, 12, 28, 17,
                 1, 15, 23, 26,
                 5, 18, 31, 10,
                 2, 8, 24, 14,
                 32, 27, 3, 9,
                 19, 13, 30, 6,
                 22, 11, 4, 25 };
uint64_t permute(uint64_t block, int* permutation, int size) {
    uint64_t permuted = 0;
    for (int i = 0; i < size; i++) {
        int bit = (block >> (64 - permutation[i])) & 1;
        permuted |= (bit << (63 - i));
    }
    return permuted;
}

void key_schedule(uint64_t key, uint64_t subkeys[16]) {

}
uint32_t expansion_permutation(uint32_t half_block) {

    return half_block; 
}
uint32_t s_boxes(uint32_t xor_result) {
    return xor_result; 
}
uint32_t p_box(uint32_t substituted) {
    return substituted; 
} 
uint32_t feistel(uint32_t right, uint64_t subkey) {
    uint32_t expanded = expansion_permutation(right);
    uint32_t xor_result = expanded ^ (subkey & 0xFFFFFFFF);  
    uint32_t substituted = s_boxes(xor_result);
    return p_box(substituted);
}
void des_decrypt(uint64_t cipher_text, uint64_t *plain_text, uint64_t *subkeys) {
    uint64_t ip = permute(cipher_text, initial_permutation, 64); 
    uint32_t left = ip >> 32;
    uint32_t right = ip & 0xFFFFFFFF;

    for (int round = 15; round >= 0; round--) {
        uint32_t new_right = left ^ feistel(right, subkeys[round]);
        left = right;
        right = new_right;
    }

    uint64_t combined = ((uint64_t)right << 32) | left;
    *plain_text = permute(combined, final_permutation, 64); 
}

int main() {
    uint64_t cipher_text = 0x1234567890ABCDEF;
    uint64_t plain_text;
    
    uint64_t subkeys[16] = {0x0101010101010101, 0x0202020202020202, /* other subkeys */};

    des_decrypt(cipher_text, &plain_text, subkeys);
    
    printf("Decrypted plaintext: %016llX\n", plain_text);
    
    return 0;
}
