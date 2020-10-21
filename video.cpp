#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>

void printASCII(cv::Mat src)
{
    float width = src.cols;
    float height = src.rows;
    double coeff = width / height;
    cv::Mat srrc;
    cv::cvtColor(src, src, cv::COLOR_RGB2GRAY);
    cv::resize(src, srrc, cv::Size((int)(50 * coeff), 50));

    std::vector<unsigned char> characters;
    characters.push_back(' ');
    characters.push_back('.');
    characters.push_back(':');
    characters.push_back('-');
    characters.push_back('=');
    characters.push_back('+');
    characters.push_back('*');
    characters.push_back('#');
    characters.push_back('%');
    characters.push_back('@');

    width = srrc.cols;
    height = srrc.rows;
    unsigned char colour;
    //std::cout << width << height << std::endl;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            colour = srrc.at<unsigned char>(cv::Point(j, i));
            //std::cout << (int)((float)colour/255*9)<<std::endl;
            std::cout << characters[(int)((float)colour / 255 * 9)] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << "\033[2J\033[1;1H";
}

cv::Mat toASCII(cv::Mat src)
{

    cv::Mat at = cv::imread("src/at.png", cv::IMREAD_GRAYSCALE);
    cv::Mat space = cv::imread("src/space.png", cv::IMREAD_GRAYSCALE);
    cv::Mat colon = cv::imread("src/colon.png", cv::IMREAD_GRAYSCALE);
    cv::Mat equal = cv::imread("src/equal.png", cv::IMREAD_GRAYSCALE);
    cv::Mat minus = cv::imread("src/minus.png", cv::IMREAD_GRAYSCALE);
    cv::Mat percent = cv::imread("src/percent.png", cv::IMREAD_GRAYSCALE);
    cv::Mat plus = cv::imread("src/plus.png", cv::IMREAD_GRAYSCALE);
    cv::Mat point = cv::imread("src/point.png", cv::IMREAD_GRAYSCALE);
    cv::Mat sharp = cv::imread("src/sharp.png", cv::IMREAD_GRAYSCALE);
    cv::Mat star = cv::imread("src/star.png", cv::IMREAD_GRAYSCALE);

    float width = src.cols;
    float height = src.rows;
    double coeff = width / height;
    cv::Mat srrc;
    cv::cvtColor(src, src, cv::COLOR_RGB2GRAY);
    cv::resize(src, srrc, cv::Size((int)(128 * coeff), 128));
    width = srrc.cols;
    height = srrc.rows;

    cv::Mat result = cv::Mat::zeros(cv::Size(width * 8, height * 8), cv::COLOR_RGB2GRAY);

    int rowS = 0;
    int rowE = 8;
    int colS = 0;
    int colE = 8;

    unsigned char colour;
    for (int i = 0; i < height; i++)
    {
        rowS = 8 * i;
        rowE = 8 * i + 8;
        for (int j = 0; j < width; j++)
        {
            colS = 8 * j;
            colE = 8 * j + 8;
            colour = srrc.at<unsigned char>(cv::Point(j, i));
            switch ((int)((float)colour / 255 * 10))
            {
            case 0:
                at.copyTo(result.rowRange(rowS, rowE).colRange(colS, colE));
                break;
            case 1:
                percent.copyTo(result.rowRange(rowS, rowE).colRange(colS, colE));
                break;
            case 2:
                sharp.copyTo(result.rowRange(rowS, rowE).colRange(colS, colE));
                break;
            case 3:
                star.copyTo(result.rowRange(rowS, rowE).colRange(colS, colE));
                break;
            case 4:
                plus.copyTo(result.rowRange(rowS, rowE).colRange(colS, colE));
                break;
            case 5:
                equal.copyTo(result.rowRange(rowS, rowE).colRange(colS, colE));
                break;
            case 6:
                minus.copyTo(result.rowRange(rowS, rowE).colRange(colS, colE));
                break;
            case 7:
                colon.copyTo(result.rowRange(rowS, rowE).colRange(colS, colE));
                break;
            case 8:
                point.copyTo(result.rowRange(rowS, rowE).colRange(colS, colE));
                break;
            default:
                space.copyTo(result.rowRange(rowS, rowE).colRange(colS, colE));
                -1;
            }
        }
    }
    return result;
}

int main(int argc, char *argv[])
{
    std::cout << argv[1] << std::endl;
    if (argc == 1)
    {
        cv::Mat frame;
        //--- INITIALIZE VIDEOCAPTURE
        cv::VideoCapture cap;
        // open the default camera using default API
        // cap.open(0);
        // OR advance usage: select any API backend
        int deviceID = 0;        // 0 = open default camera
        int apiID = cv::CAP_ANY; // 0 = autodetect default API
        // open selected camera using selected API
        cap.open(deviceID, apiID);
        // check if we succeeded
        if (!cap.isOpened())
        {
            std::cerr << "ERROR! Unable to open camera\n";
            return -1;
        }
        //--- GRAB AND WRITE LOOP
        std::cout << "Start grabbing" << std::endl
                  << "Press any key to terminate" << std::endl;
        cv::Mat src;
        for (;;)
        {

            // wait for a new frame from camera and store it into 'frame'
            cap.read(frame);
            // check if we succeeded
            if (frame.empty())
            {
                std::cerr << "ERROR! blank frame grabbed\n";
                break;
            }
            // show live and wait for a key with timeout long enough to show images
            cv::imshow("Live", frame);
            if (cv::waitKey(5) >= 0)
                //
                break;
        }
        cap.read(frame);
        src = frame.clone();
        cv::Mat result = toASCII(src);
        cv::imwrite("aaaaaaa.png", result);
        // the camera will be deinitialized automatically in VideoCapture destructor
        return 0;
    }

    else if (argc == 2)
    {
        std::string liveOrImage(argv[1]);
        if (liveOrImage == "live")
        {
            cv::Mat frame;
            cv::VideoCapture cap;
            int deviceID = 0;
            int apiID = cv::CAP_ANY;
            cap.open(deviceID, apiID);
            if (!cap.isOpened())
            {
                std::cerr << "ERROR! Unable to open camera\n";
                return -1;
            }

            for (;;)
            {
                cap.read(frame);
                if (frame.empty())
                {
                    std::cerr << "ERROR! blank frame grabbed\n";
                    break;
                }
                printASCII(frame);
                if (cv::waitKey(5) >= 0)
                    break;
            }
            return 0;
        }
    }
}