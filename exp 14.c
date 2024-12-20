#include <stdio.h>
#include <string.h>
void encrypt(const char *plaintext, const int *key, char *ciphertext) {
    int length = strlen(plaintext),i;
    for (i = 0; i < length; i++) {
        int p = plaintext[i] - 'a';
        int k = key[i];
        ciphertext[i] = ((p + k) % 26) + 'A'; 
    }
    ciphertext[length] = '\0';
}
void decrypt(const char *ciphertext, const int *key, char *plaintext) {
    int length = strlen(ciphertext),i;
    for (i = 0; i < length; i++) {
        int c = ciphertext[i] - 'A';
        int k = key[i];
        plaintext[i] = ((c - k + 26) % 26) + 'a'; 
    }
    plaintext[length] = '\0';
}

int main() {
    char plaintext[] = "sendmoremoney";
    int key[] = {9, 0, 1, 7, 23, 15, 21, 14, 11, 11, 2, 8, 9};
    int length = strlen(plaintext);
    char ciphertext[length + 1];
    encrypt(plaintext, key, ciphertext);
    printf("Ciphertext: %s\n", ciphertext);
    char desired_plaintext[] = "cashnotneeded";
    char ciphertext_to_decrypt[] = "BEOKJDMSXZPMH";
    int key_for_decryption[length],i;
    for (i = 0; i < length; i++) {
        int c = ciphertext_to_decrypt[i] - 'A';
        int p = desired_plaintext[i] - 'a';
        key_for_decryption[i] = (c - p + 26) % 26;
    }
    printf("Decryption key: ");
    for (i = 0; i < length; i++) {
        printf("%c", key_for_decryption[i] + 'A');
    }
    printf("\n");
    char decrypted_plaintext[length + 1];
    decrypt(ciphertext_to_decrypt, key_for_decryption, decrypted_plaintext);
    printf("Decrypted plaintext: %s\n", decrypted_plaintext);

    return 0;
}
