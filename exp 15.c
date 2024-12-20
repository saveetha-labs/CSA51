#include <stdio.h>
#include <string.h>
#include <stdlib.h>

double englishFreq[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 
    0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 
    6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};

void calculateFrequency(char *ciphertext, double freq[26]) {
    int length = strlen(ciphertext);
    int count[26] = {0};
    int i;
    for (i = 0; i < length; i++) {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            count[ciphertext[i] - 'A']++;
        }
    }

    for (i = 0; i < 26; i++) {
        freq[i] = ((double)count[i] / length) * 100.0;
    }
}

void decrypt(char *ciphertext, char *plaintext, int key) {
    int length = strlen(ciphertext),i;
    for (i = 0; i < length; i++) {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            plaintext[i] = ((ciphertext[i] - 'A' - key + 26) % 26) + 'A';
        } else {
            plaintext[i] = ciphertext[i];  
        }
    }
    plaintext[length] = '\0';
}

double chiSquared(char *plaintext) {
    double observed[26] = {0};
    calculateFrequency(plaintext, observed);
    int i;
    double chiSquaredStat = 0.0;
    for (i = 0; i < 26; i++) {
        double expected = englishFreq[i];
        chiSquaredStat += ((observed[i] - expected) * (observed[i] - expected)) / expected;
    }

    return chiSquaredStat;
}

void letterFrequencyAttack(char *ciphertext, int topN) {
    int length = strlen(ciphertext);
    char possiblePlaintexts[26][length + 1];
    double chiSquaredValues[26];
    int key,i,j;
    for (key = 0; key < 26; key++) {
        decrypt(ciphertext, possiblePlaintexts[key], key);
        chiSquaredValues[key] = chiSquared(possiblePlaintexts[key]);
    }
 
    for (i = 0; i < 25; i++) {
        for (j = i + 1; j < 26; j++) {
            if (chiSquaredValues[i] > chiSquaredValues[j]) {
                double tempValue = chiSquaredValues[i];
                chiSquaredValues[i] = chiSquaredValues[j];
                chiSquaredValues[j] = tempValue;

                char tempText[length + 1];
                strcpy(tempText, possiblePlaintexts[i]);
                strcpy(possiblePlaintexts[i], possiblePlaintexts[j]);
                strcpy(possiblePlaintexts[j], tempText);
            }
        }
    }

    printf("Top %d possible plaintexts:\n", topN);
    for (i = 0; i < topN; i++) {
        printf("%d. %s (Chi-squared: %.2f)\n", i + 1, possiblePlaintexts[i], chiSquaredValues[i]);
    }
}

int main() {
    char ciphertext[1024];
    int topN;

    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0'; 

    printf("Enter the number of top possible plaintexts to display: ");
    scanf("%d", &topN);

    letterFrequencyAttack(ciphertext, topN);

    return 0;
}
