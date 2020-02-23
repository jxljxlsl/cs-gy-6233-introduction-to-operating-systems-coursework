#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define MAX_WORD_LENGTH 1024
#define MAX_LINES 256

char buf[MAX_WORD_LENGTH];
char str[MAX_LINES][MAX_WORD_LENGTH];
char temp[MAX_WORD_LENGTH];
char s[MAX_WORD_LENGTH];
int lines = 0;

void print_all_str() {
    int i;
    for (i = 0; i < lines; i++) {
        printf(1, "%s\n", str[i]);
    }
}

int get_line(int fd_in, char* ptr) {
    char c;
    int count = 0;
    if (fd_in < 0) {
        exit();
    }
    while (read(fd_in, &c, 1) > 0) {
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

void write_str_into_file(int fd_out) {
   
    int i;

    for (i = 0; i < lines; i++) {
        str[i][strlen(str[i])] = '\n';
        write(fd_out, str[i], strlen(str[i]));
    }
}

void write_number_into_file(int fd_out, int numbers[]) {

    int i = 0, j, neg;

    for (i = 0; i < lines; i++) {
        int number = numbers[i];
        int digit = 0;
        if (number < 0) {
            neg = 1;
            number = number * -1;
        } else {
            neg = 0;
        }
        if (number == 0) {
            s[0] = '0';
            digit = 1;
        }
        while (number != 0) {
            temp[digit] =(char)((number % 10) + '0');
            number = number / 10;
            digit++;
        }
        
        if (neg == 1) {
            s[0] = '-';
            digit++;
            for (j = 1; j < digit; j++) {
                s[j] = temp[digit - 1 - j];
            }
        } else  {
            for (j = 0; j < digit; j++) {
                s[j] = temp[digit- 1 - j];
            }
        }
        s[digit] = '\n';
        s[digit + 1] = '\0';
    
        write(fd_out, s, strlen(s));
    }
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

void sort_str(int fd_in,int flag_r, int flag_o, int flag_n, int fd_out) {

    int i, j, n;
    lines = 0;

    while ((n = get_line(fd_in, buf)) > 0) {
        strcpy(str[lines], buf);
        str[lines][n - 1] = '\0';
        lines++;
    }

    for (i = 0; i < lines - 1; i++) {
        for (j = i + 1; j < lines; j++) {
            if (flag_r == 0) {
                if (string_compare(str[i], str[j]) > 0) {
                    strcpy(temp, str[i]);
                    strcpy(str[i], str[j]);
                    strcpy(str[j], temp);
                }
            } else {
                // sorting based on flag_r for -r
                if (string_compare(str[i], str[j]) < 0) {
                    strcpy(temp, str[i]);
                    strcpy(str[i], str[j]);
                    strcpy(str[j], temp);
                }
            }
        }
    }
    
    // printing or writing into file based on flag_o for -o
    if (flag_o == 0) {
        print_all_str();
    } else {
        write_str_into_file(fd_out);
    }
}

void sort_numbers(int fd_in, int flag_r, int flag_o, int flag_n, int fd_out) {
    int status=flag_r;
    int output[MAX_LINES];
    int index=0;
    int i,j,n;
    lines = 0;

    while ((n = get_line(fd_in, buf)) > 0) {
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

    if (flag_o == 0) {
        for(i=0; i<lines; i++) {
            printf(1,"%d\n", output[i]);
        }
    } else {
        write_number_into_file(fd_out, output);
    }
}  

void sort(int fd_in, int flag_r, int flag_o, int flag_n, int fd_out) {

    if(flag_n == 0) {
        sort_str(fd_in, flag_r, flag_o, flag_n, fd_out);
    } else {
        sort_numbers(fd_in, flag_r, flag_o, flag_n, fd_out);
    }
}

int main(int argc, char * argv[]) { 

    int fd_in = -1, fd_out = -1, i;
    int flag_r = 0, flag_o = 0 , flag_n = 0;

    for( i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            flag_r = 1;
            continue;
        }
        if (strcmp(argv[i], "-n") == 0) {
            flag_n = 1;
            continue;
        }
        if (strcmp(argv[i], "-o") == 0) {
            // immediately read the output file name after -o
            fd_out = open(argv[++i], O_CREATE | O_RDWR);
            if (fd_out < 0) {
                printf(1, "sort: cannot create or modify output file <%s>\n", argv[i]);
                exit();
            }
            flag_o = 1;
            continue;
        }
        if ((fd_in = open(argv[i], 0)) < O_RDONLY) {
            printf(1, "sort: cannot read input file <%s>\n", argv[i]);
            exit();
        }
    }

    if (fd_in == -1) {
        fd_in = 0;
    }
    sort(fd_in, flag_r, flag_o, flag_n, fd_out);

    close(fd_in);
    if (flag_o == 1) {
        close(fd_out);
    }
    exit();
}
