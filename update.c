#include "inverted.h"
/* update_database(HT, head) */

int update_database(hash *HT, F_node **head)
{
    char filename[50];

    printf("Enter the backup filename to load : ");
    scanf("%49s", filename);

    int len = strlen(filename);
    if (len < 4 || strcmp(filename + len - 4, ".txt") != 0)
    {
        printf("INFO : %s is not a valid backup file\n", filename);
        return FAILURE;
    }

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("INFO : %s is not a valid backup file\n", filename);
        return FAILURE;
    }

    int index, filecount, wordcount;
    char word[25], subfilename[20];

    /* scan until EOF - one full record ("#index;word;filecount;file;count;...;#") per iteration */
    while (fscanf(fp, " #%d;%24[^;];%d;", &index, word, &filecount) == 3)
    {
        /* main node creation */
        M_node *newMain = malloc(sizeof(M_node));
        strcpy(newMain->word, word);
        newMain->file_count = filecount;
        newMain->main_link = NULL;
        newMain->sub_link = NULL;

        S_node *subTail = NULL;

        /* run filecount time(s) - read one sub node per file this word appeared in */
        for (int i = 0; i < filecount; i++)
        {
            if (fscanf(fp, "%19[^;];%d;", subfilename, &wordcount) != 2)
            {
                break;   /* malformed record - stop reading sub nodes for this word */
            }

            S_node *newSub = malloc(sizeof(S_node));
            strcpy(newSub->filename, subfilename);
            newSub->word_count = wordcount;
            newSub->sub_link = NULL;

            if (newMain->sub_link == NULL)
            {
                newMain->sub_link = newSub;
            }
            else
            {
                subTail->sub_link = newSub;
            }
            subTail = newSub;
        }

        /* consume the trailing '#' that closes this record */
        fscanf(fp, " #");

        /* append this main node to the end of its bucket's chain */
        if (HT[index].H_link == NULL)
        {
            HT[index].H_link = newMain;
        }
        else
        {
            M_node *mtemp = HT[index].H_link;
            while (mtemp->main_link != NULL)
            {
                mtemp = mtemp->main_link;
            }
            mtemp->main_link = newMain;
        }
    }

    fclose(fp);
    printf("INFO : Database successfully loaded from %s\n", filename);

    /* add a new file (e.g. f3.txt) by calling create_database */
    char newfile[20];
    printf("Enter a new file to add to the database : ");
    scanf("%19s", newfile);

    int flen = strlen(newfile);
    if (flen < 4 || strcmp(newfile + flen - 4, ".txt") != 0)
    {
        printf("INFO : %s is not a valid .txt file\n", newfile);
        return SUCCESS;   /* backup load still succeeded, just skip adding the file */
    }

    /* build a single-node file list containing just this new file,
       then merge its words into the already-loaded table */
    F_node onlyNew;
    strcpy(onlyNew.f_name, newfile);
    onlyNew.link = NULL;

    create_database(HT, &onlyNew);
    printf("INFO : %s successfully added to the database\n", newfile);

    /* also record this file in the caller's master file list */
    F_node *newNode = malloc(sizeof(F_node));
    strcpy(newNode->f_name, newfile);
    newNode->link = NULL;

    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        F_node *temp = *head;
        while (temp->link != NULL)
        {
            temp = temp->link;
        }
        temp->link = newNode;
    }

    printf("INFO : Database update completed successfully\n");
    return SUCCESS;
}