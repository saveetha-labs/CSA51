#include <stdio.h>
#include <string.h>
#include <ctype.h>
int modInverse(int a, int m) {
	int x;
    a = a % m;
    for (x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1;
}
void matrixMultiply(int a[2][2], int b[2], int result[2]) {
    result[0] = (a[0][0] * b[0] + a[0][1] * b[1]) % 26;
    result[1] = (a[1][0] * b[0] + a[1][1] * b[1]) % 26;
}
void encrypt(char plaintext[], int key[2][2]) {
    int i;
	int length = strlen(plaintext);
    for (i = 0; i < length; i += 2) {
        int pair[2];
        pair[0] = toupper(plaintext[i]) - 'A';
        pair[1] = toupper(plaintext[i+1]) - 'A';
        
        int result[2];
        matrixMultiply(key, pair, result);
        
        plaintext[i] = result[0] + 'A';
        plaintext[i+1] = result[1] + 'A';
    }
}
void decrypt(char ciphertext[], int key[2][2]) {
    int det = (key[0][0] * key[1][1] - key[0][1] * key[1][0]) % 26;
    if (det < 0) det += 26;
    int i,j;
    int detInverse = modInverse(det, 26);
    
    int inverseKey[2][2];
    inverseKey[0][0] = (key[1][1] * detInverse) % 26;
    inverseKey[0][1] = (-key[0][1] * detInverse) % 26;
    inverseKey[1][0] = (-key[1][0] * detInverse) % 26;
    inverseKey[1][1] = (key[0][0] * detInverse) % 26;
    
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if (inverseKey[i][j] < 0) inverseKey[i][j] += 26;
        }
    }
    
    encrypt(ciphertext, inverseKey);  
}

int main() {
    char plaintext[] = "MEETMEATTHEUSUALPLACEATTENRATHERTHANEIGHTOCLOCK";
    int key[2][2] = {{9, 4}, {5, 7}};
    
    printf("Plaintext: %s\n", plaintext);
    
    encrypt(plaintext, key);
    printf("Encrypted: %s\n", plaintext);
    
    decrypt(plaintext, key);
    printf("Decrypted: %s\n", plaintext);
    
    return 0;
}
