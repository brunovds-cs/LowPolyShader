#pragma once

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

struct Window
{
	static int width, height;
	static const char* name;
	static cv::Mat image;
	static cv::Point center;
	static int current_frame;
	static int fps;

	//init global window
	static void init(const char* name, int width, int height, int fps);

	//reset window (black screen)
	static void reset();

	//update window
	static void update();
	
	//get image center
	static cv::Point getCenter();
};