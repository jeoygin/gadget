#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;

cv::Mat invert(cv::Mat src) {
  cv::Mat dst = (255 - src);
  return dst;
}

int main(int argc, char** argv) {
  if (argc >= 3) {
    cv::Mat src = cv::imread(argv[1]);
    cv::Mat dst = invert(src);
    cv::imwrite(argv[2], dst);
  } else {
    string srcpath, dstpath;
    while (cin >> srcpath >> dstpath) {
      cv::Mat src = cv::imread(srcpath);
      cv::Mat dst = invert(src);
      cv::imwrite(dstpath, dst);
    }
  }

  return 0;
}
