
#include <stdio.h> 
#include <stdlib.h>  // required for pre-c89 compilers see https://stackoverflow.com/questions/15418262/troubling-converting-string-to-long-long-in-c
#include <getopt.h>  // getopt
#include <dirent.h>  // directory functions
#include <string.h>  // strcat, strcmp
#include <libgen.h>  // header files for dirname()
#include <sys/stat.h> // file stats
#include <time.h>  // time

/*
* Perform an action designated in 'action' on the file
* given in 'fullpath'.
* If no action is given, a default action of printing out 'fullpath'
* is taken.
*/
void do_action(char * fullpath, char * action, char ** remaining) {
    // available actions
    char * delete = "delete";
    char * cat = "cat";
    char * rm = "rm";
    char * mv = "mv";

    if (action == NULL) {
        // no action, print full path
        printf("%s\n", fullpath);
    } else if (strcmp(action, delete) == 0 || strcmp(action, rm) == 0) {
        // delete file at full path recursivley in case it is a directory.
        char * argv[4] = {rm, "-r", fullpath, NULL};
        execvp(rm, argv);
    } else if (strcmp(action, cat) == 0) {
        // perform cat on full path
        char * argv[3] = {cat, fullpath, NULL};
        execvp(cat, argv);
    } else if (strcmp(action, mv) == 0) {
        // perform mv on found file
        char * argv[4] = {mv, fullpath, remaining[0], NULL};
        execvp(mv, argv);
    }
}

/*
* Check if the file at 'fulllpath' meets any given criteria.
* return 1 (true) if it does, 0 (false) otherwise.
*/
int meets_criteria(char * fullpath, char * entry_name, char * name, char * mmin, char * inum) {

    int criteria_met = 0;
    struct stat filestats;

    if (name != NULL) {
        // if the entry name matches the critera name
        if(strcmp(entry_name, name) == 0) {
            criteria_met = 1;
        }
    } else if (mmin != NULL) {
        stat(fullpath, &filestats);
        char modifier;
        int minutes;
        int lastmodified;
        // check for '+' or '-' and convert
        // time given in minutes to minutes
        if (!isdigit(mmin[0])) {
            modifier = mmin[0];
            minutes = atoi(++mmin);
        } else {
            minutes = atoi(mmin);
        }
        lastmodified = (int)(time(0) - filestats.st_mtime) / 60;
        // greater than 
        if (modifier == '+' && lastmodified > minutes) {
            criteria_met = 1;
        // less than
        } else if ( modifier == '-' && lastmodified < minutes) {
            criteria_met = 1;
        // equal (exactly)
        } else if (lastmodified == minutes) {
            criteria_met = 1;            
        }
    } else if (inum != NULL) {
        stat(fullpath, &filestats);
        // file's inode number equals input inode number
        // cast/convert to long long to support 32bit architectures
        if ((long long)filestats.st_ino == atoll(inum)) {
            criteria_met = 1;            
        }
    } else {
        criteria_met = 1;
    }

    return criteria_met;
}


/*
* find files that meet critera if any is given, and perform an action on
* those files if any action is given. 
* Criteria available:
    * default none
    * match file name 
    * file modified greater than, less than, or exactly at specified time
    * file inode matches given inode

* Actions available:
    * default print full path of file/directory
    * delete file
    * remove file
    * cat file
    * mv file
*/
void find(char * where, char * name, char * mmin, char * inum, char * action, char ** remaining)  {
    DIR * dir;// directory stream
    struct dirent * entry;     
    char * e_name; 
    char fullpath[1024];
    char curr_dir[] = ".";
    char pre_dir[] = "..";
    int e_type;    
    int is_dots;  
    int criteria = name != NULL || mmin != NULL || inum != NULL;

    if (dir = opendir(where)) {

        while ((entry = readdir(dir)) != NULL) {
            e_type = entry -> d_type;
            e_name = entry -> d_name;
            is_dots = strcmp(e_name, curr_dir) == 0 || strcmp(e_name, pre_dir) == 0;
            
            // concat for full path
            snprintf(fullpath, sizeof(fullpath),
            "%s/%s", where, e_name);
    
            // if the entry is a sub directory and is not "." or ".."
            if (!is_dots) {
                
                // perform action on entry
                if (meets_criteria(fullpath, e_name, name, mmin, inum)) {
                    // perform some action on file
                    do_action(fullpath, action, remaining);
                }

                // if entry is directory, recursivly call find for entry
                if (e_type == DT_DIR) {
                    // recursive for directory
                    find(fullpath, name, mmin, inum, action, remaining);
                }
    
            } 
  
        }
    
        // close directory stream
        closedir(dir);  
    } else {
        // error opening diretory stream
        printf("Could not open directory %s\n", where);
    }
}


int main (int argc, char ** argv)
{
    // valid options given for find
    // set to 1 (true) initially because the user can
    // run ./find without any args.
    int valid = 1;

    // criteria variables
    char *where = NULL;
    char *name = NULL;
    char *mmin = NULL;
    char *inum = NULL;
    // action variables
    char *action = NULL;
    char *data = NULL;
    // sepcified option variable
    char c;

    static struct option long_options[] = {
        {"name", required_argument, NULL, 'n'},
        {"min", required_argument, NULL, 'm'},
        {"inum", required_argument, NULL, 'i'},
        {"exec", required_argument, NULL, 'e'},
        {NULL, 0, NULL, 0}
    };

    // set where-to-look
    if (argc > 1 && argv[1][0] != '-') {
        where = argv[1];
        argc--;
        argv++;
    } else {
        where = ".";
    }

    while ((c = getopt_long_only(argc, argv, "w:n:m:i:e:", long_options, NULL)) != -1) {

        switch (c) {
            case 'n':
                name = optarg;
                break;
            case 'm':
                mmin = optarg;
                break;
            case 'i':
                inum = optarg;
                break;
            case 'e':
                action = optarg;
                break;
            case '?':
                // invalid arg
                valid = 0;
                break;
            default:
                // invalid
                valid = 0;
                break;
        }
    }

    // adjust after processing of arguments
    argc -= optind;
    argv += optind;

    if (valid) {
        find(where, name, mmin, inum, action, argv);        
    }

    return 0;

}