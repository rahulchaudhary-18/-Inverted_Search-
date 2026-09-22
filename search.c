#include "inverted.h"
/* search_database(HT) */

void search_database(hash *HT)
{
    char word[25];

    printf("Enter the word to search : ");
    scanf("%24s", word);

    int index = get_index(word);
    M_node *m = HT[index].H_link;

    while (m != NULL)
    {
        if (strcmp(m->word, word) == 0)
        {
            printf("%s is present in %d file(s).\n", word, m->file_count);

            S_node *s = m->sub_link;
            while (s != NULL)
            {
                printf("   In file %s - word count %d time(s)\n", s->filename, s->word_count);
                s = s->sub_link;
            }

            return;
        }

        m = m->main_link;
    }

    printf("INFO : %s not found in the database\n", word);
}