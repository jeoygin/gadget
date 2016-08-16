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

void image_info(const string& srcpath) {
  cv::Mat img = cv::imread(srcpath, CV_LOAD_IMAGE_UNCHANGED);

  if (!img.empty()) {
    cout << srcpath << " " << img.cols << " " << img.rows
         << " " << img.channels() << endl;
  } else {
    cerr << "FAIL: " << srcpath << endl;
  }
}

int main(int argc, char** argv) {
  string app_name = boost::filesystem::basename(argv[0]);
  bool stdin = false;
  string srcpath;

  po::options_description desc("Options");
  desc.add_options()
    ("help", "print help messages")
    ("stdin", "read arguments from standard input")
    ("src,s", po::value<string>(&srcpath), "src path");

  po::positional_options_description positionalOptions;
  positionalOptions.add("src", 1);

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

  if (!stdin) {
    if (srcpath.empty()) {
      print_usage(app_name, desc);
      return -1;
    }
    image_info(srcpath);
  } else {
    int processed = 0;
    while (cin >> srcpath) {
      try {
        image_info(srcpath);
      } catch (cv::Exception& e) {
        cerr << "FAIL: " << srcpath << endl;
      }
      ++processed;
      if (processed % 1000 == 0) {
        cerr << "Processed " << processed << " files." << endl;
      }
    }
    if (processed % 1000 != 0) {
      cerr << "Processed " << processed << " files." << endl;
    }
  }

  return 0;
}
