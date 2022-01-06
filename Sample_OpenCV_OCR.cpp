// sample_opencv.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2) {
    double i = fabs(contourArea(cv::Mat(contour1)));
    double j = fabs(contourArea(cv::Mat(contour2)));
    return (i < j);
}

int main()
{
    // Read the image file
    Mat image = imread("C:/C++/sample_opencv/sample_opencv/Proje1.jpeg");

    // Check for failure
    if (image.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
        std::cin.get(); //wait for any key press
        return -1;
    }

    Mat img_gray;

    cvtColor(image, img_gray, COLOR_BGR2GRAY);

    // apply binary thresholding

    Mat thresh;

    threshold(img_gray, thresh, 155, 255, THRESH_BINARY);


    std::vector<std::vector<Point> > contours;
    std::vector<Vec4i> hierarchy;

    findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    //sort contours
    std::sort(contours.begin(), contours.end(), compareContourAreas);

    bool find = false; int x = 0, y = 0, width = 0, height = 0;

    for (int i = contours.size() - 1; i >= contours.size() - 5; i--)
    {
        Rect rect = boundingRect(contours[i]);

        if (rect.height > 140 && rect.width > 190 && rect.width < image.size[1] - 10 && rect.height < image.size[0] - 10)
        {
            x = rect.x;
            y = rect.y;
            width = rect.width;
            height = rect.height;

            find = true;

            Mat mod_image = image.clone();
            
            rectangle(mod_image, rect, Scalar(0, 0, 255),2);
                
            imwrite("green_box_outlined.jpg", mod_image);

            break;
        }
    }

    if (find)
    {
        Range rows(y, y + height-50);
        Range cols(x, x + width-30);
        Mat rez1 = image(rows, cols);
        Mat rez;
        resize(rez1, rez, cv::Size(), 1, 1);
        tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI();
        ocr->Init(NULL, "eng", tesseract::OEM_TESSERACT_LSTM_COMBINED);
        ocr->SetPageSegMode(tesseract::PSM_AUTO);
        ocr->SetImage(rez.data, rez.cols, rez.rows, 3, rez.step);
        String outText = ocr->GetUTF8Text();
        std::cout << outText << std::endl;
        ocr->End();
    }


    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
