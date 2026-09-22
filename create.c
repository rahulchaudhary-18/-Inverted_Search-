#include "inverted.h"
/* create_database(HT, head) */

void create_database(hash *HT, F_node *head)
{
    /* every hash bucket must start empty - nothing else initializes this */
    for (int i = 0; i < 27; i++)
    {
        HT[i].index = i;
        HT[i].H_link = NULL;
    }

    if (head == NULL)
    {
        printf("INFO : No files loaded - nothing to create. "
               "Run the program with .txt filenames as arguments.\n");
        return;
    }

    int files_processed = 0;
    int words_read = 0;

    F_node *file = head;

    while (file != NULL)
    {
        FILE *fp = fopen(file->f_name, "r");

        if (fp == NULL)
        {
            /* couldn't open this file - skip it, move to the next */
            file = file->link;
            continue;
        }

        files_processed++;

        char word[25];

        /* read one word at a time until end of file */
        while (fscanf(fp, "%24s", word) == 1)
        {
            words_read++;
            int index = get_index(word);

            /* search the main node chain at this bucket for this word */
            M_node *mtemp = HT[index].H_link;
            M_node *mprev = NULL;
            int found = 0;

            while (mtemp != NULL)
            {
                if (strcmp(mtemp->word, word) == 0)
                {
                      found = 1;
                    break;
                }
                mprev = mtemp;
                mtemp = mtemp->main_link;
            }

            if (!found)
            {
                /* brand new word for this bucket - create main node + sub node */
                M_node *newMain = malloc(sizeof(M_node));
                strcpy(newMain->word, word);
                newMain->file_count = 1;
                newMain->main_link = NULL;

                S_node *newSub = malloc(sizeof(S_node));
                strcpy(newSub->filename, file->f_name);
                newSub->word_count = 1;
                newSub->sub_link = NULL;

                newMain->sub_link = newSub;

                if (HT[index].H_link == NULL)
                {
                    /* first word in this bucket */
                    HT[index].H_link = newMain;
                }
                else
                {
                    /* append to the end of the main node chain */
                    mprev->main_link = newMain;
                }
            }
            else
            {
                /* word already exists (mtemp) - check if this file already
                   has a sub node under it */
                S_node *stemp = mtemp->sub_link;
                S_node *sprev = NULL;
                int sfound = 0;

                while (stemp != NULL)
                {
                    if (strcmp(stemp->filename, file->f_name) == 0)
                    {
                        sfound = 1;
                        break;
                    }
                    sprev = stemp;
                    stemp = stemp->sub_link;
                }

                if (sfound)
                {
                    /* word already counted for this file - just bump the count */
                    stemp->word_count++;
                }
                else
                {
                    /* first time this word appears in this particular file */
                    S_node *newSub = malloc(sizeof(S_node));
                    strcpy(newSub->filename, file->f_name);
                    newSub->word_count = 1;
                    newSub->sub_link = NULL;

                    sprev->sub_link = newSub;   /* append to end of sub chain */
                    mtemp->file_count++;
                }
            }
        }

        fclose(fp);
        file = file->link;
    }

    printf("INFO : Database successfully created - %d word(s) read from %d file(s)\n",
           words_read, files_processed);
}