#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur untuk menyimpan data paper
typedef struct Paper {
    char title[256];
    char field_of_study[100];
    int year;
    int citations;
    struct Paper* next; // untuk linked list
} Paper;

// Struktur untuk Binary Search Tree berdasarkan field of study
typedef struct TreeNode {
    char field_of_study[100];
    Paper* papers; // linked list of papers untuk field ini
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Global variables
TreeNode* root = NULL;

// Function prototypes
TreeNode* createTreeNode(char* field);
TreeNode* insertField(TreeNode* node, char* field);
TreeNode* searchField(TreeNode* node, char* field);
Paper* createPaper(char* title, char* field, int year, int citations);
void insertPaper(Paper** head, Paper* newPaper);
void sortPapersByYearAndCitations(Paper** head);
void loadDataFromCSV(char* filename);
void displayPapersByField(char* field);
void printMenu();
Paper* mergePapers(Paper* left, Paper* right);
Paper* mergeSort(Paper* head);
Paper* getMiddle(Paper* head);

// Membuat node baru untuk tree
TreeNode* createTreeNode(char* field) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(newNode->field_of_study, field);
    newNode->papers = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Insert field ke dalam BST
TreeNode* insertField(TreeNode* node, char* field) {
    if (node == NULL) {
        return createTreeNode(field);
    }
    
    int cmp = strcmp(field, node->field_of_study);
    if (cmp < 0) {
        node->left = insertField(node->left, field);
    } else if (cmp > 0) {
        node->right = insertField(node->right, field);
    }
    // Jika field sudah ada, tidak perlu insert lagi
    return node;
}

// Mencari field dalam BST
TreeNode* searchField(TreeNode* node, char* field) {
    if (node == NULL || strcmp(field, node->field_of_study) == 0) {
        return node;
    }
    
    if (strcmp(field, node->field_of_study) < 0) {
        return searchField(node->left, field);
    }
    return searchField(node->right, field);
}

// Membuat paper baru
Paper* createPaper(char* title, char* field, int year, int citations) {
    Paper* newPaper = (Paper*)malloc(sizeof(Paper));
    strcpy(newPaper->title, title);
    strcpy(newPaper->field_of_study, field);
    newPaper->year = year;
    newPaper->citations = citations;
    newPaper->next = NULL;
    return newPaper;
}

// Insert paper ke dalam linked list (akan di-sort nanti)
void insertPaper(Paper** head, Paper* newPaper) {
    newPaper->next = *head;
    *head = newPaper;
}

// Fungsi untuk mencari middle node (untuk merge sort)
Paper* getMiddle(Paper* head) {
    if (head == NULL) return head;
    
    Paper* slow = head;
    Paper* fast = head;
    Paper* prev = NULL;
    
    while (fast != NULL && fast->next != NULL) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    
    if (prev != NULL) {
        prev->next = NULL;
    }
    
    return slow;
}

// Merge dua linked list yang sudah terurut
Paper* mergePapers(Paper* left, Paper* right) {
    if (left == NULL) return right;
    if (right == NULL) return left;
    
    Paper* result = NULL;
    
    // Sort berdasarkan tahun descending, jika sama sort berdasarkan citations descending
    if (left->year > right->year || 
        (left->year == right->year && left->citations > right->citations)) {
        result = left;
        result->next = mergePapers(left->next, right);
    } else {
        result = right;
        result->next = mergePapers(left, right->next);
    }
    
    return result;
}

// Merge sort untuk linked list
Paper* mergeSort(Paper* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    
    Paper* middle = getMiddle(head);
    Paper* left = head;
    Paper* right = middle;
    
    left = mergeSort(left);
    right = mergeSort(right);
    
    return mergePapers(left, right);
}

// Load data dari file CSV
void loadDataFromCSV(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Tidak dapat membuka file %s\n", filename);
        return;
    }
    
    char line[1024];
    char title[256], field[100];
    int year, citations;
    
    // Skip header
    fgets(line, sizeof(line), file);
    
    printf("Loading data from CSV...\n");
    int count = 0;
    
    while (fgets(line, sizeof(line), file)) {
        // Parse CSV line
        char* token = strtok(line, ",");
        if (token) strcpy(title, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(field, token);
        
        token = strtok(NULL, ",");
        if (token) year = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) citations = atoi(token);
        
        // Insert field ke BST jika belum ada
        root = insertField(root, field);
        
        // Cari node field dan tambahkan paper
        TreeNode* fieldNode = searchField(root, field);
        if (fieldNode) {
            Paper* newPaper = createPaper(title, field, year, citations);
            insertPaper(&(fieldNode->papers), newPaper);
            count++;
        }
    }
    
    printf("Data loaded successfully! Total papers: %d\n", count);
    fclose(file);
}

// Menampilkan paper berdasarkan field, terurut berdasarkan tahun dan citations
void displayPapersByField(char* field) {
    TreeNode* fieldNode = searchField(root, field);
    
    if (fieldNode == NULL) {
        printf("Field of study '%s' tidak ditemukan!\n", field);
        return;
    }
    
    if (fieldNode->papers == NULL) {
        printf("Tidak ada paper untuk field '%s'\n", field);
        return;
    }
    
    // Sort papers berdasarkan tahun dan citations
    fieldNode->papers = mergeSort(fieldNode->papers);
    
    printf("\n=== Papers dalam Field of Study: %s ===\n", field);
    printf("%-60s | %-6s | %-10s\n", "Title", "Year", "Citations");
    printf("---------------------------------------------------------------------\n");
    
    Paper* current = fieldNode->papers;
    int count = 1;
    while (current != NULL && count <= 20) { // Tampilkan maksimal 20 paper
        printf("%-60s | %-6d | %-10d\n", 
               current->title, current->year, current->citations);
        current = current->next;
        count++;
    }
    
    if (current != NULL) {
        printf("... dan masih ada %d+ paper lainnya\n", count);
    }
}

// Menu utama
void printMenu() {
    printf("\n=== PAPER MANAGEMENT SYSTEM ===\n");
    printf("1. Load Data dari CSV\n");
    printf("2. Cari Paper berdasarkan Field of Study\n");
    printf("3. Exit\n");
    printf("Pilih opsi (1-3): ");
}

int main() {
    int choice;
    char field[100];
    char filename[100];
    
    printf("=== PAPER MANAGEMENT SYSTEM ===\n");
    printf("Program ini menggunakan Binary Search Tree untuk field of study\n");
    printf("dan Linked List untuk menyimpan papers dalam setiap field.\n\n");
    
    while (1) {
        printMenu();
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Masukkan nama file CSV: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0; // remove newline
                loadDataFromCSV(filename);
                break;
                
            case 2:
                printf("Masukkan Field of Study: ");
                fgets(field, sizeof(field), stdin);
                field[strcspn(field, "\n")] = 0; // remove newline
                displayPapersByField(field);
                break;
                
            case 3:
                printf("Terima kasih! Program selesai.\n");
                exit(0);
                
            default:
                printf("Pilihan tidak valid!\n");
        }
    }
    
    return 0;
}