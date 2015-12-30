#include <iostream>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>

using namespace std;

int makedirs(char * path, mode_t mode) {
    struct stat st = {0};

    if (stat(path, &st) == 0) {
        if (S_ISDIR(st.st_mode) == 0) {
            return -1;
        }
        return 0;
    }

    char subpath[512] = "";
    char * delim = strrchr(path, '/');
    if (delim != NULL) {
        strncat(subpath, path, delim - path);
        makedirs(subpath, mode);
    }
    if (mkdir(path, mode) != 0) {
        return -1;
    }
    return 0;
}

int main(int argc, char** argv) {
    string oldpath, newpath;
    char buf[512];
    int processed = 0;
    while (cin >> oldpath >> newpath) {
        struct stat st = {0};
        strncpy(buf, newpath.c_str(), sizeof(buf));
        char * dir = dirname(buf);
        makedirs(dir, 0775);
        if (rename(oldpath.c_str(), newpath.c_str()) == -1) {
            int errsv = errno;
            cerr << errsv << ": " << oldpath << " " << newpath << endl;
        }
        ++processed;
        if (processed % 1000 == 0) {
            cout << "Processed " << processed << " files." << endl;
        }
    }
    if (processed % 1000 != 0) {
        cout << "Processed " << processed << " files." << endl;
    }
    return 0;
}
