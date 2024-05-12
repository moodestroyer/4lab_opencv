#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    VideoCapture cap("C:/Users/User/Downloads/output.avi");

    if (!cap.isOpened())
    {
        cout << "Error" << endl;
        return -1;
    }

    int width = cap.get(CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CAP_PROP_FRAME_HEIGHT);

    int video = VideoWriter::fourcc('X', 'V', 'I', 'D');
    VideoWriter videoOutput("C:/Users/User/Desktop/output.mp4", video, 20, Size(width, height));


    if (!videoOutput.isOpened())
    {
        cout << "Error with file" << endl;
        return -1;
    }

    Mat frame, gray, gauss, contur;

    while (true)
    {
        cap.read(frame);

        if (frame.empty())
            break;

        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, gauss, Size(5, 5), 0);
        Canny(gauss, contur, 240, 255);

        vector<vector<Point>> contours;
        findContours(contur, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (size_t i = 0; i < contours.size(); i++)
        {
            double area = contourArea(contours[i]);

            if (area > 300)
            {
                RotatedRect rotatedRect = minAreaRect(contours[i]);
                Point2f vertices[4];
                rotatedRect.points(vertices);

                for (int j = 0; j < 4; j++)
                    line(frame, vertices[j], vertices[(j + 1) % 4], Scalar(0, 0, 255), 2);
            }
        }
        imshow("Video", frame);
        videoOutput.write(frame);

        if (waitKey(25) == 27)
            break;
    }

    cap.release();
    videoOutput.release();
    destroyAllWindows();

    return 0;
}