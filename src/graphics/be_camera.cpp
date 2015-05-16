#include "be_camera.h"
#include <cmath>

using namespace std;

BeCamera::BeCamera() : m_sceneNode(0)
{
	//TODO: remove settings from here, it should be handled at a higher level
	settings = Settings::Instance();
}

btVector3 BeCameraPerspective::getScreenDirection(const float x, const float y)
{

	btTransform tr;
	if(m_sceneNode)
	{
		tr=m_sceneNode->getTransform();
	}
	else
	{
		tr.setIdentity();
	}
	const btVector3& rayFrom = tr.getOrigin();

	btVector3 rayForward( -tr.getBasis()[0][2], -tr.getBasis()[1][2], -tr.getBasis()[2][2]); 
	btVector3 rayUp( tr.getBasis()[0][1], tr.getBasis()[1][1], tr.getBasis()[2][1]); 

	rayForward.normalize();

	rayForward *= m_zFar;

	btVector3 hor(rayForward.cross(rayUp));
	hor.normalize();

	btVector3 vertical(hor.cross(rayForward));
	vertical.normalize();

	const float tanfov( tanf( 0.5f * m_fovy ) );

	hor *= 2.f * m_zFar * tanfov;
	vertical *= 2.f * m_zFar * tanfov;

	hor *= (float)*settings->winWidth / *settings->winHeight;

	btVector3 rayTo = (rayFrom + rayForward) - 0.5f * hor + 0.5f * vertical;
	
	rayTo += x * hor * 1.f / *settings->winWidth;
	rayTo -= y * vertical * 1.f / *settings->winHeight;

	
	
	
	
	return rayTo;
}


btVector3 BeCameraPerspective::getScreenDirection(const float x, const float y, const btTransform& tr)
{
	const btVector3& rayFrom = tr.getOrigin();

	btVector3 rayForward( -tr.getBasis()[0][2], -tr.getBasis()[1][2], -tr.getBasis()[2][2]); 
	btVector3 rayUp( tr.getBasis()[0][1], tr.getBasis()[1][1], tr.getBasis()[2][1]); 

	rayForward.normalize();

	rayForward *= m_zFar;

	btVector3 hor(rayForward.cross(rayUp));
	hor.normalize();

	btVector3 vertical(hor.cross(rayForward));
	vertical.normalize();

	const float tanfov( tanf( 0.5f * m_fovy ) );

	hor *= 2.f * m_zFar * tanfov;
	vertical *= 2.f * m_zFar * tanfov;

	hor *= (float)*settings->winWidth / *settings->winHeight;

	btVector3 rayTo = (rayFrom + rayForward) - 0.5f * hor + 0.5f * vertical;
	
	rayTo += x * hor * 1.f / *settings->winWidth;
	rayTo -= y * vertical * 1.f / *settings->winHeight;

	
	
	
	
	return rayTo;
}






