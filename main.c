#include <stdio.h>

#include "sb.h"
#include "lib/src/xlsx_drone.h"

/* 
 * convert column name to integer 
 * like "A" to a */
int get_colname_int(const char *s) {
    int col = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        col = col * 26 + (s[i] - 'A' + 1);
    }
    return col;
}

/* convert column number to string
 * like 1 to "A" */
void int_to_string(int col, char *buf) {
    char tmp[10];
    int i = 0;

    while (col > 0) {
        col--;  /* Excel columns are 1-based */
        tmp[i++] = 'A' + (col % 26);
        col /= 26;
    }

    /* reverse */
    for (int j = 0; j < i; j++) {
        buf[j] = tmp[i - j - 1];
    }
    buf[i] = '\0';
}

/* 
 * handle the error that is returned by xlsx_drone library functions
 * 
 */
int xlsx_errcheck(int retval)
{

}

int main(int argc, char **argv)
{
    /* error if no arguments have been passed */
    if (argc < 1) {
        fprintf(stderr, "Usage: ./xlsx_reader <filename> ...");
        exit(EXIT_FAILURE);
    }

    int book_idx = 1;
    int sheet_idx = 1;
    char **books = argv;
    xlsx_workbook_t wb;
    xlsx_sheet_t *sheet;

    /* iterate all the given books to open and read them */
    for (; book_idx < argc; book_idx++) {
        xlsx_open(books[book_idx], &wb);

        /* read sheets one by one */
        for (sheet_idx = 1; sheet_idx <= wb.n_sheets ; sheet_idx++){
            sheet = xlsx_load_sheet(&wb, sheet_idx, NULL);
            if (sheet == NULL) {
                if (xlsx_errno == XLSX_LOAD_SHEET_ERRNO_NON_EXISTENT) {
                    /* successfully read all the sheets the book have */
                    break;
                } else {
                    fprintf(stderr, "ERROR: xlsx_load_sheet() failed\n");
                    break;
                }
            }

            /* get last row and column  */
            char *last_column = xlsx_get_last_column(sheet);
            int last_column_int = get_colname_int(last_column);
            if (xlsx_errno == XLSX_GET_LAST_COLUMN_ERRNO_SHEET_NOT_LOADED) {
                fprintf(stderr, "ERROR: xlsx_get_last_column() failed\n");
                break;
            }

            printf("--+-------------------+\n");
            printf("  | A | B | C | D | E |\n");
            printf("--+-------------------+\n");
            for (int row = 1 ;row <= sheet->last_row; row++) {
                printf("%d ", row);

                for (int col = 1; col <= last_column_int; col++) {
                    /* get colname */
                    char colname[4] = {0};
                    int_to_string(col, colname);

                    /* read one cell */
                    xlsx_cell_t cell_data_holder;
                    xlsx_read_cell(sheet, row, colname, &cell_data_holder);

                    switch(cell_data_holder.value_type) {
                    case XLSX_POINTER_TO_CHAR:
                        printf("|%-3s", cell_data_holder.value.pointer_to_char_value);
                        break;
                    case XLSX_INT:
                        printf("|%3d", cell_data_holder.value.int_value);
                        break;
                    case XLSX_LONG_LONG:
                        break;
                    case XLSX_DOUBLE:
                        break;
                    case XLSX_NULL:
                        break;
                    default:
                        break;
                    }
                }
                printf("|\n");
            }
        }
    }

    


    xlsx_close(&wb);
    return 0;
}
