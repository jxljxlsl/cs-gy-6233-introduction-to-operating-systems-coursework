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

int get_line(int fd, char* ptr) {
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

char to_lowercase(char c) {
    if (c >= 'A' && c <= 'Z') {
        return (char)(c - 'A' + 'a');
    } else {
        return c;
    }
}

char is_uppercase(char c) {
    return c >= 'A' && c <= 'Z';
}

void sort(int fd, char* name) {

    int lines = 0, i, j, n;
    char temp[MAX_WORD_LENGTH + 1];

    while ((n = get_line(fd, buf)) > 0) {
        strcpy(words[lines], buf);
        words[lines][n - 1] = '\0';
        lines++;
    }
    
    
    for (i = 0; i < lines - 1; i++) {
        for (j = i + 1; j < lines; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                strcpy(temp, words[i]);
                strcpy(words[i], words[j]);
                strcpy(words[j], temp);
            } else if (strcmp(words[i], words[j]) == 0) {
            }
        }
    }
    
    //--Print all words for testing
    print_all_words(words, lines);
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
