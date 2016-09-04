#ifndef COMMAND_HANDLER
#define COMMAND_HANDLER

#include <string>
#include <vector>

#include "config.hpp"
#include "operation.hpp"
#include "db/db.hpp"

namespace cmd {

    class CommandHandler {
    public:
        int process(const std::string& cmd, ImageDBConfig& config);
    };

    class CommandProcessor {
    public:
        virtual int run(db::DB* db, const int idx,
                        const std::vector<string>& fields) = 0;
        virtual void refresh() = 0;
        virtual void done() = 0;
        virtual bool good() = 0;
    };

    class PipeProcessor : public CommandProcessor {
    public:
        PipeProcessor(const std::vector<boost::shared_ptr<op::Operation>>& ops): ops_(ops) {}

        ~PipeProcessor() {

        }

        int run(db::DB* db, const int idx,
                const std::vector<string>& fields) {
            if (ops_.empty()) {
                return -1;
            } else if (db == NULL) {
                return -2;
            } else if (fields.size() < 1) {
                return -3;
            }

            string key = fields[0];
            content_.clear();
            db->get(key, content_);
            if (!content_.empty()) {
                cv::Mat img = cv::imdecode(content_, CV_LOAD_IMAGE_UNCHANGED);
                ops_[0]->execute(img, fields);
            } else {
                return -4;
            }

            return 0;
        }

        void refresh() {
            for (int i = 0; i < (int) ops_.size(); i++) {
                ops_[i]->flush();
            }
        }

        void done() {
            for (int i = 0; i < (int) ops_.size(); i++) {
                ops_[i]->finish();
            }
        }

        bool good() {
            return ops_.size() > 0;
        }
    private:
        const std::vector<boost::shared_ptr<op::Operation>>& ops_;
        std::vector<unsigned char> content_;
    };

    class SaveProcessor : public CommandProcessor {
    public:
        SaveProcessor(const string& url) {
            db_ = db::open_db(url, db::WRITE);
            if (db_) {
                writer_ = db_->new_writer();
            }
        }

        ~SaveProcessor() {}

        int run(db::DB* db, const int idx,
                const std::vector<string>& fields) {
            if (!writer_) {
                return -1;
            } else if (db == NULL) {
                return -2;
            } else if (fields.size() < 2) {
                return -3;
            }

            std::string src_key = fields[0], dst_key = fields[1];
            int ret = db->copy(src_key, writer_, dst_key, content_);
            if (ret == -2) {
                return -4;
            }

            return 0;
        }

        void refresh() {
            if (writer_) {
                writer_->flush();
                delete writer_;
                writer_ = db_->new_writer();
            }
        }

        void done() {
            if (writer_) {
                delete writer_;
                writer_ = NULL;
            }

            if (db_) {
                db_->close();
                db_ = NULL;
            }
        }

        bool good() {
            return writer_ != NULL;
        }
    private:
        db::DB* db_;
        db::Writer* writer_ = NULL;
        std::vector<unsigned char> content_;
    };

} // cmd

#endif
