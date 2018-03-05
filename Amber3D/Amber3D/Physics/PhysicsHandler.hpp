#pragma once
#include <bullet\btBulletDynamicsCommon.h>

namespace Amber3D
{
	struct PhysicsHandler
	{
	public:
		PhysicsHandler(const btVector3 &gravity)
		{
			collisionConfig = new btDefaultCollisionConfiguration();
			dispatcher = new btCollisionDispatcher(collisionConfig);
			broadphase = new btDbvtBroadphase();
			solver = new btSequentialImpulseConstraintSolver();
			world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
			world->setGravity(gravity);
		}
		PhysicsHandler(const btVector3 &gravity, const btVector3 &worldMin, const btVector3 &worldMax)
		{
			collisionConfig = new btDefaultCollisionConfiguration();
			dispatcher = new btCollisionDispatcher(collisionConfig);
			broadphase = new btAxisSweep3(worldMin, worldMax);
			solver = new btSequentialImpulseConstraintSolver();
			world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
			world->setGravity(gravity);
		}
		~PhysicsHandler()
		{
			//cleanup in the reverse order of creation/initialization

			//remove the rigidbodies from the dynamics world and delete them
			for (int i = world->getNumCollisionObjects() - 1; i >= 0; --i)
			{
				btCollisionObject* obj = world->getCollisionObjectArray()[i];
				btRigidBody* body = btRigidBody::upcast(obj);
				if (body && body->getMotionState())
				{
					delete body->getMotionState();
				}
				world->removeCollisionObject(obj);
				delete obj;
			}

			//delete collision shapes
			for (int j = 0; j < collisionShapes.size(); ++j)
			{
				btCollisionShape* shape = collisionShapes[j];
				delete shape;
			}

			delete solver;
			delete broadphase;
			delete dispatcher;
			delete collisionConfig;
			delete world;
		}


	public:
		btDynamicsWorld * world;
		btDispatcher* dispatcher;
		btCollisionConfiguration* collisionConfig;
		btBroadphaseInterface* broadphase;
		btConstraintSolver* solver;

		btAlignedObjectArray<btCollisionShape*> collisionShapes;
	};
}