/*
* Práctica 1
* Computación Gráfica Avanzada
* Integrantes:
* 	- Azul Alfaro Fernández
*	- Aranza Abril Núñez Luna
*/

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

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

#include "Headers/AnimationUtils.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere skyboxSphere(20, 20);
Box boxCesped;
Box boxWalls;
Box boxHighway;
Box boxLandingPad;
Sphere esfera1(10, 10);
// Models complex instances
Model modelRock;
Model modelAircraft;
Model modelEclipseChasis;
Model modelEclipseRearWheels;
Model modelEclipseFrontalWheels;
Model modelHeliChasis;
Model modelHeliHeli;
Model modelHeliRear;
Model modelLambo;
Model modelLamboLeftDor;
Model modelLamboRightDor;
Model modelLamboFrontLeftWheel;
Model modelLamboFrontRightWheel;
Model modelLamboRearLeftWheel;
Model modelLamboRearRightWheel;
// Dart lego
Model modelDartLegoBody;
Model modelDartLegoHead;
Model modelDartLegoMask;
Model modelDartLegoLeftArm;
Model modelDartLegoRightArm;
Model modelDartLegoLeftHand;
Model modelDartLegoRightHand;
Model modelDartLegoLeftLeg;
Model modelDartLegoRightLeg;

//Buzz right
Model modelBuzzRightWing2;
Model modelBuzzRightWing1;
Model modelBuzzRightTigh;
Model modelBuzzRightHand;
Model modelBuzzRightForearm;
Model modelBuzzRightFoot;
Model modelBuzzRightCalf;
Model modelBuzzRightArm;
//Buzz left
Model modelBuzzLeftWing2;
Model modelBuzzLeftWing1;
Model modelBuzzLeftTigh;
Model modelBuzzLeftHand;
Model modelBuzzLeftForearm;
Model modelBuzzLeftFoot;
Model modelBuzzLeftCalf;
Model modelBuzzLeftArm;
//Buzz middle
Model modelBuzzTorso;
Model modelBuzzHip;
Model modelBuzzHead;

//Modelos descargados
Model modelCat;
Model modelWood;

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;
bool LamboFlag = false;

// Model matrix definitions
glm::mat4 modelMatrixEclipse = glm::mat4(1.0f);
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 modelMatrixHeli = glm::mat4(1.0f);
glm::mat4 modelMatrixLambo = glm::mat4(1.0);
glm::mat4 modelMatrixAircraft = glm::mat4(1.0);
glm::mat4 modelMatrixDart = glm::mat4(1.0f);
glm::mat4 modelMatrixBuzz = glm::mat4(1.0f);
glm::mat4 modelMatrixCat = glm::mat4(1.0);
glm::mat4 modelMatrixWood = glm::mat4(1.0);

float rotDartHead = 0.0, rotDartLeftArm = 0.0, rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;
int modelSelected = 0;
bool enableCountSelected = true;

//Rotar Buzz (faltan variables de rotar)
float rotBuzzHead = 0.0, rotBuzzLeftArm =0.0, rotBuzzLeftForearm = 0.0, rotBuzzLeftHand = 0.0, rotBuzzRightArm =0.0, rotBuzzRightForearm = 0.0, rotBuzzRightHand = 0.0,
 rotBuzzLeftThigh=0.0, rotBuzzLeftCalf = 0.0, rotBuzzLeftFoot = 0.0, rotBuzzRightThigh=0.0, rotBuzzRightCalf = 0.0, rotBuzzRightFoot = 0.0;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

// Joints interpolations Dart Lego
//El vector de flotantes es un movimiento parametrizado
std::vector<std::vector<float>> keyFramesDartJoints; // Arreglo de todas las articulaciones que puede haber
//El segundo vector es una estructura de vectores. Usa una estructura de 4 donde se tienen 2 variables: de las articulaciones y de la posicion
//Se mueve la cabeza con tab + objeto seleccionado 1 + 1 o shift 1
std::vector<std::vector<glm::mat4>> keyFramesDart; // Arreglo de fotogramas
int indexFrameDartJoints = 0;
int indexFrameDartJointsNext = 1;
float interpolationDartJoints = 0.0;
int maxNumPasosDartJoints = 20;
int numPasosDartJoints = 0;
int indexFrameDart = 0;
int indexFrameDartNext = 1;
float interpolationDart = 0.0;
int maxNumPasosDart = 200;
int numPasosDart = 0;
//Se puede seguir movimiento hasta el 7

//Buzz INTERPOLATION
std::vector<std::vector<float>> keyFramesBuzzJoints;
std::vector<std::vector<glm::mat4>> keyFramesBuzz;
int indexFrameBuzzJoints = 0;
int indexFrameBuzzJointsNext = 1;
float interpolationBuzzJoints = 0.0;
int maxNumPasosBuzzJoints = 40; //20 previamente
int numPasosBuzzJoints = 0;
int indexFrameBuzz = 0;
int indexFrameBuzzNext = 1;
float interpolationBuzz = 0.0;
int maxNumPasosBuzz = 200;
int numPasosBuzz = 0;

// Var animate lambo dor
int stateDoor = 0;
float dorRotCount = 0.0;

double deltaTime;
double currTime, lastTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
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
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

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
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_texture_res.vs", "../Shaders/multipleLights.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

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

	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	// Eclipse
	modelEclipseChasis.loadModel("../models/Eclipse/2003eclipse_chasis.obj");
	modelEclipseChasis.setShader(&shaderMulLighting);
	modelEclipseFrontalWheels.loadModel("../models/Eclipse/2003eclipse_frontal_wheels.obj");
	modelEclipseFrontalWheels.setShader(&shaderMulLighting);
	modelEclipseRearWheels.loadModel("../models/Eclipse/2003eclipse_rear_wheels.obj");
	modelEclipseRearWheels.setShader(&shaderMulLighting);
	// Helicopter
	modelHeliChasis.loadModel("../models/Helicopter/Mi_24_chasis.obj");
	modelHeliChasis.setShader(&shaderMulLighting);
	modelHeliHeli.loadModel("../models/Helicopter/Mi_24_heli.obj");
	modelHeliHeli.setShader(&shaderMulLighting);
	modelHeliRear.loadModel("../models/Helicopter/Mi_24_heli_rear.obj");
	modelHeliRear.setShader(&shaderMulLighting);
	// Lamborginhi
	modelLambo.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_chasis.obj");
	modelLambo.setShader(&shaderMulLighting);
	modelLamboLeftDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_left_dor.obj");
	modelLamboLeftDor.setShader(&shaderMulLighting);
	modelLamboRightDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_right_dor.obj");
	modelLamboRightDor.setShader(&shaderMulLighting);
	modelLamboFrontLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_left_wheel.obj");
	modelLamboFrontLeftWheel.setShader(&shaderMulLighting);
	modelLamboFrontRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_right_wheel.obj");
	modelLamboFrontRightWheel.setShader(&shaderMulLighting);
	modelLamboRearLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_left_wheel.obj");
	modelLamboRearLeftWheel.setShader(&shaderMulLighting);
	modelLamboRearRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_right_wheel.obj");
	modelLamboRearRightWheel.setShader(&shaderMulLighting);

	// Dart Lego
	modelDartLegoBody.loadModel("../models/LegoDart/LeoDart_body.obj");
	modelDartLegoBody.setShader(&shaderMulLighting);
	modelDartLegoMask.loadModel("../models/LegoDart/LeoDart_mask.obj");
	modelDartLegoMask.setShader(&shaderMulLighting);
	modelDartLegoHead.loadModel("../models/LegoDart/LeoDart_head.obj");
	modelDartLegoHead.setShader(&shaderMulLighting);
	modelDartLegoLeftArm.loadModel("../models/LegoDart/LeoDart_left_arm.obj");
	modelDartLegoLeftArm.setShader(&shaderMulLighting);
	modelDartLegoRightArm.loadModel("../models/LegoDart/LeoDart_right_arm.obj");
	modelDartLegoRightArm.setShader(&shaderMulLighting);
	modelDartLegoLeftHand.loadModel("../models/LegoDart/LeoDart_left_hand.obj");
	modelDartLegoLeftHand.setShader(&shaderMulLighting);
	modelDartLegoRightHand.loadModel("../models/LegoDart/LeoDart_right_hand.obj");
	modelDartLegoRightHand.setShader(&shaderMulLighting);
	modelDartLegoLeftLeg.loadModel("../models/LegoDart/LeoDart_left_leg.obj");
	modelDartLegoLeftLeg.setShader(&shaderMulLighting);
	modelDartLegoRightLeg.loadModel("../models/LegoDart/LeoDart_right_leg.obj");
	modelDartLegoRightLeg.setShader(&shaderMulLighting);

	//Buzz model
	
	//Buzz right
	modelBuzzRightWing2.loadModel("../models/buzz/buzzlightyRightWing2.obj");
	modelBuzzRightWing2.setShader(&shaderMulLighting);
	modelBuzzRightWing1.loadModel("../models/buzz/buzzlightyRightWing1.obj");
	modelBuzzRightWing1.setShader(&shaderMulLighting);
	modelBuzzRightTigh.loadModel("../models/buzz/buzzlightyRightThigh.obj");
	modelBuzzRightTigh.setShader(&shaderMulLighting);
	modelBuzzRightHand.loadModel("../models/buzz/buzzlightyRightHand.obj");
	modelBuzzRightHand.setShader(&shaderMulLighting);
	modelBuzzRightForearm.loadModel("../models/buzz/buzzlightyRightForearm.obj");
	modelBuzzRightForearm.setShader(&shaderMulLighting);
	modelBuzzRightFoot.loadModel("../models/buzz/buzzlightyRightFoot.obj");
	modelBuzzRightFoot.setShader(&shaderMulLighting);
	modelBuzzRightCalf.loadModel("../models/buzz/buzzlightyRightCalf.obj");
	modelBuzzRightCalf.setShader(&shaderMulLighting);
	modelBuzzRightArm.loadModel("../models/buzz/buzzlightyRightArm.obj");
	modelBuzzRightArm.setShader(&shaderMulLighting);
	//Buzz left
	modelBuzzLeftWing2.loadModel("../models/buzz/buzzlightyLeftWing2.obj");
	modelBuzzLeftWing2.setShader(&shaderMulLighting);
	modelBuzzLeftWing1.loadModel("../models/buzz/buzzlightyLeftWing1.obj");
	modelBuzzLeftWing1.setShader(&shaderMulLighting);
	modelBuzzLeftTigh.loadModel("../models/buzz/buzzlightyLeftThigh.obj");
	modelBuzzLeftTigh.setShader(&shaderMulLighting);
	modelBuzzLeftHand.loadModel("../models/buzz/buzzlightyLeftHand.obj");
	modelBuzzLeftHand.setShader(&shaderMulLighting);
	modelBuzzLeftForearm.loadModel("../models/buzz/buzzlightyLeftForearm.obj");
	modelBuzzLeftForearm.setShader(&shaderMulLighting);
	modelBuzzLeftFoot.loadModel("../models/buzz/buzzlightyLeftFoot.obj");
	modelBuzzLeftFoot.setShader(&shaderMulLighting);
	modelBuzzLeftCalf.loadModel("../models/buzz/buzzlightyLeftCalf.obj");
	modelBuzzLeftCalf.setShader(&shaderMulLighting);
	modelBuzzLeftArm.loadModel("../models/buzz/buzzlightyLeftArm.obj");
	modelBuzzLeftArm.setShader(&shaderMulLighting);
	//Buzz middle
	modelBuzzTorso.loadModel("../models/buzz/buzzlightyTorso.obj");
	modelBuzzTorso.setShader(&shaderMulLighting);
	modelBuzzHip.loadModel("../models/buzz/buzzlightyHip.obj");
	modelBuzzHip.setShader(&shaderMulLighting);
	modelBuzzHead.loadModel("../models/buzz/buzzlightyHead.obj");
	modelBuzzHead.setShader(&shaderMulLighting);

	//Modelos descargados
	modelCat.loadModel("../models/cat/Cat.obj");
	modelCat.setShader(&shaderMulLighting);
	modelWood.loadModel("../models/Wood/Wood.obj");
	modelWood.setShader(&shaderMulLighting);

	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));
	
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
	Texture textureCesped("../Textures/grassy2.png");
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

	// Definiendo la textura a utilizar
	Texture textureWall("../Textures/whiteWall.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureWall.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureWallID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWallID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureWall.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureWall.getChannels() == 3 ? GL_RGB : GL_RGBA, textureWall.getWidth(), textureWall.getHeight(), 0,
		textureWall.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureWall.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWall.freeImage();

	// Definiendo la textura a utilizar
	Texture textureWindow("../Textures/ventana.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureWindow.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureWindowID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWindowID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureWindow.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureWindow.getChannels() == 3 ? GL_RGB : GL_RGBA, textureWindow.getWidth(), textureWindow.getHeight(), 0,
		textureWindow.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureWindow.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWindow.freeImage();

	// Definiendo la textura a utilizar
	Texture textureHighway("../Textures/highway.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureHighway.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureHighwayID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureHighwayID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureHighway.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureHighway.getChannels() == 3 ? GL_RGB : GL_RGBA, textureHighway.getWidth(), textureHighway.getHeight(), 0,
		textureHighway.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureHighway.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureHighway.freeImage();

	// Definiendo la textura
	Texture textureLandingPad("../Textures/landingPad.jpg");
	textureLandingPad.loadImage(); // Cargar la textura
	glGenTextures(1, &textureLandingPadID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureLandingPadID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureLandingPad.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureLandingPad.getChannels() == 3 ? GL_RGB : GL_RGBA, textureLandingPad.getWidth(), textureLandingPad.getHeight(), 0,
		textureLandingPad.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureLandingPad.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureLandingPad.freeImage(); // Liberamos memoria

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

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	boxWalls.destroy();
	boxHighway.destroy();
	boxLandingPad.destroy();
	esfera1.destroy();

	// Custom objects Delete
	modelAircraft.destroy();
	modelDartLegoBody.destroy();
	modelDartLegoHead.destroy();
	modelDartLegoLeftArm.destroy();
	modelDartLegoLeftHand.destroy();
	modelDartLegoLeftLeg.destroy();
	modelDartLegoMask.destroy();
	modelDartLegoRightArm.destroy();
	modelDartLegoRightHand.destroy();
	modelDartLegoRightLeg.destroy();
	modelEclipseChasis.destroy();
	modelEclipseFrontalWheels.destroy();
	modelEclipseRearWheels.destroy();
	modelHeliChasis.destroy();
	modelHeliHeli.destroy();
	modelHeliRear.destroy();
	modelLambo.destroy();
	modelLamboFrontLeftWheel.destroy();
	modelLamboFrontRightWheel.destroy();
	modelLamboLeftDor.destroy();
	modelLamboRearLeftWheel.destroy();
	modelLamboRearRightWheel.destroy();
	modelLamboRightDor.destroy();
	modelRock.destroy();
	//Buzz
	modelBuzzRightWing2.destroy();
	modelBuzzRightWing1.destroy();
	modelBuzzRightTigh.destroy();
	modelBuzzRightHand.destroy();
	modelBuzzRightForearm.destroy();
	modelBuzzRightFoot.destroy();
	modelBuzzRightCalf.destroy();
	modelBuzzRightArm.destroy();
	modelBuzzLeftWing2.destroy();
	modelBuzzLeftWing1.destroy();
	modelBuzzLeftTigh.destroy();
	modelBuzzLeftHand.destroy();
	modelBuzzLeftForearm.destroy();
	modelBuzzLeftFoot.destroy();
	modelBuzzLeftCalf.destroy();
	modelBuzzLeftArm.destroy();
	modelBuzzTorso.destroy();
	modelBuzzHip.destroy();
	modelBuzzHead.destroy();
	// Modelos descargados
	modelCat.destroy();
	modelWood.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);

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

bool processInput(bool continueApplication) { // Procesamiento de los controles de entrada y salida
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 4)
			modelSelected = 0;
		if(modelSelected == 1)
			fileName = "../animaciones/animation_dart_joints.txt";
		if (modelSelected == 2)
			fileName = "../animaciones/animation_dart.txt";
		if(modelSelected == 3)
			fileName = "../animaciones/animation_buzz_joints.txt";
		if (modelSelected == 4)
			fileName = "../animaciones/animation_buzz.txt";
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	// Guardar key frames
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = true;
		if(myfile.is_open())
			myfile.close();
		myfile.open(fileName);
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = false;
		myfile.close();
		if(modelSelected == 1)
			keyFramesDartJoints = getKeyRotFrames(fileName);
		if (modelSelected == 2)
			keyFramesDart = getKeyFrames(fileName);
		if(modelSelected == 3)
			keyFramesBuzzJoints = getKeyRotFrames(fileName);
		if (modelSelected == 4)
			keyFramesBuzz = getKeyFrames(fileName);
	}
	if(availableSave && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
		saveFrame = true;
		availableSave = false;
	}if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		availableSave = true;

	// Dart Lego model movements
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg -= 0.02;
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(-0.02, 0.0, 0.0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(0.02, 0.0, 0.0));

	//Movimiento de buzz
	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixBuzz = glm::rotate(modelMatrixBuzz, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixBuzz = glm::rotate(modelMatrixBuzz, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(0.0, 0.0, 0.02));
	else if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(0.0, 0.0, -0.02));
	//Rotaciones de buzz
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotBuzzHead += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotBuzzHead -= 0.02;
	//
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		rotBuzzLeftArm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		rotBuzzLeftArm -= 0.02;
	//
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		rotBuzzLeftForearm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		rotBuzzLeftForearm -= 0.02;
	//
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		rotBuzzLeftHand += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		rotBuzzLeftHand -= 0.02;
	//
	//RIGHT HAND COMMANDS ADD LEFT SHIFT && CONTROL
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rotBuzzRightArm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rotBuzzRightArm -= 0.02;
	//
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rotBuzzRightForearm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rotBuzzRightForearm -= 0.02;
	//
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rotBuzzRightHand += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rotBuzzRightHand -= 0.02;

	//LOWER BODY PARTS
	//Left
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		rotBuzzLeftThigh += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		rotBuzzLeftThigh -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		rotBuzzLeftCalf += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		rotBuzzLeftCalf -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		rotBuzzLeftFoot += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		rotBuzzLeftFoot -= 0.02;
	//Right
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rotBuzzRightThigh += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rotBuzzRightThigh -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rotBuzzRightCalf += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rotBuzzRightCalf -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rotBuzzRightFoot += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rotBuzzRightFoot -= 0.02;

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		LamboFlag = true;

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(27.5, 0, 30.0));
	modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(180.0f), glm::vec3(0, 1, 0));
	int state = 0;
	float advanceCount = 0.0; /* Cuenta de avance */
	float rotCount = 0.0; /* Cuenta de movimiento de rotación */
	float rotWheelsX = 0.0; /* Giro de la llanta en x */
	float rotWheelsY = 0.0; /* Giro de la llanta en y */
	int numberAdvance = 0; /* Variable del curso */
	int maxAdvance = 0.0; /* Movimiento máximo */
	const float avanceEclipse = 0.3; /* Velocidad de movimiento eclipse */
	const float rotEclipse = 0.3; /* Velocidad de movimiento de rotación*/

	matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));

	modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(5.0, 10.0, -5.0));
	int stateHeli = 0;
	float alturaHeli = 10.0;
	float maxAlturaHeli = 0.0;
	int numberHeli = 0;
	int countHeli;
	float changeRotHeli;
	float rotHelHelY = 100000.0;
	float rotHelHelBack = 100000.0;
	float avHeli = 0.015;

	modelMatrixAircraft = glm::translate(modelMatrixAircraft, glm::vec3(10.0, 2.0, -17.5));

	modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(-22.7, 0, -35.5));
	//Variable del movimiento de lambo
	int stateLambo =0;
	float advanceCountLambo = 0.0;
	float rotCountLambo = 0.0;
	float rotWheelsXLambo = 0.0;
	float rotWheelsYLambo = 0.0;
	int numberAdvanceLambo = 0;
	int maxAdvanceLambo = 0.0;
	const float turn180 = 180;
	const float turn90 = 90;
	float avanceLambo = 0.2;
	float rotLambo = 0.5;

	modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(3.0, 0.0, 20.0));

	modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(1.0f, 0.0, 20.0f));

	modelMatrixCat = glm::translate(modelMatrixCat, glm::vec3(-3.0, 0.0, -17.5));
	modelMatrixCat = glm::rotate(modelMatrixCat, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	modelMatrixCat = glm::scale(modelMatrixCat, glm::vec3(0.02f));

	modelMatrixWood = glm::translate(modelMatrixWood, glm::vec3(0.0, 0.2, -17.5));
	modelMatrixWood = glm::scale(modelMatrixWood, glm::vec3(0.25f));

	// Variables to interpolation key frames
	keyFramesDartJoints = getKeyRotFrames("../animaciones/animation_dart_joints.txt");
	keyFramesDart = getKeyFrames("../animaciones/animation_dart.txt");
	keyFramesBuzzJoints = getKeyRotFrames("../animaciones/animation_buzz_joints.txt");
	keyFramesBuzz= getKeyFrames("./animaciones/animation_buzz.txt");

	lastTime = TimeManager::Instance().GetTime();

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

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;
		std::vector<float> matrixBuzzJoints;
		std::vector<glm::mat4> matrixBuzz;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

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

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		shaderMulLighting.setInt("spotLightCount", 0);

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", 0);

		/*******************************************
		 * Cesped
		 *******************************************/
		glm::mat4 modelCesped = glm::mat4(1.0);
		modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
		modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
		// Se activa la textura del agua
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCespedID);
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(200, 200)));
		boxCesped.render(modelCesped);
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************ww
		 * Casa
		 *******************************************/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureWallID);
		// Frontal wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 2.5, 0.0));
		boxWalls.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxWalls.render();
		// Left wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-15.0, 2.5, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Right walls
		// First wall
		boxWalls.setScale(glm::vec3(3.75, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -1.875));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Second wall
		boxWalls.setScale(glm::vec3(3.75, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -8.125));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Third wall
		boxWalls.setScale(glm::vec3(2.5, 1.25, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 4.375, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Four wall
		boxWalls.setScale(glm::vec3(2.5, 1.25, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 0.625, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Back wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 2.5, -10.0));
		boxWalls.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxWalls.render();
		// Ceilling
		boxWalls.setScale(glm::vec3(12.0, 12.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 5, -5.0));
		boxWalls.setOrientation(glm::vec3(90.0, 0.0, 0.0));
		boxWalls.render();
		// Window
		glBindTexture(GL_TEXTURE_2D, textureWindowID);
		boxWalls.setScale(glm::vec3(2.5, 2.5, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Highway 0
		glBindTexture(GL_TEXTURE_2D, textureHighwayID);
		boxHighway.setScale(glm::vec3(40.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(0.0, 0.05, 10.0));
		boxHighway.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxHighway.render();
		// Highway 1
		boxHighway.setScale(glm::vec3(80.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(25.0, 0.05, 0.0));
		boxHighway.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2, 1)));
		boxHighway.render();
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		// Highway 2
		boxHighway.setScale(glm::vec3(80.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(-25.0, 0.05, 0.0));
		boxHighway.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2, 1)));
		boxHighway.render();
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		// Highway 3
		boxHighway.setScale(glm::vec3(40.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(0.0, 0.05, -35.0));
		boxHighway.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxHighway.render();

		/*******************************************
		 * Esfera 1
		*********************************************/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHighwayID);
		shaderMulLighting.setInt("texture1", 0);
		esfera1.setScale(glm::vec3(3.0, 3.0, 3.0));
		esfera1.setPosition(glm::vec3(3.0f, 2.0f, -10.0f));
		esfera1.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureWallID);
		shaderMulLighting.setInt("texture1", 0);
		esfera1.setScale(glm::vec3(10.0, 10.0, 10.0));
		esfera1.setPosition(glm::vec3(3.0f, 2.0f, 10.0f));
		esfera1.enableWireMode();
		esfera1.render();
		esfera1.enableFillMode();

		/******************************************
		 * Landing pad
		*******************************************/
		boxLandingPad.setScale(glm::vec3(10.0f, 0.05f, 10.0f));
		boxLandingPad.setPosition(glm::vec3(5.0f, 0.05f, -5.0f));
		boxLandingPad.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureLandingPadID);
		shaderMulLighting.setInt("texture1", 0);
		//shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 2.0)));
		boxLandingPad.render();
		//shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************
		 * Custom objects obj
		 *******************************************/
		//Rock render
		modelRock.render(matrixModelRock);
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		// Render for the aircraft model
		modelAircraft.render(modelMatrixAircraft);

		// Render for the eclipse car
		glm::mat4 modelMatrixEclipseChasis = glm::mat4(modelMatrixEclipse);
		modelMatrixEclipseChasis = glm::scale(modelMatrixEclipse, glm::vec3(0.5, 0.5, 0.5));
		modelEclipseChasis.render(modelMatrixEclipseChasis);

		glm::mat4 modelMatrixFrontalWheels = glm::mat4(modelMatrixEclipseChasis); /* (891)(892)(893)(894)*/
		/* Matriz con mayor jerarquía // La última que se aplica */
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, 1.05813, 4.11483 )); 
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsY, glm::vec3(0, 1, 0));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, -1.05813, -4.11483));
		/* Matriz con menor jerarquía // La primera que se aplica */
		modelEclipseFrontalWheels.render(modelMatrixFrontalWheels);

		glm::mat4 modelMatrixRearWheels = glm::mat4(modelMatrixEclipseChasis);
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, 1.05813, -4.35157 ));
		modelMatrixRearWheels = glm::rotate(modelMatrixRearWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, -1.05813, 4.35157));
		modelEclipseRearWheels.render(modelMatrixRearWheels);

		// Helicopter
		glm::mat4 modelMatrixHeliChasis = glm::mat4(modelMatrixHeli);
		modelHeliChasis.render(modelMatrixHeliChasis);

		glm::mat4 modelMatrixHeliHeli = glm::mat4(modelMatrixHeliChasis);
		modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, -0.249548));
		modelMatrixHeliHeli = glm::rotate(modelMatrixHeliHeli, rotHelHelY, glm::vec3(0, 1, 0));
		modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, 0.249548));
		modelHeliHeli.render(modelMatrixHeliHeli);

		//Cuando gire se coloca el pivote
		//Se utiliza en este caso la misma variable para que tengan la mism velocidad de giro
		glm::mat4 modelMatrixHeliRear = glm::mat4(modelMatrixHeliChasis);
		//Se coloca la helic trasera en la posicion que le corresponde
		modelMatrixHeliRear =glm::translate(modelMatrixHeliRear, glm::vec3(0.4085,2.093,-5.649));
		modelMatrixHeliRear =glm::rotate(modelMatrixHeliRear, rotHelHelY,glm::vec3(1,0,0));
		//Se coloca la rotación del pivote con sus respectivas coordenadas.
		modelMatrixHeliRear =glm::translate(modelMatrixHeliRear, glm::vec3(-0.4085,-2.093,5.649));
		modelHeliRear.render(modelMatrixHeliRear);

		// Lambo car
		glDisable(GL_CULL_FACE);
		glm::mat4 modelMatrixLamboChasis = glm::mat4(modelMatrixLambo);
		modelMatrixLamboChasis = glm::scale(modelMatrixLamboChasis, glm::vec3(1.3, 1.3, 1.3));
		//Se le realizaba una rotacion para voltear el carro en la primera implementacion
		//modelMatrixLamboChasis = glm::rotate(modelMatrixLamboChasis, glm::radians(turn180), glm::vec3(0,1,0));
		//modelMatrixLamboChasis = glm::rotate(modelMatrixLamboChasis,glm::radians(180), glm::vec3(1.0, 0, 0.0));
		modelLambo.render(modelMatrixLamboChasis);
		glActiveTexture(GL_TEXTURE0);
		//Matrices para las 4 llantas pegadas al chasis
		glm::mat4 modelMatrixLamboFrontLeftWheel = glm::mat4(modelMatrixLamboChasis);
		glm::mat4 modelMatrixLamboFrontRightWheel = glm::mat4(modelMatrixLamboChasis);
		glm::mat4 modelMatrixLamboRearLeftWheel = glm::mat4(modelMatrixLamboChasis);
		glm::mat4 modelMatrixLamboRearRightWheel = glm::mat4(modelMatrixLamboChasis);
		//Transformaciones para cada pivote  
		//Rueda izquierda delantera
		modelMatrixLamboFrontLeftWheel = glm::translate(modelMatrixLamboFrontLeftWheel, glm::vec3(0.93, 0.3791, 1.399));
		modelMatrixLamboFrontLeftWheel = glm::rotate(modelMatrixLamboFrontLeftWheel, rotWheelsYLambo, glm::vec3(0, 1, 0));
		modelMatrixLamboFrontLeftWheel = glm::rotate(modelMatrixLamboFrontLeftWheel, rotWheelsXLambo, glm::vec3(1, 0, 0));
		modelMatrixLamboFrontLeftWheel = glm::translate(modelMatrixLamboFrontLeftWheel, glm::vec3(-0.93, -0.3791, -1.399));
		modelLamboFrontLeftWheel.render(modelMatrixLamboFrontLeftWheel);
		//Rueda derecha delantera
		modelMatrixLamboFrontRightWheel = glm::translate(modelMatrixLamboFrontRightWheel, glm::vec3(-0.93, 0.3791, 1.399));
		modelMatrixLamboFrontRightWheel = glm::rotate(modelMatrixLamboFrontRightWheel, rotWheelsYLambo, glm::vec3(0, 1, 0));
		modelMatrixLamboFrontRightWheel = glm::rotate(modelMatrixLamboFrontRightWheel, rotWheelsXLambo, glm::vec3(1, 0, 0));
		modelMatrixLamboFrontRightWheel = glm::translate(modelMatrixLamboFrontRightWheel, glm::vec3(0.93, -0.3791, -1.399));
		modelLamboFrontRightWheel.render(modelMatrixLamboFrontRightWheel);
		//Rueda izquierda trasera
		modelMatrixLamboRearLeftWheel = glm::translate(modelMatrixLamboRearLeftWheel, glm::vec3(0.93, 0.3991,-1.6));
		modelMatrixLamboRearLeftWheel = glm::rotate(modelMatrixLamboRearLeftWheel, rotWheelsYLambo, glm::vec3(0, 1, 0));
		modelMatrixLamboRearLeftWheel = glm::rotate(modelMatrixLamboRearLeftWheel, rotWheelsXLambo, glm::vec3(1, 0, 0));
		modelMatrixLamboRearLeftWheel = glm::translate(modelMatrixLamboRearLeftWheel, glm::vec3(-0.93, -0.3991,1.6));
		modelLamboRearLeftWheel.render(modelMatrixLamboRearLeftWheel);
		//Rueda derecha trasera
		modelMatrixLamboRearRightWheel = glm::translate(modelMatrixLamboRearRightWheel, glm::vec3(-0.93, 0.3991,-1.6));
		modelMatrixLamboRearRightWheel = glm::rotate(modelMatrixLamboRearRightWheel, rotWheelsYLambo, glm::vec3(0, 1, 0));
		modelMatrixLamboRearRightWheel = glm::rotate(modelMatrixLamboRearRightWheel, rotWheelsXLambo, glm::vec3(1, 0, 0));
		modelMatrixLamboRearRightWheel = glm::translate(modelMatrixLamboRearRightWheel, glm::vec3(0.93, -0.3991,1.6));
		modelLamboRearRightWheel.render(modelMatrixLamboRearRightWheel);
		//Transformaciones de las puertas
		glm::mat4 modelMatrixLamboLeftDor = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(1.08676, 0.707316, 0.982601));
		modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor, glm::radians(dorRotCount), glm::vec3(1.0, 0, 0));
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(-1.08676, -0.707316, -0.982601));
		modelLamboLeftDor.render(modelMatrixLamboLeftDor);
		modelLamboRightDor.render(modelMatrixLamboChasis);
		
		// Se regresa el cull faces IMPORTANTE para las puertas
		glEnable(GL_CULL_FACE);

		// Dart lego
		// Se deshabilita el cull faces IMPORTANTE para la capa
		glDisable(GL_CULL_FACE);
		glm::mat4 modelMatrixDartBody = glm::mat4(modelMatrixDart);
		modelMatrixDartBody = glm::scale(modelMatrixDartBody, glm::vec3(0.5, 0.5, 0.5));
		modelDartLegoBody.render(modelMatrixDartBody);
		glm::mat4 modelMatrixDartHead = glm::mat4(modelMatrixDartBody);
		modelMatrixDartHead = glm::rotate(modelMatrixDartHead, rotDartHead, glm::vec3(0, 1, 0));
		modelDartLegoHead.render(modelMatrixDartHead);
		modelDartLegoMask.render(modelMatrixDartHead);
		glm::mat4 modelMatrixDartLeftArm = glm::mat4(modelMatrixDartBody);
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(-0.023515, 2.43607, 0.446066));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, rotDartLeftArm, glm::vec3(0, 0, 1));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(0.023515, -2.43607, -0.446066));
		modelDartLegoLeftArm.render(modelMatrixDartLeftArm);
		glm::mat4 modelMatrixDartLeftHand = glm::mat4(modelMatrixDartLeftArm);
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(0.201343, 1.68317, 0.99774));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, rotDartLeftHand, glm::vec3(0, 1, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(-0.201343, -1.68317, -0.99774));
		modelDartLegoLeftHand.render(modelMatrixDartLeftHand);
		glm::mat4 modelMatrixDartRightArm = glm::mat4(modelMatrixDartBody);
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(-0.023515, 2.43607, -0.446066));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, rotDartRightArm, glm::vec3(0, 0, 1));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(0.023515, -2.43607, 0.446066));
		modelDartLegoRightArm.render(modelMatrixDartRightArm);
		glm::mat4 modelMatrixDartRightHand = glm::mat4(modelMatrixDartRightArm);
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(0.201343, 1.68317, -0.99774));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, rotDartRightHand, glm::vec3(0, 1, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(-0.201343, -1.68317, 0.99774));
		modelDartLegoRightHand.render(modelMatrixDartRightHand);
		glm::mat4 modelMatrixDartLeftLeg = glm::mat4(modelMatrixDartBody);
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, 1.12632, 0.423349));
		modelMatrixDartLeftLeg = glm::rotate(modelMatrixDartLeftLeg, rotDartLeftLeg, glm::vec3(0, 0, 1));
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, -1.12632, -0.423349));
		modelDartLegoLeftLeg.render(modelMatrixDartLeftLeg);
		glm::mat4 modelMatrixDartRightLeg = glm::mat4(modelMatrixDartBody);
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, 1.12632, -0.423349));
		modelMatrixDartRightLeg = glm::rotate(modelMatrixDartRightLeg, rotDartRightLeg, glm::vec3(0, 0, 1));
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, -1.12632, 0.423349));
		modelDartLegoRightLeg.render(modelMatrixDartRightLeg);
		// Se regresa el cull faces IMPORTANTE para la capa
		glEnable(GL_CULL_FACE);

		//Buzz
		//
		glDisable(GL_CULL_FACE);
		//Torso
		glm::mat4 modelMatrixBuzzBody = glm::mat4(modelMatrixBuzz);
		modelMatrixBuzzBody = glm::scale(modelMatrixBuzzBody,glm::vec3(3.0f));
		modelMatrixBuzzBody = glm::rotate(modelMatrixBuzzBody, glm::radians(turn90), glm::vec3(0,1,0));
		modelBuzzTorso.render(modelMatrixBuzzBody);
		//Cabeza
		glm::mat4 modelMatrixBuzzHead = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzHead = rotate (modelMatrixBuzzHead, rotBuzzHead,glm::vec3(0,1,0));
		modelBuzzHead.render(modelMatrixBuzzHead);
		//Cadera
		glm::mat4 modelMatrixBuzzHip = glm::mat4(modelMatrixBuzzBody);
		modelBuzzHip.render(modelMatrixBuzzHip);
		//Ala izquierda 1 (la mas pegada al torso)
		glm::mat4 modelMatrixBuzzLeftWing1 = glm::mat4 (modelMatrixBuzzBody);
		modelBuzzLeftWing1.render(modelMatrixBuzzLeftWing1);
		//Ala izquierda 2
		glm::mat4 modelmatrixBuzzLeftWing2 = glm::mat4 (modelMatrixBuzzBody);
		modelBuzzLeftWing2.render(modelmatrixBuzzLeftWing2);
		//Ala derecha 1 (la mas pegada al torso)
		glm::mat4 modelMatrixBuzzRightWing1 = glm::mat4 (modelMatrixBuzzBody);
		modelBuzzRightWing1.render(modelMatrixBuzzRightWing1);
		//Ala derecha 2
		glm::mat4 modelmatrixBuzzRightWing2 = glm::mat4 (modelMatrixBuzzBody);
		modelBuzzRightWing2.render(modelmatrixBuzzRightWing2);

		//BUZZ POR LADOS:
		//Brazo izquierdo y su Pivote 
		glm::mat4 modelMatrixBuzzLeftArm = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzLeftArm = glm::translate(modelMatrixBuzzLeftArm,glm::vec3(0.178185,0.58335,-0.026702));
		modelMatrixBuzzLeftArm = glm::rotate(modelMatrixBuzzLeftArm, rotBuzzLeftArm, glm::vec3(1.0,0.0,0.0));
		modelMatrixBuzzLeftArm = glm::translate(modelMatrixBuzzLeftArm,glm::vec3(-0.178185,-0.58335,0.026702));
		modelBuzzLeftArm.render(modelMatrixBuzzLeftArm);
		//Antebrazo izquierdo y su pivote
		glm::mat4 modelMatrixBuzzLeftForearm = glm::mat4 (modelMatrixBuzzLeftArm);
		modelMatrixBuzzLeftForearm = glm::translate(modelMatrixBuzzLeftForearm,glm::vec3(0.3002,0.58,0.008279));
		modelMatrixBuzzLeftForearm = glm::rotate(modelMatrixBuzzLeftForearm, rotBuzzLeftForearm, glm::vec3(1.0,0.0,0.0));
		modelMatrixBuzzLeftForearm = glm::translate(modelMatrixBuzzLeftForearm,glm::vec3(-0.3002,-0.58,-0.008279));
		modelBuzzLeftForearm.render(modelMatrixBuzzLeftForearm);
		//Mano izquierda y su pivote
		glm::mat4 modelMatrixBuzzLeftHand = glm::mat4 (modelMatrixBuzzLeftForearm);
		modelMatrixBuzzLeftHand = glm::translate(modelMatrixBuzzLeftHand,glm::vec3(0.4055,0.5853,0.06706));
		modelMatrixBuzzLeftHand = glm::rotate(modelMatrixBuzzLeftHand, rotBuzzLeftHand, glm::vec3(1.0,0.0,0.0));
		modelMatrixBuzzLeftHand = glm::translate(modelMatrixBuzzLeftHand,glm::vec3(-0.4055,-0.5853,-0.06706));
		modelBuzzLeftHand.render(modelMatrixBuzzLeftHand);
		//Thigh izquierda
		glm::mat4 modelMatrixBuzzLeftThigh = glm::mat4 (modelMatrixBuzzBody);
		modelMatrixBuzzLeftThigh = glm::translate(modelMatrixBuzzLeftThigh,glm::vec3(0.05909,0.397,0.007883));
		modelMatrixBuzzLeftThigh = glm::rotate(modelMatrixBuzzLeftThigh, rotBuzzLeftThigh, glm::vec3(1.0,0.0,0.0));
		modelMatrixBuzzLeftThigh = glm::translate(modelMatrixBuzzLeftThigh,glm::vec3(-0.05909,-0.397,-0.007883));
		modelBuzzLeftTigh.render(modelMatrixBuzzLeftThigh);
		//Calf izquierdo
		glm::mat4 modelMatrixBuzzLeftCalf = glm::mat4 (modelMatrixBuzzLeftThigh);
		modelMatrixBuzzLeftCalf = glm::translate(modelMatrixBuzzLeftCalf,glm::vec3(0.065,0.2168,-0.007884));
		modelMatrixBuzzLeftCalf = glm::rotate(modelMatrixBuzzLeftCalf, rotBuzzLeftCalf, glm::vec3(1.0,0.0,0.0));
		modelMatrixBuzzLeftCalf = glm::translate(modelMatrixBuzzLeftCalf,glm::vec3(-0.065,-0.2168,0.007884));
		modelBuzzLeftCalf.render(modelMatrixBuzzLeftCalf);
		//Pie izquierdo
		glm::mat4 modelMatrixBuzzLeftFoot= glm::mat4 (modelMatrixBuzzLeftCalf);
		modelMatrixBuzzLeftFoot = glm::translate(modelMatrixBuzzLeftFoot,glm::vec3(0.72,0.08426,-0.02621));
		modelMatrixBuzzLeftFoot = glm::rotate(modelMatrixBuzzLeftFoot, rotBuzzLeftFoot, glm::vec3(1.0,0.0,0.0));
		modelMatrixBuzzLeftFoot = glm::translate(modelMatrixBuzzLeftFoot,glm::vec3(-0.72,-0.08426,0.02621));
		modelBuzzLeftFoot.render(modelMatrixBuzzLeftFoot);

		//RIGHT SIDE
		//Brazo derecho y su Pivote 
		glm::mat4 modelMatrixBuzzRightArm = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzRightArm = glm::translate(modelMatrixBuzzRightArm,glm::vec3(-0.178185,0.58335,-0.026702));
		modelMatrixBuzzRightArm = glm::rotate(modelMatrixBuzzRightArm, rotBuzzRightArm, glm::vec3(1.0,0.0,0.0));
		modelMatrixBuzzRightArm = glm::translate(modelMatrixBuzzRightArm,glm::vec3(0.178185,-0.58335,0.026702));
		modelBuzzRightArm.render(modelMatrixBuzzRightArm);
		//Antebrazo derecho y su pivote
		glm::mat4 modelMatrixBuzzRightForearm = glm::mat4 (modelMatrixBuzzRightArm);
		modelMatrixBuzzRightForearm = glm::translate(modelMatrixBuzzRightForearm,glm::vec3(-0.3002,0.58,0.008279));
		modelMatrixBuzzRightForearm = glm::rotate(modelMatrixBuzzRightForearm, rotBuzzRightForearm, glm::vec3(1.0,0.0,0.0));
		modelMatrixBuzzRightForearm = glm::translate(modelMatrixBuzzRightForearm,glm::vec3(0.3002,-0.58,-0.008279));
		modelBuzzRightForearm.render(modelMatrixBuzzRightForearm);
		//Mano derecha y su pivote
		glm::mat4 modelMatrixBuzzRightHand = glm::mat4 (modelMatrixBuzzRightForearm);
		modelMatrixBuzzRightHand = glm::translate(modelMatrixBuzzRightHand,glm::vec3(-0.4055,0.5853,0.06706));
		modelMatrixBuzzRightHand = glm::rotate(modelMatrixBuzzRightHand, rotBuzzRightHand, glm::vec3(1.0,0.0,0.0));
		modelMatrixBuzzRightHand = glm::translate(modelMatrixBuzzRightHand,glm::vec3(0.4055,-0.5853,-0.06706));
		modelBuzzRightHand.render(modelMatrixBuzzRightHand);
		//cchnagyy
		//Thigh derecha
		glm::mat4 modelMatrixBuzzRightThigh = glm::mat4 (modelMatrixBuzzBody);
		modelMatrixBuzzRightThigh = glm::translate(modelMatrixBuzzRightThigh,glm::vec3(-0.05909,0.397,0.007883));
		modelMatrixBuzzRightThigh = glm::rotate(modelMatrixBuzzRightThigh, rotBuzzRightThigh, glm::vec3(1.0,0.0,0.0));
		modelMatrixBuzzRightThigh = glm::translate(modelMatrixBuzzRightThigh,glm::vec3(0.05909,-0.397,-0.007883));
		modelBuzzRightTigh.render(modelMatrixBuzzRightThigh);
		//Calf derecha
		glm::mat4 modelMatrixBuzzRightCalf = glm::mat4 (modelMatrixBuzzRightThigh);
		modelMatrixBuzzRightCalf = glm::translate(modelMatrixBuzzRightCalf,glm::vec3(-0.065,0.2168,-0.007884));
		modelMatrixBuzzRightCalf = glm::rotate(modelMatrixBuzzRightCalf, rotBuzzRightCalf, glm::vec3(1.0,0.0,0.0));
		modelMatrixBuzzRightCalf = glm::translate(modelMatrixBuzzRightCalf,glm::vec3(0.065,-0.2168,0.007884));
		modelBuzzRightCalf.render(modelMatrixBuzzRightCalf);
		//Pie derecho
		glm::mat4 modelMatrixBuzzRightFoot= glm::mat4 (modelMatrixBuzzRightCalf);
		modelMatrixBuzzRightFoot = glm::translate(modelMatrixBuzzRightFoot,glm::vec3(-0.72,0.08426,-0.02621));
		modelMatrixBuzzRightFoot = glm::rotate(modelMatrixBuzzRightFoot, rotBuzzRightFoot, glm::vec3(1.0,0.0,0.0));
		modelMatrixBuzzRightFoot = glm::translate(modelMatrixBuzzRightFoot,glm::vec3(0.72,-0.08426,0.02621));
		modelBuzzRightFoot.render(modelMatrixBuzzRightFoot);

		//Modelos descargados
		modelCat.render(modelMatrixCat);
		glActiveTexture(GL_TEXTURE0);

		modelWood.render(modelMatrixWood);
		glActiveTexture(GL_TEXTURE0);

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

		// Para salvar los keyframes
		if(modelSelected==1 && record){
			matrixDartJoints.push_back(rotDartHead);
			matrixDartJoints.push_back(rotDartLeftArm);
			matrixDartJoints.push_back(rotDartLeftHand);
			matrixDartJoints.push_back(rotDartRightArm);
			matrixDartJoints.push_back(rotDartRightHand);
			matrixDartJoints.push_back(rotDartLeftLeg);
			matrixDartJoints.push_back(rotDartRightLeg);
			if(saveFrame){
				appendFrame(myfile, matrixDartJoints);
				saveFrame = false;
			}
		}
		else if(keyFramesDartJoints.size() > 0){
			//esta variable define el numero de pasos y e que paso estamos
			interpolationDartJoints = numPasosDartJoints / (float) maxNumPasosDartJoints;
			//Inicialmente el numero de pasos tiene 0 y el maximo es 20
			numPasosDartJoints++;
			if(interpolationDartJoints > 1.0){
				numPasosDartJoints = 0;
				interpolationDartJoints = 0;
				indexFrameDartJoints = indexFrameDartJointsNext;
				indexFrameDartJointsNext++;
			}
			//Cuando se pasa del numero de keyframes que tenemos, lo reiniciamos
			if(indexFrameDartJointsNext > keyFramesDartJoints.size() -1)
				indexFrameDartJointsNext = 0;
			//Se intepolan los joints
			rotDartHead = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 0, interpolationDartJoints);
			rotDartLeftArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 1, interpolationDartJoints);
			rotDartLeftHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 2, interpolationDartJoints);
			rotDartRightArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 3, interpolationDartJoints);
			rotDartRightHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 4, interpolationDartJoints);
			rotDartLeftLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 5, interpolationDartJoints);
			rotDartRightLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 6, interpolationDartJoints);
		}
		
		// Dart completo
		if(modelSelected == 2 && record){
			matrixDart.push_back(modelMatrixDart);
			if(saveFrame){
				saveFrame = false;
				appendFrame(myfile, matrixDart);
			}
		}
		else if(keyFramesDart.size() > 0){
			interpolationDart = numPasosDart / (float) maxNumPasosDart;
			numPasosDart++;
			if(interpolationDart > 1.0){
				numPasosDart = 0;
				interpolationDart = 0;
				indexFrameDart = indexFrameDartNext;
				indexFrameDartNext++;
			}
			if(indexFrameDartNext > keyFramesDart.size() -1){
				indexFrameDartNext = 0;
			}
			modelMatrixDart = interpolate(keyFramesDart, indexFrameDart, indexFrameDartNext, 0, interpolationDart);
		}

		//PARA SALVAR LOS KEYFRAMES DE BUZZ 
		if(modelSelected ==3 && record){
			//Con el pushback agregamos un elemento al arreglo de los joints
			matrixBuzzJoints.push_back(rotBuzzHead);
			matrixBuzzJoints.push_back(rotBuzzLeftArm);
			matrixBuzzJoints.push_back(rotBuzzLeftForearm);
			matrixBuzzJoints.push_back(rotBuzzLeftHand);
			matrixBuzzJoints.push_back(rotBuzzRightArm);
			matrixBuzzJoints.push_back(rotBuzzRightForearm);
			matrixBuzzJoints.push_back(rotBuzzRightHand);
			matrixBuzzJoints.push_back(rotBuzzLeftThigh);
			matrixBuzzJoints.push_back(rotBuzzLeftCalf);
			matrixBuzzJoints.push_back(rotBuzzLeftFoot);
			matrixBuzzJoints.push_back(rotBuzzRightThigh);
			matrixBuzzJoints.push_back(rotBuzzRightCalf);
			matrixBuzzJoints.push_back(rotBuzzRightFoot);

			if(saveFrame){
				appendFrame(myfile, matrixBuzzJoints);
				saveFrame = false;
			}
		}
		else if(keyFramesBuzzJoints.size() >0){
			//esta variable define el numero de pasos y e que paso estamos
			interpolationBuzzJoints = numPasosBuzzJoints/ (float) maxNumPasosBuzzJoints;
			//Inicialmente el numero de pasos tiene 0 y el maximo es 20
			numPasosBuzzJoints++;
			if(interpolationBuzzJoints > 1.0){
				numPasosBuzzJoints = 0;
				interpolationBuzzJoints = 0;
				indexFrameBuzzJoints = indexFrameBuzzJointsNext;
				indexFrameBuzzJointsNext++;

			}
			//Cuando se pasa del numero de keyframes que tenemos, lo reiniciamos
			if (indexFrameBuzzJointsNext > keyFramesBuzzJoints.size()-1)
				indexFrameBuzzJointsNext=0;
			//Se intepolan los joints

			rotBuzzHead = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 0, interpolationBuzzJoints);
			rotBuzzLeftArm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 1, interpolationBuzzJoints);
			rotBuzzLeftForearm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 2, interpolationBuzzJoints);
			rotBuzzLeftHand = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 3, interpolationBuzzJoints);
			rotBuzzRightArm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 4, interpolationBuzzJoints);
			rotBuzzRightForearm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 5, interpolationBuzzJoints);
			rotBuzzRightHand = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 6, interpolationBuzzJoints);
			//under
			rotBuzzLeftThigh = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 7, interpolationBuzzJoints);
			rotBuzzLeftCalf = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 8, interpolationBuzzJoints);
			rotBuzzLeftFoot = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 9, interpolationBuzzJoints);
			rotBuzzRightThigh = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 10, interpolationBuzzJoints);
			rotBuzzRightCalf = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 11, interpolationBuzzJoints);
			rotBuzzRightFoot = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 12, interpolationBuzzJoints);

			if (modelSelected == 4 && record){
				//Aqui interpolamos en la matriz de transformacion
				//[Quaternion matrix]
				matrixBuzz.push_back(modelMatrixBuzz);
				if(saveFrame){
					saveFrame=false;
					appendFrame(myfile, matrixBuzz);
				}
			}
			else if (keyFramesBuzz.size() >0){
				interpolationBuzz = numPasosBuzz / (float) maxNumPasosBuzz;
				numPasosBuzz++;
				if(interpolationBuzz > 1.0){
					numPasosBuzz =0;
					interpolationBuzz =0;
					indexFrameBuzz = indexFrameBuzzNext;
					indexFrameBuzzNext++;
				}
				if(indexFrameBuzzNext > keyFramesBuzz.size()-1){
					indexFrameBuzzNext =0;
				}
				modelMatrixBuzz = interpolate (keyFramesBuzz, indexFrameBuzz, indexFrameBuzzNext, 0, interpolationBuzz);

				
			}

		}

		/** Máquinas de estado **/
		switch(state){
			case 0: /* Evaluación de cuanto se tiene que avanzar en cada recorrido */
				/* If conforme al curso con medidas obtenidas a prueba y error */
				if(numberAdvance == 0)
					maxAdvance = 65.0f;
				else if(numberAdvance == 1)
					maxAdvance = 49.0f;
				else if(numberAdvance == 2)
					maxAdvance = 44.5f;
				else if(numberAdvance == 3)
					maxAdvance = 49.0f;
				else if(numberAdvance == 4)
					maxAdvance = 44.5f;
				state = 1;
				break;
			case 1: /* Recorrido como tal*/
				modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0, 0.0, avanceEclipse)); /* Desplazamiento en z */
				advanceCount += avanceEclipse;
				rotWheelsX += 0.05;
				if(rotWheelsY > 0)
					rotWheelsY -= 0.0025;
				if(advanceCount > maxAdvance){
					advanceCount = 0.0f;
					numberAdvance++;
					if(numberAdvance > 4) 
						numberAdvance = 1;
					state = 2;
				}
				break;
			case 2:
				modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0f, 0.0f, 0.025f));
				modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(rotEclipse), glm::vec3(0.0f, 1.0f, 0.0f));
				rotCount += rotEclipse;
				rotWheelsX += 0.025;
				if(rotWheelsY < 0.25)
					rotWheelsY += 0.0025;
				if(rotCount > 90.0){
					rotCount = 0;
					state = 0;
				}
				break;
			default:
				break;
		}

		//Maquina de estados para que el lambo se mueva
		if(LamboFlag){
			switch (stateLambo)
				{
				case 0:
				//Case 0 sirve para que el carro avance en el carril interior
					if(numberAdvanceLambo == 0)
						maxAdvanceLambo = 39.78f;
					else if(numberAdvanceLambo == 1)
						maxAdvanceLambo = 39.5f;
					else if (numberAdvanceLambo == 2)
						maxAdvanceLambo = 34.5f;
					else if (numberAdvanceLambo == 3)
						maxAdvanceLambo = 39.5f;
					else if (numberAdvanceLambo == 4){
						maxAdvanceLambo = 0.0f;
						avanceLambo = 0.0;
						rotLambo=0;
					}
					stateLambo = 1;
					break;
				
				case 1:
				//Case 1 es para definir cuando el auto avanza en el carril
					modelMatrixLambo=glm::translate(modelMatrixLambo, glm::vec3(0.0,0.0,avanceLambo));
					advanceCountLambo+=avanceLambo;
					rotWheelsXLambo+=0.025;
					//Regresamos a la llanta a su posicion
					rotWheelsYLambo-=0.0025;
					//Cuando la llanta se endereza hacemos que se detenga
					if (rotWheelsYLambo<0)
						rotWheelsYLambo=0.0;
					if(advanceCountLambo>maxAdvanceLambo){
						advanceCountLambo =0.0;
						//Se sube de nivel para que avance más
						numberAdvanceLambo++;
						stateLambo=2;							
					}
					if(numberAdvanceLambo==4){
							advanceCountLambo=0;
							rotWheelsYLambo=0;
							rotWheelsXLambo=0;
							
						}
					break;

				case 2:
				//Case 2 es para definir cuando el auto esta dando vuelta
					modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0,0,0.025));
				//Checamos que el lambo se pueda rotar en base de y, utilizando la variable rotLambo
					modelMatrixLambo = glm::rotate(modelMatrixLambo,glm::radians(rotLambo),glm::vec3(0.0f, 1.0f, 0.0f));
					rotCountLambo+=rotLambo;
					rotWheelsYLambo+=0.0025;
					//Giramos las llantas
					rotWheelsXLambo +=0.025;
					if(rotWheelsYLambo>0.25)
						rotWheelsYLambo=0.25f;
					//Limitamos la rotacion
					if(rotCountLambo>90.0){
						//Al terminar de rotar, se regresa el valor a 0 para poder realizar el giro nuevamente
						rotCountLambo = 0;
						stateLambo = 0;
						if(numberAdvanceLambo==4){
							rotCountLambo=90;
							advanceCountLambo=0;
							rotLambo=0;

						}

					}
					break;

				default:
					break;
				}
		}

		//Se utiliza para que se pueda abrir y cerrar la puerta
			if(numberAdvanceLambo==4 && rotLambo==0 ){
				switch (stateDoor)
				{
				case 0:
					dorRotCount++;
					if(dorRotCount>75.0f)
						stateDoor=1;
					break;
				
				case 1:
					dorRotCount--;
					if(dorRotCount<0.0f)
						dorRotCount=0;
					break;
				
				default:
					break;
				}
			}
			
		//Máquina de estados helicopter
		switch(stateHeli){
			case 0: // Establece el curso del helicóptero
				changeRotHeli = 1.0;
				if(numberHeli == 0){ // Bajada
					maxAlturaHeli = 0.1;
					stateHeli = 1;
				}else{ // Subida
					maxAlturaHeli = 10.0;
					stateHeli = 2;
				}
				break;
			case 1: // Baja hasta 0.1
				modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(0.0f, -avHeli, 0.0f));
				alturaHeli -= avHeli;
				if (rotHelHelY > 0){
					rotHelHelY -= changeRotHeli;
					changeRotHeli += 0.1;
				}
				if (alturaHeli <= maxAlturaHeli){
					alturaHeli = 0.1;
					changeRotHeli = 45.0;
					stateHeli = 3;
				}
				break;
			case 2: // Sube hasta 10
				modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(0.0f, avHeli, 0.0f));
				alturaHeli += avHeli;
				if (rotHelHelY < 1000000){
					rotHelHelY += changeRotHeli;
					changeRotHeli ++;
				}
				if (alturaHeli >= maxAlturaHeli){
					alturaHeli = 10.0;
					stateHeli = 3;
				}
				break;
			case 3: //Espera hélices
				if (numberHeli == 0 && rotHelHelY > 0){
					rotHelHelY -= 20 + changeRotHeli;
					if (changeRotHeli > 0) changeRotHeli -= 0.25;
				}else if (numberHeli == 0){
					rotHelHelY = 0.0;
					numberHeli = 1;
					stateHeli = 0;
				}else if (numberHeli == 1 && rotHelHelY < 1000000){
					rotHelHelY += (5 + changeRotHeli);
					changeRotHeli++;
				}else{
					rotHelHelY = 1000000.0;
					numberHeli = 0;
					stateHeli = 0;
				}
				break;
			default:
				break;
		}

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}