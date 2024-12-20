#include <stdio.h>
#include <math.h>
unsigned long long factorial(int n) {
	int i;
    if (n == 0) return 1;
    unsigned long long result = 1;
    for (i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    unsigned long long total_permutations = factorial(25);
    unsigned long long row_col_permutations = factorial(5) * factorial(5);
    unsigned long long unique_keys = total_permutations / row_col_permutations;
    double total_permutations_log2 = log2((double)total_permutations);
    double unique_keys_log2 = log2((double)unique_keys);

    printf("Total number of possible keys: 2^%.2f\n", total_permutations_log2);
    printf("Number of effectively unique keys: 2^%.2f\n", unique_keys_log2);

    return 0;
}
