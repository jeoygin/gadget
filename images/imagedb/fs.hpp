#ifndef FS_HPP
#define FS_HPP

#include <unistd.h>

#include <string>
#include <vector>

using namespace std;

namespace fs {
    int make_dir(char * dir, mode_t mode);
    int make_dir(const string& dir);
    string get_dirname(const string& path);
    string get_basename(const string& path);
    void list_file(const string& dir, vector<string>& files);
} // namespace fs

#endif // FS_HPP
