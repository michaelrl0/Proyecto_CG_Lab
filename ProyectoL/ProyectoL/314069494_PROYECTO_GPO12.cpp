#include <iostream>
#include <cmath>
#include <ctime>
#include <time.h>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float spotAngle = 0.0f;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

// Variables para encender el arbol
bool esfera1,
esfera2,
esfera3,
esfera4,
estrella;

float contador = 0.0f;

// Variables para animación del traíler
bool animaCarro = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false,
recorrido5 = false,
recorrido6 = false,
recorrido7 = false;

float movt_X = 0.0f,
movt_Z = 0.0f,
girollanta = 0.0f,
rotacion_tX = 0.0f,
movL_Z1 = 0.0f,
movL_Z2 = 0.0f,
movL_X1 = 0.0f,
movL_X2 = 0.0f,
rot_L1 = 0.0f,
rot_L2 = 0.0f,
rot_L2_1 = 0.0f;

// Variables para animación del snowman
bool animaSnow = false,
mov1 = true,
mov2 = false,
mov3 = false,
mov4 = false;

float mov_brazo1 = 0.0f,
mov_brazo2 = 0.0f,
salto = 0.0f;

// Variables para animación del helicoptero
bool animaH = false,
movh1 = true,
movh2 = false,
movh3 = false,
movh4 = false,
movh5 = false,
movh6 = false,
movh7 = false,
movh8 = false,
movh9 = false,
movh10 = false,
movh11 = false,
movh12 = false,
movh13 = false,
movh14 = false;

float elevar = 0.0f,
rot_heli = 0.0f,
rot_cuerpo = 0.0f,
mov_cuerpoX = 0.0f,
mov_cuerpoZ = 0.0f,
mov_heliX = 0.0f,
mov_heliZ = 0.0f;

// Variables para luces de bastones
bool animaB = false,
rec1 = true,
rec2 = false;

float posA = 0.0f,
posB = 0.0f;

int seg = 1.0f;

// Posiciones para las luces del arbol y bastones
glm::vec3 pointLightPositions[] = {
	//esferas
	glm::vec3(-4.159f, 1.064f, 4.362f),
	glm::vec3(-4.438f, 1.156f, 3.272f),
	glm::vec3(-4.554f, 2.203f, 4.208f),
	glm::vec3(-4.864f, 3.196f, 3.853f),
	glm::vec3(-4.848f, 3.488f, 4.018f),
	// bastones
	glm::vec3(2.5f, 0.0f, 7.029f),
	glm::vec3(5.5f, 0.0f, 7.029f),

};

glm::vec3 Light1 = glm::vec3(0);
//SE CREAN OTRAS LIGHTS PARA CADA COLOR
glm::vec3 Light2 = glm::vec3(0);
glm::vec3 Light3 = glm::vec3(0);
glm::vec3 Light4 = glm::vec3(0);
glm::vec3 Light5 = glm::vec3(0);

glm::vec3 Light6 = glm::vec3(0);

// Posición de la spotlight
glm::vec3 posicionSP = glm::vec3(0);

// Dirección de la spotlight
glm::vec3 direccionSP = glm::vec3(0);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Función para obtener un delay en las luces de los bastones
void delay(int segundos) {
	for (int a = (time(NULL) + segundos); time(NULL) != a; time(NULL));
}

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	
	Model Sillon((char*)"Models/Sillon/sillon.obj");
	Model Arbol((char*)"Models/Arbol/arbol.obj");
	Model Sofa((char*)"Models/Sofa/sofa.obj");
	Model Piso((char*)"Models/Piso/Piso_casa.obj");
	Model MesaC((char*)"Models/mesa centro/mesaC.obj");
	Model Chim((char*)"Models/Chimenea/chime.obj");
	Model Ventana((char*)"Models/Ventana/ventana.obj");
	Model Puerta((char*)"Models/Puerta/puerta.obj");
	Model Paredes((char*)"Models/Paredes/paredes.obj");
	Model Techo1((char*)"Models/Paredes/techo1.obj");
	Model Techo2((char*)"Models/Paredes/techo2.obj");
	Model Techo3((char*)"Models/Paredes/techo3.obj");
	Model Techo4((char*)"Models/Paredes/techo4.obj");
	Model Techo5((char*)"Models/Paredes/techo5.obj");
	Model Columna((char*)"Models/Paredes/columna.obj");
	Model Pasto((char*)"Models/Piso/pasto.obj");
	Model piso_piedra((char*)"Models/Piso/piso_piedra.obj");
	Model Bota1((char*)"Models/Botas/bota1.obj");
	Model Bota2((char*)"Models/Botas/bota2.obj");
	Model Bota3((char*)"Models/Botas/bota3.obj");
	Model baston((char*)"Models/Baston/baston.obj");
	Model Cama((char*)"Models/Cama/cama.obj");
	Model Desk((char*)"Models/Desk/desk.obj");
	Model Lampara((char*)"Models/Desk/lampara.obj");
	Model Repisa((char*)"Models/Desk/repisa.obj");
	Model Silla((char*)"Models/Silla/silla.obj");
	Model Libro((char*)"Models/Desk/libro.obj");
	// Snowman
	Model cuerpo((char*)"Models/Snowman/cuerpo.obj");
	Model cabeza((char*)"Models/Snowman/cabeza.obj");
	Model sombrero((char*)"Models/Snowman/sombrero.obj");
	Model brazo1((char*)"Models/Snowman/brazo1.obj");
	Model brazo2((char*)"Models/Snowman/brazo2.obj");

	//Carro
	Model carroceria((char*)"Models/Carro/coche.obj");

	Model Trailer((char*)"Models/Trailer/trailer.obj");
	Model Llanta1((char*)"Models/Trailer/llanta.obj");
	Model Llanta2((char*)"Models/Trailer/llantaD.obj");

	// helicoptero
	Model Cuerpo((char*)"Models/Helicopter/cuerpo.obj");
	Model Helice((char*)"Models/Helicopter/helice.obj");

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"), 1);

	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.0f, 0.0f, 0.0f);


		// Point light 1: Luz azul
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		// Point light 2: Luz verde
		glm::vec3 lightColor2;
		lightColor2.x = abs(sin(glfwGetTime() * Light2.x));
		lightColor2.y = abs(sin(glfwGetTime() * Light2.y));
		lightColor2.z = sin(glfwGetTime() * Light2.z);

		// Point light 3: Luz amarilla
		glm::vec3 lightColor3;
		lightColor3.x = abs(sin(glfwGetTime() * Light3.x));
		lightColor3.y = abs(sin(glfwGetTime() * Light3.y));
		lightColor3.z = sin(glfwGetTime() * Light3.z);

		// Point light 4: Luz roja
		glm::vec3 lightColor4;
		lightColor4.x = abs(sin(glfwGetTime() * Light4.x));
		lightColor4.y = abs(sin(glfwGetTime() * Light4.y));
		lightColor4.z = sin(glfwGetTime() * Light4.z);

		// Point light 5: Luz azul
		glm::vec3 lightColor5;
		lightColor5.x = abs(sin(glfwGetTime() * Light5.x));
		lightColor5.y = abs(sin(glfwGetTime() * Light5.y));
		lightColor5.z = sin(glfwGetTime() * Light5.z);

		// Pint light movible A
		glm::vec3 lightColor6;
		lightColor6.x = abs(sin(glfwGetTime() * Light6.x));
		lightColor6.y = abs(sin(glfwGetTime() * Light6.y));
		lightColor6.z = sin(glfwGetTime() * Light6.z);

		// Point Light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.02f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 8.0f);

		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.2f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.02f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 8.0f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lightColor3.x, lightColor3.y, lightColor3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lightColor3.x, lightColor3.y, lightColor3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.2f, 0.2f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.02f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 8.0f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), lightColor4.x, lightColor4.y, lightColor4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), lightColor4.x, lightColor4.y, lightColor4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.2f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.02f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 8.0f);

		// Point light 5
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), lightColor5.x, lightColor5.y, lightColor5.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), lightColor5.x, lightColor5.y, lightColor5.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.2f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.02f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 8.0f);

		// Point light 6
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z + posA);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 8.0f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), posicionSP.x, posicionSP.y, posicionSP.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), direccionSP.x, direccionSP.y, direccionSP.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);


		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);

		//SE CARGAN LOS MODELOS
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(1.0f, 0.01f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.01f, 0.0f, 11.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		piso_piedra.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pasto.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.12f, -6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Chim.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 4.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Arbol.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 3.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sofa.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.5f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sillon.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MesaC.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.5f, 0.0f, -15.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cama.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.15f, 0.0f, -15.1f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Desk.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.5f, 0.0f, -15.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.5f, 1.2f, -14.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.6f, 2.5f, -11.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Repisa.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.6f, 3.0f, -12.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Repisa.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.0f, 1.2f, -14.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Libro.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.9f, 3.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Bota1.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.9f, 3.1f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Bota2.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.9f, 3.1f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Bota3.Draw(lightingShader);

		//************************************************************
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 7.029f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		baston.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 9.029f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		baston.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 11.029f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		baston.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 13.029f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		baston.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 15.029f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		baston.Draw(lightingShader);
		//----------------------
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.5f, 0.0f, 7.029f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		baston.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.5f, 0.0f, 9.029f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		baston.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.5f, 0.0f, 11.029f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		baston.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.5f, 0.0f, 13.029f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		baston.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.5f, 0.0f, 15.029f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		baston.Draw(lightingShader);
		//******************************************************

		// ------------------ SNOWMAN ---------------------
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f + salto, 12.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cuerpo.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 1.3f + salto, 12.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cabeza.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 2.05f + salto, 12.0f));
		//model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sombrero.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.5f, 1.0f + salto, 12.0f));
		model = glm::rotate(model, glm::radians(mov_brazo1), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		brazo1.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-0.5f, 1.0f + salto, 12.0f));
		model = glm::rotate(model, glm::radians(mov_brazo2), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		brazo2.Draw(lightingShader);

		// carro
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.5f, 2.57f, -11.5f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		carroceria.Draw(lightingShader);

		//------------------------------------------------------------------

		//Se cargan las paredes blancas y ventanas
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Paredes.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.0f, 3.0f, 6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ventana.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.0f, 3.0f, 3.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ventana.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.0f, 3.0f, -3.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ventana.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.0f, 3.0f, -9.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ventana.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.0f, 3.0f, -15.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ventana.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 3.0f, -18.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ventana.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-5.0f, 3.0f, -12.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ventana.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, 6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 5.8f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Techo1.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 6.3f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Techo2.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 7.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Techo3.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.0f, 5.5f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Techo4.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 6.32f, -12.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Techo5.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.6f, 2.5f, 6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Columna.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.6f, 2.5f, -5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Columna.Draw(lightingShader);

		// --------------- TRAILER ----------------------------
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.0f + movt_X, 0.0f, 3.0f + movt_Z));
		model = glm::rotate(model, glm::radians(rotacion_tX), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Trailer.Draw(lightingShader);
		// llantas delanteras
		// derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.945f + movt_X, 0.024f, 2.945f + movt_Z));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(girollanta), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Llanta1.Draw(lightingShader);
		// izq
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.053f + movt_X, 0.024f, 2.945f + movt_Z));
		model = glm::rotate(model, glm::radians(girollanta), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Llanta1.Draw(lightingShader);
		// llantas traseras
		// derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.954f + movL_X1, 0.023f, 3.094f + movL_Z1));
		model = glm::rotate(model, glm::radians(rot_L1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(girollanta), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Llanta2.Draw(lightingShader);
		// izq
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.051f + movL_X2, 0.023f, 3.094f + movL_Z2));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rot_L2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rot_L2_1), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(girollanta), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Llanta2.Draw(lightingShader);

		//---------------------------------------------

		// -------------- HELICOPTER ---------------------
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.0f + mov_cuerpoX, 6.55f + elevar, 0.0f + mov_cuerpoZ));
		model = glm::rotate(model, glm::radians(rot_cuerpo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cuerpo.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.0f + mov_heliX, 6.55f + elevar, 0.0f + mov_heliZ));
		model = glm::rotate(model, glm::radians(rot_heli), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Helice.Draw(lightingShader);
		// -----------------------------------------


		glEnable(GL_BLEND);
    	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_BLEND);
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{
		range += 0.001;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_2])
	{
		range -= 0.001;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_3])
	{
        spotAngle += 0.001;
		printf("El spotangle es %f\n", spotAngle);
	}

	if (keys[GLFW_KEY_4])
	{
		spotAngle -= 0.001;
		printf("El spotangle es %f\n", spotAngle);
	}

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	//Animación del tráiler
	if (animaCarro)
	{
		// Avanza hacia adelante
		if (recorrido1)
		{
			if (movt_Z < -8.77f)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
			else
			{
				movt_Z -= 0.01f;
				movL_Z1 -= 0.01f;
				movL_Z2 -= 0.01f;
				girollanta += 5.0f;
			}
		}
		// Choque con la pared
		if (recorrido2)
		{
			if (rotacion_tX < -10.0f)
			{
				recorrido2 = false;
				recorrido3 = true;
			}
			else
			{
				rotacion_tX -= 1.0f;
			}
		}

		if (recorrido3)
		{
			if (rotacion_tX > 5.0f)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
			else
			{
				rotacion_tX += 1.0f;
			}
		}
		// Se despegan las llantas traseras y avanzan hacia adelante hasta chocar con pared
		if (recorrido4)
		{
			if (movL_Z1 < -8.97f)
			{
				recorrido4 = false;
				recorrido5 = true;
				recorrido6 = true;
			}
			else
			{
				rot_L1 = -30.0f;
				rot_L2 = 30.0f;
				movL_Z1 -= 0.001f;
				movL_Z2 -= 0.001f;
				movL_X1 += 0.001f;
				movL_X2 -= 0.001f;
			}
		}
		// La llanta derecha solo restrocede un poco despues de tocar la pared
		if (recorrido5)
		{
			if (movL_Z1 > -8.9f)
			{
				recorrido5 = false;
			}
			else
			{
				movL_Z1 += 0.0005f;
				movL_X1 -= 0.0005f;
			}
		}
		// La llanta izq se cae de lado 
		if (recorrido6)
		{
			if (rot_L2_1 > 89.0f)
			{
				recorrido6 = false;
			}
			else
			{
				rot_L2_1 += 15.0f;
			}
		}


	}

	if (animaSnow)
	{
		// Preparación para el salto
		if (mov1)
		{
			if (mov_brazo1 > 15.0f && mov_brazo2 < -15.0f)
			{
				mov1 = false;
				mov2 = true;
			}
			else
			{
				mov_brazo1 += 1.0f;
				mov_brazo2 -= 1.0f;
			}
		}
		// Salto del muñeco de nieve
		if (mov2)
		{
			if (salto > 1.0f && (mov_brazo1 < -70.0f && mov_brazo2 > 70.0f))
			{
				mov2 = false;
				mov3 = true;
			}
			else
			{
				salto += 0.02f;
				mov_brazo1 -= 1.0f;
				mov_brazo2 += 1.0f;
			}
		}

		//Caida del muñeco de nieve
		if (mov3)
		{
			if (salto < 0.0f)
			{
				mov3 = false;
				mov1 = true;
			}
			else 
			{
				salto -= 0.02f;
				mov_brazo1 += 1.0f;
				mov_brazo2 -= 1.0f;
			}
		}
	}

	// Luz azul
	if (esfera1)
	{
		Light1 = glm::vec3(0.0f, 0.0f, 0.8f);
		Light2 = glm::vec3(0.0f, 0.7f, 0.0f);
		Light3 = glm::vec3(0.7f, 0.7f, 0.0f);
		Light4 = glm::vec3(0.7f, 0.0f, 0.0f);
		Light5 = glm::vec3(0.0f, 0.0f, 0.7f);
	}
	else
	{
		Light1 = glm::vec3(0);
		Light2 = glm::vec3(0);
		Light3 = glm::vec3(0);
		Light4 = glm::vec3(0);
		Light5 = glm::vec3(0);
	}
	
	// Animación helicoptero
	if (animaH)
	{
		if (movh1)
		{
			if (elevar > 2.0f)
			{
				movh1 = false;
				movh2 = true;
			}
			else
			{
				elevar += 0.01f;
				rot_heli += 8.0f;
			}
		}

		if (movh2)
		{
			if (mov_cuerpoX > 2.0f)
			{
				movh2 = false;
				movh3 = true;
			}
			else
			{
				mov_cuerpoX += 0.02f;
				mov_heliX += 0.02f;
				rot_heli += 8.0f;
			}
		}
		
		if (movh3)
		{
			if (rot_cuerpo < -90.0f)
			{
				movh3 = false;
				movh4 = true;
			}
			else
			{
				rot_cuerpo -= 1.0f;
				rot_heli += 8.0f;
			}
		}

		if (movh4)
		{
			if (mov_cuerpoZ > 9.0f)
			{
				movh4 = false;
				movh5 = true;
			}
			else
			{
				mov_cuerpoZ += 0.02f;
				mov_heliZ += 0.02f;
				rot_heli += 8.0f;
			}
		}

		if (movh5)
		{
			if (rot_cuerpo < -180.0f) 
			{
				movh5 = false;
				movh6 = true;
			}
			else
			{
				rot_cuerpo -= 1.0f;
				rot_heli += 8.0f;
			}
		}

		if (movh6)
		{
			if (elevar < -4.0f)
			{
				movh6 = false;
				movh7 = true;
			}
			else
			{
				elevar -= 0.01f;
				rot_heli += 8.0f;
			}
		}

		if (movh7)
		{
			if (mov_cuerpoX < -16.0f)
			{
				movh7 = false;
				movh8 = true;
			}
			else
			{
				mov_cuerpoX -= 0.02f;
				mov_heliX -= 0.02f;
				rot_heli += 8.0f;
			}
		}

		if (movh8)
		{
			if (rot_cuerpo < -270.0f)
			{
				movh8 = false;
				movh9 = true;
			}
			else
			{
				rot_cuerpo -= 1.0f;
				rot_heli += 8.0f;
			}
		}

		if (movh9)
		{
			if (mov_cuerpoZ < -8.0f)
			{
				movh9 = false;
				movh10 = true;
			}
			else
			{
				mov_cuerpoZ -= 0.02f;
				mov_heliZ -= 0.02f;
				rot_heli += 8.0f;
			}
		}

		if (movh10)
		{
			if (rot_cuerpo < -395.0f)
			{
				movh10 = false;
				movh11 = true;
			}
			else
			{
				rot_cuerpo -= 1.0f;
				rot_heli += 8.0f;
			}
		}

		if (movh11)
		{
			if (elevar > 1.99f)
			{
				movh11 = false;
				movh12 = true;
			}
			else
			{
				elevar += 0.01f;
				rot_heli += 5.0f;
			}
		}

		if (movh12)
		{
			if (mov_cuerpoX > -8.05f)
			{
				movh12 = false;
				movh13 = true;
			}
			else
			{
				mov_cuerpoX += 0.02f;
				mov_cuerpoZ += 0.02f;
				mov_heliX += 0.02f;
				mov_heliZ += 0.02f;
				rot_heli += 5.0f;
			}
		}

		if (movh13)
		{
			if (rot_cuerpo > -360.0f)
			{
				movh13 = false;
				movh14 = true;
			}
			else
			{
				rot_cuerpo += 1.0f;
				rot_heli += 5.0f;
			}
		}
		
		if (movh14)
		{
			if (mov_cuerpoX > 0.0f)
			{
				movh14 = false;
				rot_cuerpo = 0.0f;
				mov_cuerpoX = 0.0f;
				mov_cuerpoZ = 0.0f;
				mov_heliX = 0.0f;
				mov_heliZ = 0.0f;
				rot_heli = 0.0f;
				movh2 = true;
			}
			else
			{
				mov_cuerpoX += 0.02f;
				mov_heliX += 0.02f;
				rot_heli += 5.0f;
			}
		}
	}

	// Animación en las luces de los bastones
	if (animaB)
	{
		Light6 = glm::vec3(0.7f, 0.7f, 0.7f);
		if (rec1)
		{
			if (posA > 8.0f)
			{
				rec1 = false;
				rec2 = true;
			}
			else
			{
				delay(seg);
				posA += 2.0f;
			}
		}

		if (rec2)
		{
			if (posA < 0.0f)
			{
				rec2 = false;
				rec1 = true;
			}
			else
			{
				delay(seg);
				posA -= 2.0f;
			}
		}
		
	}
	else
	{
		Light6 = glm::vec3(0.0f);
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
	
	// tecla SPACE para activar las luces del arbol
	if (keys[GLFW_KEY_SPACE])
	{
		esfera1 = !esfera1;
	}

	// tecla C para activar la animación del carro
	if (keys[GLFW_KEY_C])
	{
		animaCarro = !animaCarro;
	}

	// tecla N para activa la animación del snowman
	if (keys[GLFW_KEY_N])
	{
		animaSnow = !animaSnow;
	}

	if (keys[GLFW_KEY_H])
	{
		animaH = !animaH;
	}

	if (keys[GLFW_KEY_B])
	{
		animaB = !animaB;
	}

}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}