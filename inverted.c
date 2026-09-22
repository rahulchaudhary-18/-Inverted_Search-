#include "inverted.h"
/* Function definitions */
// check for duplicates

int check_duplicates(F_node *head, char *f_name)
{
    F_node *temp = head;

    while (temp != NULL)
    {
        if (strcmp(temp->f_name, f_name) == 0)
        {
            return SUCCESS; // duplicate found
        }
        temp = temp->link;
    }
    return FAILURE; // no duplicates
}

// insert the new filename at the end of the singly linked list
int insert_last(F_node **head, char *f_name)
{
    F_node *new_node = (F_node *)malloc(sizeof(F_node));

    if (new_node == NULL)
    {
        printf("ERROR : memory allocation failed\n");
        return FAILURE;
    }

    /* f_name in F_node is a fixed char[20] - use strncpy so a longer
       command-line argument can never overflow it */
    strncpy(new_node->f_name, f_name, sizeof(new_node->f_name) - 1);
    new_node->f_name[sizeof(new_node->f_name) - 1] = '\0';
    new_node->link = NULL;

    if (*head == NULL)
    {
        *head = new_node;
    }
    else
    {
        F_node *temp = *head;
        while (temp->link != NULL)
        {
            temp = temp->link;
        }
        temp->link = new_node;
    }
    return SUCCESS;
}

// print all files stored in the list, shown as a chain: a.txt->b.txt->NULL
int print_filenames(F_node *head)
{
    F_node *temp = head;
    int count = 0;

    while (temp != NULL)
    {
        printf("%s->", temp->f_name);
        temp = temp->link;
        count++;
    }
    printf("NULL\n");

    return count;
}

void validate_files(char *argv[], F_node **head)
{
    FILE *fp;
    char *extension;
    long size;

    for (int i = 0; argv[i] != NULL; i++)
    {
        // 1. check the extension (must be .txt) - use strrchr (last '.')
        //    so a name like "my.file.txt" is matched on its real extension
        //    instead of the first dot it happens to contain
        extension = strrchr(argv[i], '.');
        if (extension == NULL || strcmp(extension, ".txt") != 0)
        {
            printf("INFO : Skipping \"%s\" - not a .txt file\n", argv[i]);
            continue;
        }

        // 2. check the duplicate files (not allowed)
        if (check_duplicates(*head, argv[i]) == SUCCESS)
        {
            printf("INFO : Skipping \"%s\" - duplicate file\n", argv[i]);
            continue;
        }

        // 3. check the file existing status
        fp = fopen(argv[i], "r");
        if (fp == NULL)
        {
            printf("INFO : Skipping \"%s\" - cannot open / does not exist\n", argv[i]);
            continue;
        }

        // 4. check the file status (empty or not)
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);

        if (size == 0)
        {
            printf("INFO : Skipping \"%s\" - file is empty\n", argv[i]);
            fclose(fp);
            continue;
        }

        fclose(fp);

        // after validation
        insert_last(head, argv[i]);
        printf("INFO : \"%s\" added to database file list\n", argv[i]);
    }
}

/*
 * get_index(word)
 * Hashes a word by its first character:
 *   'a'/'A' -> 0, 'b'/'B' -> 1, ... 'z'/'Z' -> 25
 *   anything non-alphabetic (digit, punctuation, etc.) -> bucket 26
 * Matches the hash table sketch: 27 buckets total (0-25 for letters, 26 for "non-alphabet").
 */
int get_index(char *word)
{
    if (isalpha((unsigned char)word[0]))
    {
        return tolower((unsigned char)word[0]) - 'a';
    }

    return 26;
}