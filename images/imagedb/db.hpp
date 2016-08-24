#ifndef DB_HPP
#define DB_HPP

#include <string>
#include <glog/logging.h>

using namespace std;

namespace db {
    enum Mode { READ, WRITE, NEW };

    class Iterator {
    public:
        Iterator() {}
        virtual ~Iterator() {}
        virtual void seek_to_first() = 0;
        virtual void next() = 0;
        virtual string key() = 0;
        virtual string value() = 0;
        virtual bool valid() = 0;
    };

    class Writer {
    public:
        Writer() {}
        virtual ~Writer() {}
        virtual void put(const string& key, const string& value) = 0;
        virtual void flush() = 0;
    };

    class DB {
    public:
        DB() {}
        virtual ~DB() {}
        virtual void open(const string& source, Mode mode) = 0;
        virtual void close() = 0;
        virtual string get(const string& key) = 0;
        virtual void put(const string& key, const string& value) = 0;
        virtual Iterator* new_iterator() = 0;
        virtual Writer* new_writer() = 0;
    };

    DB* get_db(const string& backend);
    DB* open_db(const string& source, Mode mode);
} // namespace db

#endif // DB_HPP
