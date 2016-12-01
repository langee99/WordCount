//
//  main.cpp
//  Ass2
//
//  Created by Eric Lange on 6/16/16.

#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <list>
#include <String>

void print(int lflag, int lines, int wflag, int words, int cflag, int chars, std::string filename = "total");

int main(int argc, char **argv) {
    
    int wflag = 0;
    int lflag = 0;
    int cflag = 0;
    
    int arg = 0;
    
    // So I can use my own warning
    opterr = 0;
    
    // Setting which options where chosen
    while ((arg = getopt(argc, argv, "wlc")) != -1) {
        switch (arg) {
            case 'w':
                wflag = 1;
                break;
            case 'l':
                lflag = 1;
                break;
            case 'c':
                cflag = 1;
                break;
            case '?':
                printf("USAGE: %s [-wlc] [file ...]\n", argv[0]);
                exit(-1);
                break;
            default:
                break;
        }
    }
    
    int words = 0;
    int chars = 0;
    int lines = 0;
    int space = 0; // 0 if previous char was a char; 1 if previous char was a space 
    
    char c;
    
    std::list<std::string> files;
    
    for (int index = optind; index < argc; index++) {
        files.push_back(argv[index]);
    }
    
    // If no files were given read from stdin
    // Else read from the file(s)
    if(files.empty()) {
        while ((c = getchar()) != EOF) {
            // If its a space and the previous char a letter
            if (!isgraph(c) && !space){
                space = 1;
                chars++;
                words++;
                
                // New line check
                if (c == '\n')
                    lines++;
            }
            // Else if it's any printable char and had a space before it
            else if ((isgraph(c) && space)) {
                chars++;
                space = 0;
            } else {
                chars++;
                
                // New line check
                if (c == '\n')
                    lines++;
            }
            
        }
        
        print(lflag, lines, wflag, words, cflag, chars, "");
        
        
    } else {
        
        int wTotal = 0;
        int cTotal = 0;
        int lTotal = 0;
        
        FILE *fp = NULL;
        std::list<std::string>::iterator iter;
        
        
        // Only runs if a file is given
        for (iter = files.begin(); iter != files.end(); iter++) {
            
            if ((fp = fopen(iter->c_str(), "r")) == NULL) { perror(iter->c_str()); continue; }
            words = 0;
            chars = 0;
            lines = 0;
            
            
            while ((c = fgetc(fp)) != EOF) {
                
                // If its a space and the previous char wasn't a space
                if (!isgraph(c) && !space){
                    space = 1;
                    chars++;
                    words++;
                    
                    // New line check
                    if (c == '\n')
                        lines++;
                }
                // Else if it's any printable char and had a space before it
                else if ((isgraph(c) && space)) {
                    chars++;
                    space = 0;
                } else {
                    chars++;
                    
                    // New line check
                    if (c == '\n')
                        lines++;
                }
                
            }
            
            print(lflag, lines, wflag, words, cflag, chars, iter->c_str());
            
            wTotal += words;
            lTotal += lines;
            cTotal += chars;
            
            fclose(fp);
        }
        
        
        // Displays totals only if there is more than one file given
        if (files.size() > 1)
            print(lflag, lTotal, wflag, wTotal, cflag, cTotal);
    }
    
    return 0;
}

void print(int lflag, int lines, int wflag, int words, int cflag, int chars, std::string filename) {
    
    if ((wflag && lflag && cflag) || (!wflag && !lflag && !cflag)) {
        
        printf("%8d%8d%8d %s\n", lines, words, chars, filename.c_str());
        
    }
    else if (wflag && lflag){
        
        printf("%8d%8d %s\n", lines, words, filename.c_str());
        
    }
    else if (wflag && cflag) {
        
        printf("%8d%8d %s\n", words, chars, filename.c_str());
        
    }
    else if (lflag && cflag) {
        
        printf("%8d%8d %s\n", lines, chars, filename.c_str());
        
    }
    else if (wflag) {
        
        printf("%8d %s\n", words, filename.c_str());
        
    }
    else if (lflag) {
        
        printf("%8d %s\n", lines, filename.c_str());
        
    }
    else if (cflag) {
        
        printf("%8d %s\n", chars, filename.c_str());
        
    }
    
}

