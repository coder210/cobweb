#ifndef MYDRAW_H
#define MYDRAW_H

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS 1
#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "draw.h"

#include <algorithm>
#include <stdio.h>
#include <thread>
#include <chrono>

#if defined(_WIN32)
#include <crtdbg.h>
#endif

#include <stdlib.h>

#define	RAND_LIMIT 32767

/// Random number in range [-1,1]
inline float RandomFloat()
{
	float r = (float)(rand() & (RAND_LIMIT));
	r /= RAND_LIMIT;
	r = 2.0f * r - 1.0f;
	return r;
}

/// Random floating point number in range [lo, hi]
inline float RandomFloat(float lo, float hi)
{
	float r = (float)(rand() & (RAND_LIMIT));
	r /= RAND_LIMIT;
	r = (hi - lo) * r + lo;
	return r;
}

struct Settings
{
	Settings()
	{
		Reset();
	}

	void Reset()
	{
		m_testIndex = 0;
		m_windowWidth = 1600;
		m_windowHeight = 900;
		m_hertz = 60.0f;
		m_velocityIterations = 8;
		m_positionIterations = 3;
		m_drawShapes = true;
		m_drawJoints = true;
		m_drawAABBs = false;
		m_drawContactPoints = false;
		m_drawContactNormals = false;
		m_drawContactImpulse = false;
		m_drawFrictionImpulse = false;
		m_drawCOMs = false;
		m_drawStats = false;
		m_drawProfile = false;
		m_enableWarmStarting = true;
		m_enableContinuous = true;
		m_enableSubStepping = false;
		m_enableSleep = true;
		m_pause = false;
		m_singleStep = false;
	}

	//void Save();
	//void Load();

	int m_testIndex;
	int m_windowWidth;
	int m_windowHeight;
	float m_hertz;
	int m_velocityIterations;
	int m_positionIterations;
	bool m_drawShapes;
	bool m_drawJoints;
	bool m_drawAABBs;
	bool m_drawContactPoints;
	bool m_drawContactNormals;
	bool m_drawContactImpulse;
	bool m_drawFrictionImpulse;
	bool m_drawCOMs;
	bool m_drawStats;
	bool m_drawProfile;
	bool m_enableWarmStarting;
	bool m_enableContinuous;
	bool m_enableSubStepping;
	bool m_enableSleep;
	bool m_pause;
	bool m_singleStep;
};

const int32 k_maxContactPoints = 2048;

struct ContactPoint
{
	b2Fixture* fixtureA;
	b2Fixture* fixtureB;
	b2Vec2 normal;
	b2Vec2 position;
	b2PointState state;
	float normalImpulse;
	float tangentImpulse;
	float separation;
};

class Test
{
public:
	Test(Settings& settings, b2World* world);
	~Test();

	void DrawTitle(const char* string);
	void Step(float hertz, int32 velocityIterations, int32 positionIterations);
	void UpdateUI() {}
	void Keyboard(int key) { B2_NOT_USED(key); }
	void KeyboardUp(int key) { B2_NOT_USED(key); }
	void ShiftMouseDown(const b2Vec2& p);
	void MouseDown(const b2Vec2& p);
	void MouseUp(const b2Vec2& p);
	void MouseMove(const b2Vec2& p);
	void LaunchBomb();
	void LaunchBomb(const b2Vec2& position, const b2Vec2& velocity);
	
	void SpawnBomb(const b2Vec2& worldPt);
	void CompleteBombSpawn(const b2Vec2& p);

	// Let derived tests know that a joint was destroyed.
	void JointDestroyed(b2Joint* joint) { B2_NOT_USED(joint); }

	void ShiftOrigin(const b2Vec2& newOrigin);

protected:
	b2AABB m_worldAABB;
	ContactPoint m_points[k_maxContactPoints];
	int32 m_pointCount;
	int32 m_textLine;
	Settings m_settings;
	b2World* m_world;
	b2Body* m_bomb;
	b2MouseJoint* m_mouseJoint;
	b2Vec2 m_bombSpawnPoint;
	bool m_bombSpawning;
	b2Vec2 m_mouseWorld;
	int32 m_stepCount;
	int32 m_textIncrement;
	b2Profile m_maxProfile;
	b2Profile m_totalProfile;
};




class MyDrawdebug {
private:
	static char s_buffer[128];
	static std::chrono::duration<double> s_frameTime;
	static std::chrono::duration<double> s_sleepAdjust;
	static GLFWwindow* s_mainWindow;
	static Test* s_test;
	static Settings s_settings;
	static bool MyDrawdebug::s_rightMouseDown;
	static b2Vec2 MyDrawdebug::s_clickPointWS;
private:
	static void glfwErrorCallback(int error, const char* description);
	static void CreateUI(GLFWwindow* window, const char* glslVersion = NULL);
	static void ResizeWindowCallback(GLFWwindow*, int width, int height);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CharCallback(GLFWwindow* window, unsigned int c);
	static void MouseButtonCallback(GLFWwindow* window, int32 button, int32 action, int32 mods);
	static void MouseMotionCallback(GLFWwindow*, double xd, double yd);
	static void ScrollCallback(GLFWwindow* window, double dx, double dy);
	static void UpdateUI();
public:
	static int Start(b2World* world);
	static bool Step(float hertz, int32 velocityIterations, int32 positionIterations);
	static void Clear();
};


#endif
