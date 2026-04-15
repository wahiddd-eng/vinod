#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 20

char prod[MAX][MAX];
char items[MAX][MAX][MAX];
int n, itemCount = 0;

// Function to check if item already exists
int exists(char arr[MAX][MAX], int count, char *str) {
    for (int i = 0; i < count; i++) {
        if (strcmp(arr[i], str) == 0)
            return 1;
    }
    return 0;
}

// Closure function
void closure(char I[MAX][MAX], int *count) {
    int changed = 1;

    while (changed) {
        changed = 0;

        for (int i = 0; i < *count; i++) {
            char *dot = strchr(I[i], '.');

            if (dot && *(dot + 1) != '\0') {
                char B = *(dot + 1);

                // If B is non-terminal (capital letter)
                if (B >= 'A' && B <= 'Z') {
                    for (int j = 0; j < n; j++) {
                        if (prod[j][0] == B) {
                            char newItem[MAX];
                            sprintf(newItem, "%c->.%s", prod[j][0], prod[j] + 3);

                            if (!exists(I, *count, newItem)) {
                                strcpy(I[*count], newItem);
                                (*count)++;
                                changed = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

// GOTO function
void gotoFunc(char I[MAX][MAX], int count, char symbol, char result[MAX][MAX], int *resCount) {
    *resCount = 0;

    for (int i = 0; i < count; i++) {
        char temp[MAX];
        strcpy(temp, I[i]);

        char *dot = strchr(temp, '.');

        if (dot && *(dot + 1) == symbol) {
            // move dot
            char t = *dot;
            *dot = *(dot + 1);
            *(dot + 1) = t;

            strcpy(result[*resCount], temp);
            (*resCount)++;
        }
    }

    closure(result, resCount);
}

// Print items
void printItems(char I[MAX][MAX], int count, int index) {
    printf("\nI%d:\n", index);
    for (int i = 0; i < count; i++) {
        printf("%s\n", I[i]);
    }
}

int main() {
    printf("Enter number of productions: ");
    scanf("%d", &n);

    printf("Enter productions (e.g., A->aB):\n");
    for (int i = 0; i < n; i++) {
        scanf("%s", prod[i]);
    }

    // Augmented grammar
    char start = prod[0][0];
    char augmented[MAX];
    sprintf(augmented, "Z->.%c", start);

    char I[MAX][MAX];
    int count = 0;

    strcpy(I[count++], augmented);

    // Add initial closure
    closure(I, &count);

    strcpy(items[itemCount][0], "");
    for (int i = 0; i < count; i++)
        strcpy(items[itemCount][i], I[i]);

    int itemSizes[MAX];
    itemSizes[itemCount] = count;
    itemCount++;

    printItems(I, count, 0);

    // Symbols extraction
    char symbols[MAX];
    int symCount = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 3; prod[i][j] != '\0'; j++) {
            if (!strchr(symbols, prod[i][j])) {
                symbols[symCount++] = prod[i][j];
            }
        }
    }

    // Generate LR(0) items
    for (int i = 0; i < itemCount; i++) {
        for (int s = 0; s < symCount; s++) {
            char newI[MAX][MAX];
            int newCount = 0;

            gotoFunc(items[i], itemSizes[i], symbols[s], newI, &newCount);

            if (newCount == 0)
                continue;

            int found = 0;

            for (int k = 0; k < itemCount; k++) {
                if (itemSizes[k] == newCount) {
                    int match = 1;
                    for (int x = 0; x < newCount; x++) {
                        if (!exists(items[k], itemSizes[k], newI[x])) {
                            match = 0;
                            break;
                        }
                    }
                    if (match) {
                        found = 1;
                        break;
                    }
                }
            }

            if (!found) {
                for (int x = 0; x < newCount; x++) {
                    strcpy(items[itemCount][x], newI[x]);
                }
                itemSizes[itemCount] = newCount;

                printItems(newI, newCount, itemCount);
                itemCount++;
            }
        }
    }

    return 0;
}