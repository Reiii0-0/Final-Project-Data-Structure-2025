#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// === File and Console UI Definitions ===
#define FILENAME "C:\\Kurikulum_Informatika\\Semester_2\\Assignment\\Data_Structure\\Final_Project\\Final-Project-Data-Structure-2025\\data\\komponen.dat"
#define MAX 999

// Console colors for styling the UI
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[1;31m"
#define COLOR_CYAN "\033[1;36m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_GREEN  "\033[1;32m"
#define COLOR_BLUE   "\033[1;34m"
#define COLOR_MAGENTA "\033[1;35m"

// === Component Data Structure ===
// This structure stores a single vehicle component's information
typedef struct {
    char number[7];      // 6-digit component number
    char name[26];       // Component name (up to 25 characters + null terminator)
    int stock;           // Available stock quantity
    float price;         // Unit price
} Component;

// === Function Declarations ===
int is_duplicate(const char *filename, const char *number);
void add_data(const char *filename);
void edit_data_by_index(const char *filename, int index);
void delete_data_by_index(const char *filename, int index);
void display_data(const Component *data, int count);
void display_data_paged(const Component *data, int count);
void display_all_unsorted(const char *filename);
void display_all_sorted(const char *filename);


// === File Operation Functions ===

// Check if the component number already exists in the binary file
int is_duplicate(const char *filename, const char *number) {
    Component c;
    FILE *fp = fopen(filename, "rb");
    if (!fp) return 0;

    while (fread(&c, sizeof(Component), 1, fp)) {
        if (strcmp(c.number, number) == 0) {
            fclose(fp);
            return 1;  // Duplicate found
        }
    }
    fclose(fp);
    return 0;  // No duplicate
}

// Add a new component to the binary file
void add_data(const char *filename) {
    Component c;
    FILE *fp = fopen(filename, "ab");
    if (!fp) {
        printf("Failed to open file.\n");
        return;
    }

    // Input component fields from user
    printf("Enter component number (6 digits): ");
    scanf("%6s", c.number); getchar();

    if (is_duplicate(filename, c.number)) {
        printf("Component number already exists. Use another.\n");
        fclose(fp);
        return;
    }

    printf("Enter component name (max 25 chars): ");
    scanf(" %26[^\n]", c.name); getchar();

    printf("Enter stock: ");
    scanf("%d", &c.stock);
    printf("Enter price: ");
    scanf("%f", &c.price);

    // Write the new component to the binary file
    fwrite(&c, sizeof(Component), 1, fp);
    fclose(fp);
    printf("Data successfully added.\n");
}

// Edit component by index
void edit_data_by_index(const char *filename, int index) {
    Component c;
    FILE *fp = fopen(filename, "rb+");
    if (!fp) {
        printf("Failed to open file.\n");
        return;
    }

    // Move to the correct record based on index
    fseek(fp, index * sizeof(Component), SEEK_SET);
    if (fread(&c, sizeof(Component), 1, fp) != 1) {
        printf("Index not found.\n");
        fclose(fp);
        return;
    }

    // User selects field to modify
    printf("\nSelect field to edit:\n");
    printf("1. Number\n2. Name\n3. Stock\n4. Price\n> ");
    int choice;
    scanf("%d", &choice); getchar();

    switch (choice) {
        case 1:
            printf("Current: %s\nNew: ", c.number);
            fgets(c.number, sizeof(c.number), stdin);
            c.number[strcspn(c.number, "\n")] = 0;
            break;
        case 2:
            printf("Current: %s\nNew: ", c.name);
            fgets(c.name, sizeof(c.name), stdin);
            c.name[strcspn(c.name, "\n")] = 0;
            break;
        case 3:
            printf("Current: %d\nNew: ", c.stock);
            scanf("%d", &c.stock);
            break;
        case 4:
            printf("Current: %.2f\nNew: ", c.price);
            scanf("%f", &c.price);
            break;
        default:
            printf("Invalid choice.\n");
            fclose(fp);
            return;
    }

    // Save updated component back to file
    fseek(fp, index * sizeof(Component), SEEK_SET);
    fwrite(&c, sizeof(Component), 1, fp);
    fclose(fp);
    printf("Data successfully updated.\n");
}

// Delete a component based on its index
void delete_data_by_index(const char *filename, int index) {
    Component c;
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Failed to open file.\n");
        return;
    }

    FILE *temp_fp = fopen("temp.dat", "wb");
    if (!temp_fp) {
        fclose(fp);
        printf("Failed to create temp file.\n");
        return;
    }

    // Copy all components except the one to be deleted
    int current = 0, found = 0;
    while (fread(&c, sizeof(Component), 1, fp)) {
        if (current != index)
            fwrite(&c, sizeof(Component), 1, temp_fp);
        else
            found = 1;
        current++;
    }

    fclose(fp);
    fclose(temp_fp);
    remove(filename);
    rename("temp.dat", filename);

    if (found)
        printf("Component at index %d deleted.\n", index);
    else
        printf("Index not found.\n");
}

// Display a single page (up to 10 records) of component data
void display_data(const Component *data, int count) {
    printf(COLOR_CYAN "+----------+---------------------------+--------+-----------------+\n" COLOR_RESET);
    printf(COLOR_CYAN "| Number   | Name                      | Stock  | Price           |\n");
    printf("+----------+---------------------------+--------+-----------------+\n" COLOR_RESET);

    for (int i = 0; i < count; i++) {
        printf(COLOR_CYAN "| " COLOR_YELLOW "%-8s" COLOR_CYAN " | "
                       COLOR_GREEN  "%-25s" COLOR_CYAN " | "
                       COLOR_BLUE   "%6d" COLOR_CYAN " | "
                       COLOR_MAGENTA "Rp%13.2f" COLOR_CYAN " |\n" COLOR_RESET,
               data[i].number, data[i].name, data[i].stock, data[i].price);
    }

    printf(COLOR_CYAN "+----------+---------------------------+--------+-----------------+\n" COLOR_RESET);
}

// Show paged data with total asset calculation
void display_data_paged(const Component *data, int count) {
    int page = 0, input;
    float total = 0;
    for (int i = 0; i < count; i++)
        total += data[i].stock * data[i].price;

    do {
        system("cls || clear");
        int start = page * 10;
        int end = (start + 10 < count) ? start + 10 : count;
        display_data(&data[start], end - start);
        printf("\nPage %d of %d. Use ←/→ arrows, ESC to exit.\n", page + 1, (count + 9) / 10);
        input = getch();
        if (input == 224) {
            input = getch();
            if (input == 77 && (page + 1) * 10 < count) page++;
            else if (input == 75 && page > 0) page--;
        }
    } while (input != 27);

    printf("\nTotal Asset Value: Rp %.2f\n", total);
}

// Load and display all components (original order)
void display_all_unsorted(const char *filename) {
    Component data[MAX];
    int count = 0;
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("File not found.\n");
        return;
    }
    while (fread(&data[count], sizeof(Component), 1, fp)) count++;
    fclose(fp);
    display_data_paged(data, count);
}

// Load and display all components (sorted by component number)
void display_all_sorted(const char *filename) {
    Component data[MAX];
    int count = 0;
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("File not found.\n");
        return;
    }
    while (fread(&data[count], sizeof(Component), 1, fp)) count++;
    fclose(fp);

    // Simple Bubble Sort by component number
    for (int i = 0; i < count - 1; i++)
        for (int j = i + 1; j < count; j++)
            if (strcmp(data[i].number, data[j].number) > 0) {
                Component tmp = data[i];
                data[i] = data[j];
                data[j] = tmp;
            }

    display_data_paged(data, count);
}

// === Main Menu Entry Point ===
int main() {
    int selected = 0, index;
    char *menu[] = {
        "1. Add Data",
        "2. Edit Data (by Index)",
        "3. Show All Data (Sorted)",
        "4. Show All Data (Unsorted)",
        "5. Delete Data (by Index)",
        "6. Exit"
    };
    const int menu_count = 6;

    while (1) {
        fflush(stdin);
        system("cls || clear");

        // Render interactive menu with arrow and number navigation
        printf("\n=== COMPONENT DATA MENU ===\n");
        for (int i = 0; i < menu_count; i++) {
            if (i == selected)
                printf(COLOR_RED "-> " COLOR_CYAN "%s" COLOR_RESET "\n", menu[i]);
            else
                printf("   %s\n", menu[i]);
        }

        printf("\nUse arrow keys ↑ ↓ or number [1–6] to select, then press Enter.\n");

        char key = getch();
        if (key == -32 || key == 224) {
            key = getch();
            if (key == 72 && selected > 0) selected--;
            else if (key == 80 && selected < menu_count - 1) selected++;
        } else if (key >= '1' && key <= '6') {
            selected = key - '1';
            key = 13; // Simulate Enter
        }

        if (key == 13) {
            switch (selected) {
                case 0: add_data(FILENAME); break;
                case 1:
                    printf("Enter index to edit: ");
                    scanf("%d", &index);
                    edit_data_by_index(FILENAME, index);
                    break;
                case 2: display_all_sorted(FILENAME); break;
                case 3: display_all_unsorted(FILENAME); break;
                case 4:
                    printf("Enter index to delete: ");
                    scanf("%d", &index);
                    delete_data_by_index(FILENAME, index);
                    break;
                case 5:
                    printf("Thank you!\n");
                    return 0;
            }
            printf("\nPress any key to return to menu...");
            getch();
        }
    }
    return 0;
}
