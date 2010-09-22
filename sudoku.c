/******************************************************************************/
/* Description: Sudoku-Solver for the pc                                      */
/* Date: 15.01.2007                                                           */
/* License: GNU General Public License 2                                      */
/* Florian Adamsky (Florian.Adamsky@iem.fh-friedberg.de)                      */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define FIELD_SIZE 81

enum BOOL {FALSE, TRUE};
int ADVANCED_MODE = FALSE;

/* Zaehlt die Zeichen in einem String. Hierdurch wird die Einbindung von  
   <string.h> unnoetig. */
static int strlen(char *s){char *p=s;while(*p++);return p-s-1;}

static int check_horizontal(unsigned char (*field)[9], int row, int nr) {
    int i;

    for(i=0; i<9; ++i) {
        if (field[row][i] == nr) {
            return TRUE;
        }
    }

    return FALSE;
}

static int check_vertical(unsigned char (*field)[9], int column, int nr) {
    int i;

    for (i=0; i<9; ++i) {
        if (field[i][column] == nr) 
            return TRUE;
    }

    return FALSE;
}

static int is_solved(unsigned char(*field)[9]) {
    int y, x;
    
    for(y=0; y<9; ++y) {
        for(x=0; x<9; ++x) {
            if (field[y][x] == 0)
                return FALSE;
        }
    }
    
    return TRUE;
}

int advanced_algo(unsigned char (*field)[9], int row, int column, int nr) {
    int row_start    = (row / 3) * 3;
    int column_start = (column / 3) * 3;
    int vertical[2], horizontal[2], i, y;

    unsigned char bit[9][9] = {{0,0,0,0,0,0,0,0,0,},
                               {0,0,0,0,0,0,0,0,0,},
                               {0,0,0,0,0,0,0,0,0,},
                               {0,0,0,0,0,0,0,0,0,},
                               {0,0,0,0,0,0,0,0,0,},
                               {0,0,0,0,0,0,0,0,0,},
                               {0,0,0,0,0,0,0,0,0,},
                               {0,0,0,0,0,0,0,0,0,},
                               {0,0,0,0,0,0,0,0,0,}}; 
    
    // Umwandlung in Bitmuster
    for(i=row_start; i<row_start+3; ++i) {
        for(y=column_start; y<column_start+3; ++y) {
            if (field[i][y] != 0)
                bit[i][y] = 1;
        }
    }

    // Herausfinden der 4 Linien
    if (row == 0 || row == 3 || row == 6) {
        vertical[0] = row +1;
        vertical[1] = vertical[0] + 1;
    }
    else if (row == 1 || row == 4 || row == 7) {
        vertical[0] = row + 1;
        vertical[1] = row - 1;
    }
    else {
        vertical[0] = row - 1;
        vertical[1] = vertical[0] - 1;
    }

    if (column == 0 || column == 3 || column == 6) {
        horizontal[0] = column +1;
        horizontal[1] = horizontal[0] + 1;
    }
    else if (column == 1 || column == 4 || column == 7) {
        horizontal[0] = column + 1;
        horizontal[1] = column - 1;
    }
    else {
        horizontal[0] = column - 1;
        horizontal[1] = horizontal[0] - 1;
    }

    for (y=0; y<2; ++y) {
        if ( check_vertical(field, horizontal[y], nr) == TRUE ) {
            for (i=0; i<9; ++i) {
                bit[i][horizontal[y]] = 1;
            }
        }
    }
    
    for (y=0; y<2; ++y) {
        if ( check_horizontal(field, vertical[y], nr) == TRUE ) {
            for (i=0; i<9; ++i) {
                bit[vertical[y]][i] = 1;
            }
        }
    }

    int cnt=0;
    for(i=row_start; i<row_start+3; ++i) {
        for(y=column_start; y<column_start+3; ++y) {
            if (bit[i][y] == 1)
                cnt++;
        }
        
    }

    return (cnt == 8 ? TRUE : FALSE);
}

static void print_field(unsigned char (*field)[9]) {
    char line[] = "|---------------------------------|\0";
    int y, x;

    for(y=0; y<9; ++y) {
        if ((y % 3) == 0) puts(line);
        for(x=0; x<9; ++x) {
            if ((x % 3) == 0) printf("| ");
            printf(" %d ", field[y][x]);
        }
        puts(" |");
    }
    puts(line);
}

static int check_box(unsigned char (*field)[9], int row, int column, int nr) {
    int row_start    = (row / 3) * 3;
    int column_start = (column / 3) * 3;
    int i, y;

    for(i=row_start; i<row_start+3; ++i) {
        for(y=column_start; y<column_start+3; ++y) {
            if (field[i][y] == nr)
                return TRUE;
        }
    }
    
    return FALSE;
}

static void solve(unsigned char (*field)[9]) {
    int y, x, nr, found, number;

    for (y=0; y<9; ++y) {
        for (x=0; x<9; ++x) {

            if(field[y][x] == 0) {

                found=0;
                
                for(nr=1; nr<10; ++nr) {

                    if(check_horizontal(field, y, nr) == FALSE) {
                        if(check_vertical(field, x, nr) == FALSE) {
                            if(check_box(field, y, x, nr) == FALSE) {

                                if(ADVANCED_MODE == TRUE) {
                                    if (advanced_algo(field, y, x, nr) == TRUE){
                                        field[y][x] = nr;
                                        solve(field);
                                    }
                                }

                                found++;
                                number = nr;
                                
                            }
                        }
                    }
                    
                }

                if(found == 1) {
                    field[y][x] = number;
                    solve(field);
                }
                
            }
            
        }
    }
}

static char *open_file(char *filename) {
    FILE *f1;
    static char sudoku[FIELD_SIZE +1];

    if ( (f1 = fopen(filename, "r")) == NULL )
        fprintf(stderr, "Error: Could't open file %s for reading\n", filename);
    else 
        fscanf(f1, "%82c", sudoku);
    
    fclose(f1);
    sudoku[82] = '\0';

    return sudoku;
}

static int getopt(char *argument, char *option) {

    if(argument[0] == '-' && argument[1] == option[0])
        return TRUE;

    return FALSE;
}

static void wrong_option(char *prog_name, char *error_msg) {

    if (error_msg)
        fprintf(stderr, "Error: %s\n", error_msg);
    
    printf("Usage: %s [OPTION] ...\n", prog_name);
    puts("  -r [Field]    raw data input (for testing purpose only)");
    puts("  -f [File]     reads a file in");
    puts("  -h            display this help"); 

    exit (error_msg ? EXIT_FAILURE : EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    char *ptr;
    
    if (argc != 3)
        wrong_option(argv[0], "Falsche Paramenteranzahl");
    
    if(getopt(argv[1], "r") == TRUE) {

        if(strlen(argv[2]) == FIELD_SIZE)
            ptr = argv[2];
        else
            wrong_option(argv[0], "Falsche Feld-Groesse");

    }
    else if (getopt(argv[1], "f") == TRUE) {
        ptr = open_file(argv[2]);
    }

    // Umwandlung des linearen Feldes in ein mehrdimensionales Array
    int y=0, x=0;
    unsigned char multi_dim_field[9][9];
    for(; *ptr; ptr++) {
        multi_dim_field[y][x] = *ptr-'0';
        x++;
        if ((x % 9) == 0) {
            y++; x=0;
        }
    }

    printf("\nSudoku\n");
    print_field(multi_dim_field);

    solve(multi_dim_field);
    printf("\nRegel-Algorithmus\n"); 
    print_field(multi_dim_field);

    if ( is_solved(multi_dim_field) == FALSE ) {
        ADVANCED_MODE = TRUE;
        solve(multi_dim_field);
        printf("\nAdvanced-Algorithmus\n");
        print_field(multi_dim_field);
    }
    
    exit(EXIT_SUCCESS);

    return 0;
}
