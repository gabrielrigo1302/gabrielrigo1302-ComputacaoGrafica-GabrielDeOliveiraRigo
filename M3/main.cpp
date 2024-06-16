#include <iostream>
#include <string>
#include <assert.h>
#include <sstream>
#include <fstream>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "stb_image.h"
#include <vector>

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Valida se pressionou ou soltou o bot�o
bool pressRelease(int action);

// Prot�tipos das fun��es
int setupGeometry();

// Carrega a textura
GLuint carregarTextura();

// Print das instru��es de funcionamento
void mostrarInstrucoes();

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 1000, HEIGHT = 1000;

// Diret�rios dos modelos
std::string diretorioModelos = "../M3/objetos/";
std::string diretorioImagemModelo = "";

bool rotateUp = false;
bool rotateDown = false; 
bool rotateLeft = false; 
bool rotateRight = false;
bool rotateAngleLeft = false;
bool rotateAngleRight = false;

bool moveUp = false;
bool moveDown = false;
bool moveLeft = false;
bool moveRight = false;
bool moveFront = false;
bool moveBack = false;

bool bigger = false;
bool smaller = false;

int objectsRef = 0;
int selectedObject = 0;
bool createObject = true;

int qtdVertices = 0;

// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	//Muita aten��o aqui: alguns ambientes n�o aceitam essas configura��es
	//Voc� deve adaptar para a vers�o do OpenGL suportada por sua placa
	//Sugest�o: comente essas linhas de c�digo para desobrir a vers�o e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "M3 - Gabriel de Oliveira Rigo.", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	Shader shader = Shader("../M3/shaders/hello.vs", "../M3/shaders/hello.fs");

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	GLuint VAO = setupGeometry();
	GLuint textureID = carregarTextura();

	glUseProgram(shader.ID);

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(shader.ID, "model");

	glUniform1i(glGetUniformLocation(shader.ID, "tex_buffer"), 0);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glEnable(GL_DEPTH_TEST);

	mostrarInstrucoes();

	float rotateXObjects[10];
	float rotateYObjects[10];
	float rotateZObjects[10];
	glm::vec3 scales[10];
	glm::vec3 positionObjects[10];

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float angle = (GLfloat)glfwGetTime();

		for (int i = 0; i <= objectsRef; i++)
		{
			model = glm::mat4(1);

			if (i == selectedObject)
			{
				if (createObject)
				{
					rotateXObjects[i] = rotateYObjects[i] = rotateZObjects[i] = 0;
					positionObjects[i] = glm::vec3(0.0f, 0.0f, 0.0f);
					scales[i] = glm::vec3(1.0f, 1.0f, 1.0f);

					createObject = false;
				}

				if (rotateUp)
				{
					rotateXObjects[i] += glm::radians(1.0f);
				}

				if (rotateDown)
				{
					rotateXObjects[i] -= glm::radians(1.0f);
				}

				if (rotateLeft)
				{
					rotateYObjects[i] += glm::radians(1.0f);
				}

				if (rotateRight)
				{
					rotateYObjects[i] -= glm::radians(1.0f);
				}

				if (rotateAngleRight)
				{
					rotateZObjects[i] -= glm::radians(1.0f);
				}
				
				if (rotateAngleLeft)
				{
					rotateZObjects[i] += glm::radians(1.0f);
				}

				if (moveUp)
				{
					positionObjects[i] += glm::vec3(0.0f, 0.01f, 0.0f);
				}

				if (moveDown)
				{
					positionObjects[i] -= glm::vec3(0.0f, 0.01f, 0.0f);
				}

				if (moveRight)
				{
					positionObjects[i] += glm::vec3(0.01f, 0.0f, 0.0f);
				}

				if (moveLeft)
				{
					positionObjects[i] -= glm::vec3(0.01f, 0.0f, 0.0f);
				}

				if (moveFront)
				{
					positionObjects[i] -= glm::vec3(0.0f, 0.0f, 0.01f);
				}

				if (moveBack)
				{
					positionObjects[i] += glm::vec3(0.0f, 0.0f, 0.01f);
				}

				if (bigger)
				{
					scales[i] += glm::vec3(0.1f, 0.1f, 0.01f);
				}

				if (smaller)
				{
					scales[i] -= glm::vec3(0.01f, 0.01f, 0.01f);
				}
			}

			model = glm::translate(model, positionObjects[i]);
			model = glm::scale(model, scales[i]);

			model = glm::rotate(model, rotateXObjects[i], glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotateYObjects[i], glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, rotateZObjects[i], glm::vec3(0.0f, 0.0f, 1.0f));

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureID);

			// Chamada de desenho - drawcall
			// Poligono Preenchido - GL_TRIANGLES (12 TRIANGLES * 3 PONTOS)
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, qtdVertices);
		}

		moveUp = moveDown = moveLeft = moveRight = false;

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}

	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();

	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_KP_8)
	{
		rotateUp = pressRelease(action);
	}

	if (key == GLFW_KEY_KP_2)
	{
		rotateDown = pressRelease(action);
	}

	if (key == GLFW_KEY_KP_4)
	{
		rotateLeft = pressRelease(action);
	}

	if (key == GLFW_KEY_KP_6)
	{
		rotateRight = pressRelease(action);
	}

	if (key == GLFW_KEY_KP_7)
	{
		rotateAngleLeft = pressRelease(action);
	}

	if (key == GLFW_KEY_KP_9)
	{
		rotateAngleRight = pressRelease(action);
	}

	if (key == GLFW_KEY_W)
	{
		moveUp = pressRelease(action);
	}

	if (key == GLFW_KEY_S)
	{
		moveDown = pressRelease(action);
	}

	if (key == GLFW_KEY_A)
	{
		moveLeft = pressRelease(action);
	}

	if (key == GLFW_KEY_D)
	{
		moveRight = pressRelease(action);
	}

	if (key == GLFW_KEY_F)
	{
		moveFront = pressRelease(action);
	}

	if (key == GLFW_KEY_B)
	{
		moveBack = pressRelease(action);
	}

	if (key == GLFW_KEY_KP_ADD)
	{
		bigger = pressRelease(action);
	}

	if (key == GLFW_KEY_KP_SUBTRACT)
	{
		smaller = pressRelease(action);
	}
}

bool pressRelease(int action)
{
	if (action == GLFW_PRESS) return true;
	else if (action == GLFW_RELEASE) return false;
}

// Esta fun��o est� bastante harcoded - objetivo � criar os buffers que armazenam a 
// geometria de um tri�ngulo
// Apenas atributo coordenada nos v�rtices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A fun��o retorna o identificador do VAO
int setupGeometry()
{
	std::string filename = diretorioModelos;
	std::string diretorioMtl;

	filename += "CuboTextured.obj";
	// filename += "bola.obj";
	// filename += "suzanneTri.obj";

	std::vector<std::vector<int>> models;
	std::vector<std::vector<GLfloat>> vectors;
	std::vector<std::vector<GLfloat>> normalVectors;
	std::vector<std::vector<GLfloat>> textureVectors;

	std::ifstream infile(filename);
	std::string line;

	while (std::getline(infile, line))
	{
		if (line[0] == 'v')
		{
			if (line[1] == 't')
			{
				std::vector<string> tokens;
				std::istringstream tokenizer{ line };
				std::string token;

				// separa as string por espa�o e coloca no vetor destino
				while (tokenizer >> token)
				{
					tokens.push_back(token);
				}

				std::vector<GLfloat> textureLine;

				textureLine.insert(textureLine.end(), std::stof(tokens[1]));
				textureLine.insert(textureLine.end(), std::stof(tokens[2]));

				textureVectors.insert(textureVectors.end(), textureLine);
			}
			if (line[1] == 'n')
			{
				std::vector<string> tokens;
				std::istringstream tokenizer{ line };
				std::string token;

				// separa as string por espa�o e coloca no vetor destino
				while (tokenizer >> token)
				{
					tokens.push_back(token);
				}

				std::vector<GLfloat> normalLine;

				normalLine.insert(normalLine.end(), std::stof(tokens[1]));
				normalLine.insert(normalLine.end(), std::stof(tokens[2]));
				normalLine.insert(normalLine.end(), std::stof(tokens[3]));

				normalVectors.insert(normalVectors.end(), normalLine);
			}
			else if (line[1] == ' ')
			{
				std::vector<string> tokens;
				std::istringstream tokenizer{ line };
				std::string token;

				// separa as string por espa�o e coloca no vetor destino
				while (tokenizer >> token)
				{
					tokens.push_back(token);
				}

				std::vector<GLfloat> vectorLine;

				vectorLine.insert(vectorLine.end(), std::stof(tokens[1]));
				vectorLine.insert(vectorLine.end(), std::stof(tokens[2]));
				vectorLine.insert(vectorLine.end(), std::stof(tokens[3]));

				vectors.insert(vectors.end(), vectorLine);
			}
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			std::vector<string> tokens;
			std::istringstream tokenizer { line };
			std::string token;

			// separa as string por espa�o e coloca no vetor destino
			while (tokenizer >> token)
			{
				tokens.push_back(token);
			}

			for (int iToken = 1; iToken < tokens.size(); iToken++)
			{
				std::string delimiter = "/";
				std::vector<int> modelsLine;

				std::string tokenBarra = tokens[iToken].substr(0, tokens[iToken].find(delimiter));
				modelsLine.insert(modelsLine.end(), (std::stoi(tokenBarra) - 1));

				std::string aux = tokens[iToken].substr(tokens[iToken].find(delimiter) + 1, tokens[iToken].length());
				tokenBarra = aux.substr(0, aux.find(delimiter));
				modelsLine.insert(modelsLine.end(), (std::stoi(tokenBarra) - 1));

				tokenBarra = aux.substr(aux.find(delimiter) + 1, aux.length());
				modelsLine.insert(modelsLine.end(), (std::stoi(tokenBarra) - 1));

				models.insert(models.end(), modelsLine);
			}
		}
		else if ((int)line.find("mtllib") >= 0)
		{
			std::vector<string> tokens;
			std::istringstream tokenizer { line };
			std::string token;

			// separa as string por espa�o e coloca no vetor destino
			while (tokenizer >> token)
			{
				tokens.push_back(token);
			}

			diretorioMtl = diretorioModelos + tokens[1];
		}
	}

	std::ifstream infileMtl(diretorioMtl);

	while (std::getline(infileMtl, line))
	{
		if ((int)line.find("map_Kd") >= 0)
		{
			std::vector<string> tokens;
			std::istringstream tokenizer { line };
			std::string token;

			// separa as string por espa�o e coloca no vetor destino
			while (tokenizer >> token)
			{
				tokens.push_back(token);
			}

			diretorioImagemModelo = diretorioModelos + tokens[1];
		}
	}

	qtdVertices = models.size();
	std::vector<GLfloat> vertices;

	int indexVectors = 0;
	int indexTexture = 1;
	int indexNormals = 2;

	for (int i = 0; i < models.size(); i++)
	{
		vertices.insert(vertices.end(), vectors[models[i][indexVectors]][0]);
		vertices.insert(vertices.end(), vectors[models[i][indexVectors]][1]);
		vertices.insert(vertices.end(), vectors[models[i][indexVectors]][2]);
		vertices.insert(vertices.end(), normalVectors[models[i][indexNormals]][0]);
		vertices.insert(vertices.end(), normalVectors[models[i][indexNormals]][1]);
		vertices.insert(vertices.end(), normalVectors[models[i][indexNormals]][2]);
		vertices.insert(vertices.end(), textureVectors[models[i][indexTexture]][0]);
		vertices.insert(vertices.end(), textureVectors[models[i][indexTexture]][1]);
	}

	GLuint VBO, VAO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posi��o (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

GLuint carregarTextura()
{
	GLuint texID;

	// Gera o identificador da textura na mem�ria 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(diretorioImagemModelo.c_str(), &width, &height, &nrChannels, 0);

	if (data) {
		if (nrChannels == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //jpg, bmp 
		else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);			   //png
		glGenerateMipmap(GL_TEXTURE_2D);                                                                               // gera��o do mipmap
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	return texID;
}

void mostrarInstrucoes() {
	cout << "WASD para mover objeto selecionado" << endl;
	cout << "F e B para mover no eixo Z (frente <> traz) objeto selecionado" << endl;
	cout << "2,4,6,7,8,9 do teclado numerico para rotacionar o objeto selecionado" << endl;
	cout << "'+' e '-' do teclado numerico para aumentar e dimuir objeto" << endl;
}