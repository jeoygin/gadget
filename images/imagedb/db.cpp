#include "db.hpp"
#include "db_leveldb.hpp"
#include "db_filedb.hpp"

#include <string>
#include <iostream>

namespace db {
    DB* get_db(const string& backend) {
        if (backend == "leveldb") {
            return new LevelDB();
        }

        if (backend == "filedb") {
            return new FileDB();
        }

        return NULL;
    }

    DB* open_db(const string& source, Mode mode) {
        size_t found = source.find(":");
        string backend = "filedb";
        string path = source;

        if (found != string::npos) {
            backend = source.substr(0, found);
            path = source.substr(found+1);
        }

        DB* db = get_db(backend);
        if (db == NULL && backend != "filedb") {
            backend = "filedb";
            db = get_db(backend);
        }

        if (db != NULL) {
            db->open(path, mode);
        }

        return db;
    }
} // namespace cb
