#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define MAX_WORD_LENGTH 1024
#define MAX_LINES 256

char buf[MAX_WORD_LENGTH];
char str[MAX_LINES][MAX_WORD_LENGTH];

void print_all_str(char str[MAX_LINES][MAX_WORD_LENGTH], int lines) {
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

void write_str_into_file(int fd, char str[MAX_LINES][MAX_WORD_LENGTH], int lines) {
    // int fd1 = open(fd, O_CREATE | O_RDWR);
    printf(1, "fd:%d\n", fd);
    int i;
    int success;

    for (i = 0; i < lines; i++) {
        str[i][strlen(str[i])] = '\n';
        printf(1, "output:%s", str[i]);
        success=write(fd, str[i], strlen(str[i]));
        printf(1, "this time: %d\n", success);
    }
    // close(fd);
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

void sort_str(int fd,int reverse, int output_file, int number, int fd1) {

    int lines = 0, i, j, n;
    char temp[MAX_WORD_LENGTH];

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
        write_str_into_file(fd1, str, lines);
    }
}

void sort_numbers(int fd, int reverse, int output_file, int number, int fd1) {
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
        // write_number_into_file(fd1, output, lines);
    }
}  

void sort(int fd, int reverse, int output_file, int number, int fd1) {
    if(number==0) {
        sort_str(fd, reverse, output_file, number, fd1);
    } else {
        sort_numbers(fd, reverse, output_file, number, fd1);
    }
}

int main(int argc, char * argv[]) { 

    int fd, i;
    int reverse, output_file, number=0;
    int two_files=0;
    int fd1;

    if(argc <= 1) {
        sort(0, 0, 0, 0, 0);
        exit();
    }
    for(i=1; i<argc; i++) {
        if(strcmp(argv[i], "-r")==0) {
            reverse=1;
            printf(1, "reverse: %d\n", reverse);
        } else if(strcmp(argv[i], "-o")==0) {
            output_file=1;
            printf(1, "output_file: %d\n", output_file);
        } else if(strcmp(argv[i], "-n")==0) {
            number=1;
            printf(1, "number: %d\n", number);
        } else {
            if(two_files==0 && (fd = open(argv[i], 0)) < 0){
                printf(1, "sort: cannot open %s\n", argv[i]);
                printf(1, "%d, %d\n", two_files, fd);
                exit();
            } 
            if(two_files==0 && (fd >= 0)) {
                two_files=1;
                continue;
            } 

            if(two_files == 1 && (fd1=open(argv[i], O_CREATE | O_RDWR)) < 0) {
                printf(1, "%d, %d\n", two_files, fd1);
                printf(1, "sort: cannot open output file %s\n", argv[i]);
                exit();
            }
        }
    }
    // fd==0: open file successfully; <0:fail; >0 initialize
    sort(fd, reverse, output_file, number, fd1);
    close(fd);
    close(fd1);
    exit();
}
