#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Audio/AudioManager.o \
	${OBJECTDIR}/Audio/LinAudio.o \
	${OBJECTDIR}/ComponentSystem/Entity/Entity.o \
	${OBJECTDIR}/ComponentSystem/EntityFactory.o \
	${OBJECTDIR}/ComponentSystem/Filter/ComponentFilter.o \
	${OBJECTDIR}/ComponentSystem/System/AudioSystem.o \
	${OBJECTDIR}/ComponentSystem/System/BlockSystem.o \
	${OBJECTDIR}/ComponentSystem/System/CollisionDamageSystem.o \
	${OBJECTDIR}/ComponentSystem/System/CollisionDeflectionSystem.o \
	${OBJECTDIR}/ComponentSystem/System/EffectSystem.o \
	${OBJECTDIR}/ComponentSystem/System/ISystem.o \
	${OBJECTDIR}/ComponentSystem/System/InputSystem.o \
	${OBJECTDIR}/ComponentSystem/System/LightSystem.o \
	${OBJECTDIR}/ComponentSystem/System/LoseLifeSystem.o \
	${OBJECTDIR}/ComponentSystem/System/ModelSystem.o \
	${OBJECTDIR}/ComponentSystem/System/MovementSystem.o \
	${OBJECTDIR}/ComponentSystem/System/PhysicsSystem.o \
	${OBJECTDIR}/ComponentSystem/System/ProjectileSystem.o \
	${OBJECTDIR}/ComponentSystem/System/RespawnBallSystem.o \
	${OBJECTDIR}/ComponentSystem/System/ScoreSystem.o \
	${OBJECTDIR}/ComponentSystem/System/TextSystem.o \
	${OBJECTDIR}/ComponentSystem/World.o \
	${OBJECTDIR}/Graphics/GraphicsManager.o \
	${OBJECTDIR}/Graphics/ICamera.o \
	${OBJECTDIR}/Graphics/IGraphics.o \
	${OBJECTDIR}/Graphics/OpenGL/GLCamera.o \
	${OBJECTDIR}/Graphics/OpenGL/GLGraphics.o \
	${OBJECTDIR}/Graphics/OpenGL/GLParticleSystem.o \
	${OBJECTDIR}/Graphics/OpenGL/GLShaderHandler.o \
	${OBJECTDIR}/Graphics/OpenGL/GLWindow.o \
	${OBJECTDIR}/Input/InputManager.o \
	${OBJECTDIR}/Input/Linux/LinInput.o \
	${OBJECTDIR}/MathHelper.o \
	${OBJECTDIR}/Program.o \
	${OBJECTDIR}/Scene/SceneManager.o \
	${OBJECTDIR}/Scenes/GameOverScene.o \
	${OBJECTDIR}/Scenes/GameScene.o \
	${OBJECTDIR}/Scenes/MainMenuScene.o \
	${OBJECTDIR}/Storage/FileManager.o \
	${OBJECTDIR}/Storage/ModelData.o \
	${OBJECTDIR}/Storage/ModelLoader.o \
	${OBJECTDIR}/Storage/MusicLoader.o \
	${OBJECTDIR}/Storage/WavLoader.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++11
CXXFLAGS=-std=c++11

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../External/lib -L../../External/lib/OpenGL -lBox2D

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/breakout

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/breakout: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/breakout ${OBJECTFILES} ${LDLIBSOPTIONS} -lSDL -lSDL_mixer -lGLEW -lGL -lSOIL

${OBJECTDIR}/Audio/AudioManager.o: Audio/AudioManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/Audio
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Audio/AudioManager.o Audio/AudioManager.cpp

${OBJECTDIR}/Audio/LinAudio.o: Audio/LinAudio.cpp 
	${MKDIR} -p ${OBJECTDIR}/Audio
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Audio/LinAudio.o Audio/LinAudio.cpp

${OBJECTDIR}/ComponentSystem/Entity/Entity.o: ComponentSystem/Entity/Entity.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/Entity
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/Entity/Entity.o ComponentSystem/Entity/Entity.cpp

${OBJECTDIR}/ComponentSystem/EntityFactory.o: ComponentSystem/EntityFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/EntityFactory.o ComponentSystem/EntityFactory.cpp

${OBJECTDIR}/ComponentSystem/Filter/ComponentFilter.o: ComponentSystem/Filter/ComponentFilter.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/Filter
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/Filter/ComponentFilter.o ComponentSystem/Filter/ComponentFilter.cpp

${OBJECTDIR}/ComponentSystem/System/AudioSystem.o: ComponentSystem/System/AudioSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/AudioSystem.o ComponentSystem/System/AudioSystem.cpp

${OBJECTDIR}/ComponentSystem/System/BlockSystem.o: ComponentSystem/System/BlockSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/BlockSystem.o ComponentSystem/System/BlockSystem.cpp

${OBJECTDIR}/ComponentSystem/System/CollisionDamageSystem.o: ComponentSystem/System/CollisionDamageSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/CollisionDamageSystem.o ComponentSystem/System/CollisionDamageSystem.cpp

${OBJECTDIR}/ComponentSystem/System/CollisionDeflectionSystem.o: ComponentSystem/System/CollisionDeflectionSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/CollisionDeflectionSystem.o ComponentSystem/System/CollisionDeflectionSystem.cpp

${OBJECTDIR}/ComponentSystem/System/EffectSystem.o: ComponentSystem/System/EffectSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/EffectSystem.o ComponentSystem/System/EffectSystem.cpp

${OBJECTDIR}/ComponentSystem/System/ISystem.o: ComponentSystem/System/ISystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/ISystem.o ComponentSystem/System/ISystem.cpp

${OBJECTDIR}/ComponentSystem/System/InputSystem.o: ComponentSystem/System/InputSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/InputSystem.o ComponentSystem/System/InputSystem.cpp

${OBJECTDIR}/ComponentSystem/System/LightSystem.o: ComponentSystem/System/LightSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/LightSystem.o ComponentSystem/System/LightSystem.cpp

${OBJECTDIR}/ComponentSystem/System/LoseLifeSystem.o: ComponentSystem/System/LoseLifeSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/LoseLifeSystem.o ComponentSystem/System/LoseLifeSystem.cpp

${OBJECTDIR}/ComponentSystem/System/ModelSystem.o: ComponentSystem/System/ModelSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/ModelSystem.o ComponentSystem/System/ModelSystem.cpp

${OBJECTDIR}/ComponentSystem/System/MovementSystem.o: ComponentSystem/System/MovementSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/MovementSystem.o ComponentSystem/System/MovementSystem.cpp

${OBJECTDIR}/ComponentSystem/System/PhysicsSystem.o: ComponentSystem/System/PhysicsSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/PhysicsSystem.o ComponentSystem/System/PhysicsSystem.cpp

${OBJECTDIR}/ComponentSystem/System/ProjectileSystem.o: ComponentSystem/System/ProjectileSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/ProjectileSystem.o ComponentSystem/System/ProjectileSystem.cpp

${OBJECTDIR}/ComponentSystem/System/RespawnBallSystem.o: ComponentSystem/System/RespawnBallSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/RespawnBallSystem.o ComponentSystem/System/RespawnBallSystem.cpp

${OBJECTDIR}/ComponentSystem/System/ScoreSystem.o: ComponentSystem/System/ScoreSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/ScoreSystem.o ComponentSystem/System/ScoreSystem.cpp

${OBJECTDIR}/ComponentSystem/System/TextSystem.o: ComponentSystem/System/TextSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem/System
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/System/TextSystem.o ComponentSystem/System/TextSystem.cpp

${OBJECTDIR}/ComponentSystem/World.o: ComponentSystem/World.cpp 
	${MKDIR} -p ${OBJECTDIR}/ComponentSystem
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComponentSystem/World.o ComponentSystem/World.cpp

${OBJECTDIR}/Graphics/GraphicsManager.o: Graphics/GraphicsManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/Graphics
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Graphics/GraphicsManager.o Graphics/GraphicsManager.cpp

${OBJECTDIR}/Graphics/ICamera.o: Graphics/ICamera.cpp 
	${MKDIR} -p ${OBJECTDIR}/Graphics
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Graphics/ICamera.o Graphics/ICamera.cpp

${OBJECTDIR}/Graphics/IGraphics.o: Graphics/IGraphics.cpp 
	${MKDIR} -p ${OBJECTDIR}/Graphics
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Graphics/IGraphics.o Graphics/IGraphics.cpp

${OBJECTDIR}/Graphics/OpenGL/GLCamera.o: Graphics/OpenGL/GLCamera.cpp 
	${MKDIR} -p ${OBJECTDIR}/Graphics/OpenGL
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Graphics/OpenGL/GLCamera.o Graphics/OpenGL/GLCamera.cpp

${OBJECTDIR}/Graphics/OpenGL/GLGraphics.o: Graphics/OpenGL/GLGraphics.cpp 
	${MKDIR} -p ${OBJECTDIR}/Graphics/OpenGL
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Graphics/OpenGL/GLGraphics.o Graphics/OpenGL/GLGraphics.cpp

${OBJECTDIR}/Graphics/OpenGL/GLParticleSystem.o: Graphics/OpenGL/GLParticleSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/Graphics/OpenGL
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Graphics/OpenGL/GLParticleSystem.o Graphics/OpenGL/GLParticleSystem.cpp

${OBJECTDIR}/Graphics/OpenGL/GLShaderHandler.o: Graphics/OpenGL/GLShaderHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/Graphics/OpenGL
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Graphics/OpenGL/GLShaderHandler.o Graphics/OpenGL/GLShaderHandler.cpp

${OBJECTDIR}/Graphics/OpenGL/GLWindow.o: Graphics/OpenGL/GLWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}/Graphics/OpenGL
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Graphics/OpenGL/GLWindow.o Graphics/OpenGL/GLWindow.cpp

${OBJECTDIR}/Input/InputManager.o: Input/InputManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/Input
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Input/InputManager.o Input/InputManager.cpp

${OBJECTDIR}/Input/Linux/LinInput.o: Input/Linux/LinInput.cpp 
	${MKDIR} -p ${OBJECTDIR}/Input/Linux
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Input/Linux/LinInput.o Input/Linux/LinInput.cpp

${OBJECTDIR}/MathHelper.o: MathHelper.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MathHelper.o MathHelper.cpp

${OBJECTDIR}/Program.o: Program.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Program.o Program.cpp

${OBJECTDIR}/Scene/SceneManager.o: Scene/SceneManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/Scene
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Scene/SceneManager.o Scene/SceneManager.cpp

${OBJECTDIR}/Scenes/GameOverScene.o: Scenes/GameOverScene.cpp 
	${MKDIR} -p ${OBJECTDIR}/Scenes
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Scenes/GameOverScene.o Scenes/GameOverScene.cpp

${OBJECTDIR}/Scenes/GameScene.o: Scenes/GameScene.cpp 
	${MKDIR} -p ${OBJECTDIR}/Scenes
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Scenes/GameScene.o Scenes/GameScene.cpp

${OBJECTDIR}/Scenes/MainMenuScene.o: Scenes/MainMenuScene.cpp 
	${MKDIR} -p ${OBJECTDIR}/Scenes
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Scenes/MainMenuScene.o Scenes/MainMenuScene.cpp

${OBJECTDIR}/Storage/FileManager.o: Storage/FileManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/Storage
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Storage/FileManager.o Storage/FileManager.cpp

${OBJECTDIR}/Storage/ModelData.o: Storage/ModelData.cpp 
	${MKDIR} -p ${OBJECTDIR}/Storage
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Storage/ModelData.o Storage/ModelData.cpp

${OBJECTDIR}/Storage/ModelLoader.o: Storage/ModelLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/Storage
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Storage/ModelLoader.o Storage/ModelLoader.cpp

${OBJECTDIR}/Storage/MusicLoader.o: Storage/MusicLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/Storage
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Storage/MusicLoader.o Storage/MusicLoader.cpp

${OBJECTDIR}/Storage/WavLoader.o: Storage/WavLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/Storage
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../External/include -I../../External/include/Box2D -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Storage/WavLoader.o Storage/WavLoader.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/breakout

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
