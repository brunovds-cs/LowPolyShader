#pragma once

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "Object.h"

namespace shader
{
	//trace a line between two points using bresenham algorithm
	void traceLine(cv::Point p1, cv::Point p2, cv::Mat& image);
	//perspective projection
	void perspective(std::vector<std::shared_ptr<Vertex>> vertices, uint32_t pov, cv::Mat& image);
}
