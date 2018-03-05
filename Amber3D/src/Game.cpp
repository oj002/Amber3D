#include "Game.hpp"
#include <algorithm>

namespace Amber3D
{
	Game::Game()
		: m_wnd(800, 600, "Demo", false, []() {glfwWindowHint(GLFW_SAMPLES, 4); })
		, m_camera({0.0f, 2.0f, 4.0f})
		, m_skyBox(std::vector<std::string>({ "res/skybox/right.jpg", "res/skybox/left.jpg", "res/skybox/top.jpg", "res/skybox/bottom.jpg", "res/skybox/back.jpg", "res/skybox/front.jpg" }))
		, m_physics({ 0.0f, -10.0f, 0.0f }, { -1000.0f, -1000.0f, -1000.0f }, { 1000.0f, 1000.0f, 1000.0f })
		, m_shader("res/shaders/lighting_model.shader")
		, m_cubModel("res/simpleShapes/cube.obj")
		, m_sphereModel("res/simpleShapes/sphere.obj")
		, m_planeModel("res/simpleShapes/plane.obj")
		, m_pointLightPositions({
			{ -1.0f,  1.0f,  1.5f },
			{ 4.5f,  3.0f, -1.0f },
			{ -5.0f,  2.5f,  2.5f },
			{ 2.5f,  1.0f, -3.5f }
			})
	{
		GLCall(glEnable(GL_DEPTH_TEST));
		//GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

		//blending
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//face culling
		GLCall(glEnable(GL_CULL_FACE));

		//multi sampling
		GLCall(glEnable(GL_MULTISAMPLE));

		initCallbacks();

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin({ 0.0f, 0.0f,0.0f });
		btStaticPlaneShape* plane = new btStaticPlaneShape({ 0.0f ,1.0f ,0.0f }, 0.0f);
		btMotionState* motion = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo info(0.0f, motion, plane);
		m_plane = new btRigidBody(info);
		m_physics.world->addRigidBody(m_plane);
		++m_objectCounter;
		
		m_vehicle;

		for (float x = 0; x < 3; x += 1.0f)
		{
			for (float y = 0; y < 3; y += 1.0f)
			{
				for (float z = 0; z < 3; z += 1.0f)
				{
					addBox(0.5f, 0.0f + x, 0.5f + y, 0.0f + z, 5.0f);
					++m_objectCounter;
				}
			}
		}
	}

	Game::~Game()
	{
	}

	void Game::run()
	{
		Clock c;

		while (m_wnd.is_open())
		{
			float dt = c.restart<float>();
			m_wnd.pollEvent();
			update(dt);
			render(dt);
		}
	}

	void Game::update(float dt)
	{
		handlePlayerInput(dt);
		if (glfwGetKey(m_wnd.pWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			m_physics.world->stepSimulation(dt / 25.0f);
		}
		else
		{
			m_physics.world->stepSimulation(dt);
		}
	}

	void Game::render(float dt)
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		m_shader.bind();
		m_shader.setVec3("viewPos", m_camera.position);
		m_shader.setFloat("material.shininess", 32.0f);


		// directional light
		m_shader.setVec3("dirLight.direction", { -0.2f, -1.0f, -0.3f });
		m_shader.setVec3("dirLight.ambient", { 0.05f, 0.05f, 0.05f });
		m_shader.setVec3("dirLight.diffuse", { 0.4f, 0.4f, 0.4f });
		m_shader.setVec3("dirLight.specular", { 0.5f, 0.5f, 0.5f });
		// point light 1
		m_shader.setVec3("pointLights[0].position", m_pointLightPositions[0]);
		m_shader.setVec3("pointLights[0].ambient", { 0.05f, 0.05f, 0.05f });
		m_shader.setVec3("pointLights[0].diffuse", { 0.8f, 0.8f, 0.8f });
		m_shader.setVec3("pointLights[0].specular", { 1.0f, 1.0f, 1.0f });
		m_shader.setFloat("pointLights[0].constant", 1.0f);
		m_shader.setFloat("pointLights[0].linear", 0.022f);
		m_shader.setFloat("pointLights[0].quadratic", 0.0019f);
		// point light 2
		m_shader.setVec3("pointLights[1].position", m_pointLightPositions[1]);
		m_shader.setVec3("pointLights[1].ambient", { 0.05f, 0.05f, 0.05f });
		m_shader.setVec3("pointLights[1].diffuse", { 0.8f, 0.8f, 0.8f });
		m_shader.setVec3("pointLights[1].specular", { 1.0f, 1.0f, 1.0f });
		m_shader.setFloat("pointLights[1].constant", 1.0f);
		m_shader.setFloat("pointLights[1].linear", 0.022f);
		m_shader.setFloat("pointLights[1].quadratic", 0.0019f);
		// point light 3
		m_shader.setVec3("pointLights[2].position", m_pointLightPositions[2]);
		m_shader.setVec3("pointLights[2].ambient", { 0.05f, 0.05f, 0.05f });
		m_shader.setVec3("pointLights[2].diffuse", { 0.8f, 0.8f, 0.8f });
		m_shader.setVec3("pointLights[2].specular", { 1.0f, 1.0f, 1.0f });
		m_shader.setFloat("pointLights[2].constant", 1.0f);
		m_shader.setFloat("pointLights[2].linear", 0.022f);
		m_shader.setFloat("pointLights[2].quadratic", 0.0019f);
		// point light 4
		m_shader.setVec3("pointLights[3].position", m_pointLightPositions[3]);
		m_shader.setVec3("pointLights[3].ambient", { 0.05f, 0.05f, 0.05f });
		m_shader.setVec3("pointLights[3].diffuse", { 0.8f, 0.8f, 0.8f });
		m_shader.setVec3("pointLights[3].specular", { 1.0f, 1.0f, 1.0f });
		m_shader.setFloat("pointLights[3].constant", 1.0f);
		m_shader.setFloat("pointLights[3].linear", 0.022f);
		m_shader.setFloat("pointLights[3].quadratic", 0.0019f);



		glm::mat4 projection = glm::perspective(glm::radians(m_camera.zoom), static_cast<float>(std::max(m_wnd.width, 1)) / static_cast<float>(std::max(m_wnd.height, 1)), 0.01f, 1000.0f);
		glm::mat4 view = m_camera.getViewMatrix();
		m_shader.setMat4("projection", projection);
		m_shader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		m_shader.setMat4("model", model);
		//nanoModel.draw(shader);

		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, m_pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.75f)); // Make it a smaller cube
			m_shader.setMat4("model", model);
			m_sphereModel.draw(m_shader);
		}
		for (int i = m_physics.world->getNumCollisionObjects() - 1; i >= 0; --i)
		{
			btCollisionObject* obj = m_physics.world->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
			{
				renderBox(body, m_cubModel, m_shader);
			}
			else if (body->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
			{
				renderSphere(body, m_cubModel, m_shader);
			}
			else if (body->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
			{
				renderPlane(body, m_planeModel, m_shader);
			}
		}
		m_skyBox.draw(m_camera, projection);

		std::string title(m_windowTitle);
		title += "     fps: " + std::to_string(static_cast<int>(1 / dt)) + "     Objects: " + std::to_string(m_objectCounter);
		glfwSetWindowTitle(m_wnd.pWindow, title.c_str());
	
		m_wnd.swapBuffers();
	}

	void Game::handlePlayerInput(float dt)
	{
		if (glfwGetMouseButton(m_wnd.pWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			btRigidBody* sphere = addBox(0.5f, m_camera.position.x + m_camera.front.x, m_camera.position.y + m_camera.front.y, m_camera.position.z + m_camera.front.z, 5.0f);

			sphere->setLinearVelocity(btVector3(m_camera.front.x * 25.0f, m_camera.front.y * 25.0f, m_camera.front.z * 25.0f));
			++m_objectCounter;

		}

		static bool once = true;
		if (glfwGetMouseButton(m_wnd.pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			if (once)
			{
				once = false;
				btRigidBody* sphere = addBox(0.5f, m_camera.position.x + m_camera.front.x, m_camera.position.y + m_camera.front.y, m_camera.position.z + m_camera.front.z, 5.0f);

				sphere->setLinearVelocity(btVector3(m_camera.front.x * 25.0f, m_camera.front.y * 25.0f, m_camera.front.z * 25.0f));

				++m_objectCounter;
			}
		}
		else
		{
			once = true;
		}

		if (glfwGetKey(m_wnd.pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(m_wnd.pWindow, true);
		}

		if (glfwGetKey(m_wnd.pWindow, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_camera.processKeyboard(CameraMovement::FORWARD, dt);
		}
		if (glfwGetKey(m_wnd.pWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_camera.processKeyboard(CameraMovement::BACKWARD, dt);
		}
		if (glfwGetKey(m_wnd.pWindow, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_camera.processKeyboard(CameraMovement::RIGHT, dt);
		}
		if (glfwGetKey(m_wnd.pWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_camera.processKeyboard(CameraMovement::LEFT, dt);
		}
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Game* g = static_cast<Game*>(glfwGetWindowUserPointer(window));
		
		static float lastX = g->m_wnd.width / 2.0f;
		static float lastY = g->m_wnd.height / 2.0f;
		static bool firstMouse = true;

		if (firstMouse)
		{
			lastX = static_cast<float>(xpos);
			lastY = static_cast<float>(ypos);
			firstMouse = false;
		}

		float xoffset = static_cast<float>(xpos) - lastX;
		float yoffset = lastY - static_cast<float>(ypos);

		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);

		g->m_camera.processMouseMovement(xoffset, yoffset);
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Game* g = static_cast<Game*>(glfwGetWindowUserPointer(window));

		g->m_camera.processMouseScroll((float)yoffset);
	}

	void Game::initCallbacks()
	{
		glfwSwapInterval(0);
		glfwSetWindowUserPointer(m_wnd.pWindow, this);
		
		glfwSetCursorPosCallback(m_wnd.pWindow, mouse_callback);
		glfwSetScrollCallback(m_wnd.pWindow, scroll_callback);
		glfwSetInputMode(m_wnd.pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	unsigned int Game::loadTexture(char const * path)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format = 0;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

	btRigidBody * Game::addBox(float dimension, float x, float y, float z, float mass, float roll, float pitch, float yaw)
	{
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin({ x, y, z });
		btBoxShape* box = new btBoxShape({ dimension, dimension, dimension });
		btVector3 inertia(0.0f, 0.0f, 0.0f);
		if (mass != 0.0f)
		{
			box->calculateLocalInertia(mass, inertia);
		}


		btMotionState* motion = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);
		btRigidBody* body = new btRigidBody(info);
		m_physics.world->addRigidBody(body);
		m_physics.collisionShapes.push_back(body->getCollisionShape());

		return body;
	}
	btRigidBody * Game::addSphere(float radius, float x, float y, float z, float mass)
	{
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin({ x, y, z });
		btSphereShape* sphere = new btSphereShape(radius);
		btVector3 inertia(0.0f, 0.0f, 0.0f);
		if (mass != 0.0f)
		{
			sphere->calculateLocalInertia(mass, inertia);
		}


		btMotionState* motion = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);
		btRigidBody* body = new btRigidBody(info);
		m_physics.world->addRigidBody(body);
		m_physics.collisionShapes.push_back(body->getCollisionShape());
		return body;
	}
	void Game::renderBox(btRigidBody * box, Model & model, Shader & shader)
	{
		if (box->getCollisionShape()->getShapeType() != BOX_SHAPE_PROXYTYPE) { return; }
		btVector3 scaling = ((btBoxShape*)box->getCollisionShape())->getHalfExtentsWithMargin();
		btTransform transform;
		box->getMotionState()->getWorldTransform(transform);

		btQuaternion rotation = transform.getRotation();
		btVector3 transformVec = transform.getOrigin();

		glm::mat4 modelTransform;
		modelTransform = glm::translate(modelTransform, glm::vec3(transformVec.getX(), transformVec.getY(), transformVec.getZ()));
		modelTransform = glm::rotate(modelTransform, rotation.getAngle(), glm::vec3(rotation.getAxis().getX(), rotation.getAxis().getY(), rotation.getAxis().getZ()));
		modelTransform = glm::scale(modelTransform, glm::vec3(scaling.x(), scaling.y(), scaling.z()));
		shader.setMat4("model", modelTransform);
		model.draw(shader);
	}
	void Game::renderSphere(btRigidBody * sphere, Model & model, Shader & shader)
	{
		if (sphere->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE) { return; }
		float radius = ((btSphereShape*)sphere->getCollisionShape())->getRadius();
		btTransform transform;
		sphere->getMotionState()->getWorldTransform(transform);

		btQuaternion rotation = transform.getRotation();
		btVector3 transformVec = transform.getOrigin();

		glm::mat4 modelTransform;
		modelTransform = glm::translate(modelTransform, glm::vec3(transformVec.getX(), transformVec.getY(), transformVec.getZ()));
		modelTransform = glm::rotate(modelTransform, rotation.getAngle(), glm::vec3(rotation.getAxis().getX(), rotation.getAxis().getY(), rotation.getAxis().getZ()));
		modelTransform = glm::scale(modelTransform, glm::vec3(radius, radius, radius));
		shader.setMat4("model", modelTransform);
		model.draw(shader);
	}
	void Game::renderPlane(btRigidBody * plane, Model & model, Shader & shader)
	{
		if (plane->getCollisionShape()->getShapeType() != STATIC_PLANE_PROXYTYPE) { return; }
		btTransform transform;
		plane->getMotionState()->getWorldTransform(transform);

		glm::mat4 modelTransform;
		modelTransform = glm::translate(modelTransform, { transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z() });
		shader.setMat4("model", modelTransform);
		model.draw(shader);
	}
}