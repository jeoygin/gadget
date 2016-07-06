#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

using namespace cv;
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

void binary_image(const string& srcpath, const string& dstpath) {
  char buf[512];
  struct stat st = {0};
  strncpy(buf, dstpath.c_str(), sizeof(buf));
  char * dir = dirname(buf);
  makedirs(dir, 0775);

  Mat gray = imread(srcpath, CV_LOAD_IMAGE_GRAYSCALE);
  Mat blur, bin;

  threshold(gray, bin, 0, 255, CV_THRESH_BINARY|CV_THRESH_OTSU);

  if (!dstpath.empty()) {
    imwrite(dstpath, bin);
  } else {
    imshow("binary", bin);
    waitKey();
  }
}

int main(int argc, char** argv) {
  string app_name = boost::filesystem::basename(argv[0]);
  bool stdin = false;
  string srcpath, dstpath;

  po::options_description desc("Options");
  desc.add_options()
    ("help", "print help messages")
    ("stdin", "read parameters from standard input")
    ("input,i", po::value<string>(&srcpath), "input image")
    ("output,o", po::value<string>(&dstpath), "output image");

  po::positional_options_description positionalOptions;
  positionalOptions.add("input", 1);
  positionalOptions.add("output", 1);

  po::variables_map vm;

  try {
    po::store(po::command_line_parser(argc, argv).options(desc)
              .positional(positionalOptions).run(), vm);

    if (vm.count("help")) {
        print_usage(app_name, desc);
        return 0;
    }

    if (vm.count("stdin")) {
        stdin = true;
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

  if (!stdin && srcpath.empty()) {
    print_usage(app_name, desc);
    return -1;
  }

  if (!stdin) {
    binary_image(srcpath, dstpath);
  } else {
    int processed = 0;
    while (cin >> srcpath >> dstpath) {
      binary_image(srcpath, dstpath);
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
