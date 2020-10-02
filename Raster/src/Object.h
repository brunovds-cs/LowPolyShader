#pragma once

#include <vector>
#include <fstream>
#include <string>

#include "geometry.h"

struct Vertex
{
	//we don't use vetex normals for flat shading

	//geometric vertex
	Vec3f v;
	//vertex texture cordinates
	Vec3f vt;

	Vertex(Vec3f vertex, Vec3f texture_coord) : v(vertex), vt(texture_coord){}
};

struct Face
{
	std::vector<std::shared_ptr<Vertex>> vertices;

	Face(std::vector<std::shared_ptr<Vertex>> v) : vertices(v){}

	//get face normal
	Vec3f getNormal();
	

};

struct Mesh
{
	std::vector<std::shared_ptr<Face>> faces;

	Mesh(std::vector<std::shared_ptr<Face>> f) : faces(f) {}
};

class Object
{
private:
	std::vector<std::shared_ptr<Mesh>> meshes;

public:
	Object(std::vector<std::shared_ptr<Mesh>> m) : meshes(m) {}
	Object() {}
	
	std::vector<std::shared_ptr<Mesh>> getMeshes() { return this->meshes; };
	
	//load object with file .obj
	void loadFromFile(const char* path);

	//get vertices count
	const int getVerticesCount() const;

	//transformations
	void rotate(double rx, double ry, double rz);
	void scale(double s);
	void scale(double sx, double sy, double sz);
	void translate(double tx, double ty, double tz);

	//get object center
	Vec3f getCenter();
};
