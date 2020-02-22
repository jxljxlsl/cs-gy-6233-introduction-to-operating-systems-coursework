#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_WORD_LENGTH 1024
#define MAX_LINES 256

char buf[MAX_WORD_LENGTH + 1];
char str[MAX_LINES][MAX_WORD_LENGTH + 1];

void print_all_str(char str[MAX_LINES][MAX_WORD_LENGTH + 1], int lines) {
    int i;
    for (i = 0; i < lines; i++) {
        printf(1, "%s\n", str[i]);
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

void to_lowercase(char c[]) {
    int i = 0;

    while (c[i] != '\0') {
        if (c[i] >= 'A' && c[i] <= 'Z') {
            c[i] = (char)(c[i] - 'A' + 'a');
        }
        i++;
    }
}

char is_uppercase(char c) {
    return c >= 'A' && c <= 'Z';
}

void sort_str(int fd,int reverse, int output_file, int number) {

    int lines = 0, i, j, k, n;
    char temp[MAX_WORD_LENGTH + 1];
    char word_in_lowercase_1[MAX_WORD_LENGTH + 1];
    char word_in_lowercase_2[MAX_WORD_LENGTH + 1];

    while ((n = get_line(fd, buf)) > 0) {
        strcpy(str[lines], buf);
        str[lines][n - 1] = '\0';
        lines++;
    }
    
    for (i = 0; i < lines - 1; i++) {
        strcpy(word_in_lowercase_1, str[i]);
        to_lowercase(word_in_lowercase_1);
        for (j = i + 1; j < lines; j++) {
            strcpy(word_in_lowercase_2, str[j]);
            to_lowercase(word_in_lowercase_2);
            if ((n = strcmp(word_in_lowercase_1, word_in_lowercase_2)) > 0) {
                strcpy(temp, str[i]);
                strcpy(str[i], str[j]);
                strcpy(str[j], temp);
                printf(1, "--swap *%s and *%s!diff by %d\n", word_in_lowercase_1, word_in_lowercase_2, n);
            } else {
                printf(1, "--no swap *%s and %s!\n", str[i], str[j]);
            }
            if (strcmp(word_in_lowercase_1, word_in_lowercase_2) == 0) {
                printf(1, "--elif entered\n");
                for (k = 0; str[i][k] == '\0' || str[j][k] == '\0'; k++) {
                    if (is_uppercase(str[i][k]) && !is_uppercase(str[j][k])) {
                        strcpy(temp, str[j]);
                        strcpy(str[j], str[i]);
                        strcpy(str[i], temp);
                        printf(1, "--%s is Uppercase\n", str[i][k]);
                        printf(1, "--swap *%s and *%s!\n", str[i], str[j]);
                        break;
                    }
                }
            }
        }
    }
    
    //--Print all str for testing
    print_all_str(str, lines);
}

void sort_numbers(int fd, int reverse, int output_file, int number) {
    int status=reverse;
    int output[MAX_LINES];
    int index=0;
    int i,j,n;
    int lines = 0;

    while ((n = get_line(fd, buf)) > 0) {
        strcpy(str[lines], buf);
        str[lines][n - 1] = '\0';
        lines++;
    }

    for(i=0; i<lines; i++) {
        int temp=0;
        int flag=-1;
        if(str[i][0]!='-') {
            flag=1;
        }
        for(j=0; str[i][j]!='\0'; j++) {
            if(str[i][j]=='-') {
                continue;
            }
            temp=temp*10+(str[i][j]-'0');
        }
        temp=temp*flag;
        output[index++]=temp;
    }
    for(i=0; i<lines -1; i++) {
        for(j=0; j<lines-i; j++) {
            if(output[j]>output[j+1] && status==0) {
                int temp=output[j];
                output[j]=output[j+1];
                output[j+1]=temp;
            } 
            else if(output[j]<output[j+1] && status==1) {
                int temp=output[j];
                output[j]=output[j+1];
                output[j+1]=temp;
            }
        }
    }
    for(i=0; i<lines; i++) {
        printf(1,"%d\n", output[i]);
    }
}  

int main(int argc, char * argv[]) { 

    int fd, i;
    int reverse, output_file, number=0;

    if(argc <= 1) {
        sort(0, 0, 0, 0);
        exit();
    }
    for(i=1; i<argc; i++) {
        if(strcmp(argv[i], "-r")==0) {
            reverse=1;
        } else if(strcmp(argv[i], "-o")==0) {
            output_file=1;
        } else if(strcmp(argv[i], "-n")==0) {
            number=1;
        } else {
            if((fd = open(argv[i], 0)) < 0){
                printf(1, "sort: cannot open %s\n", argv[i]);
                exit();
            }
        }
    }
    sort(fd, reverse, output_file, number);
    close(fd);
    exit();
}
