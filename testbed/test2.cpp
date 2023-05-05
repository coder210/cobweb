#include "test.h"
#include <iostream>
#include <list>

struct TileEntity
{
	float x;
	float y;
	float z;
};


class Test2 : public Test
{
public:
	Test2()
	{
		int sizeX = 1;
		int sizeY = 1;
		int tileCount = 10;
		std::list<TileEntity> tiles;
		for (size_t i = 0; i < tileCount; i++) {
			TileEntity tileEntity;
			tileEntity.x = 1;
			tileEntity.y = 1;
			tileEntity.z = 1;
			tiles.push_back(tileEntity);
		};

		m_world->SetGravity(b2Vec2(0.0f, -10.0f));
		{
			std::list<TileEntity>::iterator it;
			for (it = tiles.begin(); it != tiles.end(); it++) {
				b2BodyDef bodyDef;
				bodyDef.type = b2_staticBody;
				bodyDef.position = b2Vec2(it->x, it->y);
				b2Body* body = m_world->CreateBody(&bodyDef);
				//创建一个2x2的正方形盒子
				b2PolygonShape boxShape;
				boxShape.SetAsBox(sizeX, sizeY);
				//为这个动态body设置材质
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = 1.0f; //密度
				fixtureDef.friction = 0.3f; //摩擦系数
				fixtureDef.restitution = 1.0f; //恢复系数
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void BeginContact(b2Contact* contact)  override {  
		b2BodyUserData bodyA = contact->GetFixtureA()->GetBody()->GetUserData();
		b2BodyUserData bodyB = contact->GetFixtureB()->GetBody()->GetUserData();

		std::cout << "contact";

	}
	
	void EndContact(b2Contact* contact)  override {  
	
	}


	static Test* Create()
	{
		return new Test2;
	}
};

//static int Test2Index = RegisterTest("Benchmark", "Test2", Test2::Create);
