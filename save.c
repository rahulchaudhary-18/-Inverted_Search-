#include "inverted.h"
/* save_database(HT) */

void save_database(hash *HT)
{
    char filename[50];

    printf("Enter the filename to save (must end with .txt) : ");
    scanf("%49s", filename);

    /* validation: filename must at least end with ".txt" */
    int len = strlen(filename);
    if (len < 4 || strcmp(filename + len - 4, ".txt") != 0)
    {
        printf("INFO : Invalid filename, saving as backup.txt\n");
        strcpy(filename, "backup.txt");
    }

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("INFO : Could not open file for writing\n");
        return;
    }

    for (int i = 0; i < 27; i++)
    {
        M_node *m = HT[i].H_link;

        while (m != NULL)
        {
            fprintf(fp, "#%d;%s;%d;", i, m->word, m->file_count);

            S_node *s = m->sub_link;
            while (s != NULL)
            {
                fprintf(fp, "%s;%d;", s->filename, s->word_count);
                s = s->sub_link;
            }

            fprintf(fp, "#\n");

            m = m->main_link;
        }
    }

    fclose(fp);
    printf("INFO : Database successfully saved to %s\n", filename);
}