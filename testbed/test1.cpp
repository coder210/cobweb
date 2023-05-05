#include "test.h"
class Test1 : public Test
{
public:
	Test1()
	{
		m_world->SetGravity(b2Vec2(0.0f, 0.0f));
		{
			b2BodyDef wall;
			b2PolygonShape wallBox;
			wall.type = b2_staticBody;
			wall.position.Set(0.0f, -1.0f);
			b2Body* wallDown = m_world->CreateBody(&wall);
			//����Χǽ����״

			wallBox.SetAsBox(20.0f, 1.0f);

			//���ò���
			wallDown->CreateFixture(&wallBox, 0.0f);

			//��������Χǽ
			wall.position.Set(0.0f, 40.0f);
			b2Body* wallUp = m_world->CreateBody(&wall);

			//����Χǽ����״
			wallBox.SetAsBox(20.0f, 1.0f);
			//���ò���
			wallUp->CreateFixture(&wallBox, 0.0f);
			//���������Χǽ
			wall.position.Set(-21.0f, 20.0f);

			b2Body* wallLeft = m_world->CreateBody(&wall);

			//����Χǽ����״
			wallBox.SetAsBox(1.0f, 20.0f);
			wallLeft->CreateFixture(&wallBox, 0.0f);

			//���������Χǽ
			wall.position.Set(21.0f, 20.0f);

			b2Body* wallRight = m_world->CreateBody(&wall);

			//����Χǽ����״
			wallBox.SetAsBox(1.0f, 20.0f);

			wallRight->CreateFixture(&wallBox, 0.0f);

			//����������̬��body
			b2BodyDef bodyDefA;

			bodyDefA.type = b2_dynamicBody;
			bodyDefA.position.Set(-10.0f, 10.0f);
			b2Body* bodyA = m_world->CreateBody(&bodyDefA);

			b2BodyDef bodyDefB;
			bodyDefB.type = b2_dynamicBody;
			bodyDefB.position.Set(10.0f, 10.0f);
			b2Body* bodyB = m_world->CreateBody(&bodyDefB);

			//Ϊ�����̬bodyA������״
			b2PolygonShape dynamicBox;
			dynamicBox.SetAsBox(1.0f, 1.0f);

			//Ϊ�����̬body���ò���
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = 1.0f; //�ܶ�
			fixtureDef.friction = 0.3f; //Ħ��ϵ��
			fixtureDef.restitution = 1.0f; //�ָ�ϵ��
			bodyA->CreateFixture(&fixtureDef);
			bodyB->CreateFixture(&fixtureDef);

			//ʩ��һ����
			//bodyA->ApplyLinearImpulse(b2Vec2(10, 0), bodyA->GetWorldCenter(), true);
			//bodyB->ApplyLinearImpulse(b2Vec2(-10, 0), bodyB->GetWorldCenter(), true);

		}

		{
				
		}
	}

	static Test* Create()
	{
		return new Test1;
	}
};

//static int test1Index = RegisterTest("Benchmark", "Test1", Test1::Create);
