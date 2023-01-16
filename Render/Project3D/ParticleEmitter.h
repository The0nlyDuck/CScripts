#pragma once
#include <glm/glm.hpp>
#include "gl_core_4_4.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

struct Particle {
	vec3  position;
	vec3  velocity;
	vec4  colour;
	float size;
	float lifetime;
	float lifespan;
};

struct ParticleVertex {
	vec4  position;
	vec4  colour;
};
class ParticleEmitter
{
public:
	ParticleEmitter();
	virtual ~ParticleEmitter();

	void initialise(unsigned int a_maxParticles, unsigned int a_emitRate, float a_lifetimeMin, float a_lifetimeMax, float a_velocityMin, float a_velocityMax, float a_startSize, float a_endSize,
		const vec4& a_startColour, const vec4& a_endColour);
	void emit();

	void update(float a_deltaTime, const mat4& a_cameraTransform, vec3 position = vec3(0));
	void draw();

protected:

	Particle* m_particles;
	unsigned int m_firstDead;
	unsigned int m_maxParticles;

	unsigned int m_vao, m_vbo, m_ibo;
	ParticleVertex* m_vertexData;

	vec3   m_position;

	float  m_emitTimer;
	float  m_emitRate;

	float  m_lifespanMin;
	float  m_lifespanMax;

	float  m_velocityMin;
	float  m_velocityMax;

	float  m_startSize;
	float  m_endSize;

	vec4   m_startColour;
	vec4   m_endColour;
};

