#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
 
int main(int argc, char *argv[])
{
 if (argc < 2) {
        fprintf(2, "Usage: xargs command ...\n");
        exit(1);
    }
    char *new_argv[MAXARG];
    int cur_argv = 0;
    for (int i = 0; i < argc-1; i++,cur_argv++) {
        new_argv[cur_argv] = argv[i+1];
    }
        char ch;
        char buf[512];
        char *cur_buf = buf;
        new_argv[cur_argv++] = buf;
        while(read(0, &ch, sizeof(char))){
                if(ch == ' '||ch=='\t'){
                        *cur_buf ='\0';
                        cur_buf++;
                        new_argv[cur_argv++] = cur_buf;
                }else if(ch == '\n'){
                        *cur_buf = '\0';
                        new_argv[cur_argv] = 0;
                        int pid = fork();
                        if(pid == 0){
                                exec(new_argv[0], new_argv);
                                exit(0);
                        }else{
                                wait((int*)0);
                                cur_buf = buf;
                                cur_argv = argc;
                        }
                }else{
                        *cur_buf = ch;
                        cur_buf++;
                }
        }
        exit(0);
}
