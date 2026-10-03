#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/geometry.hpp>

int main()
{
	// Open the supplied video file.
	cv::VideoCapture video("resources/camera_test_fixed.mp4");
	if (!video.isOpened()) {
		std::cerr << "Could not open the video.\n";
		return 1;
	}
	cv::Mat frame;
	cv::Mat gray;
	cv::Mat binary;

	//Store all contours found in the binary image.
	std::vector<std::vector<cv::Point>> contours;

	// Read and display one frame at a time.
	while (video.read(frame)) 
	{
		int thresholdValue = 128;
		int maxValue = 255;

		//Green color för accepted candidates
		int blueValue = 0;
		int greenValue = 255;
		int redValue = 0;

		//Red color for rejected candidates
		int rejectedBlueValue = 0;
		int rejectedGreenValue = 0;
		int rejectedRedValue = 255;

		//Values used to filter contour candidates
		double minContourArea = 500;
		double epsilonFactor = 0.02;
		double minAspectRatio = 0.8;
		double maxAspectRatio = 1.2;

		//Convert the fram to grayscale and then to a binary image
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		cv::threshold(gray, binary, thresholdValue, maxValue, cv::THRESH_BINARY);

		//Find contours in the binary image, including nested contours
		cv::findContours(binary, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

		cv::Mat contourImage = frame.clone();

		//Check each detected contour.
		for(const auto& contour : contours)
		{
			double contourArea = cv::contourArea(contour);

			//Ignore conoturs that are too small
			if(contourArea > minContourArea)
			{
				std::vector<cv::Point> approximate;

				//Simplify the contour to find its main corner points.
				double perimeter = cv::arcLength(contour, true);
				double epsilon = epsilonFactor * perimeter;

				cv::approxPolyDP(contour, approximate, epsilon, true);

				//A marker candidate should have four corners and be convex
				if(approximate.size() == 4 && cv::isContourConvex(approximate))
				{
					cv::Rect boundingBox = cv::boundingRect(approximate);

					//Compare width and height to check if the shape is roughly square.
					double aspectRatio = static_cast<double>(boundingBox.width) / boundingBox.height;

					if(aspectRatio >= minAspectRatio && aspectRatio <= maxAspectRatio)
					{
						//Draw accepted candidates in green
						std::vector<std::vector<cv::Point>> filteredContour = {contour};

						cv::drawContours(contourImage, filteredContour, -1, 
						cv::Scalar(blueValue, greenValue, redValue));
					}

				}
				else
				{
					//Draw rejected shape in red for debugging.
					std::vector<std::vector<cv::Point>> rejectedContour = {contour};

					cv::drawContours(contourImage, rejectedContour, -1,
					cv::Scalar(rejectedBlueValue, rejectedGreenValue, rejectedRedValue));

				}
				
			}

		}


		//cv::drawContours(contourImage, contours, -1, cv::Scalar(blueValue, greenValue, redValue));

		cv::imshow("OpenCV video test", contourImage);

		// Wait briefly; Escape closes the program.
		if (cv::waitKey(20) == 27) {
			break;
		}
	}
	return 0;
}