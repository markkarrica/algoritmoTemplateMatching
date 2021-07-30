#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <filesystem>

using namespace std;
using namespace cv;
using filesystem::directory_iterator;
using filesystem::path;

int match_method = TM_CCOEFF_NORMED;

int main()
{
	Mat img;
	Mat templ = Mat(), second_templ = Mat();
	Mat img_display;

	for(const auto & sorgenteS : directory_iterator("./FotoDaLeggere/")){
		string sorgenteStringa = sorgenteS.path().string();

		if(sorgenteStringa.find("YMinus") != sorgenteStringa.npos){
			templ = imread("templateYMinus.jpg");
		} else {
			templ = imread("templateYPlus.jpg");
		}

		img = imread(sorgenteStringa);

		Rect rect_img_crop = Rect(0, 40, img.size().width, img.size().height - 40);
		Rect rect_templ_crop = Rect(0, 40, templ.size().width, templ.size().height - 40);

		img = img(rect_img_crop);
		templ = templ(rect_templ_crop);

		img.copyTo( img_display );
		Mat result;

		matchTemplate( img, templ, result, match_method);

		double minVal, maxVal;
		Point minLoc, maxLoc;
		Point matchLoc;
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		if( match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED ){
			matchLoc = minLoc;
		}else{
			matchLoc = maxLoc;
		}
		if (maxVal > 0.5) rectangle( img, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar(0, 0, 255), 2, 8, 0 );
		imwrite("FotoLette\\" + path(sorgenteStringa).filename().string(), img);
	}

  return 0;
}
