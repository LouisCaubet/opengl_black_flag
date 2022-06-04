#pragma once

#include "cgp/cgp.hpp"
#include <iostream>
#include <vector>
#include "interpolation.hpp"

float interpolation(float t, cgp::buffer<float> x_axis,cgp::buffer<float> y_axis);

struct perlin_noise_parameters
{
	float persistency = 0.4f;
	float noisescale = 0.3f;
	float frequency_gain = 2.0f;
	int octave = 5;
	float terrain_height = 1.5f;
	//use curve function here
	cgp::buffer<float> x_axis = { 0,0,0.5f,1.0f,1.0f };
	cgp::buffer<float> y_axis ={0,0 ,0.25f,1.0f,1.0f};
	float height(float noise);
	
};



struct Terraintype {
	std::string name;
	float heightvalue;
	cgp::vec3 color;
	Terraintype(std::string nom, float valeur, cgp::vec3 couleur) : name{ nom }, heightvalue{valeur},color{couleur}
	{};
	Terraintype() {};
};


class island {
public:
	cgp::buffer<float> falloffmap; 
	std::vector<Terraintype> regions;
	// Initialize the mesh of the terrain
	cgp::mesh create_terrain_mesh();

	// Recompute the vertices of the terrain everytime a parameter is modified
	//  and update the mesh_drawable accordingly
	void update_terrain(cgp::mesh& terrain, cgp::mesh_drawable& terrain_visual, perlin_noise_parameters & parameters);
	static float evaluate(float height);
	static float clampnoise(float noise);
};


