#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

double english_freq[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153,
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056,
    2.758, 0.978, 2.361, 0.150, 1.974, 0.074
};

void calculate_frequencies(const char *text, int *frequencies) {
    int length = strlen(text);
    for (int i = 0; i < length; i++) {
        if (isalpha(text[i])) {
            frequencies[tolower(text[i]) - 'a']++;
        }
    }
}

void generate_mapping(int *frequencies, char *mapping) {
    int sorted_indices[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sorted_indices[i] = i;
    }

    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (frequencies[sorted_indices[i]] < frequencies[sorted_indices[j]]) {
                int temp = sorted_indices[i];
                sorted_indices[i] = sorted_indices[j];
                sorted_indices[j] = temp;
            }
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        mapping[sorted_indices[i]] = 'a' + i;
    }
}

void decrypt(const char *ciphertext, char *plaintext, const char *mapping) {
    int length = strlen(ciphertext);
    for (int i = 0; i < length; i++) {
        if (isalpha(ciphertext[i])) {
            char offset = islower(ciphertext[i]) ? 'a' : 'A';
            plaintext[i] = mapping[tolower(ciphertext[i]) - 'a'] - 'a' + offset;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[length] = '\0';
}

double compute_score(const char *plaintext) {
    int length = strlen(plaintext);
    int count[ALPHABET_SIZE] = {0};
    int total = 0;

    for (int i = 0; i < length; i++) {
        if (isalpha(plaintext[i])) {
            count[tolower(plaintext[i]) - 'a']++;
            total++;
        }
    }

    double score = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        double frequency = (double)count[i] / total * 100;
        score += (frequency - english_freq[i]) * (frequency - english_freq[i]);
    }

    return score;
}
 
void frequency_attack(const char *ciphertext, int num_results) {
    int length = strlen(ciphertext);
    char plaintext[length + 1];
    double scores[ALPHABET_SIZE];
    char *plaintexts[ALPHABET_SIZE];
    int frequencies[ALPHABET_SIZE] = {0};
    char mapping[ALPHABET_SIZE];

    calculate_frequencies(ciphertext, frequencies);
    generate_mapping(frequencies, mapping);

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        plaintexts[i] = (char*)malloc(length + 1);
        if (plaintexts[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        decrypt(ciphertext, plaintexts[i], mapping);
        scores[i] = compute_score(plaintexts[i]);

        char temp = mapping[0];
        for (int j = 0; j < ALPHABET_SIZE - 1; j++) {
            mapping[j] = mapping[j + 1];
        }
        mapping[ALPHABET_SIZE - 1] = temp;
    }

    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (scores[i] > scores[j]) {
                double temp_score = scores[i];
                scores[i] = scores[j];
                scores[j] = temp_score;

                char *temp_text = plaintexts[i];
                plaintexts[i] = plaintexts[j];
                plaintexts[j] = temp_text;
            }
        }
    }

    printf("Top %d possible plaintexts:\n", num_results);
    for (int i = 0; i < num_results && i < ALPHABET_SIZE; i++) {
        printf("%d: %s\n", i + 1, plaintexts[i]);
        free(plaintexts[i]);
    }
}

int main() {
    char ciphertext[1024];
    int num_results;

    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';  

    printf("Enter the number of top possible plaintexts to display: ");
    scanf("%d", &num_results);

    frequency_attack(ciphertext, num_results);

    return 0;
}
