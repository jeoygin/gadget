#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;

cv::Mat resize(cv::Mat src, int width, int height) {
  cv::Mat dst;
  cv::resize(src, dst, cv::Size(width, height));
  return dst;
}

int main(int argc, char** argv) {
  if (argc >= 5) {
    cv::Mat src = cv::imread(argv[1]);
    cv::Mat dst = resize(src, atoi(argv[3]), atoi(argv[4]));
    cv::imwrite(argv[2], dst);
  } else {
    string srcpath, dstpath;
    int width, height;

    while (cin >> srcpath >> dstpath >> width >> height) {
      cv::Mat src = cv::imread(srcpath);
      cv::Mat dst = resize(src, width, height);
      cv::imwrite(dstpath, dst);
    }
  }

  return 0;
}
