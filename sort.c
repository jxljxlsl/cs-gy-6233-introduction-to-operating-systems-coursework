#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

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

void write_into_file(char* file_name, int lines) {
    int fd = open(file_name, O_CREATE, O_RDWR);
    int i;

    for (i = 0; i < lines; i++) {
        str[i][strlen(str[i])] = '\n';
        write(fd, str[i], strlen(str[i]));
    }
    close(fd);
    printf(1, "--Written!");
}

int string_compare(char* str1_ptr, char* str2_ptr) {
    char* i1 = str1_ptr;
    char* i2 = str2_ptr;
    char* j1 = str1_ptr;
    char* j2 = str2_ptr;

    while (to_lowercase(*i1) == to_lowercase(*i2) && *i1 != '\0' && *i2 != '\0') {
        i1++;
        i2++;
    }

    if (*i1 == '\0' && *i2 == '\0') {
        while (*j1 == *j2 && j1 != '\0' && j2 != '\0') {
            j1++;
            j2++;
        }
        if (*j1 == '\0') {
            return -1;
        }
        if (*j2 == '\0') {
            return 1;
        }
        int u1 = is_uppercase(*j1) != 0, u2 = is_uppercase(*j2) != 0;
        return to_lowercase(*j1) == to_lowercase(*j2) ? u1 - u2 : to_lowercase(*j1) - to_lowercase(*j2);
    } else if (*i1 == '\0') {
        return -1;
    } else if (*i2 == '\0') {
        return 1;
    }

    int u1 = is_uppercase(*str1_ptr) != 0, u2 = is_uppercase(*str2_ptr) !=0;
    return to_lowercase(*i1) == to_lowercase(*i2) ? u1 - u2 : to_lowercase(*i1) - to_lowercase(*i2);
}

void sort_str(int fd,int reverse, int output_file, int number, char* file_name) {

    int lines = 0, i, j, n;
    char temp[MAX_WORD_LENGTH + 1];

    while ((n = get_line(fd, buf)) > 0) {
        strcpy(str[lines], buf);
        str[lines][n - 1] = '\0';
        lines++;
    }
    
    for (i = 0; i < lines - 1; i++) {
        for (j = i + 1; j < lines; j++) {
            if (reverse == 0) {
                if (string_compare(str[i], str[j]) > 0) {
                    strcpy(temp, str[i]);
                    strcpy(str[i], str[j]);
                    strcpy(str[j], temp);
                }
            } else {
                if (string_compare(str[i], str[j]) < 0) {
                    strcpy(temp, str[i]);
                    strcpy(str[i], str[j]);
                    strcpy(str[j], temp);
                }
            }
        }
    }
    
    if (output_file == 0) {
        print_all_str(str, lines);
    } else {
        write_into_file(file_name, lines);
    }
}

void sort_numbers(int fd, int reverse, int output_file, int number, char* fi) {
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

    printf(1, "--before\n");
    print_all_str(str, lines);



    printf(1, "--number of lines: %d\n", lines);
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
    printf(1,"--converted to numbers\n");
    for(i=0; i<lines; i++) {
        printf(1,"%d\n", output[i]);
    }
    printf(1, "----------result\n");
    for(i=0; i<lines -1; i++) {
        for(j=0; j<lines - 1 - i; j++) {
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
    if (output_file == 0) {
        for(i=0; i<lines; i++) {
            printf(1,"%d\n", output[i]);
        }
    } else {
        write_into_file(output, lines);
    }
}  

int main(int argc, char * argv[]) { 

    int fd, i;
    int reverse, output_file, number=0;

    if(argc <= 1) {
        sort_numbers(0, 0, 0, 0);
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
    if (number == 1) {
        sort_numbers(fd, reverse, output_file, number);
    } else {
        sort_str(fd, reverse, output_file, number);
    }

    close(fd);
    exit();
}
