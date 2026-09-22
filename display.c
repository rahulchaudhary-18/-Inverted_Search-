#include "inverted.h"
/*display_database(HT)*/

void display_database(hash *HT)
{
    int records_printed = 0;

    printf("%-8s%-15s%-12s%-15s%-10s\n", "index", "word", "filecount", "filename", "wordcount");

    for (int i = 0; i < 27; i++)
    {
        M_node *m = HT[i].H_link;

        while (m != NULL)
        {
            S_node *s = m->sub_link;
            int first = 1;   /* only print index/word/filecount on the first sub-node row */

            while (s != NULL)
            {
                if (first)
                {
                    printf("%-8d%-15s%-12d%-15s%-10d\n", i, m->word, m->file_count, s->filename, s->word_count);
                    first = 0;
                    records_printed++;
                }
                else
                {
                    printf("%-8s%-15s%-12s%-15s%-10d\n", "", "", "", s->filename, s->word_count);
                }

                s = s->sub_link;
            }

            m = m->main_link;
        }
    }

    if (records_printed == 0)
    {
        printf("INFO : Database is empty - nothing to display\n");
    }
    else
    {
        printf("INFO : Database successfully displayed - %d word(s) shown\n", records_printed);
    }
}