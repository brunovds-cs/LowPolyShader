#include <iostream>

#include "opencv2/core.hpp"

#include "Window.h"

int			Window::width;
int			Window::height;
const char* Window::name;
cv::Mat		Window::image;
cv::Point	Window::center;
int			Window::current_frame;
int			Window::fps;

void Window::init(const char* name, int width, int height, int fps)
{
	Window::width = width;
	Window::height = height;
	Window::name = name;
	Window::center = getCenter();
	Window::current_frame = 0;
	Window::fps = fps;

	Window::reset();
	Window::update();
}

void Window::reset()
{
	Window::image = cv::Mat::zeros(height, width, CV_8UC3);
}

void Window::update()
{
	Window::current_frame++;
	cv::imshow(Window::name, Window::image);
	cv::waitKey(1000 / Window::fps);
}

cv::Point Window::getCenter()
{
	return cv::Point((int)(width / 2), (int)(height / 2));
}
