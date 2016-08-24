#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <dirent.h>

#include <sstream>

#include "fs.hpp"

namespace fs {
    int make_dir(const char * dir, mode_t mode) {
        struct stat st = {0};

        if (stat(dir, &st) == 0) {
            if (S_ISDIR(st.st_mode) == 0) {
                return -1;
            }
            return 0;
        }

        char subpath[512] = "";
        char * delim = strrchr(dir, '/');
        if (delim != NULL) {
            strncat(subpath, dir, delim - dir);
            make_dir((const char *)subpath, mode);
        }
        if (mkdir(dir, mode) != 0) {
            return -1;
        }
        return 0;
    }

    int make_dir(const string& dir) {
        return make_dir(dir.c_str(), 0775);
    }

    string get_dirname(const string& path) {
        char* buf = strdup(path.c_str());
        if (buf != NULL) {
            char* dname = dirname(buf);
            string ret(dname);
            free(buf);
            return ret;
        }
        return NULL;
    }

    string get_basename(const string& path) {
        char* buf = strdup(path.c_str());
        if (buf != NULL) {
            char* bname = basename(buf);
            string ret(bname);
            free(buf);
            return ret;
        }
        return NULL;
    }

    static void file_tree_walk(const string& dir, vector<string>& files,
                               const int root_len) {
        DIR* dirp = opendir(dir.c_str());
        if (dirp == NULL) {
            return;
        }
        struct dirent* dp;
        while ((dp = readdir(dirp)) != NULL) {
            string fname(dp->d_name, dp->d_namlen);
            stringstream ss;
            ss << dir << "/" << fname;
            string fpath = ss.str();
            if (dp->d_type == DT_DIR) {
                if (fname != "." && fname != "..") {
                    file_tree_walk(fpath, files, root_len);
                }
            } else if (dp->d_type == DT_REG) {
                files.push_back(fpath.substr(root_len + 1));
            }
        }
        closedir(dirp);
    }

    void list_file(const string& dir, vector<string>& files) {
        file_tree_walk(dir, files, dir.size());
    }
}
