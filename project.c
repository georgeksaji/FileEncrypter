#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FileNode {
    char filename[100];
    char operation[20];
    char result[100];
    struct FileNode *next;
};

struct FileNode *head = NULL;

void add_history(char *filename, char *operation, char *result) {
    struct FileNode *new_node = (struct FileNode *)malloc(sizeof(struct FileNode));
    strcpy(new_node->filename, filename);
    strcpy(new_node->operation, operation);
    strcpy(new_node->result, result);
    new_node->next = head;
    head = new_node;
}

void display_processed_files() {
    struct FileNode *current = head;
    if (current == NULL) {
        printf("No files have been processed yet.\n");
        return;
    }
    printf("\nProcessed Files:\n");
    while (current != NULL) {
        printf("Operation: %s, Filename: %s, Result: %s\n",  current->operation,current->filename, current->result);
        current = current->next;
    }
    printf("\n\n");

}

void encrypt_or_decrypt() {
    char input_file[100], output_file[100], password[100];
    printf("Enter input file name: ");
    scanf("%s", input_file);
    printf("Enter output file name: ");
    scanf("%s", output_file);
    printf("Enter encryption key: ");
    scanf("%s", password);

    FILE *inputted_f = fopen(input_file, "rb");
    FILE *outputted_f = fopen(output_file, "wb");
    if (inputted_f == NULL || outputted_f == NULL) {
        printf("Error opening file!\n");
        add_history(input_file, "Encrypt/Decrypt", "Error");
        return;
    }

    size_t password_len = strlen(password);
    size_t i = 0;
    char ch;

    while ((ch = fgetc(inputted_f)) != EOF) {
        fputc(ch ^ password[i % password_len], outputted_f);
        i++;
    }

    fclose(inputted_f);
    fclose(outputted_f);
    printf("File processed successfully.\n");

    // Add to the linked list
    add_history(input_file, "Encrypt/Decrypt", "Success");
}

void check_hash() {
    char filename[100];
    printf("Enter file name: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file!\n");
        add_history(filename, "Hash Check", "Error");
        return;
    }

    unsigned long hash = 0;
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        hash = hash ^ ((hash << 4) + ch + (hash >> 3));

    }

    fclose(file);
    printf("Hash value: %lu\n", hash);

    char result[100];
    sprintf(result, "Hash: %lu", hash);
    add_history(filename, "Hash Check", result);

    printf("Do you want to save the hash value to a file? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'y') {
        char hash_filename[100];
        printf("Enter the file name to save the hash value: ");
        scanf("%s", hash_filename);
        FILE *hash_file = fopen(hash_filename, "w");
        if (hash_file == NULL) {
            printf("Error opening file!\n");
            return;
        }
        fprintf(hash_file, "%lu", hash);
        fclose(hash_file);
        add_history(hash_filename, "Hash Save", "Success");
        printf("Hash value saved to %s\n", hash_filename);
    }
}

void select_option() {
    while (1) {
    int choice;
    printf("1. Encrypt/Decrypt\n");
    printf("2. Check Hash\n");
    printf("3. Display Processed Files\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            encrypt_or_decrypt();
            break;
        case 2:
            check_hash();
            break;
        case 3:
            display_processed_files();
            break;
        case 4:
            printf("Exiting...\n");
            return;
        default:
            printf("Invalid choice! Please try again.\n");
    }
    }
}

int main() {
    printf("\n");
    printf("**************************************************************\n");
    printf("*                       FILE ENCRYPTOR                       *\n");
    printf("**************************************************************\n");
    printf("\t\tWelcome to File Encryptor!\n\n");
    select_option();
    return 0;
}
