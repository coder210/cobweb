#include "mydraw.h"
#include <stdio.h>

DebugDraw g_debugDraw;

Test::Test(Settings& settings, b2World* world)
{
	m_settings = settings;
	m_world = world;
	m_bomb = NULL;
	m_textLine = 30;
	m_textIncrement = 13;
	m_mouseJoint = NULL;
	m_pointCount = 0;

	m_world->SetDebugDraw(&g_debugDraw);
	
	m_bombSpawning = false;

	m_stepCount = 0;

	memset(&m_maxProfile, 0, sizeof(b2Profile));
	memset(&m_totalProfile, 0, sizeof(b2Profile));
}

Test::~Test()
{
	// By deleting the world, we delete the bomb, mouse joint, etc.
	//delete m_world;
	//m_world = NULL;
}


void Test::DrawTitle(const char *string)
{
    g_debugDraw.DrawString(5, 5, string);
    m_textLine = int32(26.0f);
}

class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point)
	{
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture) override
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
};

void Test::MouseDown(const b2Vec2& p)
{
	m_mouseWorld = p;
	
	if (m_mouseJoint != NULL)
	{
		return;
	}

	// Make a small box.
	b2AABB aabb;
	b2Vec2 d;
	d.Set(0.001f, 0.001f);
	aabb.lowerBound = p - d;
	aabb.upperBound = p + d;

	// Query the world for overlapping shapes.
	QueryCallback callback(p);
	m_world->QueryAABB(&callback, aabb);

	if (callback.m_fixture)
	{
		/*float frequencyHz = 5.0f;
		float dampingRatio = 0.7f;

		b2Body* body = callback.m_fixture->GetBody();
		b2MouseJointDef jd;
		jd.bodyA = body;
		jd.bodyB = body;
		jd.target = p;
		jd.maxForce = 1000.0f * body->GetMass();
		b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);

		m_mouseJoint = (b2MouseJoint*)m_world->CreateJoint(&jd);
		body->SetAwake(true);*/
	}
}

void Test::SpawnBomb(const b2Vec2& worldPt)
{
	m_bombSpawnPoint = worldPt;
	m_bombSpawning = true;
}
    
void Test::CompleteBombSpawn(const b2Vec2& p)
{
	if (m_bombSpawning == false)
	{
		return;
	}

	const float multiplier = 30.0f;
	b2Vec2 vel = m_bombSpawnPoint - p;
	vel *= multiplier;
	LaunchBomb(m_bombSpawnPoint,vel);
	m_bombSpawning = false;
}

void Test::ShiftMouseDown(const b2Vec2& p)
{
	m_mouseWorld = p;
	
	if (m_mouseJoint != NULL)
	{
		return;
	}

	SpawnBomb(p);
}

void Test::MouseUp(const b2Vec2& p)
{
	if (m_mouseJoint)
	{
		m_world->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
	}
	
	if (m_bombSpawning)
	{
		CompleteBombSpawn(p);
	}
}

void Test::MouseMove(const b2Vec2& p)
{
	m_mouseWorld = p;
	
	if (m_mouseJoint)
	{
		m_mouseJoint->SetTarget(p);
	}
}

void Test::LaunchBomb()
{
	b2Vec2 p(RandomFloat(-15.0f, 15.0f), 30.0f);
	b2Vec2 v = -5.0f * p;
	LaunchBomb(p, v);
}

void Test::LaunchBomb(const b2Vec2& position, const b2Vec2& velocity)
{
	if (m_bomb)
	{
		m_world->DestroyBody(m_bomb);
		m_bomb = NULL;
	}

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position = position;
	bd.bullet = true;
	m_bomb = m_world->CreateBody(&bd);
	m_bomb->SetLinearVelocity(velocity);
	
	b2CircleShape circle;
	circle.m_radius = 0.3f;

	b2FixtureDef fd;
	fd.shape = &circle;
	fd.density = 20.0f;
	fd.restitution = 0.0f;
	
	b2Vec2 minV = position - b2Vec2(0.3f,0.3f);
	b2Vec2 maxV = position + b2Vec2(0.3f,0.3f);
	
	b2AABB aabb;
	aabb.lowerBound = minV;
	aabb.upperBound = maxV;

	m_bomb->CreateFixture(&fd);
}

void Test::Step(float hertz, int32 velocityIterations, int32 positionIterations)
{
	m_settings.m_hertz = hertz;
	m_settings.m_velocityIterations = velocityIterations;
	m_settings.m_positionIterations = positionIterations;

	float timeStep = m_settings.m_hertz > 0.0f ? 1.0f / m_settings.m_hertz : float(0.0f);

	if (m_settings.m_pause)
	{
		if (m_settings.m_singleStep)
		{
			m_settings.m_singleStep = 0;
		}
		else
		{
			timeStep = 0.0f;
		}

		g_debugDraw.DrawString(5, m_textLine, "****PAUSED****");
		m_textLine += m_textIncrement;
	}

	uint32 flags = 0;
	flags += m_settings.m_drawShapes * b2Draw::e_shapeBit;
	flags += m_settings.m_drawJoints * b2Draw::e_jointBit;
	flags += m_settings.m_drawAABBs * b2Draw::e_aabbBit;
	flags += m_settings.m_drawCOMs * b2Draw::e_centerOfMassBit;
	g_debugDraw.SetFlags(flags);

	m_world->SetAllowSleeping(m_settings.m_enableSleep);
	m_world->SetWarmStarting(m_settings.m_enableWarmStarting);
	m_world->SetContinuousPhysics(m_settings.m_enableContinuous);
	m_world->SetSubStepping(m_settings.m_enableSubStepping);

	m_pointCount = 0;

	m_world->Step(timeStep, m_settings.m_velocityIterations, m_settings.m_positionIterations);

	m_world->DebugDraw();
    g_debugDraw.Flush();

	if (timeStep > 0.0f)
	{
		++m_stepCount;
	}

	if (m_settings.m_drawStats)
	{
		int32 bodyCount = m_world->GetBodyCount();
		int32 contactCount = m_world->GetContactCount();
		int32 jointCount = m_world->GetJointCount();
		g_debugDraw.DrawString(5, m_textLine, "bodies/contacts/joints = %d/%d/%d", bodyCount, contactCount, jointCount);
		m_textLine += m_textIncrement;

		int32 proxyCount = m_world->GetProxyCount();
		int32 height = m_world->GetTreeHeight();
		int32 balance = m_world->GetTreeBalance();
		float quality = m_world->GetTreeQuality();
		g_debugDraw.DrawString(5, m_textLine, "proxies/height/balance/quality = %d/%d/%d/%g", proxyCount, height, balance, quality);
		m_textLine += m_textIncrement;
	}

	// Track maximum profile times
	{
		const b2Profile& p = m_world->GetProfile();
		m_maxProfile.step = b2Max(m_maxProfile.step, p.step);
		m_maxProfile.collide = b2Max(m_maxProfile.collide, p.collide);
		m_maxProfile.solve = b2Max(m_maxProfile.solve, p.solve);
		m_maxProfile.solveInit = b2Max(m_maxProfile.solveInit, p.solveInit);
		m_maxProfile.solveVelocity = b2Max(m_maxProfile.solveVelocity, p.solveVelocity);
		m_maxProfile.solvePosition = b2Max(m_maxProfile.solvePosition, p.solvePosition);
		m_maxProfile.solveTOI = b2Max(m_maxProfile.solveTOI, p.solveTOI);
		m_maxProfile.broadphase = b2Max(m_maxProfile.broadphase, p.broadphase);

		m_totalProfile.step += p.step;
		m_totalProfile.collide += p.collide;
		m_totalProfile.solve += p.solve;
		m_totalProfile.solveInit += p.solveInit;
		m_totalProfile.solveVelocity += p.solveVelocity;
		m_totalProfile.solvePosition += p.solvePosition;
		m_totalProfile.solveTOI += p.solveTOI;
		m_totalProfile.broadphase += p.broadphase;
	}

	if (m_settings.m_drawProfile)
	{
		const b2Profile& p = m_world->GetProfile();

		b2Profile aveProfile;
		memset(&aveProfile, 0, sizeof(b2Profile));
		if (m_stepCount > 0)
		{
			float scale = 1.0f / m_stepCount;
			aveProfile.step = scale * m_totalProfile.step;
			aveProfile.collide = scale * m_totalProfile.collide;
			aveProfile.solve = scale * m_totalProfile.solve;
			aveProfile.solveInit = scale * m_totalProfile.solveInit;
			aveProfile.solveVelocity = scale * m_totalProfile.solveVelocity;
			aveProfile.solvePosition = scale * m_totalProfile.solvePosition;
			aveProfile.solveTOI = scale * m_totalProfile.solveTOI;
			aveProfile.broadphase = scale * m_totalProfile.broadphase;
		}

		g_debugDraw.DrawString(5, m_textLine, "step [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.step, aveProfile.step, m_maxProfile.step);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "collide [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.collide, aveProfile.collide, m_maxProfile.collide);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "solve [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solve, aveProfile.solve, m_maxProfile.solve);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "solve init [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solveInit, aveProfile.solveInit, m_maxProfile.solveInit);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "solve velocity [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solveVelocity, aveProfile.solveVelocity, m_maxProfile.solveVelocity);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "solve position [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solvePosition, aveProfile.solvePosition, m_maxProfile.solvePosition);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "solveTOI [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solveTOI, aveProfile.solveTOI, m_maxProfile.solveTOI);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "broad-phase [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.broadphase, aveProfile.broadphase, m_maxProfile.broadphase);
		m_textLine += m_textIncrement;
	}

	if (m_bombSpawning)
	{
		b2Color c;
		c.Set(0.0f, 0.0f, 1.0f);
		g_debugDraw.DrawPoint(m_bombSpawnPoint, 4.0f, c);

		c.Set(0.8f, 0.8f, 0.8f);
		g_debugDraw.DrawSegment(m_mouseWorld, m_bombSpawnPoint, c);
	}

	if (m_settings.m_drawContactPoints)
	{
		const float k_impulseScale = 0.1f;
		const float k_axisScale = 0.3f;

		for (int32 i = 0; i < m_pointCount; ++i)
		{
			ContactPoint* point = m_points + i;

			if (point->state == b2_addState)
			{
				// Add
				g_debugDraw.DrawPoint(point->position, 10.0f, b2Color(0.3f, 0.95f, 0.3f));
			}
			else if (point->state == b2_persistState)
			{
				// Persist
				g_debugDraw.DrawPoint(point->position, 5.0f, b2Color(0.3f, 0.3f, 0.95f));
			}

			if (m_settings.m_drawContactNormals == 1)
			{
				b2Vec2 p1 = point->position;
				b2Vec2 p2 = p1 + k_axisScale * point->normal;
				g_debugDraw.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.9f));
			}
			else if (m_settings.m_drawContactImpulse == 1)
			{
				b2Vec2 p1 = point->position;
				b2Vec2 p2 = p1 + k_impulseScale * point->normalImpulse * point->normal;
				g_debugDraw.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.3f));
			}

			if (m_settings.m_drawFrictionImpulse == 1)
			{
				b2Vec2 tangent = b2Cross(point->normal, 1.0f);
				b2Vec2 p1 = point->position;
				b2Vec2 p2 = p1 + k_impulseScale * point->tangentImpulse * tangent;
				g_debugDraw.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.3f));
			}
		}
	}
}

void Test::ShiftOrigin(const b2Vec2& newOrigin)
{
	m_world->ShiftOrigin(newOrigin);
}


void 
MyDrawdebug::glfwErrorCallback(int error, const char* description) {
	fprintf(stderr, "GLFW error occured. Code: %d. Description: %s\n", error, description);
}

void 
MyDrawdebug::CreateUI(GLFWwindow* window, const char* glslVersion) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	bool success;
	success = ImGui_ImplGlfw_InitForOpenGL(window, false);
	if (success == false)
	{
		printf("ImGui_ImplGlfw_InitForOpenGL failed\n");
		assert(false);
	}

	success = ImGui_ImplOpenGL3_Init(glslVersion);
	if (success == false)
	{
		printf("ImGui_ImplOpenGL3_Init failed\n");
		assert(false);
	}

	// Search for font file
	const char* fontPath1 = "data/droid_sans.ttf";
	const char* fontPath2 = "../data/droid_sans.ttf";
	const char* fontPath = nullptr;
	FILE* file1 = fopen(fontPath1, "rb");
	FILE* file2 = fopen(fontPath2, "rb");
	if (file1)
	{
		fontPath = fontPath1;
		fclose(file1);
	}

	if (file2)
	{
		fontPath = fontPath2;
		fclose(file2);
	}

	if (fontPath)
	{
		ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath, 13.0f);
	}
}

void 
MyDrawdebug::ResizeWindowCallback(GLFWwindow*, int width, int height) {
	g_camera.m_width = width;
	g_camera.m_height = height;
}

void 
MyDrawdebug::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
	if (ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Quit
			glfwSetWindowShouldClose(MyDrawdebug::s_mainWindow, GL_TRUE);
			break;

		case GLFW_KEY_LEFT:
			// Pan left
			if (mods == GLFW_MOD_CONTROL)
			{
				b2Vec2 newOrigin(2.0f, 0.0f);
				MyDrawdebug::s_test->ShiftOrigin(newOrigin);
			}
			else
			{
				g_camera.m_center.x -= 0.5f;
			}
			break;

		case GLFW_KEY_RIGHT:
			// Pan right
			if (mods == GLFW_MOD_CONTROL)
			{
				b2Vec2 newOrigin(-2.0f, 0.0f);
				MyDrawdebug::s_test->ShiftOrigin(newOrigin);
			}
			else
			{
				g_camera.m_center.x += 0.5f;
			}
			break;

		case GLFW_KEY_DOWN:
			// Pan down
			if (mods == GLFW_MOD_CONTROL)
			{
				b2Vec2 newOrigin(0.0f, 2.0f);
				MyDrawdebug::s_test->ShiftOrigin(newOrigin);
			}
			else
			{
				g_camera.m_center.y -= 0.5f;
			}
			break;

		case GLFW_KEY_UP:
			// Pan up
			if (mods == GLFW_MOD_CONTROL)
			{
				b2Vec2 newOrigin(0.0f, -2.0f);
				MyDrawdebug::s_test->ShiftOrigin(newOrigin);
			}
			else
			{
				g_camera.m_center.y += 0.5f;
			}
			break;

		case GLFW_KEY_HOME:
			// Reset view
			g_camera.m_zoom = 1.0f;
			g_camera.m_center.Set(0.0f, 20.0f);
			break;

		case GLFW_KEY_Z:
			// Zoom out
			g_camera.m_zoom = b2Min(1.1f * g_camera.m_zoom, 20.0f);
			break;

		case GLFW_KEY_X:
			// Zoom in
			g_camera.m_zoom = b2Max(0.9f * g_camera.m_zoom, 0.02f);
			break;

		case GLFW_KEY_R:
			// Reset test
			delete MyDrawdebug::s_test;
			break;

		case GLFW_KEY_SPACE:
			// Launch a bomb.
			if (MyDrawdebug::s_test)
			{
				//MyDrawdebug::s_test->LaunchBomb();
			}
			break;

		case GLFW_KEY_O:
			MyDrawdebug::MyDrawdebug::s_settings.m_singleStep = true;
			break;

		case GLFW_KEY_P:
			MyDrawdebug::s_settings.m_pause = !MyDrawdebug::s_settings.m_pause;
			break;

		case GLFW_KEY_LEFT_BRACKET:
			// Switch to previous test
			break;

		case GLFW_KEY_RIGHT_BRACKET:
			// Switch to next test
			break;

		case GLFW_KEY_TAB:
			g_debugDraw.m_showUI = !g_debugDraw.m_showUI;

		default:
			if (MyDrawdebug::s_test)
			{
				MyDrawdebug::s_test->Keyboard(key);
			}
		}
	}
	else if (action == GLFW_RELEASE)
	{
		MyDrawdebug::s_test->KeyboardUp(key);
	}
	// else GLFW_REPEAT
}

void 
MyDrawdebug::CharCallback(GLFWwindow* window, unsigned int c) {
	ImGui_ImplGlfw_CharCallback(window, c);
}

void 
MyDrawdebug::MouseButtonCallback(GLFWwindow* window, int32 button, int32 action, int32 mods) {
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

	double xd, yd;
	glfwGetCursorPos(MyDrawdebug::s_mainWindow, &xd, &yd);
	b2Vec2 ps((float)xd, (float)yd);

	// Use the mouse to move things around.
	if (button == GLFW_MOUSE_BUTTON_1)
	{
		//<##>
		//ps.Set(0, 0);
		b2Vec2 pw = g_camera.ConvertScreenToWorld(ps);
		if (action == GLFW_PRESS)
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				MyDrawdebug::s_test->ShiftMouseDown(pw);
			}
			else
			{
				MyDrawdebug::s_test->MouseDown(pw);
			}
		}

		if (action == GLFW_RELEASE)
		{
			MyDrawdebug::s_test->MouseUp(pw);
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_2)
	{
		if (action == GLFW_PRESS)
		{
			MyDrawdebug::s_clickPointWS = g_camera.ConvertScreenToWorld(ps);
			MyDrawdebug::s_rightMouseDown = true;
		}

		if (action == GLFW_RELEASE)
		{
			MyDrawdebug::s_rightMouseDown = false;
		}
	}
}

void 
MyDrawdebug::MouseMotionCallback(GLFWwindow*, double xd, double yd) {
	b2Vec2 ps((float)xd, (float)yd);

	b2Vec2 pw = g_camera.ConvertScreenToWorld(ps);
	MyDrawdebug::s_test->MouseMove(pw);

	if (MyDrawdebug::s_rightMouseDown)
	{
		b2Vec2 diff = pw - MyDrawdebug::s_clickPointWS;
		g_camera.m_center.x -= diff.x;
		g_camera.m_center.y -= diff.y;
		MyDrawdebug::s_clickPointWS = g_camera.ConvertScreenToWorld(ps);
	}
}

void 
MyDrawdebug::ScrollCallback(GLFWwindow* window, double dx, double dy) {
	ImGui_ImplGlfw_ScrollCallback(window, dx, dy);
	if (ImGui::GetIO().WantCaptureMouse)
	{
		return;
	}

	if (dy > 0)
	{
		g_camera.m_zoom /= 1.1f;
	}
	else
	{
		g_camera.m_zoom *= 1.1f;
	}
}

void 
MyDrawdebug::UpdateUI() {
	int menuWidth = 180;
	if (g_debugDraw.m_showUI)
	{
		ImGui::SetNextWindowPos(ImVec2((float)g_camera.m_width - menuWidth - 10, 10));
		ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)g_camera.m_height - 20));
		ImGui::Begin("Tools", &g_debugDraw.m_showUI, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::End();
	}
}

GLFWwindow* MyDrawdebug::s_mainWindow = nullptr;
Test* MyDrawdebug::s_test = nullptr;
Settings MyDrawdebug::s_settings;
bool MyDrawdebug::MyDrawdebug::s_rightMouseDown = false;
b2Vec2 MyDrawdebug::MyDrawdebug::s_clickPointWS = b2Vec2_zero;
char MyDrawdebug::s_buffer[128] = { 0 };
std::chrono::duration<double> MyDrawdebug::s_frameTime(0.0);
std::chrono::duration<double> MyDrawdebug::s_sleepAdjust(0.0);

int
MyDrawdebug::Start(b2World* world) {
#if defined(_WIN32)
	// Enable memory-leak reports
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif

	glfwSetErrorCallback(MyDrawdebug::glfwErrorCallback);

	g_camera.m_width = MyDrawdebug::s_settings.m_windowWidth;
	g_camera.m_height = MyDrawdebug::s_settings.m_windowHeight;

	if (glfwInit() == 0)
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

#if __APPLE__
	const char* glslVersion = "#version 150";
#else
	const char* glslVersion = NULL;
#endif

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	sprintf(s_buffer, "Box2D Testbed Version %d.%d.%d", b2_version.major, b2_version.minor, b2_version.revision);

	bool fullscreen = false;
	if (fullscreen)
	{
		MyDrawdebug::s_mainWindow = glfwCreateWindow(1920, 1080, s_buffer, glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		MyDrawdebug::s_mainWindow = glfwCreateWindow(g_camera.m_width, g_camera.m_height, s_buffer, NULL, NULL);
	}

	if (MyDrawdebug::s_mainWindow == NULL)
	{
		fprintf(stderr, "Failed to open GLFW MyDrawdebug::s_mainWindow.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(MyDrawdebug::s_mainWindow);

	// Load OpenGL functions using glad
	int version = gladLoadGL(glfwGetProcAddress);
	printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	glfwSetScrollCallback(MyDrawdebug::s_mainWindow, MyDrawdebug::ScrollCallback);
	glfwSetWindowSizeCallback(MyDrawdebug::s_mainWindow, MyDrawdebug::ResizeWindowCallback);
	glfwSetKeyCallback(MyDrawdebug::s_mainWindow, MyDrawdebug::KeyCallback);
	glfwSetCharCallback(MyDrawdebug::s_mainWindow, MyDrawdebug::CharCallback);
	glfwSetMouseButtonCallback(MyDrawdebug::s_mainWindow, MyDrawdebug::MouseButtonCallback);
	glfwSetCursorPosCallback(MyDrawdebug::s_mainWindow, MyDrawdebug::MouseMotionCallback);
	glfwSetScrollCallback(MyDrawdebug::s_mainWindow, MyDrawdebug::ScrollCallback);

	g_debugDraw.Create();

	CreateUI(MyDrawdebug::s_mainWindow, glslVersion);

	MyDrawdebug::s_test = new Test(s_settings, world);

	// Control the frame rate. One draw per monitor refresh.
	//glfwSwapInterval(1);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	return 0;
}

bool
MyDrawdebug::Step(float hertz, int32 velocityIterations, int32 positionIterations) {
	bool ret = glfwWindowShouldClose(MyDrawdebug::s_mainWindow);
	if (!ret)
	{
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

		glfwGetWindowSize(MyDrawdebug::s_mainWindow, &g_camera.m_width, &g_camera.m_height);

		int bufferWidth, bufferHeight;
		glfwGetFramebufferSize(MyDrawdebug::s_mainWindow, &bufferWidth, &bufferHeight);
		glViewport(0, 0, bufferWidth, bufferHeight);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		if (g_debugDraw.m_showUI)
		{
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImVec2(float(g_camera.m_width), float(g_camera.m_height)));
			ImGui::SetNextWindowBgAlpha(0.0f);
			ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
			ImGui::End();
			MyDrawdebug::s_test->DrawTitle(s_buffer);
		}

		MyDrawdebug::s_test->Step(hertz, velocityIterations, positionIterations);

		MyDrawdebug::UpdateUI();

		// ImGui::ShowDemoWindow();

		if (g_debugDraw.m_showUI)
		{
			sprintf(s_buffer, "%.1f ms", 1000.0 * MyDrawdebug::s_frameTime.count());
			g_debugDraw.DrawString(5, g_camera.m_height - 20, s_buffer);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(MyDrawdebug::s_mainWindow);

		glfwPollEvents();

		// Throttle to cap at 60Hz. This adaptive using a sleep adjustment. This could be improved by
		// using mm_pause or equivalent for the last millisecond.
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		std::chrono::duration<double> target(1.0 / 60.0);
		std::chrono::duration<double> timeUsed = t2 - t1;
		std::chrono::duration<double> sleepTime = target - timeUsed + MyDrawdebug::s_sleepAdjust;
		if (sleepTime > std::chrono::duration<double>(0))
		{
			std::this_thread::sleep_for(sleepTime);
		}

		std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
		MyDrawdebug::s_frameTime = t3 - t1;

		// Compute the sleep adjustment using a low pass filter
		MyDrawdebug::s_sleepAdjust = 0.9 * MyDrawdebug::s_sleepAdjust + 0.1 * (target - MyDrawdebug::s_frameTime);
	}
	return ret;
}

void MyDrawdebug::Clear() {
	delete MyDrawdebug::s_test;
	MyDrawdebug::s_test = nullptr;
	g_debugDraw.Destroy();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	glfwTerminate();
}

