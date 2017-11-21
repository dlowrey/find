# find 

A simplified implementation of the 'find' utility on Linux systems. This utility is used to locate 
files on a Unix or Linux system, searching any set of directories that are specified for files that 
match the supplied search criteria, including:
- search by file name,
- search by last modified,
- search by inode number.

and optionally perform an action once that file has been found. Supported actions so far include:

- default printing the full path,
- deleteing/removing the file (delete, rm),
- output the file contents (cat),
- move/rename the fiel (mv)

## Syntax 

### find where-to-look

Displays the pathnames of all files and directories in the specified directory and all subdirectories. If no
directory is specified, the default is the current working directory.
- `find testdir`
- `find`

### find where-to-look criteria

**find where-to-look -name <specified name>**

This will search the specified directory (where-to-look) and all subdirectories for any
files or directories named <specified name> and display their paths.

- `find testdir -name testfile`
- `find -name testfile`

**find where-to-look -min <specified number of minutes>**

This will find files and directories modified with the specified number of minutes ago
You can specify a number “n” to mean exactly n, “-n” to mean less than n, and “+n”
to mean more than n.

- `find testdir -min +10`
- `find testdir -min -10`
- `find testdir -min 10`
- `find -min +10`

**find where-to-look -inum <specified i-node number>**

Find a file or directory that has i-node number given.

- `find testdir -inum 25`
- `find -inum 25`

### find where-to-look criteria execute

**find where-to-look criteria -exec [delete|rm]**

Find files or directories matching any specified criteria and remove them.

- `find testdir -name testfile -exec delete`
- `find -name dir3 -exec rm`
- `find -name testfile2 -exec rm`

**find where-to-look criteria -exec cat**

Find files matching any specified criteria and output their contents.

- `find testdir -name testfile -exec cat`
- `find -name testfile2 -exec cat`

**find where-to-look criteria -exec mv newfilename**

Find files matching any specified criteria and rename/move them.

- `find testdir -name testfile -exec mv dummyfile`  // moves file named testfile to current directory and renames it dummyfile
- `find -name testfile2 -exec mv testfile21`
