/*    RAHUL CHAUDHARY     */


#include "inverted.h"

int main(int argc, char *argv[])
{
    F_node *head = NULL;
    hash HT[27];

    // Validate_files


	if(argc >= 2)
	{
		validate_files(argv + 1, &head);
	}
	else
	{
		printf("file not present or invalid\n");
	}

    int option;
    do
    {
	printf("Select your choice among following operations:\n1. Create Database\n2. Display Database\n3. Save Database\n4. Search\n5. Update Database\n6. List Files\n7. Exit\n\nEnter your choice : ");

	scanf("%d", &option);

	switch (option)
	{
	    case 1:
		create_database(HT, head);
		break;

	    case 2:
		display_database(HT);
		break;

	    case 3:
		save_database(HT);
		break;

	    case 4:
		search_database(HT);
		break;

	    case 5:
		update_database(HT, &head);
		break;

	    case 6:
	    {
		int count = print_filenames(head);
		if (count == 0)
		{
		    printf("INFO : No files loaded yet\n");
		}
		else
		{
		    printf("INFO : %d file(s) successfully listed\n", count);
		}
		break;
	    }

	    case 7:
		break;

	    default:
		printf("INFO : Please enter the valid option\n");
	}
    }while( option != 7 );

    return 0;
}