#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 512

// Struktur untuk menyimpan paper
typedef struct Paper {
    char title[256];
    int year;
    int inCitations;
    char field[100];
} Paper;

// SLL Node (sort by year)
typedef struct SLLNode {
    Paper data;
    struct SLLNode* next;
} SLLNode;

// DLL Node (sort by inCitations)
typedef struct DLLNode {
    Paper data;
    struct DLLNode* prev;
    struct DLLNode* next;
} DLLNode;

// --------------------------
// Fungsi parsing CSV sederhana
int read_csv(Paper papers[], int max) {
    FILE* fp = fopen("data.csv", "r");
    if (!fp) {
        printf("Gagal membuka file.\n");
        return 0;
    }

    char line[MAX_LINE];
    int count = 0;

    fgets(line, MAX_LINE, fp); // skip header

    while (fgets(line, MAX_LINE, fp) && count < max) {
        sscanf(line, "%[^,],%d,%d,%[^\n]", 
               papers[count].title,
               &papers[count].year,
               &papers[count].inCitations,
               papers[count].field);
        count++;
    }

    fclose(fp);
    return count;
}

// --------------------------
// Buat SLL (diurutkan berdasarkan tahun)
SLLNode* build_sll(Paper papers[], int n, const char* field) {
    // filter
    Paper filtered[100];
    int fcount = 0;
    for (int i = 0; i < n; i++) {
        if (strstr(papers[i].field, field)) {
            filtered[fcount++] = papers[i];
        }
    }

    // sort by year (bubble sort)
    for (int i = 0; i < fcount-1; i++) {
        for (int j = 0; j < fcount-i-1; j++) {
            if (filtered[j].year > filtered[j+1].year) {
                Paper tmp = filtered[j];
                filtered[j] = filtered[j+1];
                filtered[j+1] = tmp;
            }
        }
    }

    // build SLL
    SLLNode* head = NULL;
    for (int i = fcount - 1; i >= 0; i--) {
        SLLNode* newNode = (SLLNode*)malloc(sizeof(SLLNode));
        newNode->data = filtered[i];
        newNode->next = head;
        head = newNode;
    }

    return head;
}

// --------------------------
// Buat DLL dari SLL (urut berdasarkan inCitations)
DLLNode* build_dll(SLLNode* head) {
    Paper arr[100];
    int count = 0;

    while (head) {
        arr[count++] = head->data;
        head = head->next;
    }

    // sort by inCitations descending
    for (int i = 0; i < count-1; i++) {
        for (int j = 0; j < count-i-1; j++) {
            if (arr[j].inCitations < arr[j+1].inCitations) {
                Paper tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }

    // build DLL
    DLLNode* dllHead = NULL;
    DLLNode* prev = NULL;

    for (int i = 0; i < count; i++) {
        DLLNode* newNode = (DLLNode*)malloc(sizeof(DLLNode));
        newNode->data = arr[i];
        newNode->prev = prev;
        newNode->next = NULL;

        if (!dllHead) dllHead = newNode;
        if (prev) prev->next = newNode;
        prev = newNode;
    }

    return dllHead;
}

// --------------------------
// Cetak hasil akhir
void print_dll(DLLNode* head) {
    printf("\nHasil:\n");
    while (head) {
        printf("- %s (Year: %d, Citations: %d)\n",
            head->data.title, head->data.year, head->data.inCitations);
        head = head->next;
    }
}

// --------------------------
// Main program
int main() {
    Paper papers[100];
    int total = read_csv(papers, 100);

    if (total == 0) return 1;

    char input[100];
    printf("Masukkan bidang studi: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  // hapus newline

    SLLNode* sll = build_sll(papers, total, input);

    if (!sll) {
        printf("Tidak ada data dengan bidang tersebut.\n");
        return 1;
    }

    DLLNode* dll = build_dll(sll);
    print_dll(dll);

    return 0;
}
