#include <cv.h>
#include <highgui.h>


/* メインプログラム */
int
main (int argc, char **argv)
{
  IplImage *src_img = 0;
  IplImage *dst_img_dilate, *dst_img_erode;
  IplImage *dst_img_opening, *dst_img_closing;
  IplImage *dst_img_gradient, *dst_img_tophat, *dst_img_blackhat;
  IplImage *tmp_img;
  IplConvKernel *element;


  //(1)画像の読み込み，演算結果画像領域の確保を行なう
  if (argc >= 2)
    src_img = cvLoadImage (argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  if (src_img == 0)
    exit (-1);
  dst_img_dilate = cvCloneImage (src_img);
  dst_img_erode = cvCloneImage (src_img);
  dst_img_opening = cvCloneImage (src_img);
  dst_img_closing = cvCloneImage (src_img);
  dst_img_gradient = cvCloneImage (src_img);
  dst_img_tophat = cvCloneImage (src_img);
  dst_img_blackhat = cvCloneImage (src_img);
  tmp_img = cvCloneImage (src_img);

  //(2)構造要素を生成する
  element = cvCreateStructuringElementEx (9, 9, 4, 4, CV_SHAPE_RECT, NULL);

  //(3)各種のモルフォロジー演算を実行する
  cvDilate (src_img, dst_img_dilate, element, 1);
  cvErode (src_img, dst_img_erode, element, 1);

  cvMorphologyEx (src_img, dst_img_opening, tmp_img, element, CV_MOP_OPEN, 1);
  cvMorphologyEx (src_img, dst_img_closing, tmp_img, element, CV_MOP_CLOSE, 1);
  cvMorphologyEx (src_img, dst_img_gradient, tmp_img, element, CV_MOP_GRADIENT, 1);
  cvMorphologyEx (src_img, dst_img_tophat, tmp_img, element, CV_MOP_TOPHAT, 1);
  cvMorphologyEx (src_img, dst_img_blackhat, tmp_img, element, CV_MOP_BLACKHAT, 1);

  //(4)モルフォロジー演算結果を表示する
  cvNamedWindow ("src", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("dilate", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("erode", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("opening", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("closing", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("gradient", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("tophat", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("blackhat", CV_WINDOW_AUTOSIZE);
  cvShowImage ("src", src_img);
  cvShowImage ("dilate", dst_img_dilate);
  cvShowImage ("erode", dst_img_erode);
  cvShowImage ("opening", dst_img_opening);
  cvShowImage ("closing", dst_img_closing);
  cvShowImage ("gradient", dst_img_gradient);
  cvShowImage ("tophat", dst_img_tophat);
  cvShowImage ("blackhat", dst_img_blackhat);
  cvWaitKey (0);

  cvDestroyWindow ("src");
  cvDestroyWindow ("dilate");
  cvDestroyWindow ("erode");
  cvDestroyWindow ("opening");
  cvDestroyWindow ("closing");
  cvDestroyWindow ("gradient");
  cvDestroyWindow ("tophat");
  cvDestroyWindow ("blackhat");
  cvReleaseImage (&src_img);
  cvReleaseImage (&dst_img_dilate);
  cvReleaseImage (&dst_img_erode);
  cvReleaseImage (&dst_img_opening);
  cvReleaseImage (&dst_img_closing);
  cvReleaseImage (&dst_img_gradient);
  cvReleaseImage (&dst_img_tophat);
  cvReleaseImage (&dst_img_blackhat);
  cvReleaseImage (&tmp_img);

  return 1;
}