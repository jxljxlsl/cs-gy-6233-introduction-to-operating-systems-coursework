#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_WORD_LENGTH 1024
#define MAX_LINES 256

char buf[MAX_WORD_LENGTH + 1];
char words[MAX_LINES][MAX_WORD_LENGTH + 1];
//void print_all_words(char[][] words, int lines);

void print_all_words(char words[MAX_LINES][MAX_WORD_LENGTH + 1], int lines) {
    int i;
    for (i = 0; i < lines; i++) {
        printf(1, "%s\n", words[i]);
    }
}

int getLine(int fd, char* ptr) {
    char c;
    int count = 0;
    if ( fd < 0) {
        exit();
    }
    while ( read(fd, &c, 1) > 0) {
        ptr[count] = c;
        count++;
        if (c == '\n') {
            break;
        }
    }
    ptr[count] = '\0';
    return count;
}


void sort(int fd, char*name) {

    int i = 0, n;
   // char words[MAX_LINES][MAX_WORD_LENGTH + 1];
   // int line = 0;
   // char word[MAX_WORD_LENGTH + 1];

    while ((n = getLine(fd, buf)) > 0) {
        strcpy(words[i], buf);
        words[i][n - 1] = '\0';
        i++;
    }
    //--Read all words into a 2d char array
    /*while ((n = read(fd, buf, sizeof(buf))) > 0) {
        for (i = 0; i < n; i++) {
            if (buf[i] == '\n') {
	        words[line][j] = '\0';
                line++;
                j = 0;
	    } else {
	        words[line][j] = buf[i];
           	j++;
            }
        }
    }*/
    
    //--Print all words for testing
    print_all_words(words, i);
}


int main(int argc, char *argv[]) {
    int fd, i;

    if (argc <= 1) {
        sort(0, "");
        exit();
    }

    for (i = 1; i < argc; i++) {
        if ((fd = open(argv[i], 0)) < 0) {
            printf(1, "sort: cannot open %s\n", argv[i]);
            exit();
        }
        sort(fd, argv[i]);
        close(fd);
    }
    exit();
}
