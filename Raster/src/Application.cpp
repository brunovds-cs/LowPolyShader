#include <iostream>
#include <thread>
#include <future>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "Object.h"
#include "Shader.h"
#include "Window.h"

int main()
{
	Object o;
    o.loadFromFile("files/gremlin.obj");
    
    //main window
    Window::init("Flat Shader", 1080, 720, 75);

    while (true)
    {
        Window::reset();

        for(auto& mesh : o.getMeshes())
            for (auto& face : mesh.get()->faces)
                shader::perspective(face.get()->vertices, 400);

        //animation example
        o.rotate(1, 1, 1);

        if (Window::current_frame % 600 <= 150)
        {
            o.scale(1.002);
            o.translate(1, 0, 0);
        }
        else if(Window::current_frame % 600 <= 300)
        {
            o.scale(0.998);
            o.translate(-1, 0, 0);
        }
        else if (Window::current_frame % 600  <= 450)
        {
            o.scale(1.002);
            o.translate(-1, 0, 0);
        }
        else if (Window::current_frame % 600 <= 600)
        {
            o.scale(0.998);
            o.translate(1, 0, 0);
        }

        Window::update();
    }

    std::cin.get();

    return 0;
}