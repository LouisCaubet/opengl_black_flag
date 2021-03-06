#include "cannon.hpp"
#include <iostream> 
#include <vector>

using namespace cgp;

const cgp::vec3 cannonball::g = cgp::vec3(0, 0, -9.81f);

void cannonballgenerator::initialize(GLuint const prog, Ship& other_ship) {
	
	shaderprogram = prog;

	//Initialisation de mesh_drawable pour le tirs de canon
	this->other_ship = &other_ship;
	texture_id = cgp::opengl_load_texture_image("assets/Explosion02_5x5.png",
		GL_REPEAT,
		GL_REPEAT);

	particle_drawable.initialize(mesh_primitive_quadrangle(vec3(-1, 0, -1) * particle_size_scale, vec3(1, 0, -1) * particle_size_scale, vec3(1, 0, 1) * particle_size_scale, vec3(-1, 0, 1) * particle_size_scale), "Particle_mesh");
	particle_drawable.shader = shaderprogram;
	particle_drawable.texture = texture_id;

	//Initialisation de mesh_drawable pour les smoketrails des boules du canon
	texture_id_smoke = cgp::opengl_load_texture_image("assets/Cloud04_8x8.png",
		GL_REPEAT,
		GL_REPEAT);

	smoke_drawable.initialize(mesh_primitive_quadrangle(vec3(-1, 0, -1) * particle_size_scale, vec3(1, 0, -1) * particle_size_scale, vec3(1, 0, 1) * particle_size_scale, vec3(-1, 0, 1) * particle_size_scale), "Particle_mesh");
	smoke_drawable.shader = shaderprogram;
	smoke_drawable.texture = texture_id_smoke;


	cannon_drawable.initialize(mesh_primitive_sphere(0.25f), "sphere");
	GLuint const texture_image_id = opengl_load_texture_image("assets/iron.png",
		GL_REPEAT,
		GL_REPEAT);
	cannon_drawable.texture = texture_image_id;
	cannontimersleft = std::vector<float>({ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,1.0f });
	cannontimersright = std::vector<float>({ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,1.0f });

	// Ajout des positions locales des canons par rapport au bateau
	cannonpositionsright.push_back((vec3(23.264f, 4.6946f, -7.8289f) + vec3(23.264f, 4.6025f, -7.8289f)) / 2);
	cannonpositionsright.push_back((vec3(20.218f, 4.45f, -7.1893f) + vec3(20.218f, 4.35865f, -7.1893f)) / 2);
	cannonpositionsright.push_back((vec3(17.343f, 4.27f, -6.8868f) + vec3(17.343f, 4.1786f, -6.8868f)) / 2);
	cannonpositionsright.push_back((vec3(14.435f, 4.1542, -6.8471) + vec3(14.435f, 4.621f, -6.8471)) / 2);
	cannonpositionsright.push_back((vec3(11.505f, 4.1542f, -6.9161) + vec3(11.505f, 4.0621, -6.9161)) / 2);
	cannonpositionsright.push_back((vec3(8.6325f, 4.3022f, -7.1158f) + vec3(8.6325f, 4.21f, -7.1158f)) / 2);
	cannonpositionsright.push_back((vec3(5.7567, 4.5144f, -7.3844f) + vec3(5.7567, 4.4223, -7.3844f)) / 2);
	cannonpositionsright.push_back((vec3(2.9597f, 4.8406f, -7.8051f) + vec3(2.9597f, 4.7485f, -7.8051f)) / 2);
	cannonpositionsright.push_back((vec3(0.051582f, 5.2053f, -8.166f) + vec3(0.051582f, 5.1132f, -8.166f)) / 2);

	
	cannonpositionsleft.push_back((vec3(23.264f, 4.6946f, -14.958) + vec3(23.264f, 4.6025f, -14.958)) / 2);
	cannonpositionsleft.push_back((vec3(20.218f, 4.45f, -15.573) + vec3(20.218f, 4.35865f, -15.573)) / 2);
	cannonpositionsleft.push_back((vec3(17.343f, 4.27f, -15.876) + vec3(17.343f, 4.1786f, -15.876)) / 2);
	cannonpositionsleft.push_back((vec3(14.435f, 4.1542, -15.915) + vec3(14.435f, 4.621f, -15.915)) / 2);
	cannonpositionsleft.push_back((vec3(11.505f, 4.1542f, -15.846) + vec3(11.505f, 4.0621, -15.846)) / 2);
	cannonpositionsleft.push_back((vec3(8.6325f, 4.3022f, -15.647) + vec3(8.6325f, 4.21f, -15.647)) / 2);
	cannonpositionsleft.push_back((vec3(5.7567, 4.5144f, -15.378) + vec3(5.7567, 4.4223, -15.378)) / 2);
	cannonpositionsleft.push_back((vec3(2.9597f, 4.8406f, -14.958) + vec3(2.9597f, 4.7485f, -14.958)) / 2);
	cannonpositionsleft.push_back((vec3(0.051582f, 5.2053f, -14.597) + vec3(0.051582f, 5.1132f, -14.597)) / 2);

}

//Mise ? jour des boules du canon pendant le vol
void cannonball::updateball(float dt) {
	speed = speed + dt * g;
	position=position + dt * speed;
	cannongen->Update(dt, 2, position, cgp::normalize(- speed));
}
void cannonballgenerator::drawballs(float dt, cgp::scene_environment_basic_camera_spherical_coords& environment,cgp::affine_rts &transformation, cgp::affine_rts& ship2_transform,float angle,bool left, bool right) {
	for (int i = 0; i < 9; i++) {
		cannontimersleft[i] -= dt;
		cannontimersright[i] -= dt;

		//Si on appuie sur le bouton K , on tire des projectils ? gauche
		if (cannontimersleft[i] < 0 && left) {
			cannontimersleft[i] = 3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 - 1)));
			cannonball *ball = new cannonball(cannonpositionsleft[i], transformation, angle, cgp::vec3(0, 25, 15));
			ParticleGenerator *p = new ParticleGenerator(shaderprogram, texture_id, 10, ball->position, cgp::normalize(-ball->speed), 1);
			ball->cannongen = p;
			cannonballs.push_back(ball);
			createblast(cannonpositionsleft[i], transformation, angle,cgp::vec3(0,1,0));
		}
		//Si on appuie sur le bouton L , on tire des projectils ? droite
		else if (cannontimersright[i] < 0 && right) {
			cannontimersright[i] = 3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 - 1)));
			cannonball* ball = new cannonball(cannonpositionsleft[i], transformation, angle, cgp::vec3(0, -25, 15));
			ParticleGenerator *p = new ParticleGenerator(shaderprogram, texture_id, 10, ball->position, cgp::normalize(- ball->speed), 1);
			ball->cannongen = p;
			cannonballs.push_back(ball);
			createblast(cannonpositionsright[i], transformation, angle, cgp::vec3(0, -1, 0));
		}
	}
	
	//On supprime les boules de canon rentr?es dans l'eau
	while (cannonballs.size() > 0 && cannonballs.front()->position.z < 0) {
		delete cannonballs.front();
		cannonballs.pop_front();
	}

	//D?tection de collision des boules du canon avec le deuxi?me bateau
	for (auto it = cannonballs.begin(); it != cannonballs.end(); ) {
		cgp::vec3 position = (*it)->position;
		cgp::affine_rts inv = inverse(ship2_transform);
		position = inv * position;
		if ((position.x > -6 && position.x < 29) && (position.y > 0 && position.x < 13) && (position.z > -14 && position.z < -8.7)) {
			it = cannonballs.erase(it);
			std::cout << "Hit detected" << std::endl;
			number_of_hits++;
			if (number_of_hits > 10) {
				other_ship->sink();
			}
		}
			
		if (it != cannonballs.end())
			++it;

	}

	//Mise ? jour et rendu des boules de canons
	for (cannonball *ball : cannonballs)
	{
		ball->updateball(dt);
		cannon_drawable.transform.translation = ball->position;
		draw(cannon_drawable, environment);
		ball->cannongen->Draw(environment, smoke_drawable);
	}
	Draw_Update_Particles(dt, transformation, angle, environment);
}

//Cr?ation de l'effet de flash de museau ? l'instant du tir d'un canon
void cannonballgenerator::createblast(cgp::vec3 theposition, cgp::affine_rts& transformation, float angle,cgp::vec3 Velocity) {

	cgp::rotation_transform RTest = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, angle);
	vec3 speed = RTest * Velocity;
	vec3 position = transformation * theposition;
	
	ParticleGenerator *p = new ParticleGenerator( shaderprogram, texture_id, 10,position,speed,0 );
	BlastParticleGenerator b { p,theposition,Velocity };
	
	particlegenerators.push_back(b);

};

//Mise ? jour et rendu du syst?me de particules de flash de museau
void cannonballgenerator::Draw_Update_Particles(float dt, cgp::affine_rts& transformation, float angle, cgp::scene_environment_basic_camera_spherical_coords& environment){
	while (particlegenerators.size() > 0 && particlegenerators.front().gen->Life < 0) {
		delete particlegenerators.front().gen;
		particlegenerators.pop_front();
	}

	for (int i = 0; i < particlegenerators.size(); i++) {
		ParticleGenerator* p = particlegenerators[i].gen;
		p->Life = p->Life - dt;
		if (p->Life > 0.0f)
		{
			cgp::rotation_transform RTest = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, angle);
			cgp::vec3 speed = RTest * particlegenerators[i].Velocity;
			cgp::vec3 position = transformation * particlegenerators[i].cannon_local_position;
			particlegenerators[i].gen->Update(dt, 2, position, speed);
			particlegenerators[i].gen->Draw(environment, particle_drawable);
		}
		
	}
};
