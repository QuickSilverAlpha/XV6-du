#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

void du(char *path, int isBlock, int thresh) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    uint folderSize = 0;
    ////////////////////////////////////////////////////////
    if ((fd = open(path, 0)) < 0) {
        printf(1, "check usage.\n");
        exit();
    }

    if (fstat(fd, &st) < 0) {
        printf(1, "check usage.\n");
        close(fd);
        exit();
    }
    if (thresh == -1) {
        printf(1, "check usage.\n");
        exit();
    }

    ////////////////////////////////////////////////////////
    switch (st.type) {
    case T_FILE:
        strcpy(buf, path);
        if (thresh != 0) {
            if (st.size > thresh) {
                if(isBlock == 0) {
                    printf(1, "%d %s\n", st.size, buf);
                    folderSize += st.size;
                }
                else {
                    int rem = st.size % 512;
                    int blockSize = st.size / 512;
                    if (rem != 0) { 
                        blockSize++;
                    }
                    folderSize += blockSize; 
                    printf(1, "%d %s\n", blockSize, buf);
                }            
            }
        }
        else if(isBlock == 1){
            int rem = st.size % 512;
            int blockSize = st.size / 512;
            if (rem != 0) { 
                blockSize++;
            }
            folderSize += blockSize; 
            printf(1, "%d %s\n", blockSize, buf);

        }
        else {
            printf(1, "%d %s\n", st.size, buf);
        }
        printf(1, "%d %s\n", st.size, buf);
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf(1, "check usage.\n");
            exit();
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0) {
                //printf(1, "ls: cannot stat %s\n", buf);
                continue;
            }
            if (st.type == T_FILE) {
                if(thresh != 0) {
                    if(st.size < thresh) { 
                        continue;
                    }
                }
                if(isBlock == 0) {
                    printf(1, "%d %s\n", st.size, buf);
                    folderSize += st.size;
                
                }
                else {
                    int rem = st.size % 512;
                    int blockSize = st.size / 512;
                    if (rem != 0) { 
                        blockSize++;
                    }
                    printf(1, "%d %s\n", blockSize, buf);
                    folderSize += blockSize;
                
                }
            } 
        }
        printf(1, "%d %s\n", folderSize, path); 
        break;
    }
    close(fd);
}
int checkChar(char *text)
{
    int j;
    j = strlen(text);
    while(j--)
    {
        if(text[j] >= '0' && text[j] <= '9')
            continue;

        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    char path[512] = "."; 
    int isBlock = 0; //-k flag
    int thresh = -1; //-t flag
    
    //no args
    if (argc < 2) {
        du(".", 0, 0);
        exit();
    }
    // -k or path 
    else if(argc == 2) {
        if(strcmp(argv[1], "-k") == 0) {
            isBlock = 1;
            thresh = 0; 
            du(".", isBlock, thresh);
            exit();
        }
        
        du(argv[1], 0, 0); //incorrect path handling covered in du function itself, not here
        exit();
    }
    else if(argc == 3) {
        if(strcmp(argv[1],"-k") == 0) {
            isBlock = 1;
            thresh = 0; 
            strcpy(path, argv[2]);
        }
        
        else if(strcmp(argv[1],"-t") == 0) { 
            int check = checkChar(argv[2]);
            if(check == 0) {
                printf(1, "check usage.\n");
                exit();
            }
            thresh = atoi(argv[2]);
            if(thresh >= 0) {}
            else {
                printf(1, "check usage.\n");
                exit();    
            }
        }
            
        else {
            printf(1, "check usage.\n");
            exit();
        }
        du(path, isBlock, thresh);
        exit(); 
        /*
        int i;
        for (i = 1; i < argc; i++) {
            if(strcmp(argv[i],"-k") == 0) {
                //printf(1, "Displaying file size in blocks... \n");
                isBlock = 1; 
                strcpy(path, argv[argc-1]);
            }
            else if(strcmp(argv[i],"-t") == 0) {
                i++; 
                thresh = atoi(argv[i]);
                if(thresh > 0) {}
                else {
                    printf(1, "check usage.\n");
                    exit();
                }
            }
            else {
                printf(1, "check usage.\n");
                exit();
            }
        }
        */
    }       
    else if(argc == 4) { 
        //case where 'du -k -t <num>  
        if(strcmp(argv[1],"-k") == 0) {
                isBlock = 1;
            if(strcmp(argv[2],"-t") == 0){
                int check = checkChar(argv[3]);
                if(check == 0) {
                    printf(1, "check usage.\n");
                    exit();
                }
                thresh = atoi(argv[3]);
                if(thresh >= 0) {}
                else {
                    printf(1, "check usage.\n");
                    exit();
                }
            }
        }
        else if(strcmp(argv[1],"-t") == 0) {
            int check = checkChar(argv[2]);
            if(check == 0) {
                printf(1, "check usage.\n");
                exit();
            }
            thresh = atoi(argv[2]);
                if(thresh >= 0) { 
                    if(strcmp(argv[3],"-k") == 0) {
                        isBlock = 1;
                    }
                    else {
                         strcpy(path, argv[3]);
                         
                    }
                }
                else {
                    printf(1, "check usage.\n");
                    exit();
                }

        }
        du(path, isBlock, thresh); 
        exit(); 
    }
    else if(argc == 5) {
        if(strcmp(argv[1],"-k") == 0) {
                isBlock = 1; 
            if(strcmp(argv[2],"-t") == 0){
                int check = checkChar(argv[3]);
                if(check == 0) {
                    printf(1, "check usage.\n");
                    exit();
                }                
                thresh = atoi(argv[3]);
                if(thresh >= 0) {
                    strcpy(path, argv[4]);
                }
                else {
                    printf(1, "check usage.\n");
                    exit();
                }
            }
            else {
                printf(1, "check usage.\n");
                exit();
            } 
        }
        else if(strcmp(argv[1],"-t") == 0) {
            int check = checkChar(argv[2]);
            if(check == 0) {
                printf(1, "check usage.\n");
                exit();
            }            
            thresh = atoi(argv[2]);
            if(thresh > 0) { 
                if(strcmp(argv[3],"-k") == 0) {
                    isBlock = 1;
                    strcpy(path, argv[4]);
                }
                else {
                    printf(1, "check usage.\n");
                    exit();
                }              
            }
            else {
                printf(1, "check usage.\n");
                exit();
            }    
        }
        else {
            printf(1, "check usage.\n");
            exit();
        }
        du(path, isBlock, thresh);
        exit(); 
        /*
        int i;
        for (i = 1; i < argc; i++) {
            if(strcmp(argv[i],"-k") == 0) {
                //printf(1, "Displaying file size in blocks... \n");
                isBlock = 1; 
                strcpy(path, argv[argc-1]);
            }
            else if(strcmp(argv[i],"-t") == 0) {
                i++; 
                thresh = atoi(argv[i]);
                if(thresh > 0) { 
                    //printf(1, "Printing files with byte size > %d ... \n" , thresh);
                }
                else {
                    printf(1, "check usage.\n");
                    exit();
                }
            }
            else if(i == (argc-1)){
                strcpy(path, argv[i]); 
            }
            else {
                printf(1, "check usage.\n");
                exit();                 
            }   
        }
        */  
    }
    else {
        printf(1, "check usage.\n");
        exit();
    }

    printf(1, "You shouldn't be here..\n");
    exit();
      
}