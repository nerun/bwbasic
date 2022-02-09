/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  unixio - Unix I/O functions written in terms of standard C       */
/*  functions.                                                       */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>

#ifdef fileno
#undef fileno
#endif

#include "unixio.h"

static FILE *files[FOPEN_MAX];
/*char *environ[] = { NULL };*/

int open(const char *fnm, int mode, ...)
{
    int x;
    char *modestr;

    for (x = 3; x < FOPEN_MAX; x++)
    {
        if (files[x] == NULL)
        {
            break;
        }
    }
    if (x == FOPEN_MAX)
    {
        return (-1);
    }
    if (mode == O_RDONLY)
    {
        modestr = "r";
    }
    else if (mode == O_WRONLY)
    {
        modestr = "w";
    }
    else if (mode == O_RDWR)
    {
        modestr = "r+";
    }
    files[x] = fopen(fnm, modestr);
    if (files[x] == NULL)
    {
        return (-1);
    }
    return (x);
}

int read(int fno, void *buf, size_t bytes)
{
    size_t rb;

    if (fno < 3)
    {
        rb = fread(buf, 1, bytes, stdin);
    }
    else
    {
        rb = fread(buf, 1, bytes, files[fno]);
    }
    return ((int)rb);
}

int write(int fno, const void *buf, size_t bytes)
{
    size_t wb = 0;

    if (fno == 1)
    {
        wb = fwrite(buf, 1, bytes, stdout);
    }
    else if (fno == 2)
    {
        wb = fwrite(buf, 1, bytes, stderr);
    }
    else if (fno > 2)
    {
        wb = fwrite(buf, 1, bytes, files[fno]);
    }
    return ((int)wb);
}

int close(int fno)
{
    if (fno >= 3)
    {
        fclose(files[fno]);
        files[fno] = NULL;
    }
    return (0);
}

char *getcwd(char *buf, int len)
{
    if (len != 0)
    {
        *buf = '\0';
    }
    return (buf);
}

void unlink(char *f)
{
    remove(f);
    return;
}

int stat(char *f, struct stat *buf)
{
    memset(buf, '\0', sizeof *buf);
    return (0);
}

int fileno(FILE *fp)
{
    return (0);
}

int access(char *f, int n)
{
    return (1);
}

int fstat(int fh, struct stat *buf)
{
    memset(buf, '\0', sizeof *buf);
    return (0);
}

int pwait(int a, int *b, int c)
{
    return (0);
}

int putenv(char *x)
{
    return (0);
}

char *mktemp(char *s)
{
    return (tmpnam(s));
}

int chdir(char *path)
{
    return (0);
}

int rmdir(char *path)
{
    return (0);
}

int mkdir(char *path, int permissions)
{
    return (0);
}
