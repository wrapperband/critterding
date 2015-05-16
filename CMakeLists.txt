cmake_minimum_required(VERSION 2.6)

IF (WIN32)
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
ENDIF (WIN32)




	# build type
			# 	set(CMAKE_BUILD_TYPE Release)
			# 	# compile option (Debug) 
			# 	set(CMAKE_CXX_FLAGS_DEBUG "-g -Og -Wall -Wextra -std=c++0x")
			# 	set(CMAKE_C_FLAGS_DEBUG "-g -Og -Wall -Wextra -std=c++0x")
			# 
			# 	# compile option (Release)
			# 	set(CMAKE_CXX_FLAGS_RELEASE "-W -Wall -Wextra -Ofast -std=c++0x")
			# 	set(CMAKE_C_FLAGS_RELEASE "-W -Wall -Wextra -Ofast -std=c++0x")




# 	SET(INSTALL_LIBS ON)
# 	add_definitions(-DINSTALL_LIBS=OFF)

# PROJECT ( lua )
# include_directories( dependencies/lua-5.1.4/src )
# add_subdirectory( dependencies/lua-5.1.4/src )



# 	PROJECT ( bullet )
	SET(BUILD_SHARED_LIBS OFF)
	SET(BUILD_CPU_DEMOS OFF)
	SET(BUILD_DEMOS OFF)
	SET(BUILD_EXTRAS OFF)
	SET(BUILD_MINICL_OPENCL_DEMOS OFF)
	SET(BUILD_MULTITHREADING OFF)
	SET(USE_GLUT OFF)
	SET(USE_DX11 OFF)
	SET(INSTALL_LIBS OFF)    # THIS DOESNT WORK THE FIRST TIME AFTER REMOVING CMAKECACHE, ALTERED BULLETS CMAKELISTS.TXT:783
# 	SET(INSTALL_EXTRA_LIBS OFF)

	include_directories( dependencies/bullet dependencies/bullet/src )
	add_subdirectory( dependencies/bullet )


# 	ADD_DEFINITIONS( " -Ldependencies/bullet/src/BulletCollision  -Bstatic -lLinearMath -lBulletMultiThreaded -lBulletDynamics -lBulletCollision" )
# 	ADD_DEFINITIONS( -Ldependencies/bullet/src/BulletCollision -Ldependencies/bullet/src/BulletMultiThreaded  )



# PROJECT ( boost )
# include_directories( dependencies/boost )
# PROJECT ( boost )
# include_directories( dependencies/boost dependencies/boost/boost )
# add_subdirectory( dependencies/boost/boost )

PROJECT(tinyxml)
include_directories(dependencies/tinyxml)
add_subdirectory(dependencies/tinyxml)

PROJECT(glew)
# IF(WIN32)
 add_definitions(-DGLEW_STATIC)
# ENDIF(WIN32)

include_directories(dependencies/glew)
add_subdirectory(dependencies/glew)



PROJECT ( bengine )

# # fmod
# SET(LIB_FMODLIBS "") 

FIND_PACKAGE ( OpenGL REQUIRED )
IF (OPENGL_FOUND)
  MESSAGE("OPENGL FOUND")
  MESSAGE(${OPENGL_LIBRARIES})
  SET(bengine_LIBS ${bengine_LIBS} ${OPENGL_LIBRARIES}) 
ELSE (OPENGL_FOUND)
  MESSAGE("OPENGL NOT FOUND")
ENDIF (OPENGL_FOUND)

# fmodex_vc.lib fmod_event.lib  QtGui4.lib QtOpenGL4.lib boost_bzip2.lib boost_iostreams.lib boost_filesystem.lib boost_system.lib 
# -DBOOST_ALL_NO_LIB   -DDOM_DYNAMIC
IF (WIN32)
  link_directories( ${CMAKE_SOURCE_DIR}/win32/lib )
  SET(bengine_WIN32LIBS SDLmain.lib SDL.lib SDL_net.lib SDL_image.lib freetype.lib ws2_32.lib  QtGui4.lib QtOpenGL4.lib ) 
  SET(bengine_WIN32INCL win32/include;src/utils/ftgl;src;dependencies;dependencies/qt/;dependencies/qt/QtOpenGL)
  add_definitions(-D__BT_SKIP_UINT64_H -D_USE_MATH_DEFINES -DFTGL_LIBRARY_STATIC -D_CRT_SECURE_NO_WARNINGS -DBOOST_FILESYSTEM_VERSION=3  )
ENDIF (WIN32)

IF (UNIX)

	add_definitions(-DUNIX -std=c++0x -DBOOST_FILESYSTEM_VERSION=3 -Wl,-Bstatic ) # -DGLEW_STATIC 
# 	ADD_DEFINITIONS( "  -rdynamic -Ldependencies/bullet/src/BulletSoftBody -Ldependencies/bullet/src/BulletDynamics -Ldependencies/bullet/src/BulletCollision -Ldependencies/bullet/src/LinearMath -Wl,-Bstatic -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath" )


# 	IF ( DISABLE_FMOD )
# 	ELSE ( DISABLE_FMOD )
# 		# 32 BIT
# 		FIND_LIBRARY( LIB_FMODEX NAMES libfmodex.so )
# 		IF (LIB_FMODEX)
# 		MESSAGE("FMODEX FOUND")
# 		MESSAGE(${LIB_FMODEX})
# 		SET(LIB_FMODLIBS ${LIB_FMODLIBS} ${LIB_FMODEX}) 
# 		ELSE (LIB_FMODEX)
# 			# 64 BIT
# 			FIND_LIBRARY( LIB_FMODEX64 NAMES libfmodex64.so )
# 			IF (LIB_FMODEX64)
# 				MESSAGE("FMODEX FOUND")
# 				MESSAGE(${LIB_FMODEX64})
# 				SET(LIB_FMODLIBS ${LIB_FMODLIBS} ${LIB_FMODEX}) 
# 			ELSE (LIB_FMODEX64)
# 				MESSAGE("FMODEX NOT FOUND")
# 			ENDIF (LIB_FMODEX64)
# 		ENDIF (LIB_FMODEX)
# 
# 		# 32 BIT
# 		FIND_LIBRARY( LIB_FMODEVENT NAMES libfmodevent.so )
# 		IF (LIB_FMODEVENT)
# 		MESSAGE("FMODEVENT FOUND")
# 		MESSAGE(${LIB_FMODEVENT})
# 		add_definitions(-DENABLE_FMOD)
# 		SET(LIB_FMODLIBS ${LIB_FMODLIBS} ${LIB_FMODEVENT}) 
# 		ELSE (LIB_FMODEVENT)
# 			# 64 BIT
# 			FIND_LIBRARY( LIB_FMODEVENT64 NAMES libfmodevent64.so )
# 			IF (LIB_FMODEVENT64)
# 				MESSAGE("FMODEVENT FOUND")
# 				MESSAGE(${LIB_FMODEVENT64})
# 				add_definitions(-DENABLE_FMOD)
# 				SET(LIB_FMODLIBS ${LIB_FMODLIBS} ${LIB_FMODEVENT64}) 
# 			ELSE (LIB_FMODEVENT64)
# 				MESSAGE("FMODEVENT NOT FOUND")
# 			ENDIF (LIB_FMODEVENT64)
# 		ENDIF (LIB_FMODEVENT)
# 	ENDIF (DISABLE_FMOD)

	FIND_PACKAGE ( SDL REQUIRED )
	IF (SDL_FOUND)
	MESSAGE("SDL FOUND")
	MESSAGE(${SDL_LIBRARY})
	SET(bengine_LIBS ${bengine_LIBS} ${SDL_LIBRARY}) 
	Include_Directories (${SDL_INCLUDE_DIR})
	ELSE (SDL_FOUND)
	MESSAGE("SDL NOT FOUND")
	ENDIF (SDL_FOUND)

# 	FIND_PACKAGE ( SDL_image REQUIRED )
# 	IF (SDLIMAGE_FOUND)
# 	MESSAGE("SDLIMAGE FOUND")
# 	MESSAGE(${SDLIMAGE_LIBRARY})
# 	SET(bengine_LIBS ${bengine_LIBS} ${SDLIMAGE_LIBRARY}) 
# 	ELSE (SDLIMAGE_FOUND)
# 	MESSAGE("SDLIMAGE NOT FOUND")
# 	ENDIF (SDLIMAGE_FOUND)

# 		SET(Boost_USE_MULTITHREAD ON) #set a flag
# 		FIND_PACKAGE(Boost)
# # 		FIND_PACKAGE ( Boost COMPONENTS filesystem system REQUIRED)
# 		IF (Boost_FOUND)
# 			MESSAGE("BOOST FOUND")
# 			MESSAGE(${Boost_INCLUDE_DIRS})
# 			SET(bengine_INCLUDES ${bengine_INCLUDES} ${Boost_INCLUDE_DIRS}/boost) 
# # 			INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
# 			ADD_DEFINITIONS( " -lboost_thread" )
# 		ENDIF()
# 	FIND_PACKAGE ( Boost 1.4.3 COMPONENTS filesystem thread REQUIRED )
# 	IF (Boost_FOUND)
# 	MESSAGE("Boost FOUND")
# 	MESSAGE(${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
# 	SET(bengine_LIBS ${bengine_LIBS} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY}) 
# 	ELSE (Boost_FOUND)
# 	MESSAGE("Boost NOT FOUND")
# 	ENDIF (Boost_FOUND)


	find_package(Qt4 4.7 COMPONENTS QtCore QtGui QtOpenGL QtDeclarative REQUIRED )
	IF (QT_FOUND)
		MESSAGE("QT 4.7 FOUND")
		INCLUDE( ${QT_USE_FILE} ) # macro
		MESSAGE(${QT_LIBRARIES})
		SET(bengine_LIBS ${bengine_LIBS} ${QT_LIBRARIES}) 
	ELSE (QT_FOUND)
		MESSAGE(FATAL_ERROR "QT NOT FOUND")
	ENDIF (QT_FOUND)


		SET(Boost_USE_MULTITHREAD ON) #set a flag
# 		FIND_PACKAGE(Boost)
		FIND_PACKAGE ( Boost COMPONENTS filesystem system iostreams thread REQUIRED)
		IF (Boost_FOUND)
			MESSAGE("BOOST FOUND")
			MESSAGE(${Boost_INCLUDE_DIRS})
			SET(bengine_INCLUDES ${bengine_INCLUDES} ${Boost_INCLUDE_DIRS}/boost) 
# 			INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
# 			ADD_DEFINITIONS( " -lboost_thread -lboost_system" )
# 			ADD_DEFINITIONS(  )
			SET(bengine_LIBS ${bengine_LIBS} ${BOOST_LIBRARIES}) 
			SET(bengine_LIBS ${bengine_LIBS} "-lboost_system -lboost_iostreams") 
			
		ENDIF()

# 	FIND_PACKAGE ( SDL_net REQUIRED )
# 	IF (SDLNET_FOUND)
# 	MESSAGE("SDLNET FOUND")
# 	MESSAGE(${SDLNET_LIBRARY})
# 	SET(bengine_LIBS ${bengine_LIBS} ${SDLNET_LIBRARY}) 
# 	ELSE (SDLNET_FOUND)
# 	MESSAGE("SDLNET NOT FOUND")
# 	ENDIF (SDLNET_FOUND)

# 	FIND_PACKAGE ( PNG REQUIRED )
# 	IF (PNG_FOUND)
# 	MESSAGE("PNG FOUND")
# 	MESSAGE(${PNG_LIBRARY})
# 	SET(bengine_LIBS ${bengine_LIBS} ${PNG_LIBRARY}) 
# 	ELSE (PNG_FOUND)
# 	MESSAGE("PNG NOT FOUND")
# 	ENDIF (PNG_FOUND)

	FIND_PACKAGE ( Freetype REQUIRED )
	IF (FREETYPE_FOUND)
	MESSAGE("FREETYPE FOUND")
	MESSAGE(${FREETYPE_LIBRARY})
	SET(bengine_LIBS ${bengine_LIBS} ${FREETYPE_LIBRARY}) 
	SET(bengine_INCLUDES ${bengine_INCLUDES} ${FREETYPE_INCLUDE_DIRS}) 
	SET(bengine_INCLUDES ${bengine_INCLUDES} "${FREETYPE_INCLUDE_DIRS}/freetype2") 
	ELSE (FREETYPE_FOUND)
	MESSAGE("FREETYPE NOT FOUND")
	ENDIF (FREETYPE_FOUND)
ENDIF (UNIX)

	FIND_PACKAGE( OpenMP )
	if(OPENMP_FOUND)
	message("OPENMP FOUND")
# 	message("OPENMP LINKER")
# 	MESSAGE("${OpenMP_EXE_LINKER_FLAGS}")
# 	message("OPENMP CXX")
# 	MESSAGE("${OpenMP_CXX_FLAGS}")
	ADD_DEFINITIONS( ${OpenMP_CXX_FLAGS} )
# 	ADD_DEFINITIONS( " -lgomp" )
	SET(OpenMP_EXE_LINKER_FLAGS ${OpenMP_EXE_LINKER_FLAGS} "-lgomp")
	SET(bengine_LIBS ${bengine_LIBS} ${OpenMP_EXE_LINKER_FLAGS}) 
# 	set(bengine_LIBS ${bengine_LIBS} ${OpenMP_EXE_LINKER_FLAGS})
# 	set(bengine_INCLUDES ${bengine_INCLUDES} ${OpenMP_CXX_FLAGS})
# 	set(bengine_LIBS "${bengine_LIBS} ${OpenMP_EXE_LINKER_FLAGS}")
# 	SET(HAVE_OPENMP 1)
	add_definitions( -DHAVE_OPENMP=1 )
	ENDIF(OPENMP_FOUND)


# FIXME
# FIND_PACKAGE ( FTGL )
# IF (FTGL_FOUND)
#   MESSAGE("FTGL FOUND")
#   MESSAGE(${FTGL_LIBRARY})
#   SET(bengine_LIBS ${bengine_LIBS} ${FTGL_LIBRARY}) 
#   SET(bengine_LIBS ${bengine_LIBS} ${FTGL_LIBRARY}) 
#   SET(bengine_LIBS ${bengine_LIBS} ${FTGL_LIBRARY}) 
# ELSE (FTGL_FOUND)
#   MESSAGE("FTGL NOT FOUND")
# ENDIF (FTGL_FOUND)

link_libraries (
   ${bengine_LIBS}
)
LINK_DIRECTORIES (
   ${bengine_LIBS_DIRS} 
)

SET(bengine_INCLUDES "${CMAKE_SOURCE_DIR};dependencies/bullet;dependencies/tinyxml;dependencies/;${FREETYPE_INCLUDE_DIRS};./${bengine_WIN32INCL}") 
# SET(bengine_INCLUDES "${CMAKE_SOURCE_DIR}/ ./ src/utils/bullet") 
#   MESSAGE(${bengine_INCLUDES})

# CMAKEDEMO_SOURCE_DIR
include_directories( ${bengine_INCLUDES} )

#find_package(KDE4 REQUIRED)

#add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

#include(KDE4Defaults)

#include(MacroLibrary)

#include(ConvenienceLibs.cmake)

#include(ManualStuff.cmake)

#include(ConfigureChecks.cmake)

#include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

add_subdirectory(src)


########### install files ###############

# install(FILES share/critterding/pixmaps/cd.bmp DESTINATION share/critterding/pixmaps/ )
# install(FILES share/critterding/font.ttf DESTINATION share/critterding/ )
# install(FILES share/critterding/settings.xml DESTINATION share/critterding/ )
# INSTALL(DIRECTORY share/critterding/maps DESTINATION share/critterding/ )
# INSTALL(DIRECTORY share/critterding/critter DESTINATION share/critterding/ )
# INSTALL(DIRECTORY share/critterding/food DESTINATION share/critterding/ )
# INSTALL(DIRECTORY share/critterding/pixmaps DESTINATION share/critterding/ )
# INSTALL(DIRECTORY share/critterding/shaders DESTINATION share/critterding/ )

INSTALL(DIRECTORY share/critterding DESTINATION share/ )