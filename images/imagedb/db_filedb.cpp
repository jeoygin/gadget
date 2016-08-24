#include "db_filedb.hpp"
#include "fs.hpp"

#include <boost/scoped_ptr.hpp>

namespace db {
    static string read_file(const string& path) {
        ifstream ifs;
        ifs.open(path, ios::in|ios::binary|ios::ate);

        if (ifs.is_open()) {
            size_t size = ifs.tellg();
            ifs.seekg(0, ios::beg);
            boost::scoped_ptr<char> data(new char[size]);
            ifs.read(data.get(), size);
            string encoded = base64_encode(reinterpret_cast<const unsigned char*>(data.get()), size);
            return encoded;
        } else {
            return NULL;
        }
    }

    static void write_file(const string&path, const string& content) {
        string dir = fs::get_dirname(path);

        if (!dir.empty()) {
            fs::make_dir(dir);
        }

        vector<unsigned char> decoded_content = base64_decode(content);
        ofstream ofs(path, ios::out|ios::binary);
        ostream_iterator<unsigned char> oi(ofs);
        std::copy(decoded_content.begin(), decoded_content.end(), oi);
        ofs.close();
    }

    FileDBIterator::FileDBIterator(const string& root) : root_(root) {
        files_.clear();
        fs::list_file(root_, files_);
        seek_to_first();
    }

    string FileDBIterator::value() {
        string path = root_ + "/" + *iter_;
        return read_file(path);
    }

    void FileDBWriter::put(const string& key, const string& value) {
        string path = root_ + "/" + key;
        write_file(path, value);
    }

    void FileDB::open(const string& source, Mode mode) {
        root_ = source;
    }

    string FileDB::get(const string& key) {
        string path = root_ + "/" + key;
        return read_file(path);
    }

    void FileDB::put(const string& key, const string& value) {
        string path = root_ + "/" + key;
        write_file(path, value);
    }
}
