// ==================== ANGGOTA 5: MAIN PROGRAM & MENU STACK ====================
// File: menu_stack.h
#ifndef MENU_STACK_H
#define MENU_STACK_H

#include <stdio.h>
#include <stdlib.h>

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
