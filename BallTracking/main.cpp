#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, const char * argv[])
{
    CvCapture* capture = cvCreateFileCapture("/Users/agata/Desktop/CodingProjects/BallTracking/test_vid/Videos/Fail.MOV"); //change this to be the path to your video file
    
    //IplImage* frame;// = NULL;
    cv::Mat frame;
    
    if(!capture)
    {
        printf("Video Not Opened\n");
        return -1;
    }
    
    int width = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
    int height = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);
    double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    int frame_count = (int)cvGetCaptureProperty(capture,  CV_CAP_PROP_FRAME_COUNT);
    
    printf("Video Size = %d x %d\n",width,height);
    printf("FPS = %f\nTotal Frames = %d\n",fps,frame_count);

    cv::Mat hsvimage;
    cv::Mat threshimage, erodeimage, dilateimage, imblur;
    
    
    for (int i = 0; i<frame_count; i++) /// if we were to grab this video from a live feed we would change to while(1)
    {
        frame = (cv::Mat)cvQueryFrame(capture);
        
        //converting to HSV color space
        cv::cvtColor(frame, hsvimage, CV_BGR2HSV);
        cv::inRange(hsvimage, cv::Scalar(75, 100, 0), cv::Scalar(130, 255, 255), threshimage); //
        
        
        //getting rid of noise
        cv::erode(threshimage, erodeimage, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);
        cv::dilate(erodeimage, dilateimage, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);
        cv::GaussianBlur( dilateimage, imblur, cv::Size(9, 9), 2, 2 );
        
        /// hough transform to find the circles
        cv::vector<cv::Vec3f> circles;
        cv::HoughCircles( imblur, circles, CV_HOUGH_GRADIENT, 1, 300, 50, 10 );
        
        std::cout << circles.size() << std::endl; //for more than one cirle: for( size_t i = 0; i < circles.size(); i++ )
        if (circles.size()>0){
            cv::Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
            int radius = cvRound(circles[0][2]);
            cv::circle( frame, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );// circle center
            cv:: circle( frame, center, radius, cv::Scalar(0,0,255), 3, 8, 0 ); // circle outline
        }
        
        IplImage iplimg = frame;
        cvShowImage("Example2",&iplimg);
        char c = cvWaitKey(1);
        if( c == 27 ) break;
        
        
    }
    
    
    cvReleaseCapture(&capture);
    cvDestroyWindow( "Example2" );

    return 0;
}