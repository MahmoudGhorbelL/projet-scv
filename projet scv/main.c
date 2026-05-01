#include <stdio.h>
#include <stdlib.h>

int hashFile(char* source, char* dest)
{
    char cmd[500];

    sprintf(cmd,
            "certutil -hashfile \"%s\" SHA256 > \"%s\"",
            source,
            dest);

    return system(cmd);
}

int main()
{
system("echo Test $>$ test.txt");
}
