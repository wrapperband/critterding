#ifndef BE_LIGHT_H_INCLUDED
#define BE_LIGHT_H_INCLUDED

#include "src/math/vector3f.h"
#include "src/math/vector4f.h"

class BeLight
{
	friend class BeGraphicsSystem;
protected:
	BeLight() : 
	m_ambient(1,1,1,1),
	m_diffuse(1,1,1,1),
	m_specular(0,0,0,1),
	m_position(0,0,1,0),
	m_spotDirection(0,0,-1),
	m_spotExponent(0),
	m_spotCutoff(180),
	m_attenuation(1,0,0)
	{
	}
public:
	void setAmbient(const Vector4f& ambient) { m_ambient=ambient; }
	void setDiffuse(const Vector4f& diffuse) { m_diffuse=diffuse; }
	void setSpecular(const Vector4f& specular) { m_specular=specular; }
	const Vector4f& getAmbient() const { return m_ambient; }
	const Vector4f& getDiffuse() const { return m_diffuse; }
	const Vector4f& getSpecular() const { return m_specular; }

protected:
	Vector4f	m_ambient;
	Vector4f	m_diffuse;
	Vector4f	m_specular;
	Vector4f	m_position;
	Vector3f	m_spotDirection;
	float		m_spotExponent;
	float		m_spotCutoff;
	Vector3f	m_attenuation;
};

class BeDirectionalLight : public BeLight
{
public:
	BeDirectionalLight()
	{
	}
	void setDirection(const Vector3f& direction)
	{
		m_position.setX(direction.x);
		m_position.setY(direction.y);
		m_position.setZ(direction.z);
		m_position.setW(0);
	}
	Vector3f getDirection() const
	{
		return Vector3f(m_position.getX(), m_position.getY(), m_position.getZ());
	}
};

class BePositionalLight : public BeLight
{
public:
	BePositionalLight()
	{
	}
	void setPosition(const Vector3f& position)
	{
		m_position.setX(position.x);
		m_position.setY(position.y);
		m_position.setZ(position.z);
		m_position.setW(1);
	}
	Vector3f getPosition() const
	{
		return Vector3f(m_position.getX(), m_position.getY(), m_position.getZ());
	}
	void setAttenuation(const Vector3f& attenuation) { m_attenuation=attenuation; }
	const Vector3f& getAttenuation() const { return m_attenuation; }
};

class BeSpotLight : public BePositionalLight
{
public:
	BeSpotLight()
	{
	}
	void setDirection(const Vector3f& direction) { m_spotDirection=direction; }
	const Vector3f& getDirection() const { return m_spotDirection; }
	void setExponent(const float exponent) { m_spotExponent=exponent; }
	float getExponent() const { return m_spotExponent; }
	void setCutoff(const float cutoff) { m_spotCutoff=cutoff; }
	float getCutoff() const { return m_spotCutoff; }
};

#endif
