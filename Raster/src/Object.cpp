#include <thread>
#include <future>
#include <sstream>
#include <memory>

#include "Object.h"

#define M_PI 2 * std::asin(1.0)

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
			count += face.get()->vertices.size();

	return count;
}

void Object::scale(double sx, double sy, double sz)
{
	for (auto mesh : meshes)
		for (auto face : mesh.get()->faces)
			for (auto vertex : face.get()->vertices)
			{
				vertex.get()->v.x = vertex.get()->v.x * sx;
				vertex.get()->v.y = vertex.get()->v.y * sy;
				vertex.get()->v.z = vertex.get()->v.z * sz;
			}
}

void Object::scale(double s)
{
	for (auto mesh : meshes)
		for (auto face : mesh.get()->faces)
			for (auto vertex : face.get()->vertices)
			{
				vertex.get()->v.x = vertex.get()->v.x * s;
				vertex.get()->v.y = vertex.get()->v.y * s;
				vertex.get()->v.z = vertex.get()->v.z * s;
			}
}

void Object::translate(double tx, double ty, double tz)
{
	for (auto mesh : meshes)
		for (auto face : mesh.get()->faces)
			for (auto vertex : face.get()->vertices)
			{
				vertex.get()->v.x = vertex.get()->v.x + tx;
				vertex.get()->v.y = vertex.get()->v.y + ty;
				vertex.get()->v.z = vertex.get()->v.z + tz;
			}
}

void Object::rotate(double rx, double ry, double rz)
{
	//to radian
	rx = rx * M_PI / 180;
	ry = ry * M_PI / 180;
	rz = rz * M_PI / 180;

	//save object position
	Vec3f center = getCenter();

	for (auto mesh : meshes)
		for (auto face : mesh.get()->faces)
			for (auto vertex : face.get()->vertices)
			{
				//translate to center (rotate aroud itself)
				vertex.get()->v.x -= center.x;
				vertex.get()->v.y -= center.y;
				vertex.get()->v.z -= center.z;

				//rotate
				if (rx != 0)
				{
					vertex.get()->v.y = vertex.get()->v.y * std::cos(rx) + vertex.get()->v.z * - std::sin(rx);
					vertex.get()->v.z = vertex.get()->v.y * std::sin(rx) + vertex.get()->v.z * std::cos(rx);
				}
				if (ry != 0)
				{
					vertex.get()->v.x = vertex.get()->v.x * std::cos(ry) + vertex.get()->v.z * std::sin(ry);
					vertex.get()->v.z = vertex.get()->v.x * -std::sin(ry) + vertex.get()->v.z * std::cos(ry);
				}
				if (rz != 0)
				{
					vertex.get()->v.x = vertex.get()->v.x * std::cos(rz) + vertex.get()->v.y * -std::sin(rz);
					vertex.get()->v.y = vertex.get()->v.x * std::sin(rz) + vertex.get()->v.y * std::cos(rz);
				}

				//back to original position
				vertex.get()->v.x += center.x;
				vertex.get()->v.y += center.y;
				vertex.get()->v.z += center.z;
				
			}
}

Vec3f Object::getCenter()
{
	double x = 0, y = 0, z = 0;
	for (auto mesh : meshes)
		for (auto face : mesh.get()->faces)
			for (auto vertex : face.get()->vertices)
			{
				x += vertex.get()->v.x;
				y += vertex.get()->v.y;
				z += vertex.get()->v.z;
			}

	return Vec3f(x / getVerticesCount(), y / getVerticesCount(), z / getVerticesCount());
}

Vec3f Face::getNormal() 
{
	Vec3f v1 = vertices.at(1)->v - vertices.at(0)->v;
	Vec3f v2 = vertices.at(vertices.size() - 1)->v - vertices.at(0)->v;

	return v1.crossProduct(v2).normalize();
}

