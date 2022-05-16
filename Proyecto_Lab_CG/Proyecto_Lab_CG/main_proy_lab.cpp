/*---------------------------------------------------------*/
/* ----------------  Proyecto LAB CG-----------*/
/* ----------------  Alvarez Reyes Juan Luis-----------*/
/* ----------------  Luna Villaseñor Angel David-----------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 100.0f, 600.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(170.0f, -200.0f, 65.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;

// mover carro 
float
	movAuto_x = 0.0f,
	movAuto_z = 0.0f,
	orienta = 0.0f,
	inclinaLlantas = 0.0f,
	giroLlantas = 0.0f;
bool
	mov_1 = false,
	mov_2 = false,
	mov_3 = false,
	mov_4 = false,
	mov_5 = false,
	soundjeep = false,
	animacionCar = false;
// mover ovni 
float
	movOvni_x = 0.0f,
	movOvni_y = 0.0f,
	movOvni_z = 0.0f,
	orientaOvni = 0.0f;
// animacion Ovni
bool
	movOvni_1 = false,
	movOvni_2 = false,
	movOvni_3 = false,
	movOvni_4 = false,
	movOvni_5 = false,
	soundovni = false,
	animacionOvni = false;
// mover anciano 
float
	movAnc_x = 0.0f,
	movAnc_y = 0.0f,
	movAnc_z = 0.0f,
	tamAnc = 0.14f;
// mover soldado
float   movS_x = 119.0f,
		movS_y = -7.0f,
		movS_z = -268.0f,
		rot = 180.0f;

// persecucion
float	movRemy_x = 0.0f,
		movAlien_x = 0.0f;

// Patada Voladora
float	alienSalto_y = 3.0f,
		alienSalto_z = -264.0,
		alienPatada_z = 0.0f,
		alienPatada_zz = -264.0f,
		alienPatada_y = -80.0f,
		soldadoEvade_y = 2.0f,
		alienSalto_rot = 0.0f,
		soldadoFall_y = -80.0f;

int		estadoPatada = 0;

bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f,

		perroX = 119.0f,
		perroY = -7.0f,
		perroZ = -268.0f,
		giroPataTI = 0.0f,
		giroPataTD = 0.0f,
		giroPataDI =0.0f,
		giroPataDD = 0.0f,
		giroPerro = 0.0f;

float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f,
	
		perroXInc = 0.0f,
		perroYInc = 0.0f,
		perroZInc = 0.0f,
		giroPataTIInc = 0.0f,
		giroPataTDInc = 0.0f,
		giroPataDIInc = 0.0f,
		giroPataDDInc = 0.0f,
		giroPerroInc = 0.0f;

float	myVariable = 0.0f;

#define MAX_FRAMES 6
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float perroX;
	float perroY;
	float perroZ;
	float giroPataTI;
	float giroPataTD;
	float giroPataDI;
	float giroPataDD;
	float giroPerro;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 6;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].perroX = perroX;
	KeyFrame[FrameIndex].perroY = perroY;
	KeyFrame[FrameIndex].perroZ = perroZ;

	KeyFrame[FrameIndex].giroPataTI = giroPataTI;
	KeyFrame[FrameIndex].giroPataTD = giroPataTD;
	KeyFrame[FrameIndex].giroPataDI = giroPataDI;
	KeyFrame[FrameIndex].giroPataDD = giroPataDD;
	KeyFrame[FrameIndex].giroPerro = giroPerro;

	FrameIndex++;
}

void resetElements(void)
{
	perroX = KeyFrame[0].perroX;
	perroY = KeyFrame[0].perroY;
	perroZ = KeyFrame[0].perroZ;

	giroPataTI = KeyFrame[0].giroPataTI;
	giroPataTD = KeyFrame[0].giroPataTD;
	giroPataDI = KeyFrame[0].giroPataDI;
	giroPataDD = KeyFrame[0].giroPataDD;
	giroPerro = KeyFrame[0].giroPerro;
}

void interpolation(void)
{
	perroXInc = (KeyFrame[playIndex + 1].perroX - KeyFrame[playIndex].perroX) / i_max_steps;
	perroYInc = (KeyFrame[playIndex + 1].perroY - KeyFrame[playIndex].perroY) / i_max_steps;
	perroZInc = (KeyFrame[playIndex + 1].perroZ - KeyFrame[playIndex].perroZ) / i_max_steps;

	giroPataTIInc = (KeyFrame[playIndex + 1].giroPataTI - KeyFrame[playIndex].giroPataTI) / i_max_steps;
	giroPataTDInc = (KeyFrame[playIndex + 1].giroPataTD - KeyFrame[playIndex].giroPataTD) / i_max_steps;
	giroPataDIInc = (KeyFrame[playIndex + 1].giroPataDI - KeyFrame[playIndex].giroPataDI) / i_max_steps;
	giroPataDDInc = (KeyFrame[playIndex + 1].giroPataDD - KeyFrame[playIndex].giroPataDD) / i_max_steps;
	giroPerroInc = (KeyFrame[playIndex + 1].giroPerro - KeyFrame[playIndex].giroPerro) / i_max_steps;

}

void musicovni() {
	if (soundovni) {
		bool played = PlaySound(L"ufoSonido.wav", NULL, SND_LOOP | SND_ASYNC);
		soundovni = false;
	}
}

void musicjeep() {
	if (soundjeep) {
		bool played = PlaySound(L"carro.wav", NULL, SND_LOOP | SND_ASYNC);
		soundjeep = false;
	}
}

void musicstop() {
		bool played = PlaySound(NULL, NULL, SND_LOOP | SND_ASYNC);
}



void animate(void)
{
	//lightPosition.x = 100.0f * cos(myVariable);
	//lightPosition.z = 100.0f * sin(myVariable);

	//myVariable += 0.1f;


	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			perroX += perroXInc;
			perroY += perroYInc;
			perroZ += perroZInc;

			giroPataTI += giroPataTIInc;
			giroPataTD += giroPataTDInc;
			giroPataDI += giroPataDIInc;
			giroPataDD += giroPataDDInc;
			giroPerro += giroPerroInc;

			i_curr_steps++;
		}
	}

	//Vehículo
	if (animacionCar == true)
	{
		//primer movimiento
		if (mov_1 == true)
		{
			if (movAuto_z <= 200.0f )
			{
				movAuto_z += 2.0f;
				giroLlantas += 5.0f;
				musicjeep();
			}
			else
			{	
				if (orienta <= 85.0f && inclinaLlantas <= 45.0f)
				{
					orienta += 4.0f;
					inclinaLlantas += 2.0f;
					giroLlantas = 0.0f;

				}
				else
				{
					if (inclinaLlantas >= 0.0f)
					{
						inclinaLlantas -= 2.0f;
					}
					else
					{
						mov_1 = false;
						mov_2 = true;
					}
					
				}
				
			}
		}
		//segundo movimiento
		if (mov_2 == true)
		{
			if (movAuto_x <= 164.0f)
			{
				movAuto_x += 2.0f;
				giroLlantas += 5.0f;
			}
			else
			{
				if (orienta >= 5.0f && inclinaLlantas >= -45.0f)
				{
					orienta -= 4.0f;
					inclinaLlantas -= 2.0f;
					giroLlantas = 0.0f;

				}
				else
				{
					if (inclinaLlantas <= 0.0f)
					{
						inclinaLlantas += 2.0f;
					}
					else
					{
						mov_2 = false;
						mov_3 = true;
					}

				}
			}
		}
		//tercer movimiento
		if (mov_3 == true)
		{
			if (movAuto_z <= 580.0f)
			{
				movAuto_z += 2.0f;
				giroLlantas += 5.0f;
			}
			else
			{
				if (orienta >= -95.0f && inclinaLlantas >= -45.0f)
				{
					orienta -= 4.0f;
					inclinaLlantas -= 2.0f;
					giroLlantas = 0.0f;

				}
				else
				{
					if (inclinaLlantas <= 0.0f)
					{
						inclinaLlantas += 2.0f;
					}
					else
					{
						mov_3 = false;
						mov_4 = true;
					}

				}
			}
		}
		//cuarto movimiento
		if (mov_4 == true)
		{
			if (movAuto_x >= 80.0f)
			{
				movAuto_x -= 2.0f;
				giroLlantas += 5.0f;
			}
			else
			{
				if (orienta >= -180.0f && inclinaLlantas >= -45.0f)
				{
					orienta -= 4.0f;
					inclinaLlantas -= 2.0f;
					giroLlantas = 0.0f;

				}
				else
				{
					if (inclinaLlantas <= 0.0f)
					{
						inclinaLlantas += 2.0f;
					}
					else
					{
						mov_4 = false;
						mov_5 = true;
					}

				}
			}
		}
		//quinto movimiento
		if (mov_5 == true)
		{
			if (movAuto_z >= 550.0f)
			{
				movAuto_z -= 2.0f;
				giroLlantas += 5.0f;
			}
			else
			{
				mov_5 = false;
				animacion = false;
				soundjeep = false;
				musicstop();
			}
		}
	}
	//Ovni secuestrador
	if (animacionOvni == true)
	{
		
		//primer movimiento
		if (movOvni_1 == true)
		{
			if (movOvni_y <= 225.0f)
			{
				movOvni_y += 2.0f;
				orientaOvni += 5.0f;
				musicovni();

			}
			else
			{
				movOvni_1 = false;
				movOvni_2 = true;
			}
		}
		//segundo movimiento
		if (movOvni_2 == true)
		{
			if (movOvni_z <= 312.0f)
			{
				movOvni_z += 2.0f;
				lightPosition.z += 2.0;
				lightPosition.y += 0.8;
				orientaOvni += 5.0f;
			}
			else
			{
				movOvni_2 = false;
				movOvni_3 = true;
			}
		}
		//tercer movimiento
		if (movOvni_3 == true)
		{
			if (movOvni_x >= -350.0f)
			{
				movOvni_x -= 2.0f;
				lightPosition.x -= 2.0;
				lightPosition.y += 0.6;
				orientaOvni += 5.0f;
			}
			else
			{
				movOvni_3 = false;
				movOvni_4 = true;
			}
		}
		//cuarto movimiento
		if (movOvni_4 == true)
		{
			if (movOvni_z >= 150.0f)
			{
				movOvni_z -= 2.0f;
				lightPosition.z -= 2.0;
				lightPosition.y += 2.0;
				movAnc_z -= 2.0f;
				movAnc_y += 3.0f;
				tamAnc -= 0.0009f;
				orientaOvni += 5.0f;
				
				
			}
			else
			{
				tamAnc = 0.0f;
				movOvni_4 = false;
				movOvni_5 = true;
			}
		}
		//quinto movimiento
		if (movOvni_5 == true)
		{
			if (movOvni_y >= -90.0f)
			{
				movOvni_y -= 2.0f;
				lightPosition.y += 2.0;
				orientaOvni += 5.0f;
				
			}
			else
			{	
				musicstop();
				movOvni_5 = false;
				animacionOvni = false;
				soundovni = false;
				
			}
		}
	}
	//persecucion alien/humano
	if (animacion and movRemy_x <= 284.0f)
	{
		movRemy_x += 2.0f;
		movAlien_x += 2.0;
	}

	if (estadoPatada == 0)
	{
		alienPatada_z = 0.0f;
		alienPatada_y = -80.0f;
		alienSalto_y = 3.0f;
		soldadoFall_y = -80.0f;
		soldadoEvade_y = 2.0f;
		alienSalto_rot = 0.0f;
		alienSalto_z = -264.0;
	}
	if (estadoPatada == 1)
	{
		if (alienPatada_z < 100)
		{
			alienPatada_z += 4.0f;
		}
		alienPatada_y = 2.0f;
		alienSalto_y = -80.0f;
		if (alienPatada_z >= 70)
		{
			soldadoFall_y = 2.0f;
			soldadoEvade_y = -80.0f;
		}
		if (alienPatada_z >= 100)
		{
			alienPatada_y = -80.0f;
			alienSalto_y = 2.0f;
			alienSalto_rot = 180.0f;
			alienSalto_z = alienPatada_zz + alienPatada_z+ 30;
		}
	}

}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model casaV1("resources/objects/CasaV1/CasaV1.obj");
	Model casaV2("resources/objects/CasaV2/casaV2.obj");
	Model edificioV1("resources/objects/EdificioV1/edificioV1.obj");
	Model edificioV2("resources/objects/EdificioV2/edificioV2.obj");
	Model arbol1("resources/objects/Arboles/Arbol1/Arbol1.obj");
	Model arbol2("resources/objects/Arboles/Arbol2/Arbol2.obj");
	Model arbol4("resources/objects/Arboles/Arbol4/Arbol4.obj");
	Model arbol6("resources/objects/Arboles/Arbol6/Arbol6.obj");
	Model arbol7("resources/objects/Arboles/Arbol7/Arbol7.obj");
	Model mesa("resources/objects/Mesa/mesa.obj");
	Model banca("resources/objects/Banca/banca.obj");
	Model ufo("resources/objects/Ufo/Ufo.obj");

	//Carro
	Model jeep("resources/objects/Carro/carro.obj");
	Model rueda("resources/objects/Carro/rueda.obj");
	
	//Perro
	Model pataDD("resources/objects/Perro/PataDelantera/PataDelDer.obj");
	Model pataDI("resources/objects/Perro/PataDelantera/PataDelIzq.obj");
	Model pataTD("resources/objects/Perro/PataTrasera/PataTrasDer.obj");
	Model pataTI("resources/objects/Perro/PataTrasera/PataTrasIzq.obj");
	Model perroT("resources/objects/Perro/Torso/PerroTorso.obj");

	ModelAnim hombreBaile("resources/objects/SillyDancing/SillyDancing.dae");
	hombreBaile.initShaders(animShader.ID);
	
	ModelAnim soldadoEspera("resources/objects/SoldierStand/SoldierSatnd.dae");
	soldadoEspera.initShaders(animShader.ID);

	ModelAnim soldadoEvade("resources/objects/SoldierEvate/SoldierEvate.dae");
	soldadoEvade.initShaders(animShader.ID);

	ModelAnim alienSalta("resources/objects/AlienJump/AlienJump.dae");
	alienSalta.initShaders(animShader.ID);

	ModelAnim soldadoFall("resources/objects/SoldierFall/SoldierFall.dae");
	soldadoFall.initShaders(animShader.ID);

	ModelAnim alienPatada("resources/objects/AlienKick/AlienKick.dae");
	alienPatada.initShaders(animShader.ID);

	ModelAnim alienRun("resources/objects/AlienRun/AlienRun.dae");
	alienRun.initShaders(animShader.ID);

	ModelAnim personRun("resources/objects/PersonRun/PersonRun.dae");
	personRun.initShaders(animShader.ID);

	//Inicialización de KeyFrames
	/*for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}*/
		KeyFrame[0].perroX = 119;
		KeyFrame[0].perroY = -7;
		KeyFrame[0].perroZ = -268;
		KeyFrame[0].giroPerro = 0;
		KeyFrame[0].giroPataDD = 0;
		KeyFrame[0].giroPataDI = 0;
		KeyFrame[0].giroPataTD = 0;
		KeyFrame[0].giroPataTI = 0;

		KeyFrame[1].perroX = 119;
		KeyFrame[1].perroY = -3;
		KeyFrame[1].perroZ = -268;
		KeyFrame[1].giroPerro = -64;
		KeyFrame[1].giroPataDD = 19;
		KeyFrame[1].giroPataDI = 24;
		KeyFrame[1].giroPataTD = 54;
		KeyFrame[1].giroPataTI = 56;

		KeyFrame[2].perroX = 119;
		KeyFrame[2].perroY = 11;
		KeyFrame[2].perroZ = -263;
		KeyFrame[2].giroPerro = 3;
		KeyFrame[2].giroPataDD = 32;
		KeyFrame[2].giroPataDI = 31;
		KeyFrame[2].giroPataTD = -45;
		KeyFrame[2].giroPataTI = -41;

		KeyFrame[3].perroX = 119;
		KeyFrame[3].perroY = 9;
		KeyFrame[3].perroZ = -263;
		KeyFrame[3].giroPerro = 145;
		KeyFrame[3].giroPataDD = -45;
		KeyFrame[3].giroPataDI = -45;
		KeyFrame[3].giroPataTD = 40;
		KeyFrame[3].giroPataTI = 28;

		KeyFrame[4].perroX = 119;
		KeyFrame[4].perroY = -2;
		KeyFrame[4].perroZ = -263;
		KeyFrame[4].giroPerro = 306;
		KeyFrame[4].giroPataDD = -45;
		KeyFrame[4].giroPataDI = -45;
		KeyFrame[4].giroPataTD = 40;
		KeyFrame[4].giroPataTI = 28;

		KeyFrame[5].perroX = 119;
		KeyFrame[5].perroY = -5;
		KeyFrame[5].perroZ = -263;
		KeyFrame[5].giroPerro = 356;
		KeyFrame[5].giroPataDD = 6;
		KeyFrame[5].giroPataDI = 8;
		KeyFrame[5].giroPataTD = 5;
		KeyFrame[5].giroPataTI = -8;

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.4f, 0.4f, 0.4f));	//color para todo el ambiente
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));	//color para ciertas partes, depende de la direccion de la iluminacion 
		staticShader.setVec3("dirLight.specular", glm::vec3(0.4f, 0.4f, 0.4f));	//brillo de los objetos

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.4f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.4f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);	//la luz viaja a mas distancia 

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 0.08f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[2].position", glm::vec3(80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 0.08f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.00032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		//soldado esquiva
		model = glm::translate(glm::mat4(1.0f), glm::vec3(364.0f, soldadoEvade_y, -190.0f));//movS_x, movS_y, movS_z));
		model = glm::scale(model, glm::vec3(0.15f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		soldadoEvade.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(364.0f, soldadoFall_y, -190.0f));//movS_x, movS_y, movS_z));
		model = glm::scale(model, glm::vec3(0.15f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		soldadoFall.Draw(animShader);

		// alien patada
		model = glm::translate(glm::mat4(1.0f), glm::vec3(367.0f, alienSalto_y, alienSalto_z));
		model = glm::scale(model, glm::vec3(0.15f));
		model = glm::rotate(model, glm::radians(alienSalto_rot), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		alienSalta.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(367.0f, alienPatada_y, alienPatada_zz + alienPatada_z));
		model = glm::scale(model, glm::vec3(0.15f));
		model = glm::rotate(model, glm::radians(alienSalto_rot), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		alienPatada.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(476.0f - movAlien_x, 3.0f, -93.0f));
		model = glm::scale(model, glm::vec3(0.14f));
		model = glm::rotate(model, glm::radians(260.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		alienRun.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(449.0f - movRemy_x, 3.0f, -94.0f));//movS_x, movS_y, movS_z));//
		model = glm::scale(model, glm::vec3(0.065f));
		model = glm::rotate(model, glm::radians(260.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		personRun.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f + movAnc_x, 0.0f + movAnc_y, 379.0f + movAnc_z));
		model = glm::scale(model, glm::vec3(tamAnc));
		model = glm::rotate(model, glm::radians(260.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		hombreBaile.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 0.3f, -353.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		casaV1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(346.0f, 0.3f, 394.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		casaV1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(381.0f, 0.3f, 55.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		casaV2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(155.0f, 0.3f, 55.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		casaV2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-324.0f, 0.3f, -345.0f));
		model = glm::scale(model, glm::vec3(0.30f));
		staticShader.setMat4("model", model);
		edificioV1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-52.0f, 0.3f, -348.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.30f));
		staticShader.setMat4("model", model);
		edificioV2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-449.0f, 0.0f, -43.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-137.0f, 0.0f, -43.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-137.0f, 0.0f, 276.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-457.0f, 0.0f, 461.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-293.0f, 0.0f, 98.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-297.0f, 0.0f, 366.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(111.0f, 0.0f, -230.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol4.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(78.0f, 0.0f, 328.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(78.0f, 0.0f, 480.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(78.0f, 0.0f, 400.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-116.0f, 0.0f, 189.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-116.0f, 0.0f, 69.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-472.0f, 0.0f, 69.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-472.0f, 0.0f, 221.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(64.0f, 0.0f, -453.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		arbol7.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(166.0f, 0.0f, -453.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		arbol7.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(120.0f, 0.0f, -391.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(120.0f, 0.0f, -356.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(120.0f, 0.0f, -322.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-76.0f, 0.0f, 414.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-382.0f, 0.0f, 414.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-368.0f, 0.0f, -38.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-261.0f, 0.0f, -38.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-445.0f, 0.0f, 141.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-151.0f, 0.0f, 140.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Perro
		// ------------------------------------------------------------------------------------------------------------------------- 
		//Torso
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 12.0f, 0.0f));
		model = glm::translate(model, glm::vec3(perroX, perroY, perroZ));
		tmp = model = glm::rotate(model, glm::radians(giroPerro), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		perroT.Draw(staticShader);
		
		//PataDDer
		model = glm::translate(tmp, glm::vec3(-0.85f, -0.5f, 2.0f));
		//model = glm::translate(model, glm::vec3(movS_x, 0.0f, movS_z));
		model = glm::rotate(model, glm::radians(giroPataDD), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		pataDD.Draw(staticShader);

		//PataDIzq
		model = glm::translate(tmp, glm::vec3(0.85f, -0.5f, 2.0f));
		model = glm::rotate(model, glm::radians(giroPataDI), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		pataDI.Draw(staticShader);

		//PataTDer
		model = glm::translate(tmp, glm::vec3(-0.75f, 0.5f, -4.5f));
		//model = glm::translate(model, glm::vec3(movS_x, 0.0f, movS_z));
		model = glm::rotate(model, glm::radians(giroPataTD), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		pataTD.Draw(staticShader);

		//PataTIzq
		model = glm::translate(tmp, glm::vec3(0.75f, 0.5f, -4.5f));
		model = glm::rotate(model, glm::radians(giroPataTI), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		pataTI.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Ovni
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(170.0f + movOvni_x, 86.0f + movOvni_y, 65.0f + movOvni_z));
		model = glm::rotate(model, glm::radians(orientaOvni), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		ufo.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-185.736f + movAuto_x, 0.0f, -458.228 + movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		jeep.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(-11.0f, 6.0f, 19.0f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(inclinaLlantas), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		rueda.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(11.0f, 6.0f, 19.0f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(inclinaLlantas), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		rueda.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-11.0f, 6.0f, -20.5f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		rueda.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(11.0f, 6.0f, -20.5f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		rueda.Draw(staticShader);	//Izq trase

		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//Car animation
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		animacionCar ^= true;
		mov_1 ^= true;
		soundjeep ^= true;
	}
	//Ovni animation
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		animacionOvni ^= true;
		movOvni_1 ^= true;
		soundovni ^= true;
	}
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		movS_z++; //posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		movS_z--;//posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		movS_x--;//posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		movS_x++;//posX++;

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		movS_y--;//posX--;
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		movS_y++;//posX++;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		//rot--;//posX--;
		giroPataTD--;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		//rot++;//posX++;
		giroPataTD++;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		//rot--;//posX--;
		giroPataTI--;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		//rot++;//posX++;
		giroPataTI++;
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		std::cout << "movS_x = " << movS_x << std::endl;
		std::cout << "movS_y = " << movS_y << std::endl;
		std::cout << "movS_z = " << movS_z << std::endl;
		std::cout << "Giro Perro = " << giroPerro << std::endl;
		std::cout << "Giro Pata DD = " << giroPataDD << std::endl;
		std::cout << "Giro Pata DI = " << giroPataDI << std::endl;
		std::cout << "Giro Pata TD = " << giroPataTD << std::endl;
		std::cout << "Giro Pata TI = " << giroPataTI << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		giroPataDD--;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		giroPataDD++;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		giroPataDI--;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroPataDI++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		giroPerro--;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		giroPerro++;
	//if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	//	lightPosition.x++;
	//if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	//	lightPosition.x--;

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		movAlien_x = 0.0f;
		movRemy_x = 0.0f;
		estadoPatada = 0;
		movOvni_x = 0.0f;
		movOvni_y = 0.0f;
		movOvni_z = 0.0f;
		movAuto_x = 0.0f;
		movAuto_z = 0.0f;
		orienta = 0.0f;
		movAnc_x = 0.0f;
		movAnc_y = 0.0f;
		movAnc_z = 0.0f;
		tamAnc = 0.14f;
		lightPosition.x = 170.0f;
		lightPosition.y = -200.0f;
		lightPosition.z = 65.0f;
		animacionCar = false;
		animacionOvni = false;
	}
	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		animacion ^= true;
		estadoPatada = 1;
	}
		

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}