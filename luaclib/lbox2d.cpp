#include "box2d/box2d.h"
#include "cobweb.h"
#include "mylib/mydraw.h"
extern "C" {
#include "lua/lapi.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}


static int
lcreate_world(struct lua_State* L) {
	float g_x = (float)luaL_checknumber(L, 1);
	float g_y = (float)luaL_checknumber(L, 2);
	b2World* world = new b2World(b2Vec2(g_x, g_y));
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
	lua_pushlightuserdata(L, world);
	return 1;
}

static int
lcreate_worldx(struct lua_State* L) {
	float g_x = (float)luaL_checknumber(L, 1);
	float g_y = (float)luaL_checknumber(L, 2);
	b2World* world = new b2World(b2Vec2(g_x, g_y));
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
	int ret = MyDrawdebug::Start(world);
	lua_pushlightuserdata(L, world);
	lua_pushinteger(L, ret);
	return 2;
}

static int
lworld_step(struct lua_State* L) {
	b2World* world = (b2World*)lua_touserdata(L, 1);
	float hertz = (float)luaL_checknumber(L, 2);
	int velocityIterations = (int)luaL_checkinteger(L, 3);
	int positionIterations = (int)luaL_checkinteger(L, 4);
	world->Step(1.0f / hertz, velocityIterations, positionIterations);
	return 0;
}

static int
lworld_stepx(struct lua_State* L) {
	b2World* world = (b2World*)lua_touserdata(L, 1);
	float hertz = (float)luaL_checknumber(L, 2);
	int velocityIterations = (int)luaL_checkinteger(L, 3);
	int positionIterations = (int)luaL_checkinteger(L, 4);
	bool ret = MyDrawdebug::Step(hertz, velocityIterations, positionIterations);
	lua_pushboolean(L, ret);
	return 1;
}

static int
ldestory_world(struct lua_State* L) {
	b2World* world = (b2World*)lua_touserdata(L, 1);
	delete world;
	return 0;
}

static int
ldestory_worldx(struct lua_State* L) {
	b2World* world = (b2World*)lua_touserdata(L, 1);
	delete world;
	MyDrawdebug::Clear();
	return 0;
}

static int
lcreate_dynamic_body(struct lua_State* L) {
	b2World* world = (b2World*)lua_touserdata(L, 1);
	float px = (float)luaL_checknumber(L, 2);
	float py = (float)luaL_checknumber(L, 3);
	float angle = (float)luaL_checknumber(L, 4);
	bool fixedRotation = lua_toboolean(L, 5);
	b2BodyDef bodyDef;
	bodyDef.type = b2BodyType::b2_dynamicBody;
	bodyDef.position.x = px;
	bodyDef.position.y = py;
	bodyDef.angle = angle;
	bodyDef.fixedRotation = fixedRotation;
	b2Body* body = world->CreateBody(&bodyDef);
	lua_pushlightuserdata(L, body);
	return 1;
}

static int
lcreate_kinematic_body(struct lua_State* L) {
	b2World* world = (b2World*)lua_touserdata(L, 1);
	float px = (float)luaL_checknumber(L, 2);
	float py = (float)luaL_checknumber(L, 3);
	float angle = (float)luaL_checknumber(L, 4);
	bool fixedRotation = lua_toboolean(L, 5);
	b2BodyDef bodyDef;
	bodyDef.type = b2BodyType::b2_kinematicBody;
	bodyDef.position.x = px;
	bodyDef.position.y = py;
	bodyDef.angle = angle;
	bodyDef.fixedRotation = fixedRotation;
	b2Body* body = world->CreateBody(&bodyDef);
	lua_pushlightuserdata(L, body);
	return 1;
}

static int
lcreate_static_body(struct lua_State* L) {
	b2World* world = (b2World*)lua_touserdata(L, 1);
	float px = (float)luaL_checknumber(L, 2);
	float py = (float)luaL_checknumber(L, 3);
	float angle = (float)luaL_checknumber(L, 4);
	bool fixedRotation = lua_toboolean(L, 5);
	b2BodyDef bodyDef;
	bodyDef.type = b2BodyType::b2_staticBody;
	bodyDef.position.x = px;
	bodyDef.position.y = py;
	bodyDef.angle = angle;
	bodyDef.fixedRotation = fixedRotation;
	b2Body* body = world->CreateBody(&bodyDef);
	lua_pushlightuserdata(L, body);
	return 1;
}

static int
ldestory_body(struct lua_State* L) {
	b2World* world = (b2World*)lua_touserdata(L, 1);
	b2Body* body = (b2Body*)lua_touserdata(L, 2);
	world->DestroyBody(body);
	return 0;
}

static int
lbody_set_userdata(struct lua_State* L) {
	b2Body* body = (b2Body*)lua_touserdata(L, 1);
	void* userdata = lua_touserdata(L, 2);
	body->GetUserData().pointer = (uintptr_t)userdata;
	return 0;
}

static int
lbody_get_userdata(struct lua_State* L) {
	b2Body* body = (b2Body*)lua_touserdata(L, 1);
	void* userdata = (void*)body->GetUserData().pointer;
	lua_pushlightuserdata(L, userdata);
	return 1;
}

static int
lbody_set_linear_velocity(struct lua_State* L) {
	b2Body* body = (b2Body*)lua_touserdata(L, 1);
	float vx = (float)luaL_checknumber(L, 2);
	float vy = (float)luaL_checknumber(L, 3);
	body->SetLinearVelocity(b2Vec2(vx, vy));
	return 0;
}

static int
lbody_get_linear_velocity(struct lua_State* L) {
	b2Body* body = (b2Body*)lua_touserdata(L, 1);
	b2Vec2 vec2 = body->GetLinearVelocity();
	lua_pushnumber(L, vec2.x);
	lua_pushnumber(L, vec2.y);
	return 2;
}

static int
lbody_set_bullet(struct lua_State* L) {
	b2Body* body = (b2Body*)lua_touserdata(L, 1);
	bool flag = lua_toboolean(L, 2);
	body->SetBullet(flag);
	return 0;
}

static int
lbody_is_bullet(struct lua_State* L) {
	b2Body* body = (b2Body*)lua_touserdata(L, 1);
	lua_pushboolean(L, body->IsBullet());
	return 1;
}

static int
lcreate_box_fixture(struct lua_State* L) {
	b2Body* body = (b2Body*)lua_touserdata(L, 1);
	float hx = (float)luaL_checknumber(L, 2);
	float hy = (float)luaL_checknumber(L, 3);
	int groupIndex = (int)luaL_checkinteger(L, 4);
	float density = (float)luaL_checknumber(L, 5);
	float friction = (float)luaL_checknumber(L, 6);
	float restitution = (float)luaL_checknumber(L, 7);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(hx, hy);
	b2FixtureDef fixtureDef;
	fixtureDef.filter.groupIndex = groupIndex;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	body->CreateFixture(&fixtureDef);
	return 0;
}

static int
lcreate_edge_fixture(struct lua_State* L) {
	b2Body* body = (b2Body*)lua_touserdata(L, 1);
	float v1_x = (float)luaL_checknumber(L, 2);
	float v1_y = (float)luaL_checknumber(L, 3);
	float v2_x = (float)luaL_checknumber(L, 4);
	float v2_y = (float)luaL_checknumber(L, 5);
	int groupIndex = (int)luaL_checkinteger(L, 6);
	float density = (float)luaL_checknumber(L, 7);
	float friction = (float)luaL_checknumber(L, 8);
	float restitution = (float)luaL_checknumber(L, 9);

	b2Vec2 v1(v1_x, v1_y);
	b2Vec2 v2(v2_x, v2_y);
	b2EdgeShape edgeShape;
	edgeShape.SetTwoSided(v1, v2);

	b2FixtureDef fixtureDef;
	fixtureDef.filter.groupIndex = groupIndex;
	fixtureDef.shape = &edgeShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	body->CreateFixture(&fixtureDef);
	return 0;
}

static int
lcreate_circle_fixture(struct lua_State* L) {
	b2Body* body = (b2Body*)lua_touserdata(L, 1);
	float radius = (float)luaL_checknumber(L, 2);
	int groupIndex = (int)luaL_checkinteger(L, 3);
	float density = (float)luaL_checknumber(L, 4);
	float friction = (float)luaL_checknumber(L, 5);
	float restitution = (float)luaL_checknumber(L, 6);
	b2CircleShape circleShape;
	circleShape.m_radius = radius;
	b2FixtureDef fixtureDef;
	fixtureDef.filter.groupIndex = groupIndex;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	body->CreateFixture(&fixtureDef);
	return 0;
}

static int
lcreate_chain_fixture(struct lua_State* L) {
	b2Body* body = (b2Body*)lua_touserdata(L, 1);
	std::vector<b2Vec2> vertices;
	lua_pushnil(L);
	while (lua_next(L, 2)) {
		int it_idx = lua_gettop(L);
		lua_pushnil(L);
		b2Vec2 item = b2Vec2_zero;
		int index = 0;
		while (lua_next(L, it_idx)) {
			if (index == 0) {
				item.x = (float)luaL_checknumber(L, -1);
			}
			else if (index == 1) {
				item.y = (float)luaL_checknumber(L, -1);
			}
			index++;
			lua_pop(L, 1);
		}
		vertices.push_back(item);
		lua_pop(L, 1);
	}
	int groupIndex = (int)luaL_checkinteger(L, 3);
	float density = (float)luaL_checknumber(L, 4);
	float friction = (float)luaL_checknumber(L, 5);
	float restitution = (float)luaL_checknumber(L, 6);
	b2ChainShape chainShape;
	chainShape.CreateChain(vertices.data(), vertices.size(), vertices.at(0), vertices[vertices.size() - 1]);
	b2FixtureDef fixtureDef;
	fixtureDef.filter.groupIndex = groupIndex;
	fixtureDef.shape = &chainShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	body->CreateFixture(&fixtureDef);
	return 0;
}

COBWEB_MOD_API int
luaopen_lbox2d(lua_State* L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{"create_world", lcreate_world},
		{"world_step", lworld_step},
		{"destory_world", ldestory_world},
		{"create_worldx", lcreate_worldx},
		{"world_stepx", lworld_stepx},
		{"destory_worldx", ldestory_worldx},
		{"create_dynamic_body", lcreate_dynamic_body},
		{"create_kinematic_body", lcreate_kinematic_body},
		{"create_static_body", lcreate_static_body},
		{"body_get_userdata", lbody_get_userdata},
		{"body_set_userdata", lbody_set_userdata},
		{"create_box_fixture", lcreate_box_fixture},
		{"create_edge_fixture", lcreate_edge_fixture},
		{"create_circle_fixture", lcreate_circle_fixture},
		{"create_chain_fixture", lcreate_chain_fixture},
		{NULL, NULL}
	};
	luaL_newlib(L, l);
	return 1;
}

