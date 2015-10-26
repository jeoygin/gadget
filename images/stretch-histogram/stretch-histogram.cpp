#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat getHistImg(const MatND& hist) {
  double maxVal = 0;
  double minVal = 0;

  minMaxLoc(hist, &minVal, &maxVal, 0, 0);
  int histSize = hist.rows;
  Mat histImg(histSize, histSize, CV_8U, Scalar(255));
  int hpt = static_cast<int>(0.9*histSize);

  for (int h = 0; h < histSize; h++) {
    float binVal = hist.at<float>(h);
    int intensity = static_cast<int>(binVal * hpt / maxVal);
    line(histImg, Point(h, histSize), Point(h, histSize - intensity), Scalar::all(0));
  }

  return histImg;
}

Mat stretch_hist(const Mat& src, const MatND& hist, double minVal) {
  Mat dst;
  int imin, imax;
  for (imin = 0; imin < 256; imin++) {
    if (hist.at<float>(imin) > minVal) {
      break;
    }
  }
  for (imax = 255; imax > -1; imax--) {
    if (hist.at<float>(imax) > minVal) {
      break;
    }
  }

  Mat lut(1, 256, CV_8U);
  for (int i = 0; i < 256; i++) {
    if (i < imin) {
      lut.at<uchar>(i) = 0;
    } else if (i > imax) {
      lut.at<uchar>(i) = 255;
    } else {
      lut.at<uchar>(i) = static_cast<uchar>(255*(i-imin)/(imax-imin)+0.5);
    }
  }

  LUT(src, lut, dst);
  return dst;
}

Mat stretch(const Mat& src) {
  const int channels[1] = {0};
  const int histSize[1] = {256};
  float hrange[2] = {0, 255};
  const float* ranges[1] = {hrange};
  MatND hist;
  Mat image;

  if (src.channels() == 3) {
    cvtColor(src, image, CV_BGR2GRAY, 0);
  } else {
    image = src;
  }

  calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges);
  return stretch_hist(image, hist, 0);
}

int main(int argc, char** argv) {
  if (argc >= 3) {
    Mat src = imread(argv[1], 1);
    Mat dst = stretch(src);
    imwrite(argv[2], dst);
  } else {
    string srcpath, dstpath;
    while (cin >> srcpath >> dstpath) {
      Mat src = imread(srcpath, 1);
      Mat dst = stretch(src);
      imwrite(dstpath, dst);
    }
  }

  return 0;
}
