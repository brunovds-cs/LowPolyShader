#include <Math.h>

#include "Shader.h"

void shader::traceLine(cv::Point p1, cv::Point p2, cv::Mat& image)
{
	int declive, dx, dy, incE, incNE, d, x, y;
	dx = p2.x - p1.x;
	dy = p2.y - p1.y;

	if (std::abs(dx) >= std::abs(dy))
	{
		declive = dy < 0 ? -1 : 1;
		if (p1.x > p2.x) shader::traceLine(p2, p1, image);

		dy *= declive;
		incE = 2 * dy;
		incNE = 2 * dy - 2 * dx;
		d = 2 * dy - dx;
		y = p1.y;

		for (x = p1.x; x <= p2.x; x++)
		{
			cv::circle(image, cv::Point(x, y), 0, cv::Scalar(255, 255, 255));

			if (d <= 0) d += incE;
			else
			{
				d += incNE;
				y += declive;
			}
		}
	}
	else
	{
		declive = dx < 0 ? -1 : 1;
		if (p1.y > p2.y) shader::traceLine(p2, p1, image);

		dx *= declive;
		incE = 2 * dx;
		incNE = 2 * dx - 2 * dy;
		d = 2 * dx - dy;
		x = p1.x;

		for (y = p1.y; y <= p2.y; y++)
		{
			cv::circle(image, cv::Point(x, y), 0, cv::Scalar(255, 255, 255));

			if (d <= 0) d += incE;
			else
			{
				d += incNE;
				x += declive;
			}
		}
	}
}



void shader::perspective(std::vector<std::shared_ptr<Vertex>> vertices, uint32_t pov, cv::Mat& image)
{
	cv::Point meio(image.cols / 2, image.rows / 2);
	std::vector<cv::Point> points;
	Vec3f camera(0, 0, -1);

	for (int i = 0; i < vertices.size(); i++)
	{
		double x = vertices.at(i).get()->v.x;
		double y = vertices.at(i).get()->v.y;
		double z = vertices.at(i).get()->v.z;
		double px, py;

		px = x * pov / (z + pov);
		py = y * pov / (z + pov);

		points.push_back(cv::Point(px + meio.x, py + meio.y));
	}

	for (uint32_t i = 0; i < points.size() - 1; i++)
		shader::traceLine(points.at(i), points.at(i + 1), image);

	//last
	shader::traceLine(points.at(points.size() - 1), points.at(0), image);
}




