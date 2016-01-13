#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <iostream>
#include <fstream>

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

void draw(cv::Mat img, istream & is) {
    string type;
    while (is >> type) {
        if (type == "rect") {
            int x, y, w, h;
            is >> x >> y >> w >> h;
            cv::rectangle(img, cv::Point(x, y), cv::Point(x+w,y+h), cvScalar(0, 128, 255), 3);
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Usage: draw-image imagefile outfile [drawfile]" << endl;
        return 1;
    }

    cv::Mat img = cv::imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    if (argc > 2) {
        ifstream drawfile(argv[3]);
        draw(img, drawfile);
        drawfile.close();
    } else {
        draw(img, cin);
    }

    char buf[512];
    struct stat st = {0};
    strncpy(buf, argv[2], sizeof(buf));
    char * dir = dirname(buf);
    makedirs(dir, 0775);
    cv::imwrite(argv[2], img);

    return 0;
}
