#include <iostream>
#include <opencv2/opencv.hpp>
int main()
{
	// Open the supplied video file.
	cv::VideoCapture video("resources/camera_test_fixed.mp4");
	if (!video.isOpened()) {
		std::cerr << "Could not open the video.\n";
		return 1;
	}
	cv::Mat frame;
	// Read and display one frame at a time.
	while (video.read(frame)) {
		cv::imshow("OpenCV video test", frame);
		// Wait briefly; Escape closes the program.
		if (cv::waitKey(20) == 27) {
			break;
		}
	}
	return 0;
}