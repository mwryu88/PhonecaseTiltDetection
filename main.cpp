#include <cstdlib>
#include "opencv2/opencv.hpp"
#include <opencv2/core/mat.hpp>  
#include <opencv2/imgcodecs.hpp>  
#include <opencv2/imgproc.hpp>  
#include <opencv2/highgui.hpp>  

using namespace std;
using namespace cv;

Mat rotate(Mat& image, double angle);

int main(int argc, char** argv) {

    Mat img_color = imread("CASE.jpg");

    float cols = (float) img_color.cols;
    float rows = (float) img_color.rows;



    //threshold(img_gray, img_binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

    //imshow("binary", img_binary);

    //cvtColor(img_gray, img_color, COLOR_GRAY2BGR);

    Mat img_rotated;

    int size_min = 1000000;
    int resultAngle = 0;
    
    int width = 0;
    int height = 0;
    
    Mat img_gray, img_binary;

    for (int angle = 0; angle>-90; angle--) {

        img_color.copyTo(img_rotated);
        img_rotated = rotate(img_rotated, angle);
        cvtColor(img_rotated, img_gray, COLOR_BGR2GRAY);
        threshold(img_gray, img_binary, 127, 255, THRESH_BINARY_INV);
        

        Mat img_labels, stats, centroids;
        int numOfLables = connectedComponentsWithStats(img_binary, img_labels, stats, centroids, 8, CV_32S);
        int size_max = 0;
        int largestLabel = 0;

        for (int j = 1; j < numOfLables; j++) {
            width = stats.at<int>(j, CC_STAT_WIDTH);
            height = stats.at<int>(j, CC_STAT_HEIGHT);
            int size = width + height;
            if (size > 100 && size > size_max) {
                largestLabel = j;
                size_max = size;
            }
        }

        int left = stats.at<int>(largestLabel, CC_STAT_LEFT);
        int top = stats.at<int>(largestLabel, CC_STAT_TOP);
        

        rectangle(img_rotated, Point(left, top), Point(left + width, top + height), Scalar(0, 0, 255), 1);
        //printf("left_top = %d, top = %d, height = %d, width = %d, area = %d \n", left, top, height, width, area_max);
        
        //printf("area_max = %d\n",area_max);
        
        printf("%d, ",size_max);

        if (size_min >= size_max) {
            size_min = size_max;
            resultAngle = angle;
            printf("area_min = %d, angle_update = %d\n",size_min,resultAngle);
        }
    }
    
    printf("angle = %d",resultAngle);


    img_rotated = rotate(img_color, resultAngle);

    imshow("case", img_rotated);

    waitKey(0);

    return 0;
}

Mat rotate(Mat& image, double angle) {
    Point2f src_center(image.cols / 2.0F, image.rows / 2.0F);

    Mat rot_matrix = getRotationMatrix2D(src_center, angle, 1.0);

    Mat rotated_img(Size(image.size().height, image.size().width), image.type());

    warpAffine(image, rotated_img, rot_matrix, image.size());

    return rotated_img;
}
