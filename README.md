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

Displays the pathnames of all files in the specified directory and all subdirectories. If no
directory is specified, the default is the current working directory.
- `find -w testdir`
- `find -w`
- `find`

### find where-to-look criteria

**find where-to-look -n <specified name>**

This will search the specified directory (where-to-look) and all subdirectories for any
files named <specified name> and display their pathnames.

- `find -w testdir -n testfile`
- `find -n testfile`

**find where-to-look -m <specified number of minutes>**

This will find those files modified with the specified number of minutes ago
You can specify a number “n” to mean exactly n, “-n” to mean less than n, and “+n”
to mean more than n.

- `find -w testdir -m +10`
- `find -w testdir -m -10`
- `find -w testdir -m 10`
- `find -m +10`

**find where-to-look -i <specified i-node number>**

Find a file that has i-node number given.

- `find -w testdir -i 25`
- `find -i 25`

### find where-to-look criteria action

**find where-to-look criteria -a delete**

Find files matching any specified criteria and delete them.

- `find testdir -n testfile -a delete`
- `find -n testfile2 -a delete`

**find where-to-look criteria -a rm**

Find files matching any specified criteria and remove them.

- `find testdir -n testfile -a rm`
- `find -n testfile2 -a rm`

**find where-to-look criteria -a cat**

Find files matching any specified criteria and output their contents.

- `find testdir -n testfile -a cat`
- `find -n testfile2 -a cat`

**find where-to-look criteria -a mv newfilename**

Find files matching any specified criteria and rename them to a specified name.

- `find testdir -n testfile -a mv dummyfile`
- `find -n testfile2 -a mv testfile21`
