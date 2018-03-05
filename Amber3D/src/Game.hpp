#pragma once
#include "../Amber3D/Logger/Logger.hpp"
#include "../Amber3D/OpenGL/Camera.hpp"
#include "../Amber3D/OpenGL/Model.hpp"
#include "../Amber3D/OpenGL/SkyBox.hpp"
#include "../Amber3D/Utils/Clock.hpp"
#include "../Amber3D/Utils/Utils.hpp"
#include "../Amber3D/Window/Window.hpp"
#include "../Amber3D/Physics/PhysicsHandler.hpp"
#include <bullet\btBulletDynamicsCommon.h>

namespace Amber3D
{
	struct Game
	{
	public:
		Game();
		~Game();

		void run();

		void update(float dt);
		void render(float dt);

		void handlePlayerInput(float dt);

		void initCallbacks();

		unsigned int loadTexture(char const * path);

		btRigidBody* addBox(float dimension, float x, float y, float z, float mass, float roll = 0.0f, float pitch = 0.0f, float yaw = 0.0f);
		btRigidBody* addSphere(float radius, float x, float y, float z, float mass);
		void renderBox(btRigidBody* box, Model& model, Shader& shader);
		void renderSphere(btRigidBody* sphere, Model& model, Shader& shader);
		void renderPlane(btRigidBody* plane, Model& model, Shader& shader);

	public:
		Window m_wnd;
		Logger m_log;
		Camera m_camera;
		SkyBox m_skyBox;

		rng_mt19937_std m_rng;
		std::string m_windowTitle = "Demo";
		size_t m_objectCounter = 0;

		PhysicsHandler m_physics;

		btRigidBody* m_plane;
		btRaycastVehicle* m_vehicle;

		Shader m_shader;
		Model m_cubModel;
		Model m_sphereModel;
		Model m_planeModel;

		bool m_inFocus = true;

		std::vector<glm::vec3> m_pointLightPositions;
	};
} // namespace Amber3D
