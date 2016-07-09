#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

using namespace std;

namespace po = boost::program_options;

void print_usage(string app_name, po::options_description desc) {
  cout << "Usage: " << app_name << " [options]" << endl << endl;
  cout << desc << endl;
}

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

cv::Mat resize(cv::Mat src, int width, int height) {
  cv::Mat dst;
  cv::resize(src, dst, cv::Size(width, height));
  return dst;
}

void resize_image(const string& srcpath, const string& dstpath,
                  int width, int height, bool keep_ratio) {
  char buf[512];
  struct stat st = {0};
  strncpy(buf, dstpath.c_str(), sizeof(buf));
  char * dir = dirname(buf);
  makedirs(dir, 0775);

  cv::Mat src = cv::imread(srcpath, CV_LOAD_IMAGE_UNCHANGED);

  cv::Mat dst;
  if (!keep_ratio) {
    dst = resize(src, width, height);
  } else {
    int tmp_width, tmp_height;
    if (width * src.rows < height * src.cols) {
      tmp_width = width;
      tmp_height = max(1, src.rows * width / src.cols);
    } else {
      tmp_height = height;
      tmp_width = max(1, src.cols * height / src.rows);
    }
    cv::Mat tmp = resize(src, tmp_width, tmp_height);

    dst = cv::Mat::zeros(width, height, src.type());
    tmp.copyTo(dst(cv::Rect((width - tmp_width) / 2,
                            (height - tmp_height) / 2,
                            tmp_width, tmp_height)));
  }

  cv::imwrite(dstpath, dst);
}

int main(int argc, char** argv) {
  string app_name = boost::filesystem::basename(argv[0]);
  bool keep_ratio = false, stdin = false, verbose = false;
  string srcpath, dstpath;
  int width, height;

  po::options_description desc("Options");
  desc.add_options()
    ("help", "print help messages")
    ("keep-aspect-ratio", "keep aspect ratio")
    ("stdin", "read arguments from standard input")
    ("src,s", po::value<string>(&srcpath), "src path")
    ("dst,d", po::value<string>(&dstpath), "dst path")
    ("width,w", po::value<int>(&width), "width")
    ("height,h", po::value<int>(&height), "height")
    ("verbose,v", "produce verbose output");

  po::positional_options_description positionalOptions;
  positionalOptions.add("src", 1);
  positionalOptions.add("dst", 1);
  positionalOptions.add("width", 1);
  positionalOptions.add("height", 1);

  po::variables_map vm;

  try {
    po::store(po::command_line_parser(argc, argv).options(desc)
              .positional(positionalOptions).run(), vm);

    if (vm.count("help")) {
      print_usage(app_name, desc);
      return 0;
    }

    if (vm.count("keep-aspect-ratio")) {
      keep_ratio = true;
    }

    if (vm.count("stdin")) {
      stdin = true;
    }

    if (vm.count("verbose")) {
      verbose = true;
    }

    po::notify(vm);
  } catch (boost::program_options::required_option& e) {
    cerr << "ERROR: " << e.what() << endl << endl;
    print_usage(app_name, desc);
    return -1;
  } catch (boost::program_options::error& e) {
    cerr << "ERROR: " << e.what() << endl << endl;
    print_usage(app_name, desc);
    return -1;
  }

  if (!stdin && (width <= 0 || height <= 0)) {
    print_usage(app_name, desc);
    return -1;
  }

  if (!stdin) {
    resize_image(srcpath, dstpath, width, height, keep_ratio);
  } else {
    int processed = 0;
    while (cin >> srcpath >> dstpath >> width >> height) {
      if (verbose) {
        cout << srcpath << " " << dstpath << " "
             << width << " " << height << endl;
      }
      resize_image(srcpath, dstpath, width, height, keep_ratio);
      ++processed;
      if (processed % 1000 == 0) {
        cout << "Processed " << processed << " files." << endl;
      }
    }
    if (processed % 1000 != 0) {
      cout << "Processed " << processed << " files." << endl;
    }
  }

  return 0;
}
