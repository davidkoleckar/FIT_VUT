/*
 * File:   triangle.h
 * Author: David Koleckar - xkolec07@stud.fit.vutbr.cz
 *         Matus Motlik - xmotli02@stud.fit.vutbr.cz
 */

#include "Triangle.h"

Triangle::Triangle(unsigned short index0, unsigned short index1, unsigned short index2){
	m_indices.push_back(index0);
	m_indices.push_back(index1);
	m_indices.push_back(index2);
	addVertexIndicesCount(index0);
	addVertexIndicesCount(index1);
	addVertexIndicesCount(index2);
	
	normal = glm::vec3(0.0f);
	reflection = glm::vec3(0.0f);
	color = glm::vec3(0.0f);
	m_index = 0;
	m_radiosity = glm::vec3(0.0f);
	m_emission = glm::vec3(0.0f);
	m_formFactor = 0.0f;
	m_formFactorCount = 0;
	m_area = computeArea();
}

void Triangle::calculateNormal(){
	this->normal = glm::cross(getVertex1() - getVertex0(), getVertex2() - getVertex1());
}

glm::vec3 Triangle::getNormal(){
	return this->normal;
}

std::vector<glm::vec3> Triangle::getVertices() {
	std::vector<glm::vec3> vertices;
	vertices.push_back(getVertex0());
	vertices.push_back(getVertex1());
	vertices.push_back(getVertex2());
	return vertices;
}

void Triangle::addVertexIndicesCount(ushort i) {
	Scene::getInstance().getVerticesVec().at(i).indicesCount++;
}

glm::vec3 Triangle::getVertex0() {
	return Scene::getInstance().getVerticesVec().at(m_indices[0]).position;
}

glm::vec3 Triangle::getVertex1() {
	return Scene::getInstance().getVerticesVec().at(m_indices[1]).position;
}

glm::vec3 Triangle::getVertex2() {
	return Scene::getInstance().getVerticesVec().at(m_indices[2]).position;
}

/**
 * Get indices of triangle points
 */
std::vector<unsigned short> Triangle::getTriangleIndices() {
	return m_indices;
}

glm::vec3 Triangle::getRadiosity(){
	return this->m_radiosity;
}

void Triangle::setRadiosity(glm::vec3 value){
	this->m_radiosity = value;
}

void Triangle::addRadiosity(glm::vec3 value) {
	m_radiosity += value;
}

glm::vec3 Triangle::getReflection(){
	return this->reflection;
}

void Triangle::setReflection(glm::vec3 value){
	this->reflection = value;
}

glm::vec3 Triangle::getColor(){
	return this->color;
}

void Triangle::setColor(glm::vec3 value){
	this->color = value;
}

glm::vec3 Triangle::getCentroid(){
	return (this->getVertex0()+this->getVertex1()+getVertex2()) * 0.33333f;
}

void Triangle::setFormFactor(float value, uint count) {
	m_formFactor = value;
	m_formFactorCount = count;
}

void Triangle::addFormFactor(float value) {
	m_formFactor += value;
	if (value > 0.0f)
		m_formFactorCount++;
}

float Triangle::getFormFactor() {
	return m_formFactor;
}

int Triangle::getFormFactorCount() {
	return m_formFactorCount;
}

void Triangle::setEmission(glm::vec3 value) {
	m_emission = value;
}

void Triangle::addEmission(glm::vec3 value) {
	m_emission += value;
}

void Triangle::resetEmission() {
	m_emission = glm::vec3(0.0f);
}

glm::vec3 Triangle::getEmission() {
	return m_emission;
}

void Triangle::setIndex(uint index) {
	m_index = index;
}

uint Triangle::getIndex() {
	return m_index;
}

float Triangle::computeArea() {
	glm::vec3 e1 = getVertex1() - getVertex0();
	glm::vec3 e2 = getVertex2() - getVertex0();
	glm::vec3 e3 = cross(e1, e2);
	e3 *= e3;
	float area = 0.5 * sqrt(e3.x + e3.y + e3.z);
	return area;
}

float Triangle::getArea() {
	return m_area;
}
