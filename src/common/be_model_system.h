#ifndef BE_MODEL_SYSTEM_H_INCLUDED
#define BE_MODEL_SYSTEM_H_INCLUDED

#include <GL/glew.h>
#include <vector>
#include <unordered_map>
#include "common/be_obj_loader.h"

#include "graphics/be_graphics_system.h"
#include "graphics/be_buffer.h"

#include "filesystem/be_filesystem.h"

#include "resource/be_texture_2d_resource.h"
#include "net/be_net_message.h"

#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>

class BeGeometry
{
private:
    typedef std::unordered_map< std::string, Material > MaterialMap;
public:
	BeGeometry() :
	m_numberVertices(0),
	m_numberVIndices(0)
	{
    }
	~BeGeometry() {}

	  void setNumberVertices( unsigned int numberVertices ) 
	  { 
		  m_numberVertices = numberVertices;
		  vertices.reset(new btScalar[m_numberVertices]);
	  }

	  unsigned int		getNumberVertices() const { return m_numberVertices; }

	  void setNumberVIndices( unsigned int numberVIndices ) 
	  { 
		  m_numberVIndices = numberVIndices;
		  indices.reset(new int[m_numberVIndices]);
	  }
	  unsigned int		getNumberVIndices() const { return m_numberVIndices; }

	  boost::scoped_array<btScalar> vertices;
	  boost::scoped_array<int> indices; 

	  MaterialMap m_materialMap;
	  std::vector<Facematerial> facematerials;

	  std::vector<GLint> elementArrayBuffer;

	  struct DrawCall
	  {
		  unsigned int m_first;
		  unsigned int m_count;
	  };

	  std::vector<DrawCall> m_drawCalls;

	  typedef std::unordered_map <std::string, Material>::const_iterator matlist_iterator;

      void buildElementArrayBuffer();

private:
	unsigned int m_numberVertices;
	unsigned int m_numberVIndices;
};

class BeGeometrySystem
{
public:
	boost::shared_ptr<BeGeometry> load( BeFilesystem& filesystem, const std::string& filename, const btVector3& scale, const btTransform& transform);
private:
// 	typedef std::unordered_map<std::string, boost::shared_ptr<BeGeometry> > ResourceMap;
// 	ResourceMap m_resources;
};


class BeGraphicsModel
{
	friend class BeGraphicsModelSystem;
	friend class BeGraphicsModelResource;
public:
	BeGraphicsModel( boost::shared_ptr<BeGraphicsSystem> system, const std::string& filename );
	
	~BeGraphicsModel() {}

		const std::string&	getFilename() const { return m_filename; }
		
		void setNumberVertices( unsigned int numberVertices ) 
		{ 
			m_numberVertices = numberVertices;
			vertices.reset(new btScalar[m_numberVertices]);
		}

		unsigned int		getNumberVertices() const { return m_numberVertices; }

		void setNumberNormals( unsigned int numberNormals ) 
		{ 
			m_numberNormals = numberNormals;
			normals.reset(new float[m_numberNormals]);
		}

		unsigned int		getNumberNormals() const { return m_numberNormals; }

		void setNumberTexCoor( unsigned int numberTexCoor ) 
		{ 
			m_numberTexCoor = numberTexCoor; 
			texcoor.reset(new float[m_numberTexCoor]);
		}

		unsigned int		getNumberTexCoor() const { return m_numberTexCoor; }

		void setNumberVIndices( unsigned int numberVIndices ) 
		{ 
			m_numberVIndices = numberVIndices;
			indices.reset(new int[m_numberVIndices]);
		}
		unsigned int		getNumberVIndices() const { return m_numberVIndices; }

		boost::scoped_array<btScalar> vertices;
		boost::scoped_array<float> normals;
		boost::scoped_array<float> texcoor;
		boost::scoped_array<int> indices; 

		std::vector<GLint> elementArrayBuffer;

		std::unordered_map<std::string, Material> matlist;
		std::vector<Facematerial> facematerials;

		virtual void		setContext( unsigned int current_material );
		virtual void		disableContext();
		unsigned int 		numberOfMaterials();
		virtual void		draw( unsigned int current_material, const btTransform& transform );
		virtual void		drawToDepth( const btTransform& transform );
		virtual void		draw( unsigned int current_material, const float* transform );
// 		virtual void		draw( const float* transform, const float x, const float y, const float z );
		virtual void		draw( unsigned int current_material, const float* transform, const btVector3& scale );
		virtual void		draw( unsigned int current_material, const btTransform& transform, const btVector3& scale );
		virtual void		drawToDepth( const float* transform, const btVector3& scale );
// 		virtual void		draw( const std::vector<const btTransform*>& transformlist );
		void				draw( unsigned int current_material );
		void				drawToDepth();

private:
// 	boost::shared_ptr<BeTexture2DResource> m_imageTexture2D;
	btScalar		m_matrix[16];
	void			upload(Texturestore& textureStore);
	void 			buildElementArrayBuffer();
		std::string		m_filename;

		unsigned int m_numberVertices;
		unsigned int m_numberNormals;
		unsigned int m_numberTexCoor;
		unsigned int m_numberVIndices;
		
		boost::scoped_ptr<BeArrayBuffer>	m_arrayBuffer;
		boost::scoped_ptr<BeElementArrayBuffer>	m_elementArrayBuffer;
		boost::scoped_ptr<BeVertexArray>	m_vertexArray;
		GLint tex1Loc;
public:
		typedef std::unordered_map <std::string, Material>::const_iterator matlist_iterator;
		matlist_iterator matit;

		struct DrawCall
		{
			unsigned int m_first;
			unsigned int m_count;
		};

		std::vector<DrawCall> m_drawCalls;

		void loadMaterialfile( const std::string& mpath, const std::string& file );

		boost::shared_ptr<BeGraphicsSystem> m_system;
private:
		const unsigned int m_glint_size;
		const int* m_glsl;
};

class BeGraphicsModelResource;

class BeGraphicsModelSystem
{
	friend class BeGraphicsModelResource;
	public:
		BeGraphicsModelSystem(BeFilesystem& filesystem) : m_filesystem(filesystem), m_textureStore()
		{
		}
		boost::shared_ptr<BeGraphicsModelResource> load( const std::string& filename, boost::shared_ptr<BeGraphicsSystem> graphicsSystem, boost::shared_ptr<BeGraphicsModelSystem> instance, const btVector3& scale, const btTransform& transform );
	private:
		BeFilesystem& m_filesystem;
// 		boost::shared_ptr<BeWorkerPool> m_workerPool;
		Texturestore m_textureStore;
// 		typedef std::unordered_map<std::string, boost::shared_ptr<BeGraphicsModelResource> > ResourceMap;
// 		ResourceMap m_resources;
};

class BeGraphicsEffect
{
public:
	BeGraphicsEffect(BeFilesystem& fileSystem) : m_fileSystem(fileSystem)
	{
		m_program = boost::shared_ptr<BeProgram>(new BeProgram);
	}

	BeGraphicsEffect(BeFilesystem& fileSystem, const std::string& vertexShaderPath, const std::string& fragmentShaderPath) : m_fileSystem(fileSystem)
	{
		m_program = boost::shared_ptr<BeProgram>(new BeProgram);
		loadCompileAndAttachVertexShader(vertexShaderPath);
		loadCompileAndAttachFragmentShader(fragmentShaderPath);
		link();
	}

	~BeGraphicsEffect()
	{
	}

	void loadCompileAndAttachVertexShader(const std::string& path)
	{
		BeFile vertexShaderFile;
		if ( m_fileSystem.load( vertexShaderFile, path) )
		{
			BeVertexShader vertexShader;
// 			std::string string;
// 			vertexShaderFile.getFile(string);
			std::string string( vertexShaderFile.getContent().str() );
			vertexShader.source(string);
			vertexShader.compile();
			m_program->attachShader(&vertexShader);
		}
	}

	void loadCompileAndAttachFragmentShader(const std::string& path)
	{
		BeFile fragmentShaderFile;
		if ( m_fileSystem.load( fragmentShaderFile, path) )
		{
			BeFragmentShader fragmentShader;
// 			std::string string;
// 			fragmentShaderFile.getFile(string);
			std::string string( fragmentShaderFile.getContent().str() );
			fragmentShader.source(string);
			fragmentShader.compile();
			m_program->attachShader(&fragmentShader);
		}
	}

	void link()
	{
		m_program->link();
	}

	boost::shared_ptr<BeProgram> m_program;
	BeFilesystem& m_fileSystem;
};

#endif
