#include <stdio.h>
#include <stdlib.h>
int modInverse(int a, int m) {
    a = a % m;
    int x;
    for (x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}
int determinantModulo26(int matrix[2][2]) {
    int det = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) % 26;
    return (det + 26) % 26; 
}
void inverseMatrixModulo26(int matrix[2][2], int inverse[2][2]) {
    int det = determinantModulo26(matrix);
    int det_inv = modInverse(det, 26);
    if (det_inv == -1) {
        printf("Matrix is not invertible\n");
        exit(1);
    }
    inverse[0][0] = matrix[1][1] * det_inv % 26;
    inverse[0][1] = (-matrix[0][1] + 26) * det_inv % 26;
    inverse[1][0] = (-matrix[1][0] + 26) * det_inv % 26;
    inverse[1][1] = matrix[0][0] * det_inv % 26;
}
void multiplyMatricesModulo26(int a[2][2], int b[2][2], int result[2][2]) {
	int i,j,k;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            result[i][j] = 0;
            for (k = 0; k < 2; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
            result[i][j] %= 26;
        }
    }
}
void multiplyMatrixVectorModulo26(int matrix[2][2], int vector[2], int result[2]) {
	int i,j;
    for (i = 0; i < 2; i++) {
        result[i] = 0;
        for (j = 0; j < 2; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
        result[i] %= 26;
    }
}

int main() {
    int plaintext[2][2] = {{0, 1}, {2, 3}};
    int ciphertext[2][2] = {{15, 4}, {11, 22}};
    int plaintext_inv[2][2];
    inverseMatrixModulo26(plaintext, plaintext_inv);
    int key[2][2];
    multiplyMatricesModulo26(ciphertext, plaintext_inv, key);
    printf("Recovered key matrix:\n");
    printf("[[%d, %d],\n [%d, %d]]\n", key[0][0], key[0][1], key[1][0], key[1][1]);
    char example_plaintext[] = "hi"; 
    int example_plaintext_vector[2] = {7, 8};
    int example_ciphertext_vector[2];
    multiplyMatrixVectorModulo26(key, example_plaintext_vector, example_ciphertext_vector);
    printf("Encrypted result for 'hi': %c%c\n", 
           example_ciphertext_vector[0] + 'a', 
           example_ciphertext_vector[1] + 'a');

    return 0;
}
