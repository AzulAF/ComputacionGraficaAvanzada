//Proyecto de semestre 2024-1
#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

// Font rendering include
#include "Headers/FontTypeRendering.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// ShadowBox include
#include "Headers/ShadowBox.h"

// OpenAL include
#include <AL/alut.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;
float MCScale = 0.01091f;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
// Shader para dibujar un objeto con solo textura
Shader shaderTexture;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;
// Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para las particulas de fountain
Shader shaderParticlesFountain;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 7.0;

Sphere skyboxSphere(20, 20);
Box boxCesped;
Box boxWalls;
Box boxHighway;
Box boxLandingPad;
Sphere esfera1(10, 10);
Box boxCollider;
Sphere sphereCollider(10, 10);
Cylinder rayModel(10, 10, 1.0, 1.0, 1.0);
Box boxIntro;
Box boxViewDepth;
// Models complex instances
Model modelRock;
Model modelTiendas1;
Model modelTiendas2;
Model modelBasura;
Model modelPared;
Model modelTecho;
Model modelMuebles;
Model modelTelevision1;
Model modelTelevision2;
Model modelTelevision3;
Model modelTiendasSalida;


// Modelos animados
// Mayow
Model mayowModelAnimate;

// Terrain model instance
Terrain terrain(-1, -1, 200, 8, "../Textures/heightmap2.png");

ShadowBox * shadowBox;

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint skyboxTextureID;
GLuint textureInit1ID, textureInit2ID, textureActivaID, textureScreenID;
GLuint textureParticleFountainID;

bool iniciaPartida = false, presionarOpcion = false;

// Modelo para el render del texto
FontTypeRendering::FontTypeRendering *modelText;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Elementos_proyecto/skybox/all_black/black.jpg",
		"../Elementos_proyecto/skybox/all_black/black.jpg",
		"../Elementos_proyecto/skybox/all_black/black.jpg",
		"../Elementos_proyecto/skybox/all_black/black.jpg",
		"../Elementos_proyecto/skybox/all_black/black.jpg",
		"../Elementos_proyecto/skybox/all_black/black.jpg" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;


// Model matrix definitions
glm::mat4 modelMatrixEclipse = glm::mat4(1.0f);
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 modelMatrixHeli = glm::mat4(1.0f);
glm::mat4 modelMatrixLambo = glm::mat4(1.0);
glm::mat4 modelMatrixAircraft = glm::mat4(1.0);
glm::mat4 modelMatrixDart = glm::mat4(1.0f);
glm::mat4 modelMatrixBuzz = glm::mat4(1.0f);

glm::mat4 modelMatrixCowboy = glm::mat4(1.0f);
glm::mat4 modelMatrixGuardian = glm::mat4(1.0f);
glm::mat4 modelMatrixCyborg = glm::mat4(1.0f);
glm::mat4 modelMatrixFountain = glm::mat4(1.0f);

//Keep this matrixes
glm::mat4 modelMatrixMayow = glm::mat4(1.0f);
glm::mat4 matrixModelTiendas1 = glm::mat4(1.0);
glm::mat4 matrixModelTiendas1Ext = glm::mat4(1.0);
glm::mat4 matrixModelTiendas2 = glm::mat4(1.0);
glm::mat4 matrixModelTiendas2Ext = glm::mat4(1.0);
glm::mat4 matrixModelBasura = glm::mat4(1.0);
glm::mat4 matrixModelPared = glm::mat4(1.0);
glm::mat4 matrixModelTecho = glm::mat4(1.0);
glm::mat4 matrixModelTelevision1 = glm::mat4(1.0);
glm::mat4 matrixModelTelevision2 = glm::mat4(1.0);
glm::mat4 matrixModelTelevision3 = glm::mat4(1.0);
glm::mat4 matrixModelTiendasSalida = glm::mat4(1.0);
glm::mat4 matrixModelTelevision1Ext = glm::mat4(1.0);
glm::mat4 matrixModelTelevision2Ext = glm::mat4(1.0);
glm::mat4 matrixModelTelevision3Ext = glm::mat4(1.0);

//Matrices para el conjunto de sillas y mesas chicas
glm::mat4 matrixModelMuebles = glm::mat4(1.0);
glm::mat4 matrixModelMuebles2 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles3 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles4 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles5 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles6 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles7 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles8 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles9 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles10 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles11 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles12 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles13 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles14 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles15 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles16 = glm::mat4(1.0);

glm::mat4 matrixModelMuebles17 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles18 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles19 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles20 = glm::mat4(1.0);

glm::mat4 matrixModelMuebles21 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles22 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles23 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles24 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles25 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles26 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles27 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles28 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles29 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles30 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles31 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles32 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles33 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles34 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles35 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles36 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles37 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles38 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles39 = glm::mat4(1.0);
glm::mat4 matrixModelMuebles40 = glm::mat4(1.0);
//Covers
glm::mat4 matrixModelMueblesExt1 = glm::mat4(1.0);
glm::mat4 matrixModelMueblesExt2 = glm::mat4(1.0);
glm::mat4 matrixModelMueblesExt3 = glm::mat4(1.0);
glm::mat4 matrixModelMueblesExt4 = glm::mat4(1.0);
glm::mat4 matrixModelMueblesExt5 = glm::mat4(1.0);
glm::mat4 matrixModelMueblesExt6 = glm::mat4(1.0);
glm::mat4 matrixModelMueblesExt7 = glm::mat4(1.0);
glm::mat4 matrixModelMueblesExt8 = glm::mat4(1.0);



int animationMayowIndex = 3;
int modelSelected = 0;
bool enableCountSelected = true;




// Var animate helicopter
float rotHelHelY = 0.0;
float rotHelHelBack = 0.0;

// Var animate lambo dor
int stateDoor = 0;
float dorRotCount = 0.0;

// Lamps position
std::vector<glm::vec3> lamp1Position = {
	//PRIMERA TELEVISION
	glm::vec3(8.0f, 3.12f, -6.2f),
	glm::vec3(-8.5f, 3.12f, 39.1f),
	glm::vec3(-25.0f, 3.12f, -6.2f),
	//repeticion de telvision
	glm::vec3(-41.5f, 3.12f, 39.1f),
	glm::vec3(-58.0f, 3.12f, -6.2f),
	glm::vec3(-74.5f, 3.12f, 39.1f)

};
std::vector<float> lamp1Orientation = {
	-17.0, -82.67, 23.70
};
std::vector<glm::vec3> lamp2Position = {
	glm::vec3(-36.52, 0, -23.24),
	glm::vec3(-52.73, 0, -3.90)
};
std::vector<float> lamp2Orientation = {
	21.37 + 90, -65.0 + 90
};

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"aircraft", glm::vec3(10.0, 0.0, -17.5)},
		{"lambo", glm::vec3(23.0, 0.0, 0.0)},
		{"heli", glm::vec3(5.0, 10.0, -5.0)}
};

double deltaTime;
double currTime, lastTime;


// Jump variables
bool isJump = false;
float GRAVITY = 1.81;
double tmv = 0;
double startTimeJump = 0;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// Variables animacion maquina de estados eclipse
const float avance = 0.1;
const float giroEclipse = 0.5f;
float rotWheelsX = 0.0;
float rotWheelsY = 0.0;

// OpenAL Defines
#define NUM_BUFFERS 3
#define NUM_SOURCES 3
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
// Source 3
ALfloat source3Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source3Vel[] = { 0.0, 0.0, 0.0 };
// Source 4
ALfloat source4Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source4Vel[] = { 0.0, 0.0, 0.0 };
// Source 5
ALfloat source5Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source5Vel[] = { 0.0, 0.0, 0.0 };
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = {true, true, true};

// Framesbuffers
GLuint depthMap, depthMapFBO;

//Definición del framebuffer
GLuint initVel, startTime, VAOParticles, nParticles = 400;
double curTimeParticlesFountainAnimation, lastTimeParticlesFountainAnimation; 

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);


// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(), glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr, nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderTexture.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");
	shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	rayModel.init();
	rayModel.setShader(&shader);
	rayModel.setColor(glm::vec4(1.0));

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxWalls.init();
	boxWalls.setShader(&shaderMulLighting);

	boxHighway.init();
	boxHighway.setShader(&shaderMulLighting);

	boxLandingPad.init();
	boxLandingPad.setShader(&shaderMulLighting);

	esfera1.init();
	esfera1.setShader(&shaderMulLighting);

	boxIntro.init();
	boxIntro.setShader(&shaderTexture);
	boxIntro.setScale(glm::vec3(2.0, 2.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);


	// Mayow
	//mayowModelAnimate.loadModel("../models/mayow/personaje2.fbx");
	mayowModelAnimate.loadModel("../Elementos_proyecto/main_animated/main_animated.fbx");
	mayowModelAnimate.setShader(&shaderMulLighting);
	
	//Primeras tiendas, segmento más corto
	modelTiendas1.loadModel("../Elementos_proyecto/Tiendas_menor/tiendas_menor_ext.fbx");
	modelTiendas1.setShader(&shaderMulLighting);

	modelTiendas2.loadModel("../Elementos_proyecto/tiendas_mayor/tiendas_mayor_ext.fbx");
	modelTiendas2.setShader(&shaderMulLighting);

	modelBasura.loadModel("../Elementos_proyecto/trash/trash.fbx");
	modelBasura.setShader(&shaderMulLighting);

	modelPared.loadModel("../Elementos_proyecto/Ambiente/Paredes.fbx");
	modelPared.setShader(&shaderMulLighting);

	modelTecho.loadModel("../Elementos_proyecto/Ambiente/Techo.fbx");
	modelTecho.setShader(&shaderMulLighting);

	modelMuebles.loadModel("../Elementos_proyecto/Sillas_Mesas/sillas_mesas_cortas.fbx");
	modelMuebles.setShader(&shaderMulLighting);

	modelTelevision1.loadModel("../Elementos_proyecto/Ambiente/tv1.fbx");
	modelTelevision1.setShader(&shaderMulLighting);

	modelTelevision2.loadModel("../Elementos_proyecto/Ambiente/tv2.fbx");
	modelTelevision2.setShader(&shaderMulLighting);

	modelTelevision3.loadModel("../Elementos_proyecto/Ambiente/tv3.fbx");
	modelTelevision3.setShader(&shaderMulLighting);

	modelTiendasSalida.loadModel("../Elementos_proyecto/Ambiente/door.fbx");
	modelTiendasSalida.setShader(&shaderMulLighting);

	// Terreno
	terrain.init();
	terrain.setShader(&shaderTerrain);

	// Se inicializa el model de render text
	modelText = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelText->Initialize();

	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);
	
	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		skyboxTexture.loadImage(true);
		if (skyboxTexture.getData()) {
			glTexImage2D(types[i], 0, skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, skyboxTexture.getWidth(), skyboxTexture.getHeight(), 0,
			skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, skyboxTexture.getData());
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage();
	}

	// Definiendo la textura a utilizar
	Texture textureCesped("../Textures/tiles.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureCesped.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureCespedID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureCesped.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		std::cout << "Numero de canales :=> " << textureCesped.getChannels() << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, textureCesped.getWidth(), textureCesped.getHeight(), 0,
		textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureCesped.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCesped.freeImage();

	// Defininiendo texturas del mapa de mezclas
	// Definiendo la textura
	Texture textureR("../Textures/mud.png");
	textureR.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainRID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureR.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, textureR.getWidth(), textureR.getHeight(), 0,
		textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureR.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureR.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureG("../Textures/grassFlowers.png");
	textureG.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainGID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureG.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, textureG.getWidth(), textureG.getHeight(), 0,
		textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureG.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureG.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureB("../Textures/path.png");
	textureB.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureB.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, textureB.getWidth(), textureB.getHeight(), 0,
		textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureB.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureB.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureBlendMap("../Textures/blendMap_black.png");
	textureBlendMap.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBlendMapID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureBlendMap.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, textureBlendMap.getWidth(), textureBlendMap.getHeight(), 0,
		textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureBlendMap.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureBlendMap.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureIntro1("../Textures/Intro1_v2.png");
	textureIntro1.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit1ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit1ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro1.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro1.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro1.getWidth(), textureIntro1.getHeight(), 0,
		textureIntro1.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro1.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro1.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureIntro2("../Textures/Intro2_v2.png");
	textureIntro2.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit2ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit2ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro2.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro2.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro2.getWidth(), textureIntro2.getHeight(), 0,
		textureIntro2.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro2.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro2.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureScreen("../Textures/Screen.png");
	textureScreen.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreenID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreenID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureScreen.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen.getWidth(), textureScreen.getHeight(), 0,
		textureScreen.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen.freeImage(); // Liberamos memoria


	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../Elementos_proyecto/sound/music.wav");
	buffer[1] = alutCreateBufferFromFile("../Elementos_proyecto/sound/music.wav");
	buffer[2] = alutCreateBufferFromFile("../Elementos_proyecto/sound/music.wav");
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR){
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	//On al GAIN last parameter is VOLUME

	float volumenAudios= 2.5f;
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, volumenAudios);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, volumenAudios);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 1000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, volumenAudios);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 2000);
	//
	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, volumenAudios);
	alSourcefv(source[2], AL_POSITION, source3Pos);
	alSourcefv(source[2], AL_VELOCITY, source3Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 2000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, volumenAudios);
	alSourcefv(source[2], AL_POSITION, source4Pos);
	alSourcefv(source[2], AL_VELOCITY, source4Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 2000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, volumenAudios);
	alSourcefv(source[2], AL_POSITION, source5Pos);
	alSourcefv(source[2], AL_VELOCITY, source5Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 2000);

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	/*shaderParticlesFountain.destroy();*/

	// Basic objects Delete
	boxCesped.destroy();
	modelTiendas1.destroy();
	modelTiendas2.destroy();
	modelBasura.destroy();
	modelPared.destroy();
	modelTecho.destroy();
	modelMuebles.destroy();
	modelTelevision1.destroy();
	modelTelevision2.destroy();
	modelTelevision3.destroy();
	modelTiendasSalida.destroy();

	// Custom objects Delete
	mayowModelAnimate.destroy();


	// Terrains objects Delete
	terrain.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureInit1ID);
	glDeleteTextures(1, &textureInit2ID);
	glDeleteTextures(1, &textureScreenID);
	glDeleteTextures(1, &textureParticleFountainID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if(!iniciaPartida){
		bool presionarEnter = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
		if(textureActivaID == textureInit1ID && presionarEnter){
			iniciaPartida = true;
			textureActivaID = textureScreenID;
		}
		else if(!presionarOpcion && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			presionarOpcion = true;
			if(textureActivaID == textureInit1ID)
				textureActivaID = textureInit2ID;
			else if(textureActivaID == textureInit2ID)
				textureActivaID = textureInit1ID;
		}
		else if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
			presionarOpcion = false;
	}

	if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE) {
		std::cout << "Esta presente el joystick" << std::endl;
		int axesCount, buttonCount;
		const float * axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		std::cout << "Número de ejes disponibles :=>" << axesCount << std::endl;
		std::cout << "Left Stick X axis: " << axes[0] << std::endl;
		std::cout << "Left Stick Y axis: " << axes[1] << std::endl;
		std::cout << "Left Trigger/L2: " << axes[2] << std::endl;
		std::cout << "Right Stick X axis: " << axes[3] << std::endl;
		std::cout << "Right Stick Y axis: " << axes[4] << std::endl;
		std::cout << "Right Trigger/R2: " << axes[5] << std::endl;

		if(fabs(axes[1]) > 0.2){
			modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0, 0, -axes[1] * 0.1));
			animationMayowIndex = 2;
		}if(fabs(axes[0]) > 0.2){
			modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-axes[0] * 0.5f), glm::vec3(0, 1, 0));
			animationMayowIndex = 2;
		}

		if(fabs(axes[3]) > 0.2){
			camera->mouseMoveCamera(axes[3], 0.0, deltaTime);
		}if(fabs(axes[4]) > 0.2){
			camera->mouseMoveCamera(0.0, axes[4], deltaTime);
		}

		const unsigned char * buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		std::cout << "Número de botones disponibles :=>" << buttonCount << std::endl;
		if(buttons[0] == GLFW_PRESS)
			std::cout << "Se presiona x" << std::endl;

		if(!isJump && buttons[0] == GLFW_PRESS){
			isJump = true;
			startTimeJump = currTime;
			tmv = 0;
		}
	}

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);
	}
		
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);

	offsetX = 0;
	offsetY = 0;

	
	// Controles de mayow
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		modelMatrixMayow = glm::rotate(modelMatrixMayow, 0.02f, glm::vec3(0, 1, 0));
		animationMayowIndex = 2;
	} else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		modelMatrixMayow = glm::rotate(modelMatrixMayow, -0.02f, glm::vec3(0, 1, 0));
		animationMayowIndex = 2;
	}
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, 0.30));
		animationMayowIndex = 2;
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, -0.30));
		animationMayowIndex = 2;
	}

	bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if(!isJump && keySpaceStatus){
		isJump = true;
		startTimeJump = currTime;
		tmv = 0;
	}

	glfwPollEvents();
	return continueApplication;
}

void prepareScene(){

	terrain.setShader(&shaderTerrain);
	
	//Segmento corto de las tiendas
	modelTiendas1.setShader(&shaderMulLighting);

	//Segmento largo de las tiendas
	modelTiendas2.setShader(&shaderMulLighting);

	//Botes de basura
	modelBasura.setShader(&shaderMulLighting);

	modelPared.setShader(&shaderMulLighting);
	modelTecho.setShader(&shaderMulLighting);
	modelMuebles.setShader(&shaderMulLighting);
	modelTelevision1.setShader(&shaderMulLighting);
	modelTelevision2.setShader(&shaderMulLighting);
	modelTelevision3.setShader(&shaderMulLighting);

	modelTiendasSalida.setShader(&shaderMulLighting);

	//Mayow
	mayowModelAnimate.setShader(&shaderMulLighting);

}

void prepareDepthScene(){

	terrain.setShader(&shaderDepth);

	//Segmento corto de las tiendas
	modelTiendas1.setShader(&shaderDepth);

	//Segmento largo de las tiendas
	modelTiendas2.setShader(&shaderDepth);

	//Basura
	modelBasura.setShader(&shaderDepth);
	modelPared.setShader(&shaderDepth);
	modelTecho.setShader(&shaderDepth);
	modelMuebles.setShader(&shaderDepth);
	modelTelevision1.setShader(&shaderDepth);
	modelTelevision2.setShader(&shaderDepth);
	modelTelevision3.setShader(&shaderDepth);

	modelTiendasSalida.setShader(&shaderDepth);


	//Mayow
	mayowModelAnimate.setShader(&shaderDepth);

}

void renderSolidScene(){
	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	// Se activa la textura del agua
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	shaderTerrain.setInt("backgroundTexture", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(80, 80)));
	terrain.setPosition(glm::vec3(100, 0, 100));
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Custom objects obj
	 *******************************************/
	// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	//Render de tiendas
	//matrixModelTiendas1[3][1] = terrain.getHeightTerrain(matrixModelTiendas1[3][0], matrixModelTiendas1[3][2]);
	modelTiendas1.render(matrixModelTiendas1);
	modelTiendas1.render(matrixModelTiendas1Ext);
	modelTiendas2.render(matrixModelTiendas2);
	modelTiendas2.render(matrixModelTiendas2Ext);
	modelBasura.render(matrixModelBasura);
	modelTecho.render(matrixModelTecho);
	modelPared.render(matrixModelPared);
	modelTelevision1.render(matrixModelTelevision1);
	modelTelevision2.render(matrixModelTelevision2);
	modelTelevision3.render(matrixModelTelevision3);
	modelTelevision1.render(matrixModelTelevision1Ext);
	modelTelevision2.render(matrixModelTelevision2Ext);
	modelTelevision3.render(matrixModelTelevision3Ext);
	modelTiendasSalida.render(matrixModelTiendasSalida);

	//Renderizacion de sillas y mesas cortas 
	modelMuebles.render(matrixModelMuebles);
	modelMuebles.render(matrixModelMuebles2);
	modelMuebles.render(matrixModelMuebles3);
	modelMuebles.render(matrixModelMuebles4);
	modelMuebles.render(matrixModelMuebles5);
	modelMuebles.render(matrixModelMuebles6);
	modelMuebles.render(matrixModelMuebles7);
	modelMuebles.render(matrixModelMuebles8);
	modelMuebles.render(matrixModelMuebles9);
	modelMuebles.render(matrixModelMuebles10);
	modelMuebles.render(matrixModelMuebles11);
	modelMuebles.render(matrixModelMuebles12);
	modelMuebles.render(matrixModelMuebles13);
	modelMuebles.render(matrixModelMuebles14);
	modelMuebles.render(matrixModelMuebles15);
	modelMuebles.render(matrixModelMuebles16);
	
	modelMuebles.render(matrixModelMuebles17);
	modelMuebles.render(matrixModelMuebles18);
	modelMuebles.render(matrixModelMuebles19);
	modelMuebles.render(matrixModelMuebles20);
	modelMuebles.render(matrixModelMuebles21);
	modelMuebles.render(matrixModelMuebles22);
	modelMuebles.render(matrixModelMuebles23);
	modelMuebles.render(matrixModelMuebles24);
	/*
	modelMuebles.render(matrixModelMuebles25);
	modelMuebles.render(matrixModelMuebles26);
	modelMuebles.render(matrixModelMuebles27);
	modelMuebles.render(matrixModelMuebles28);
	modelMuebles.render(matrixModelMuebles29);
	modelMuebles.render(matrixModelMuebles30);
	modelMuebles.render(matrixModelMuebles31);
	modelMuebles.render(matrixModelMuebles32);
	modelMuebles.render(matrixModelMuebles33);
	modelMuebles.render(matrixModelMuebles34);
	modelMuebles.render(matrixModelMuebles35);
	modelMuebles.render(matrixModelMuebles36);
	modelMuebles.render(matrixModelMuebles37);
	modelMuebles.render(matrixModelMuebles38);
	modelMuebles.render(matrixModelMuebles39);
	modelMuebles.render(matrixModelMuebles40);
*/
//Cover
	modelMuebles.render(matrixModelMueblesExt1);
	modelMuebles.render(matrixModelMueblesExt2);
	modelMuebles.render(matrixModelMueblesExt3);
	modelMuebles.render(matrixModelMueblesExt4);
	modelMuebles.render(matrixModelMueblesExt5);
	modelMuebles.render(matrixModelMueblesExt6);
	modelMuebles.render(matrixModelMueblesExt7);
	modelMuebles.render(matrixModelMueblesExt8);

	glActiveTexture(GL_TEXTURE0);



	/*****************************************
	 * Objetos animados por huesos
	 * **************************************/
	glm::vec3 ejey = glm::normalize(terrain.getNormalTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]));
	glm::vec3 ejex = glm::vec3(modelMatrixMayow[0]);
	glm::vec3 ejez = glm::normalize(glm::cross(ejex, ejey));
	ejex = glm::normalize(glm::cross(ejey, ejez));
	modelMatrixMayow[0] = glm::vec4(ejex, 0.0);
	modelMatrixMayow[1] = glm::vec4(ejey, 0.0);
	modelMatrixMayow[2] = glm::vec4(ejez, 0.0);
	modelMatrixMayow[3][1] = -GRAVITY * tmv * tmv + 3.5 * tmv + terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
	tmv = currTime - startTimeJump;
	if(modelMatrixMayow[3][1] < terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2])){
		isJump = false;
		modelMatrixMayow[3][1] = terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
	}
	glm::mat4 modelMatrixMayowBody = glm::mat4(modelMatrixMayow);
	
	//modelMatrixMayowBody = glm::scale(modelMatrixMayowBody, glm::vec3(MCScale));
	modelMatrixMayowBody = glm::scale(modelMatrixMayowBody, glm::vec3(0.0012f));
	mayowModelAnimate.setAnimationIndex(animationMayowIndex);
	mayowModelAnimate.render(modelMatrixMayowBody);
	//animationMayowIndex = 1;


	/*******************************************
	 * Skybox
	 *******************************************/
	GLint oldCullFaceMode;
	GLint oldDepthFuncMode;
	// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
	glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
	shaderSkybox.setFloat("skybox", 0);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glActiveTexture(GL_TEXTURE0);
	skyboxSphere.render();
	glCullFace(oldCullFaceMode);
	glDepthFunc(oldDepthFuncMode);
}

void renderAlphaScene(bool render = true){
	/**********
	 * Update the position with alpha objects
	 */
	// Update the aircraft
	blendingUnsorted.find("aircraft")->second = glm::vec3(modelMatrixAircraft[3]);
	// Update the lambo
	blendingUnsorted.find("lambo")->second = glm::vec3(modelMatrixLambo[3]);
	// Update the helicopter
	blendingUnsorted.find("heli")->second = glm::vec3(modelMatrixHeli[3]);

	/**********
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for(itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++){
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

	/**********
	 * Render de las transparencias
	 */

	if(render){
		/************Render de imagen de frente**************/
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureActivaID);
		shaderTexture.setInt("outTexture", 0);
		glEnable(GL_BLEND);
		boxIntro.render();
		glDisable(GL_BLEND);

		modelText->render("Videojuego de terror, Mapa 1", -1, 0);
	}
}

void renderScene(){
	renderSolidScene();
	renderAlphaScene(false);
}

void applicationLoop() {
	bool psi = true;

	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	//Matrices de tiendas (1) corto, (2) largo
	matrixModelTiendas1 = glm::translate(matrixModelTiendas1, glm::vec3(23.0f, 2.0f, -5.0f));
	matrixModelTiendas1 = glm::rotate(matrixModelTiendas1, glm::radians(-90.0f), glm::vec3(1,0,0));

	matrixModelTiendas2 = glm::translate(matrixModelTiendas2, glm::vec3(-1.0f, 2.0f, -7.2f));
	matrixModelTiendas2 = glm::rotate(matrixModelTiendas2, glm::radians(-90.0f), glm::vec3(1,0,0));
	matrixModelTiendas2 = glm::rotate(matrixModelTiendas2, glm::radians(90.0f), glm::vec3(0,0,1));

	matrixModelTiendas2Ext = glm::translate(matrixModelTiendas2Ext, glm::vec3(-58.0f, 2.0f, 40.2f));
	matrixModelTiendas2Ext = glm::rotate(matrixModelTiendas2Ext, glm::radians(-90.0f), glm::vec3(1,0,0));
	matrixModelTiendas2Ext = glm::rotate(matrixModelTiendas2Ext, glm::radians(-90.0f), glm::vec3(0,0,1));

	matrixModelTiendas1Ext = glm::translate(matrixModelTiendas1Ext, glm::vec3(-84.0f, 2.0f, 38.10f));
	matrixModelTiendas1Ext = glm::rotate(matrixModelTiendas1Ext, glm::radians(-90.0f), glm::vec3(1,0,0));
	matrixModelTiendas1Ext = glm::rotate(matrixModelTiendas1Ext, glm::radians(180.0f), glm::vec3(0,0,1));
	//

	//Puerta
	matrixModelTiendasSalida = glm::translate(matrixModelTiendasSalida, glm::vec3(-82.0f, 1.0f, 40.0f));
	matrixModelTiendasSalida = glm::rotate(matrixModelTiendasSalida, glm::radians(-90.0f), glm::vec3(1,0,0));
	matrixModelTiendasSalida = glm::rotate(matrixModelTiendasSalida, glm::radians(90.0f), glm::vec3(0,0,1));
	matrixModelTiendasSalida= glm::scale(matrixModelTiendasSalida, glm::vec3(0.00129f));

	//Basura
	matrixModelBasura = glm::translate(matrixModelBasura, glm::vec3(5.0f, 1.5f, 7.2f));
	matrixModelBasura = glm::rotate(matrixModelBasura, glm::radians(-90.0f), glm::vec3(1,0,0));
	matrixModelBasura = glm::scale(matrixModelBasura, glm::vec3(0.01f, 0.5f, 0.5f));

	//Muebles

	/*********MESAS Y SILLAS CHICAS******
	
	*************************************/
	matrixModelMuebles = glm::translate(matrixModelMuebles, glm::vec3(0.0f, 0.99f, 7.2f));
	matrixModelMuebles = glm::scale(matrixModelMuebles, glm::vec3(0.012f));

	matrixModelMuebles2 = glm::translate(matrixModelMuebles2, glm::vec3(-8.50f, 0.99f, 7.2f));
	matrixModelMuebles2 = glm::scale(matrixModelMuebles2, glm::vec3(0.012f));

	matrixModelMuebles3 = glm::translate(matrixModelMuebles3, glm::vec3(-25.0f, 0.99f, 7.2f));
	matrixModelMuebles3 = glm::scale(matrixModelMuebles3, glm::vec3(0.012f));

	matrixModelMuebles4 = glm::translate(matrixModelMuebles4, glm::vec3(-33.50f, 0.99f, 7.2f));
	matrixModelMuebles4 = glm::scale(matrixModelMuebles4, glm::vec3(0.012f));

	matrixModelMuebles5 = glm::translate(matrixModelMuebles5, glm::vec3(-42.0f, 0.99f, 7.2f));
	matrixModelMuebles5 = glm::scale(matrixModelMuebles5, glm::vec3(0.012f));

	matrixModelMuebles6 = glm::translate(matrixModelMuebles6, glm::vec3(-50.5f, 0.99f, 7.2f));
	matrixModelMuebles6 = glm::scale(matrixModelMuebles6, glm::vec3(0.012f));

	matrixModelMuebles7 = glm::translate(matrixModelMuebles7, glm::vec3(-59.0f, 0.99f, 7.2f));
	matrixModelMuebles7 = glm::scale(matrixModelMuebles7, glm::vec3(0.012f));

	matrixModelMuebles8 = glm::translate(matrixModelMuebles8, glm::vec3(-67.50f, 0.99f, 7.2f));
	matrixModelMuebles8 = glm::scale(matrixModelMuebles8, glm::vec3(0.012f));

// rep al lado 2
	matrixModelMuebles9 = glm::translate(matrixModelMuebles9, glm::vec3(0.0f, 0.99f, 17.2f));
	matrixModelMuebles9 = glm::scale(matrixModelMuebles9, glm::vec3(0.012f));

	matrixModelMuebles10 = glm::translate(matrixModelMuebles10, glm::vec3(-8.50f, 0.99f, 17.2f));
	matrixModelMuebles10 = glm::scale(matrixModelMuebles10, glm::vec3(0.012f));

	matrixModelMuebles11 = glm::translate(matrixModelMuebles11, glm::vec3(-25.0f, 0.99f, 17.2f));
	matrixModelMuebles11 = glm::scale(matrixModelMuebles11, glm::vec3(0.012f));

	matrixModelMuebles12 = glm::translate(matrixModelMuebles12, glm::vec3(-33.50f, 0.99f, 17.2f));
	matrixModelMuebles12 = glm::scale(matrixModelMuebles12, glm::vec3(0.012f));

	matrixModelMuebles13 = glm::translate(matrixModelMuebles13, glm::vec3(-42.0f, 0.99f, 17.2f));
	matrixModelMuebles13 = glm::scale(matrixModelMuebles13, glm::vec3(0.012f));

	matrixModelMuebles14 = glm::translate(matrixModelMuebles14, glm::vec3(-50.5f, 0.99f, 17.2f));
	matrixModelMuebles14 = glm::scale(matrixModelMuebles14, glm::vec3(0.012f));

	matrixModelMuebles15 = glm::translate(matrixModelMuebles15, glm::vec3(-59.0f, 0.99f, 17.2f));
	matrixModelMuebles15 = glm::scale(matrixModelMuebles15, glm::vec3(0.012f));

	matrixModelMuebles16 = glm::translate(matrixModelMuebles16, glm::vec3(-67.50f, 0.99f, 17.2f));
	matrixModelMuebles16 = glm::scale(matrixModelMuebles16, glm::vec3(0.012f));
	
	//Rep al lado 3

	matrixModelMuebles17 = glm::translate(matrixModelMuebles17, glm::vec3(0.0f, 0.99f, 27.2f));
	matrixModelMuebles17 = glm::scale(matrixModelMuebles17, glm::vec3(0.012f));

	matrixModelMuebles18 = glm::translate(matrixModelMuebles18, glm::vec3(-8.50f, 0.99f, 27.2f));
	matrixModelMuebles18 = glm::scale(matrixModelMuebles18, glm::vec3(0.012f));

	matrixModelMuebles19 = glm::translate(matrixModelMuebles19, glm::vec3(-25.0f, 0.99f, 27.2f));
	matrixModelMuebles19 = glm::scale(matrixModelMuebles19, glm::vec3(0.012f));

	matrixModelMuebles20 = glm::translate(matrixModelMuebles20, glm::vec3(-33.50f, 0.99f, 27.2f));
	matrixModelMuebles20 = glm::scale(matrixModelMuebles20, glm::vec3(0.012f));

	matrixModelMuebles21 = glm::translate(matrixModelMuebles21, glm::vec3(-42.0f, 0.99f, 27.2f));
	matrixModelMuebles21 = glm::scale(matrixModelMuebles21, glm::vec3(0.012f));

	matrixModelMuebles22 = glm::translate(matrixModelMuebles22, glm::vec3(-50.5f, 0.99f, 27.2f));
	matrixModelMuebles22 = glm::scale(matrixModelMuebles22, glm::vec3(0.012f));

	matrixModelMuebles23 = glm::translate(matrixModelMuebles23, glm::vec3(-59.0f, 0.99f, 27.2f));
	matrixModelMuebles23 = glm::scale(matrixModelMuebles23, glm::vec3(0.012f));

	matrixModelMuebles24 = glm::translate(matrixModelMuebles24, glm::vec3(-67.50f, 0.99f, 27.2f));
	matrixModelMuebles24 = glm::scale(matrixModelMuebles24, glm::vec3(0.012f));

	//Rep al lado 0

	matrixModelMuebles25 = glm::translate(matrixModelMuebles25, glm::vec3(0.0f, 0.99f, 27.2f));
	matrixModelMuebles25 = glm::scale(matrixModelMuebles25, glm::vec3(0.012f));

	matrixModelMuebles26 = glm::translate(matrixModelMuebles26, glm::vec3(-8.50f, 0.99f, 27.2f));
	matrixModelMuebles26 = glm::scale(matrixModelMuebles26, glm::vec3(0.012f));

	matrixModelMuebles27 = glm::translate(matrixModelMuebles27, glm::vec3(-25.0f, 0.99f, 27.2f));
	matrixModelMuebles27 = glm::scale(matrixModelMuebles27, glm::vec3(0.012f));

	matrixModelMuebles28 = glm::translate(matrixModelMuebles28, glm::vec3(-33.50f, 0.99f, 27.2f));
	matrixModelMuebles28 = glm::scale(matrixModelMuebles28, glm::vec3(0.012f));

	matrixModelMuebles29 = glm::translate(matrixModelMuebles29, glm::vec3(-42.0f, 0.99f, 27.2f));
	matrixModelMuebles29 = glm::scale(matrixModelMuebles29, glm::vec3(0.012f));

	matrixModelMuebles30 = glm::translate(matrixModelMuebles30, glm::vec3(-50.5f, 0.99f, 27.2f));
	matrixModelMuebles30 = glm::scale(matrixModelMuebles30, glm::vec3(0.012f));

	matrixModelMuebles31 = glm::translate(matrixModelMuebles31, glm::vec3(-59.0f, 0.99f, 27.2f));
	matrixModelMuebles31 = glm::scale(matrixModelMuebles31, glm::vec3(0.012f));

	matrixModelMuebles32 = glm::translate(matrixModelMuebles32, glm::vec3(-67.50f, 0.99f, 27.2f));
	matrixModelMuebles32 = glm::scale(matrixModelMuebles32, glm::vec3(0.012f));

//Cover line for x=-17 & x=8
//x=8
	/*
	matrixModelMueblesExt = glm::translate(matrixModelMueblesExt, glm::vec3(0.0f, 0.99f, 27.2f));
	matrixModelMueblesExt = glm::scale(matrixModelMueblesExt, glm::vec3(0.012f));
	*/
	matrixModelMueblesExt1 = glm::translate(matrixModelMueblesExt1, glm::vec3(8.0f, 0.99f, 0.0f));
	matrixModelMueblesExt1 = glm::scale(matrixModelMueblesExt1, glm::vec3(0.012f));
	matrixModelMueblesExt2 = glm::translate(matrixModelMueblesExt2, glm::vec3(8.0, 0.99f, 7.2f));
	matrixModelMueblesExt2 = glm::scale(matrixModelMueblesExt2, glm::vec3(0.012f));
	matrixModelMueblesExt3 = glm::translate(matrixModelMueblesExt3, glm::vec3(8.0f, 0.99f, 17.2f));
	matrixModelMueblesExt3 = glm::scale(matrixModelMueblesExt3, glm::vec3(0.012f));
	matrixModelMueblesExt4 = glm::translate(matrixModelMueblesExt4, glm::vec3(8.0f, 0.99f, 27.2f));
	matrixModelMueblesExt4 = glm::scale(matrixModelMueblesExt4, glm::vec3(0.012f));

//x=-17
	matrixModelMueblesExt5 = glm::translate(matrixModelMueblesExt5, glm::vec3(-17.0f, 0.99f, 0.0f));
	matrixModelMueblesExt5 = glm::scale(matrixModelMueblesExt5, glm::vec3(0.012f));
	matrixModelMueblesExt6 = glm::translate(matrixModelMueblesExt6, glm::vec3(-17.0, 0.99f, 7.2f));
	matrixModelMueblesExt6 = glm::scale(matrixModelMueblesExt6, glm::vec3(0.012f));
	matrixModelMueblesExt7 = glm::translate(matrixModelMueblesExt7, glm::vec3(-17.0f, 0.99f, 17.2f));
	matrixModelMueblesExt7 = glm::scale(matrixModelMueblesExt7, glm::vec3(0.012f));
	matrixModelMueblesExt8 = glm::translate(matrixModelMueblesExt8, glm::vec3(-17.0f, 0.99f, 27.2f));
	matrixModelMueblesExt8 = glm::scale(matrixModelMueblesExt8, glm::vec3(0.012f));


/***********************FIN DE SILLAS CORTAS ******************************/

	//Televisiones
	//pared normal
	matrixModelTelevision1 = glm::translate(matrixModelTelevision1, glm::vec3(8.0f, 5.99f, -6.2f));
	matrixModelTelevision1 = glm::rotate(matrixModelTelevision1, glm::radians(-90.0f), glm::vec3(1,0,0));
	//matrixModelTelevision1 = glm::scale(matrixModelTelevision1, glm::vec3(0.5f));

	//pared opuesta
	matrixModelTelevision2 = glm::translate(matrixModelTelevision2, glm::vec3(-8.5f, 5.99f, 39.1f));
	matrixModelTelevision2 = glm::rotate(matrixModelTelevision2, glm::radians(-90.0f), glm::vec3(1,0,0));
	matrixModelTelevision2 = glm::rotate(matrixModelTelevision2, glm::radians(180.0f), glm::vec3(0,0,1));
	//matrixModelTelevision2 = glm::scale(matrixModelTelevision2, glm::vec3(0.5f));

	//pared normal
	matrixModelTelevision3 = glm::translate(matrixModelTelevision3, glm::vec3(-25.0f, 5.99f, -6.2f));
	matrixModelTelevision3 = glm::rotate(matrixModelTelevision3, glm::radians(-90.0f), glm::vec3(1,0,0));
	//matrixModelTelevision3 = glm::scale(matrixModelTelevision3, glm::vec3(0.5f));

//Repeticion de las televisiones:
	//Televisiones
	//pared opuesta
	matrixModelTelevision1Ext = glm::translate(matrixModelTelevision1Ext, glm::vec3(-41.5f, 5.99f, 39.1f));
	matrixModelTelevision1Ext = glm::rotate(matrixModelTelevision1Ext, glm::radians(-90.0f), glm::vec3(1,0,0));
	matrixModelTelevision1Ext = glm::rotate(matrixModelTelevision1Ext, glm::radians(180.0f), glm::vec3(0,0,1));
	//matrixModelTelevision1Ext = glm::scale(matrixModelTelevision1Ext, glm::vec3(0.5f));

	//pared normal
	matrixModelTelevision2Ext = glm::translate(matrixModelTelevision2Ext, glm::vec3(-58.0f, 5.99f, -6.2f));
	matrixModelTelevision2Ext = glm::rotate(matrixModelTelevision2Ext, glm::radians(-90.0f), glm::vec3(1,0,0));
	//matrixModelTelevision2Ext = glm::rotate(matrixModelTelevision2Ext, glm::radians(180.0f), glm::vec3(0,0,1));
	//matrixModelTelevision2 = glm::scale(matrixModelTelevision2, glm::vec3(0.5f));s

	//pared opuesta
	matrixModelTelevision3Ext = glm::translate(matrixModelTelevision3Ext, glm::vec3(-74.5f, 5.99f, 39.1f));
	matrixModelTelevision3Ext = glm::rotate(matrixModelTelevision3Ext, glm::radians(-90.0f), glm::vec3(1,0,0));
	matrixModelTelevision3Ext = glm::rotate(matrixModelTelevision3Ext, glm::radians(180.0f), glm::vec3(0,0,1));
	//matrixModelTelevision3 = glm::scale(matrixModelTelevision3, glm::vec3(0.5f));

	//Paredes y techo
	matrixModelPared = glm::translate(matrixModelPared, glm::vec3(-120.0f, 2.0f, 17.0f));
	matrixModelPared = glm::rotate(matrixModelPared, glm::radians(-180.0f), glm::vec3(1,0,0));
	matrixModelPared = glm::rotate(matrixModelPared, glm::radians(-90.0f), glm::vec3(0,1,0));
	matrixModelPared = glm::scale(matrixModelPared, glm::vec3(25.0f,10.0f,7.0f));
	

	matrixModelTecho = glm::translate(matrixModelTecho, glm::vec3(-30.0f, 10.0f, 15.0f));
	matrixModelTecho = glm::rotate(matrixModelTecho, glm::radians(90.0f), glm::vec3(1,0,0));
	matrixModelTecho = glm::scale(matrixModelTecho, glm::vec3(60.0f,25.0f,45.0f));

	//Personaje principal
	modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(13.0f, 0.05f, -5.0f));
	modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-90.0f), glm::vec3(0, 1, 0));



	lastTime = TimeManager::Instance().GetTime();

	textureActivaID = textureInit1ID;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, -10.0);

	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1f, 45.0f);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) screenWidth / (float) screenHeight, 0.01f, 100.0f);

		
		axis = glm::axis(glm::quat_cast(modelMatrixMayow));
		angleTarget = glm::angle(glm::quat_cast(modelMatrixMayow));
		target = modelMatrixMayow[3];
		
		if(std::isnan(angleTarget))
			angleTarget = 0.0;
		if(axis.y < 0)
			angleTarget = -angleTarget;
		if(modelSelected == 1)
			angleTarget -= glm::radians(90.0f);
		camera->setCameraTarget(target);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();
		glm::mat4 view = camera->getViewMatrix();

		shadowBox->update(screenWidth, screenHeight);
		glm::vec3 centerBox = shadowBox->getCenter();

		// Projection light shadow mapping
		glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		lightProjection[0][0] = 2.0f / shadowBox->getWidth();
		lightProjection[1][1] = 2.0f / shadowBox->getHeight();
		lightProjection[2][2] = -2.0f / shadowBox->getLength();
		lightProjection[3][3] = 1.0f;
		lightView = glm::lookAt(centerBox, centerBox + glm::normalize(-lightPos), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));


		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		/*
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));
*/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.002, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));
		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderTerrain.setInt("spotLightCount", 1);
		glm::vec3 spotPosition = glm::vec3(modelMatrixHeli * glm::vec4(0.0, 0.2, 1.75, 1.0));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.002, 0.003, 0.002)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.003, 0.003, 0.003)));
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.07);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.03);
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.002, 0.003, 0.002)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.003, 0.003, 0.003)));
		shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		shaderTerrain.setFloat("spotLights[0].constant", 1.0);
		shaderTerrain.setFloat("spotLights[0].linear", 0.07);
		shaderTerrain.setFloat("spotLights[0].quadratic", 0.03);
		shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", lamp1Position.size() + lamp2Position.size());
		shaderTerrain.setInt("pointLightCount", lamp1Position.size() + lamp2Position.size());
		for(int i = 0; i < lamp1Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp1Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp1Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(0.5));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.0, 5.99, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.6)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.6)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}
		
		
		for(int i = 0; i < lamp2Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp2Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.75, 5.0, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.002, 0.0016, 0.0001)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.004, 0.0032, 0.0002)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.006, 0.0058, 0.0003)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.02);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.002, 0.0016, 0.0001)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.004, 0.0032, 0.0002)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.006, 0.0058, 0.0003)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.02);
		}

		/************Render de imagen de frente**************/
		if(!iniciaPartida){
			shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
			shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureActivaID);
			shaderTexture.setInt("outTexture", 0);
			boxIntro.render();
			glfwSwapBuffers(window);
			continue;
		}

		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene();
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*******************************************
		 * Debug to view the texture view map
		 *******************************************/
		// reset viewport
		/*glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render Depth map to quad for visual debugging
		shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setFloat("near_plane", near_plane);
		shaderViewDepth.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
		boxViewDepth.render();*/

		/*******************************************
		 * 2.- We render the normal objects
		 *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		renderSolidScene();

		/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/

		// Collider de mayow

		AbstractModel::OBB mayowCollider;
		glm::mat4 modelmatrixColliderMayow = glm::mat4(modelMatrixMayow);
		modelmatrixColliderMayow = glm::rotate(modelmatrixColliderMayow,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		mayowCollider.u = glm::quat_cast(modelmatrixColliderMayow);
		//MCSCALE HERE WAS USED, BUT ITS JUST THE SAME VALUES
		modelmatrixColliderMayow = glm::scale(modelmatrixColliderMayow, glm::vec3(0.1f));
		modelmatrixColliderMayow = glm::translate(modelmatrixColliderMayow,
				glm::vec3(mayowModelAnimate.getObb().c.x,
						mayowModelAnimate.getObb().c.y,
						mayowModelAnimate.getObb().c.z));
						//QUI TAMBIEN SE USO MCSCALE
		mayowCollider.e = mayowModelAnimate.getObb().e * glm::vec3(0.1f) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
		mayowCollider.c = glm::vec3(modelmatrixColliderMayow[3]);
		addOrUpdateColliders(collidersOBB, "mayow", mayowCollider, modelMatrixMayow);


		// Collider de tiendas (parte menor)
		glm::mat4 modelMatrixColliderTiendas1= glm::mat4(matrixModelTiendas1);
		AbstractModel::OBB Tiendas1Collider;
		// Set the orientation of collider before doing the scale
		Tiendas1Collider.u = glm::quat_cast(matrixModelTiendas1);
		modelMatrixColliderTiendas1 = glm::scale(modelMatrixColliderTiendas1, glm::vec3(1.01));
		modelMatrixColliderTiendas1 = glm::translate(modelMatrixColliderTiendas1, 
						glm::vec3(modelTiendas1.getObb().c.x,
						modelTiendas1.getObb().c.y,
						modelTiendas1.getObb().c.z));
		Tiendas1Collider.c = glm::vec3(modelMatrixColliderTiendas1[3]);
		Tiendas1Collider.e = modelTiendas1.getObb().e * glm::vec3(1.0);
		addOrUpdateColliders(collidersOBB, "tiendas1", Tiendas1Collider, matrixModelTiendas1);
		
		// Collider de tiendas (parte mayor)
		glm::mat4 modelMatrixColliderTiendas2= glm::mat4(matrixModelTiendas2);
		AbstractModel::OBB Tiendas2Collider;
		// Set the orientation of collider before doing the scale
		Tiendas2Collider.u = glm::quat_cast(matrixModelTiendas2);
		modelMatrixColliderTiendas2 = glm::scale(modelMatrixColliderTiendas2, glm::vec3(1.0));
		modelMatrixColliderTiendas2 = glm::translate(modelMatrixColliderTiendas2, 
						glm::vec3(modelTiendas2.getObb().c.x,
						modelTiendas2.getObb().c.y,
						modelTiendas2.getObb().c.z));
		Tiendas2Collider.c = glm::vec3(modelMatrixColliderTiendas2[3]);
		Tiendas2Collider.e = modelTiendas2.getObb().e * glm::vec3(0.995);
		addOrUpdateColliders(collidersOBB, "Tiendas2", Tiendas2Collider, matrixModelTiendas2);

		// Collider de tiendas (parte mayor) VERSION OPUESTA
		glm::mat4 modelMatrixColliderTiendas2Ext= glm::mat4(matrixModelTiendas2Ext);
		AbstractModel::OBB Tiendas2ExtCollider;
		// Set the orientation of collider before doing the scale
		Tiendas2ExtCollider.u = glm::quat_cast(matrixModelTiendas2Ext);
		modelMatrixColliderTiendas2Ext = glm::scale(modelMatrixColliderTiendas2Ext, glm::vec3(1.0));
		modelMatrixColliderTiendas2Ext = glm::translate(modelMatrixColliderTiendas2Ext, 
						glm::vec3(modelTiendas2.getObb().c.x,
						modelTiendas2.getObb().c.y,
						modelTiendas2.getObb().c.z));
		Tiendas2ExtCollider.c = glm::vec3(modelMatrixColliderTiendas2Ext[3]);
		Tiendas2ExtCollider.e = modelTiendas2.getObb().e * glm::vec3(0.999);
		addOrUpdateColliders(collidersOBB, "Tiendas2Ext", Tiendas2ExtCollider, matrixModelTiendas2Ext);

		// Collider de tiendas (parte menor) VERSION OPUESTA
		glm::mat4 modelMatrixColliderTiendas1Ext= glm::mat4(matrixModelTiendas1Ext);
		AbstractModel::OBB Tiendas1ExtCollider;
		// Set the orientation of collider before doing the scale
		Tiendas1ExtCollider.u = glm::quat_cast(matrixModelTiendas1Ext);
		modelMatrixColliderTiendas1Ext = glm::scale(modelMatrixColliderTiendas1Ext, glm::vec3(1.01));
		modelMatrixColliderTiendas1Ext = glm::translate(modelMatrixColliderTiendas1Ext, 
						glm::vec3(modelTiendas1.getObb().c.x,
						modelTiendas1.getObb().c.y,
						modelTiendas1.getObb().c.z));
		Tiendas1ExtCollider.c = glm::vec3(modelMatrixColliderTiendas1Ext[3]);
		Tiendas1ExtCollider.e = modelTiendas1.getObb().e * glm::vec3(1.0);
		addOrUpdateColliders(collidersOBB, "Tiendas1Ext", Tiendas1ExtCollider, matrixModelTiendas1Ext);

		// Collider de Muebles1
		glm::mat4 modelMatrixColliderMuebles= glm::mat4(matrixModelMuebles);
		AbstractModel::OBB MueblesCollider;
		// Set the orientation of collider before doing the scale
		MueblesCollider.u = glm::quat_cast(matrixModelMuebles);
		modelMatrixColliderMuebles = glm::scale(modelMatrixColliderMuebles, glm::vec3(0.1f));
		modelMatrixColliderMuebles = glm::translate(modelMatrixColliderMuebles, 
						glm::vec3(modelMuebles.getObb().c.x,
						modelMuebles.getObb().c.y,
						modelMuebles.getObb().c.z));
		MueblesCollider.c = glm::vec3(modelMatrixColliderMuebles[3]);
		MueblesCollider.e = modelMuebles.getObb().e * glm::vec3(0.01f);
		addOrUpdateColliders(collidersOBB, "Muebles", MueblesCollider, matrixModelMuebles);

		// Collider de Puerta de salida
		glm::mat4 modelMatrixColliderTiendasSalida= glm::mat4(matrixModelTiendasSalida);
		AbstractModel::OBB TiendasSalidaCollider;
		// Set the orientation of collider before doing the scale
		TiendasSalidaCollider.u = glm::quat_cast(matrixModelTiendasSalida);
		modelMatrixColliderTiendasSalida = glm::scale(modelMatrixColliderTiendasSalida, glm::vec3(0.1029f,0.1029f,0.5029f));
		//modelMatrixColliderTiendasSalida = glm::rotate(modelMatrixColliderTiendasSalida, glm::radians(90.0f), glm::vec3(0,0,1));
		modelMatrixColliderTiendasSalida = glm::translate(modelMatrixColliderTiendasSalida, 
						glm::vec3(modelTiendasSalida.getObb().c.x,
						modelTiendasSalida.getObb().c.y,
						modelTiendasSalida.getObb().c.z));
		TiendasSalidaCollider.c = glm::vec3(modelMatrixColliderTiendasSalida[3]);
		TiendasSalidaCollider.e = modelTiendasSalida.getObb().e * glm::vec3(0.00219f);
		addOrUpdateColliders(collidersOBB, "TiendasSalida", TiendasSalidaCollider, matrixModelTiendasSalida);


		// Botes de basura (par)
		//POr alguna razon el collider es mas largo de lo que deberia ser, asi que lo estoy haciendo
		//mas chico en la linea de BasuraCollider.e
		glm::mat4 modelMatrixColliderBasura= glm::mat4(matrixModelBasura);
		AbstractModel::OBB BasuraCollider;
		// Set the orientation of collider before doing the scale
		BasuraCollider.u = glm::quat_cast(matrixModelBasura);
		modelMatrixColliderBasura = glm::scale(modelMatrixColliderBasura, glm::vec3(0.001f,1.0f,1.0f));
		modelMatrixColliderBasura = glm::translate(modelMatrixColliderBasura, 
						glm::vec3(modelBasura.getObb().c.x,
						modelBasura.getObb().c.y,
						modelBasura.getObb().c.z));
		BasuraCollider.c = glm::vec3(modelMatrixColliderBasura[3]);
		BasuraCollider.e = modelBasura.getObb().e * glm::vec3(0.011f);
		addOrUpdateColliders(collidersOBB, "Basura", BasuraCollider, matrixModelBasura);

		/*******************************************
		 * Render de colliders
		 *******************************************/
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			sphereCollider.enableWireMode();
			sphereCollider.render(matrixCollider);
		}

		/**********Render de transparencias***************/
		renderAlphaScene();

		/*********************Prueba de colisiones****************************/
		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt && testSphereSphereIntersection(
					std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay collision entre " << it->first <<
						" y el modelo " << jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isColision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt && 
					testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision entre " << it->first << " y el modelo" <<
						jt->first << std::endl;
					isColision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isColision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision del " << it->first << " y el modelo" <<
						jt->first << std::endl;
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, true);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator itCollision;
		for (itCollision = collisionDetection.begin(); 
			itCollision != collisionDetection.end(); itCollision++) {
			std::map<std::string, std::tuple<AbstractModel::SBB, 
				glm::mat4, glm::mat4>>::iterator sbbBuscado = 
				collidersSBB.find(itCollision->first);
			std::map<std::string, std::tuple<AbstractModel::OBB,
				glm::mat4, glm::mat4>>::iterator obbBuscado =
				collidersOBB.find(itCollision->first);
			if (sbbBuscado != collidersSBB.end()) {
				if (!itCollision->second) 
					addOrUpdateColliders(collidersSBB, itCollision->first);
			}
			if (obbBuscado != collidersOBB.end()) {
				if (!itCollision->second) 
					addOrUpdateColliders(collidersOBB, itCollision->first);
				else {
					if (itCollision->first.compare("mayow") == 0)
						modelMatrixMayow = std::get<1>(obbBuscado->second);
					if (itCollision->first.compare("dart") == 0)
						modelMatrixDart = std::get<1>(obbBuscado->second);
				}
			}
		}

		glm::mat4 modelMatrixRayMay = glm::mat4(modelMatrixMayow);
		modelMatrixRayMay = glm::translate(modelMatrixRayMay, glm::vec3(0, 1, 0));
		float maxDistanceRay = 10.0;
		glm::vec3 rayDirection = modelMatrixRayMay[2];
		glm::vec3 ori = modelMatrixRayMay[3];
		glm::vec3 rmd = ori + rayDirection * (maxDistanceRay / 2.0f);
		glm::vec3 targetRay = ori + rayDirection * maxDistanceRay;
		modelMatrixRayMay[3] = glm::vec4(rmd, 1.0);
		modelMatrixRayMay = glm::rotate(modelMatrixRayMay, glm::radians(90.0f), 
			glm::vec3(1, 0, 0));
		modelMatrixRayMay = glm::scale(modelMatrixRayMay, 
			glm::vec3(0.05, maxDistanceRay, 0.05));
		rayModel.render(modelMatrixRayMay);

		std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::
			iterator itSBB;
		for (itSBB = collidersSBB.begin(); itSBB != collidersSBB.end(); itSBB++) {
			float tRint;
			if (raySphereIntersect(ori, targetRay, rayDirection,
				std::get<0>(itSBB->second), tRint)) {
				std::cout << "Collision del rayo con el modelo " << itSBB->first 
				<< std::endl;
			}
		}
		
		
		/**********Maquinas de estado*************/
		

		// Constantes de animaciones
		rotHelHelY += 0.5;
		rotHelHelBack += 0.5;
		animationMayowIndex = 3;

		glfwSwapBuffers(window);

		/****************************+
		 * Open AL sound data
		 */
		source0Pos[0] = matrixModelTelevision1[3].x;
		source0Pos[1] = matrixModelTelevision1[3].y;
		source0Pos[2] = matrixModelTelevision1[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		source1Pos[0] = matrixModelTelevision2[3].x;
		source1Pos[1] = matrixModelTelevision2[3].y;
		source1Pos[2] = matrixModelTelevision2[3].z;
		alSourcefv(source[1], AL_POSITION, source1Pos);
		
		source2Pos[0] = matrixModelTelevision3[3].x;
		source2Pos[1] = matrixModelTelevision3[3].y;
		source2Pos[2] = matrixModelTelevision3[3].z;
		alSourcefv(source[2], AL_POSITION, source2Pos);
//
		source3Pos[0] = matrixModelTelevision1Ext[3].x;
		source3Pos[1] = matrixModelTelevision1Ext[3].y;
		source3Pos[2] = matrixModelTelevision1Ext[3].z;
		alSourcefv(source[3], AL_POSITION, source3Pos);

		source4Pos[0] = matrixModelTelevision2Ext[3].x;
		source4Pos[1] = matrixModelTelevision2Ext[3].y;
		source4Pos[2] = matrixModelTelevision2Ext[3].z;
		alSourcefv(source[4], AL_POSITION, source4Pos);

		source5Pos[0] = matrixModelTelevision3Ext[3].x;
		source5Pos[1] = matrixModelTelevision3Ext[3].y;
		source5Pos[2] = matrixModelTelevision3Ext[3].z;
		alSourcefv(source[5], AL_POSITION, source5Pos);

		// Listener for the Thris person camera
		listenerPos[0] = modelMatrixMayow[3].x;
		listenerPos[1] = modelMatrixMayow[3].y;
		listenerPos[2] = modelMatrixMayow[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixMayow[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixMayow[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;

		// Listener for the First person camera
		// listenerPos[0] = camera->getPosition().x;
		// listenerPos[1] = camera->getPosition().y;
		// listenerPos[2] = camera->getPosition().z;
		// alListenerfv(AL_POSITION, listenerPos);
		// listenerOri[0] = camera->getFront().x;
		// listenerOri[1] = camera->getFront().y;
		// listenerOri[2] = camera->getFront().z;
		// listenerOri[3] = camera->getUp().x;
		// listenerOri[4] = camera->getUp().y;
		// listenerOri[5] = camera->getUp().z;
		alListenerfv(AL_ORIENTATION, listenerOri);

		for(unsigned int i = 0; i < sourcesPlay.size(); i++){
			if(sourcesPlay[i]){
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);
			}
		}
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}