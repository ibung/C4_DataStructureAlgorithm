// ==================== ANGGOTA 5: MENU STACK & MAIN PROGRAM ====================
// File: menu_stack.h
#ifndef MENU_STACK_H
#define MENU_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MenuStack {
    int menu_id;
    char menu_name[100];
    struct MenuStack* next;
} MenuStack;

void push_menu(MenuStack** top, int id, const char* name);
MenuStack* pop_menu(MenuStack** top);
void display_menu_history(MenuStack* top);
void free_menu_stack(MenuStack* top);

#endif

// File: menu_stack.c
#include "menu_stack.h"

void push_menu(MenuStack** top, int id, const char* name) {
    MenuStack* new_menu = (MenuStack*)malloc(sizeof(MenuStack));
    if (new_menu == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    new_menu->menu_id = id;
    strcpy(new_menu->menu_name, name);
    new_menu->next = *top;
    *top = new_menu;
    
    printf("Menu '%s' added to history.\n", name);
}

MenuStack* pop_menu(MenuStack** top) {
    if (*top == NULL) {
        return NULL;
    }
    
    MenuStack* popped = *top;
    *top = (*top)->next;
    return popped;
}

void display_menu_history(MenuStack* top) {
    if (top == NULL) {
        printf("No menu history available.\n");
        return;
    }
    
    printf("\n=== MENU HISTORY (Recent to Oldest) ===\n");
    MenuStack* current = top;
    int count = 1;
    
    while (current != NULL) {
        printf("%d. Menu ID: %d - %s\n", count, current->menu_id, current->menu_name);
        current = current->next;
        count++;
    }
    printf("========================================\n");
}

void free_menu_stack(MenuStack* top) {
    while (top != NULL) {
        MenuStack* temp = top;
        top = top->next;
        free(temp);
    }
}

// File: main.c
#include "bst_field_index.h"
#include "dll_papers.h"
#include "sll_citations.h"
#include "heap_sorter.h"
#include "menu_stack.h"

// Global variables
BST_Node* field_index = NULL;
MenuStack* menu_history = NULL;

// Function prototypes
void display_main_menu();
void search_by_field();
void display_all_fields();
void load_json_data();
void show_menu_history();
void clear_menu_history();
void initialize_sample_data();
void cleanup_and_exit();

void display_main_menu() {
    printf("\n===============================================\n");
    printf("         ACADEMIC PAPER SORTER v1.0\n");
    printf("===============================================\n");
    printf("1. Search papers by field of study\n");
    printf("2. Display all available fields\n");
    printf("3. Load data from JSON file\n");
    printf("4. Show menu history\n");
    printf("5. Clear menu history\n");
    printf("6. Exit program\n");
    printf("===============================================\n");
    printf("Choose option (1-6): ");
}

void search_by_field() {
    char field[100];
    printf("\n=== SEARCH PAPERS BY FIELD ===\n");
    printf("Enter field of study: ");
    fgets(field, sizeof(field), stdin);
    field[strcspn(field, "\n")] = 0; // Remove newline
    
    if (strlen(field) == 0) {
        printf("Error: Field name cannot be empty.\n");
        return;
    }
    
    BST_Node* node = search_field(field_index, field);
    if (node == NULL) {
        printf("Field '%s' not found in the database.\n", field);
        printf("Available fields:\n");
        inorder_traversal(field_index);
        return;
    }
    
    printf("\n=== PAPERS IN FIELD: %s ===\n", field);
    printf("Papers sorted by citation count (highest to lowest):\n");
    printf("--------------------------------------------------------\n");
    
    if (node->papers_head == NULL) {
        printf("No papers found in this field.\n");
        return;
    }
    
    // Count total papers first
    int paper_count = 0;
    Paper* counter = node->papers_head;
    while (counter != NULL) {
        paper_count++;
        counter = counter->next;
    }
    
    printf("Total papers found: %d\n\n", paper_count);
    
    // Sort by citations using heap
    MaxHeap* heap = create_heap(paper_count + 10); // Extra space for safety
    Paper* current = node->papers_head;
    
    while (current != NULL) {
        insert_heap(heap, current);
        current = current->next;
    }
    
    // Extract and display in descending order of citations
    int rank = 1;
    while (heap->size > 0) {
        Paper* paper = extract_max(heap);
        printf("Rank %d:\n", rank++);
        printf("  Title: %s\n", paper->title);
        printf("  Year: %d\n", paper->year);
        printf("  Citations: %d\n", paper->citation_count);
        printf("  Citation Details: %d total citations\n", count_citations(paper->citations_head));
        printf("--------------------------------------------------------\n");
    }
    
    free_heap(heap);
}

void display_all_fields() {
    printf("\n=== ALL AVAILABLE FIELDS ===\n");
    if (field_index == NULL) {
        printf("No fields available in the database.\n");
        return;
    }
    
    printf("Fields are displayed in alphabetical order:\n");
    printf("------------------------------------------\n");
    inorder_traversal(field_index);
    printf("------------------------------------------\n");
}

void load_json_data() {
    char filename[256];
    printf("\n=== LOAD DATA FROM JSON FILE ===\n");
    printf("Enter JSON filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline
    
    if (strlen(filename) == 0) {
        printf("Error: Filename cannot be empty.\n");
        return;
    }
    
    // Try to open file to check if it exists
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file '%s'. Please check if the file exists.\n", filename);
        return;
    }
    fclose(file);
    
    printf("JSON loading feature is under development.\n");
    printf("File '%s' was found and can be processed in future versions.\n", filename);
    printf("For now, the system uses sample data.\n");
}

void show_menu_history() {
    printf("\n=== MENU USAGE HISTORY ===\n");
    display_menu_history(menu_history);
}

void clear_menu_history() {
    printf("\n=== CLEAR MENU HISTORY ===\n");
    if (menu_history == NULL) {
        printf("Menu history is already empty.\n");
        return;
    }
    
    char confirm;
    printf("Are you sure you want to clear all menu history? (y/n): ");
    scanf(" %c", &confirm);
    getchar(); // consume newline
    
    if (confirm == 'y' || confirm == 'Y') {
        free_menu_stack(menu_history);
        menu_history = NULL;
        printf("Menu history cleared successfully.\n");
    } else {
        printf("Operation cancelled.\n");
    }
}

void initialize_sample_data() {
    printf("Initializing Academic Paper Sorter with sample data...\n");
    
    // Create sample papers
    Paper* paper1 = create_paper("Deep Learning in Computer Vision", "Computer Science", 2020, 150);
    Paper* paper2 = create_paper("Machine Learning Algorithms", "Computer Science", 2019, 200);
    Paper* paper3 = create_paper("Neural Networks and AI", "Computer Science", 2021, 180);
    Paper* paper4 = create_paper("DNA Sequencing Methods", "Biology", 2021, 75);
    Paper* paper5 = create_paper("Genetic Engineering Techniques", "Biology", 2020, 95);
    Paper* paper6 = create_paper("Quantum Computing Fundamentals", "Physics", 2022, 45);
    Paper* paper7 = create_paper("Particle Physics Research", "Physics", 2019, 120);
    
    // Add sample citations to papers
    Citation* cite1 = create_citation("P001", "Advanced CV Techniques");
    Citation* cite2 = create_citation("P002", "Modern AI Applications");
    add_citation(&(paper1->citations_head), cite1);
    add_citation(&(paper1->citations_head), cite2);
    
    Citation* cite3 = create_citation("P003", "ML in Healthcare");
    Citation* cite4 = create_citation("P004", "Automated Learning Systems");
    Citation* cite5 = create_citation("P005", "AI Ethics and Implementation");
    add_citation(&(paper2->citations_head), cite3);
    add_citation(&(paper2->citations_head), cite4);
    add_citation(&(paper2->citations_head), cite5);
    
    // Build field index
    field_index = insert_field(field_index, "Computer Science");
    field_index = insert_field(field_index, "Biology");
    field_index = insert_field(field_index, "Physics");
    field_index = insert_field(field_index, "Mathematics");
    field_index = insert_field(field_index, "Chemistry");
    
    // Add papers to respective fields
    BST_Node* cs_node = search_field(field_index, "Computer Science");
    if (cs_node != NULL) {
        insert_paper_end(&(cs_node->papers_head), paper1);
        insert_paper_end(&(cs_node->papers_head), paper2);
        insert_paper_end(&(cs_node->papers_head), paper3);
    }
    
    BST_Node* bio_node = search_field(field_index, "Biology");
    if (bio_node != NULL) {
        insert_paper_end(&(bio_node->papers_head), paper4);
        insert_paper_end(&(bio_node->papers_head), paper5);
    }
    
    BST_Node* physics_node = search_field(field_index, "Physics");
    if (physics_node != NULL) {
        insert_paper_end(&(physics_node->papers_head), paper6);
        insert_paper_end(&(physics_node->papers_head), paper7);
    }
    
    printf("Sample data loaded successfully!\n");
    printf("- 3 papers in Computer Science\n");
    printf("- 2 papers in Biology\n");
    printf("- 2 papers in Physics\n");
    printf("- Additional empty fields: Mathematics, Chemistry\n");
}

void cleanup_and_exit() {
    printf("\n=== CLEANING UP AND EXITING ===\n");
    printf("Freeing allocated memory...\n");
    
    // Free BST and all associated papers
    free_bst(field_index);
    
    // Free menu history
    free_menu_stack(menu_history);
    
    printf("Memory cleanup completed.\n");
    printf("Thank you for using Academic Paper Sorter!\n");
    printf("Program developed by Team Members 1-5.\n");
    exit(0);
}

int main() {
    int choice;
    
    // Initialize system
    printf("=========================================\n");
    printf("  WELCOME TO ACADEMIC PAPER SORTER\n");
    printf("=========================================\n");
    
    initialize_sample_data();
    
    // Main program loop
    while (1) {
        display_main_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf("Error: Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                push_menu(&menu_history, 1, "Search by Field");
                search_by_field();
                break;
                
            case 2:
                push_menu(&menu_history, 2, "Display All Fields");
                display_all_fields();
                break;
                
            case 3:
                push_menu(&menu_history, 3, "Load JSON Data");
                load_json_data();
                break;
                
            case 4:
                push_menu(&menu_history, 4, "Show Menu History");
                show_menu_history();
                break;
                
            case 5:
                push_menu(&menu_history, 5, "Clear Menu History");
                clear_menu_history();
                break;
                
            case 6:
                push_menu(&menu_history, 6, "Exit Program");
                cleanup_and_exit();
                break;
                
            default:
                printf("Invalid option! Please choose between 1-6.\n");
                printf("Press Enter to continue...");
                getchar();
        }
        
        // Pause before showing menu again
        printf("\nPress Enter to continue...");
        getchar();
    }
    
    return 0;
}

// Additional helper functions that might be missing from other members

// For BST traversal (if not implemented in member 1)
void inorder_traversal(BST_Node* root) {
    if (root != NULL) {
        inorder_traversal(root->left);
        
        // Count papers in this field
        int paper_count = 0;
        Paper* current = root->papers_head;
        while (current != NULL) {
            paper_count++;
            current = current->next;
        }
        
        printf("- %s (%d papers)\n", root->field, paper_count);
        inorder_traversal(root->right);
    }
}

// For BST cleanup (if not implemented in member 1)
void free_bst(BST_Node* root) {
    if (root != NULL) {
        free_bst(root->left);
        free_bst(root->right);
        
        // Free all papers in this field
        free_papers_list(root->papers_head);
        
        free(root);
    }
}

// For paper list cleanup (if not implemented in member 2)
void free_papers_list(Paper* head) {
    while (head != NULL) {
        Paper* temp = head;
        head = head->next;
        
        // Free citations for this paper
        free_citations(temp->citations_head);
        
        free(temp);
    }
}

// For citations cleanup (if not implemented in member 3)
void free_citations(Citation* head) {
    while (head != NULL) {
        Citation* temp = head;
        head = head->next;
        free(temp);
    }
}

// For heap operations (if missing from member 4)
void heapify_down(MaxHeap* heap, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    
    if (left < heap->size && heap->papers[left]->citation_count > heap->papers[largest]->citation_count) {
        largest = left;
    }
    
    if (right < heap->size && heap->papers[right]->citation_count > heap->papers[largest]->citation_count) {
        largest = right;
    }
    
    if (largest != index) {
        // Swap
        Paper* temp = heap->papers[index];
        heap->papers[index] = heap->papers[largest];
        heap->papers[largest] = temp;
        
        heapify_down(heap, largest);
    }
}

void free_heap(MaxHeap* heap) {
    if (heap != NULL) {
        free(heap->papers);
        free(heap);
    }
}