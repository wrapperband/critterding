#ifndef BE_GRAPHICS_SYSTEM_H_INCLUDED
#define BE_GRAPHICS_SYSTEM_H_INCLUDED

#include "be_framebuffer.h"
#include "be_vertex_array.h"
#include "be_texture.h"
#include "be_shader.h"
#include "be_mesh.h"
#include "be_light.h"
#include "be_material.h"
#include "be_buffer.h"
#include "be_camera.h"

#include "src/math/vector3f.h"
#include "src/math/vector4f.h"

#include "tinyxml/tinyxml.h"

#include "common/be_xml_helper.h"
#include "resource/be_texture_2d_resource.h"

#include <unordered_map>


class BeGraphicsMaterial
{
public:
	BeGraphicsMaterial(const int sort=0, const int priority=0, const int depthMask=1, const int depthTest=1, const int blend=0)
	{
		m_sort=sort;
		m_priority=priority;
		m_depthMask=depthMask;
		m_depthTest=depthTest;
		m_blend=blend;
	}
	int getSort() const 
	{
		return m_sort;
	}
	int getPriority() const
	{
		return m_priority;
	}
	int getDepthMask() const
	{
		return m_depthMask;
	}
	int getDepthTest() const
	{
		return m_depthTest;
	}
	int getBlend() const
	{
		return m_blend;
	}
private:
	int m_sort;
	int m_priority;
	int m_depthMask;
	int m_depthTest;
	int m_blend;
};

class BeGraphicsSystem
{
public:
	BeGraphicsSystem(BeFilesystem& filesystem);

	bool loadGraphicsMaterials(const std::string& path)
	{
		//TODO: use BeFileSystem to find the correct path
		TiXmlDocument document;
		const bool result = document.LoadFile( path.c_str() );
		if(result)
		{
			const TiXmlElement* const rootElementPtr=document.RootElement();
			if(rootElementPtr)
			{
				const TiXmlElement& rootElement=(*rootElementPtr);
				const TiXmlElement* elementPtr=rootElement.FirstChildElement();
				while(elementPtr)
				{
					const TiXmlElement& element=(*elementPtr);
					if(element.ValueTStr()=="graphics_material")
					{
						std::string texturePath;
						int sort = 0;
						int priority = 0;
						int depthMask = 1;
						int depthTest = 1;
						int blend = 0;
						BeXmlHelper xmlHelper;
						xmlHelper.addString("texture_path", texturePath);
						xmlHelper.addInt("sort", sort);
						xmlHelper.addInt("priority", priority);
						xmlHelper.addInt("depth_mask", depthMask);
						xmlHelper.addInt("depth_test", depthTest);
						xmlHelper.addInt("blend", blend);
						xmlHelper.parse(element);
						m_graphicsMaterialMap[texturePath]=BeGraphicsMaterial(sort, priority, depthMask, depthTest, blend);
					}
					elementPtr=element.NextSiblingElement();
				}
			}
			return true;
		}
		return false;
	}

	const BeGraphicsMaterial* getEffect(const std::string& name)
	{
		std::unordered_map< std :: string, BeGraphicsMaterial >:: iterator it=m_graphicsMaterialMap.find(name);
		if(it!=m_graphicsMaterialMap.end())
		{
			return &it->second;
		}	
		return 0;
 	}

	void reset2D(const int width, const int height);
	void reset3D(const int offsetX, const int offsetY, const int width, const int height);

	void flush();
	
	void viewport(const int x, const int y, const size_t width, const size_t height)
	{
		glViewport(x, y, width, height);
	}

	void clear(const GLenum mask);
	void enable(const GLenum cap);
	void disable(const GLenum cap);
	void hint(const GLenum target, const GLenum hint);
	void shadeModel(const GLenum mode);
	void cullFace(const GLenum mode);
	void bindFramebuffer(BeFramebuffer* const framebuffer);
	void bindVertexArray(BeVertexArray* const vertexArray);
	void bindTexture2D(BeTexture2D* const texture);
	void bindTexture2DWhitePixel();
	void bindMultiTexture2D(const GLint index, BeTexture2D* const texture );
	void useProgram(BeProgram* const program);
	BeProgram* getActiveProgram() const { return m_program; }
	
	void drawMesh(const BeMesh* const mesh);

	void matrixPush(const GLenum matrixMode);
	void matrixPop(const GLenum matrixMode);
	void matrixLoad(const GLenum matrixMode, GLfloat* const m);
	void matrixLoadIdentity(const GLenum matrixMode);
	void matrixOrtho(const GLenum matrixMode, const double l, const double r, const double b, const double t, const double n, const double f);
	void matrixTranslate(const GLenum matrixMode, const float x, const float y, const float z);
	void matrixRotate(const GLenum matrixMode, const GLfloat angle, const float x, const float y, const float z);
	void matrixScale(const GLenum matrixMode, const float x, const float y, const float z);
	void matrixMult(const GLenum matrixMode, const float* const m);
	void matrixMult(const float* const m);

	void matrixFrustum(const GLenum matrixMode, const double l, const double r, const double b, const double t, const double n, const double f);
	void matrixFrustum(const double l, const double r, const double b, const double t, const double n, const double f);
	void matrixPerspective(const GLenum matrixMode, const float fovy, const float aspect, const float zNear, const float zFar);

	void lightModel(const GLenum pname, const Vector4f& params);
	void lightModel(const GLenum pname, const GLenum param);

	void light(const GLenum lightId, const GLenum pname, const GLfloat* const params);

	void material(const GLenum face, const GLenum pname, const BeColor& params);
	void material(const GLenum face, const GLenum pname, const Vector4f& params);
    void material(const GLenum face, const GLenum pname, const btVector3& params);
	void material(const GLenum face, const GLenum pname, const float param);
	void color( const BeColor& c );

	void applyCamera(const BeCameraPerspective& camera);

	void applyLight(const GLenum lightId, const BeLight* const l);
	void applyMaterial(const GLenum face, const BeMaterial* const m);

	void bindBuffer(const GLenum target, BeBuffer* const buffer);


	//Legacy, to be replaced by VBO/VAO
	void drawQuad()
	{
		glBegin (GL_QUADS);
		glTexCoord2f (0.0, 0.0);
		glVertex3f (0, 1, 0.0);
		glTexCoord2f (1.0, 0.0);
		glVertex3f (1, 1, 0.0);
		glTexCoord2f (1.0, 1.0);
		glVertex3f (1, 0.0, 0.0);
		glTexCoord2f (0.0, 1.0);
		glVertex3f (0.0, 0.0, 0.0);
		glEnd ();
	}

	void setNormalMappingIDs( GLint t, GLint m )
	{
		m_tex1Loc = t;
		m_doNormalMapping = m;
	}
	GLint getNormalMappingTexLoc() const { return m_tex1Loc; }
	GLint getNormalMappingDo() const { return m_doNormalMapping; }
	
private:
	BeGraphicsKernel m_graphicsKernel;
	std::unordered_map<std::string, BeGraphicsMaterial > m_graphicsMaterialMap;

	BeFilesystem& m_filesystem;
	
	// HACK NORMAL MAPPING ID'S
	GLint m_tex1Loc;
	GLint m_doNormalMapping;
	
// 	BeColor m_current_color;
	
	boost::shared_ptr<BeTexture2DResource> m_imageTexture2D;
	
	GLint m_pixel_handle;
	bool m_pixel_load_attempt_done;

	BeProgram* m_program;
	
};

#endif
