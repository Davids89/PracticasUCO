#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <exception>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <valarray>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;

int levenshtein(const string &s1, const string &s2);
