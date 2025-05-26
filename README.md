# C4_DataStructureAlgorithm

SPESIFIKASI PROGRAM DAN PEMBAGIAN TUGAS
Academic Paper Sorter dengan Struktur Data
ANALISIS DATASET
Dataset S2ORC (Scientific Paper Open Research Corpus) berformat JSON lebih cocok karena:

Struktur data hierarkis (nested objects/arrays)
Field yang kompleks seperti citations, authors, abstracts
Metadata yang kaya per paper

FITUR PROGRAM UTAMA

Input: Field of Study (contoh: "Computer Science", "Biology", "Physics")
Process:

Filter papers berdasarkan field of study
Sort berdasarkan tahun publikasi
Sort berdasarkan jumlah citations (tinggi ke rendah)


Output: Daftar judul paper yang sesuai dengan field of study, terurut

IDENTIFIKASI STRUKTUR DATA

Binary Search Tree (BST): Untuk indexing dan pencarian field of study
Double Linked List: Untuk menyimpan dan mengurutkan papers berdasarkan tahun
Single Linked List: Untuk menyimpan citations per paper
Priority Queue/Heap: Untuk sorting berdasarkan citation count


PEMBAGIAN TUGAS (5 ANGGOTA)
Anggota 1: Dzakir Tsabit Asy-syafiq (241511071)
Struktur Data: Binary Search Tree (BST)
Tugas Programming:

Implementasi BST untuk indexing field of study
Modul pencarian field of study dengan BST
Insert, search, dan traversal BST
File: bst_field_index.h, bst_field_index.c

Tugas Laporan: BAB I - Spesifikasi Program (1.1 & 1.2)
Anggota 2: Fatimah Hawwa Alkhansa (241511074)
Struktur Data: Double Linked List (DLL)
Tugas Programming:

Implementasi DLL untuk menyimpan paper data
Sorting papers berdasarkan tahun publikasi
Insert, delete, dan traversal DLL
File: dll_papers.h, dll_papers.c

Tugas Laporan: BAB I - Spesifikasi Program (1.3) & BAB II - Perancangan (2.1)
Anggota 3: Ibnu Hilmi Athaillah (241511079)
Struktur Data: Single Linked List (SLL)
Tugas Programming:

Implementasi SLL untuk citations per paper
Counting dan managing citations
Merge sort untuk SLL berdasarkan citation count
File: sll_citations.h, sll_citations.c

Tugas Laporan: BAB II - Perancangan (2.2 & 2.3)
Anggota 4: Rizky Satria Gunawan (241511089)
Struktur Data: Priority Queue/Max Heap
Tugas Programming:

Implementasi Max Heap untuk sorting berdasarkan citations
JSON parser untuk membaca dataset S2ORC
Data preprocessing dan filtering
File: heap_sorter.h, heap_sorter.c, json_parser.c

Tugas Laporan: BAB III - Implementasi (3.1 & 3.2)
Anggota 5: Varian Abidarma Syuhada (241511091)
Struktur Data: Stack (untuk menu navigation)
Tugas Programming:

Main program dan user interface
Menu system dengan Stack
Integration semua modul
Output formatting dan display
File: main.c, menu_stack.h, menu_stack.c

Tugas Laporan: BAB IV - Kesimpulan & koordinasi keseluruhan laporan

STRUKTUR PROGRAM DETAIL
Data Structures
c// Paper structure
typedef struct Paper {
    char title[256];
    char field_of_study[100];
    int year;
    int citation_count;
    struct Citation* citations_head; // SLL
    struct Paper* next;  // untuk DLL
    struct Paper* prev;  // untuk DLL
} Paper;

// BST Node for field indexing
typedef struct BST_Node {
    char field[100];
    struct Paper* papers_head; // DLL of papers
    struct BST_Node* left;
    struct BST_Node* right;
} BST_Node;

// Citation SLL
typedef struct Citation {
    char citing_paper_id[50];
    struct Citation* next;
} Citation;

// Heap for citation sorting
typedef struct {
    Paper** papers;
    int size;
    int capacity;
} MaxHeap;
Alur Program

Load Data: Parse JSON dataset (Anggota 4)
Index Fields: Build BST dengan field of study (Anggota 1)
Store Papers: Simpan dalam DLL per field (Anggota 2)
Count Citations: Build SLL citations per paper (Anggota 3)
User Interface: Menu dengan Stack navigation (Anggota 5)
Search & Sort: Kombinasi semua struktur data
Display Results: Format output yang rapi

File Structure
project/
├── headers/
│   ├── bst_field_index.h      (Anggota 1)
│   ├── dll_papers.h           (Anggota 2)  
│   ├── sll_citations.h        (Anggota 3)
│   ├── heap_sorter.h          (Anggota 4)
│   └── menu_stack.h           (Anggota 5)
├── src/
│   ├── bst_field_index.c      (Anggota 1)
│   ├── dll_papers.c           (Anggota 2)
│   ├── sll_citations.c        (Anggota 3)
│   ├── heap_sorter.c          (Anggota 4)
│   ├── json_parser.c          (Anggota 4)
│   ├── menu_stack.c           (Anggota 5)
│   └── main.c                 (Anggota 5)
├── data/
│   └── s2orc_sample.json
└── docs/
    └── laporan.docx
SKENARIO PENGUJIAN
NoFitur ProgramInputHasil yang DiharapkanStatus1Search by Field"Computer Science"List papers CS, sorted by year & citations-2Year SortingPapers 2015-2023Chronological order-3Citation SortingPapers with varying citationsDescending by citation count-4Field IndexingMultiple fieldsFast search via BST-5Memory ManagementLarge datasetNo memory leaks-
TIMELINE PENGERJAAN

Minggu 1: Setup project, parse sample data (Anggota 4)
Minggu 2: Implementasi individual structures (Semua)
Minggu 3: Integration dan testing (Anggota 5 koordinasi)
Minggu 4: Laporan dan finalisasi (Sesuai pembagian)

KONTRIBUSI SETIAP ANGGOTA
AnggotaStruktur DataKompleksitas AlgoritmaEstimasi EffortDzakirBSTO(log n) search20%FatimahDLLO(n) traversal, O(n²) sort20%IbnuSLLO(n) operations15%RizkyHeap + ParserO(log n) heap, O(n) parse25%VarianStack + IntegrationO(1) stack, O(n) display20%
Setiap anggota mendapat struktur data yang sesuai dengan tingkat kesulitan yang seimbang dan berkontribusi pada fitur utama program.