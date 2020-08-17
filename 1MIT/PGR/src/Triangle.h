/*
 * File:   Triangle.h
 * Author: David Koleckar - xkolec07@stud.fit.vutbr.cz
 *         Matus Motlik - xmotli02@stud.fit.vutbr.cz
 */

#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <vector>
#include <stdexcept>
#include "Scene.h"

class Triangle {

public:
	Triangle(unsigned short index0, unsigned short index1, unsigned short index2);
	virtual ~Triangle() {};

	//Method
	std::vector<unsigned short> getTriangleIndices();

	void calculateNormal();
	glm::vec3 getNormal();
	
	glm::vec3 getRadiosity();
	void setRadiosity(glm::vec3 value);
	void addRadiosity(glm::vec3 value);
	
	glm::vec3 getReflection();
	void setReflection(glm::vec3 value);
	
	glm::vec3 getColor();
	void setColor(glm::vec3 value);
	glm::vec3 getCentroid();

	void setFormFactor(float value, uint count);
	void addFormFactor(float value);
	float getFormFactor();
	int getFormFactorCount();

	void setEmission(glm::vec3 value);
	void addEmission(glm::vec3 value);
	void resetEmission();
	glm::vec3 getEmission();

	glm::vec3 getVertex0();
	glm::vec3 getVertex1();
	glm::vec3 getVertex2();

	void setIndex(uint index);
	uint getIndex();

	void addVertexIndicesCount(ushort i);

	float getArea();

	inline bool operator== (const Triangle& other){ 
		return m_indices == other.m_indices;
	};

	inline bool operator< (const Triangle& other){ 
		return glm::length(m_emission) < glm::length(other.m_emission);
	};

private:
	std::vector<glm::vec3> getVertices();
	float computeArea();

	//Variable
private:
	std::vector<unsigned short> m_indices;
	glm::vec3 normal;
	glm::vec3 reflection;
	glm::vec3 color;
	uint m_index;

	glm::vec3 m_emission;
	glm::vec3 m_radiosity;
	float m_formFactor;
	uint m_formFactorCount;
	float m_area;
};
