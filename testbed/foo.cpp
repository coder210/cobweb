#include "test.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

class FooTest : public Test
{
public:
    b2Body* dynamicBody;
    FooTest()
    {
        b2BodyDef myBodyDef;

        myBodyDef.type = b2_dynamicBody;
        //start 10 units further to the right, 20 units higher
        myBodyDef.position.Set(0, 20);
        myBodyDef.angle = 0;
        // b2Body* dynamicBody = m_world->CreateBody(&myBodyDef);
        dynamicBody = m_world->CreateBody(&myBodyDef);

        b2PolygonShape boxShape;
        boxShape.SetAsBox(2, 1);
        b2FixtureDef boxFixtureDef;
        boxFixtureDef.shape = &boxShape;
        boxFixtureDef.density = 1;

        dynamicBody->CreateFixture(&boxFixtureDef);

        //change the starting position and angle
        dynamicBody->SetTransform(b2Vec2(10, 20), 1);

        // set the linear velocity and angular velocity of the body
        dynamicBody->SetLinearVelocity(b2Vec2(-5, 5));
        dynamicBody->SetAngularVelocity(-90 * DEGTORAD);

        //static Bodies
        myBodyDef.type = b2_staticBody;
        myBodyDef.position.Set(0, 10);
        b2Body* staticBody = m_world->CreateBody(&myBodyDef);
        staticBody->CreateFixture(&boxFixtureDef);

        //kinematic body
        myBodyDef.type = b2_kinematicBody;
        myBodyDef.position.Set(-18, 11);
        b2Body* kinematicBody = m_world->CreateBody(&myBodyDef);
        kinematicBody->CreateFixture(&boxFixtureDef);
        kinematicBody->SetLinearVelocity(b2Vec2(1, 0));
        kinematicBody->SetAngularVelocity(360 * DEGTORAD);


    };
    void Step(Settings* setttings)
    {
        Test::Step(*setttings);
        m_textLine += 15;

        b2Vec2 pos = dynamicBody->GetPosition();
        float angle = dynamicBody->GetAngle();
        b2Vec2 vel = dynamicBody->GetLinearVelocity();
        float angularvel = dynamicBody->GetAngularVelocity();
        m_textLine += 15;
        m_textLine += 15;

        for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
        {
            b2Vec2 pos1 = b->GetPosition();
            m_textLine += 15;
        }

    }

    static Test* Create()
    {
        return new FooTest;
    }
};

//static int testIndex = RegisterTest("Benchmark", "FooTest", FooTest::Create);

