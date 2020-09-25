#include <iostream>
#include <thread>
#include <future>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "Object.h"
#include "Shader.h"

int main()
{
	Object o;
	o.loadFromFile("files/esfera.obj");
    
    const cv::String wndname = "Flat Shader";
    int width = 1280, height = 720;

    while (true)
    {
        cv::Mat image = cv::Mat::zeros(height, width, CV_8UC3);

        for(auto& mesh : o.getMeshes())
            for (auto& face : mesh.get()->faces)
                shader::perspective(face.get()->vertices, 400, image);

        cv::imshow(wndname, image);
        cv::waitKey(1000 / 60); //60fps
    }

    cv::waitKey(0);
    std::cin.get();

    return 0;

}