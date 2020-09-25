#include <thread>
#include <future>
#include <sstream>
#include <memory>

#include "Object.h"

void Object::loadFromFile(const char* path)
{
	std::ifstream file;
	file.open(path);

	if (file.is_open())
	{
		std::string line;
		std::vector<Vec3f> vertex_vector; //v
		std::vector<Vec3f> vertex_textures; //vt
		std::vector<std::shared_ptr<Face>> face_vector;

		while (std::getline(file, line)) //full line
		{
			//split line
			std::vector<std::string> elements;
			std::istringstream istring(line);
			std::string s;

			while (std::getline(istring, s, ' '))
				elements.push_back(s);
			
			if (elements.at(0) == "v") //vertices
				vertex_vector.push_back(Vec3f(std::stof(elements.at(1).c_str()), std::stof(elements.at(2).c_str()), std::stof(elements.at(3).c_str())));
			else 
			if (elements.at(0) == "vt") //textures cordinates
				vertex_textures.push_back(Vec3f(std::stof(elements.at(1)), std::stof(elements.at(2)), std::stof(elements.at(3))));
			else
			if (elements.at(0) == "f") //faces
			{
				std::vector<std::vector<std::string>> vec_face;

				for (uint32_t i = 1; i < elements.size(); ++i) //parse face elements
				{
					std::string str = elements.at(i);
					std::string delimiter = "/";
					std::string token;
					std::vector<std::string> vec_token;
					int pos = 0;
					while ((pos = str.find(delimiter)) != std::string::npos) {
						token = str.substr(0, pos);
						vec_token.push_back(token);
						str.erase(0, pos + delimiter.length());
					}

					vec_face.push_back(vec_token);
				}
				
				std::vector<std::shared_ptr<Vertex>> vertices_vec;

				for (auto& _vertex : vec_face)
				{
					int v_pos, vt_pos;
					v_pos = _vertex[0] == "" ? -1 : std::stoi(_vertex[0]);
					vt_pos = _vertex[1] == "" ? -1 : std::stoi(_vertex[1]);

					if(vt_pos == -1)
						vertices_vec.push_back(std::make_shared<Vertex>(vertex_vector.at(v_pos - 1), 0));
					else
						vertices_vec.push_back(std::make_shared<Vertex>(vertex_vector.at(v_pos - 1), vertex_textures.at(vt_pos - 1)));
				}
				
				face_vector.push_back(std::make_shared<Face>(vertices_vec));
			}
		}

		//TODO - implement for more meshes
		this->meshes.emplace_back(std::make_shared<Mesh>(face_vector));

		file.close();
	}
	else
		std::cout << "Error to load file: " << path << '\n';
}

const int Object::getVerticesCount() const
{
	int count = 0;
	for (auto mesh : meshes)
		for (auto face : mesh.get()->faces)
			for (auto vertex : face.get()->vertices)
				count++;

	return count;
}

Vec3f Face::getNormal() 
{
	Vec3f v1 = vertices.at(1)->v - vertices.at(0)->v;
	Vec3f v2 = vertices.at(vertices.size() - 1)->v - vertices.at(0)->v;

	return v1.crossProduct(v2).normalize();
}