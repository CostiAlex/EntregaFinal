#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

void configScene();
void renderScene();
void setLights(glm::mat4 P, glm::mat4 V);
void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);

void drawObject(Model model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawFarola(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawFarolas(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCamino(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawArboles(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCar(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSkyBox(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCrystalBall(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods);
void funSetScroll(GLFWwindow* window, double xoffset, double yoffset);
void funSetCursorPos(GLFWwindow* window, double xpos, double ypos);
void funMouseButton(GLFWwindow* _window, int button, int action, int mods);

void movimiento();

    // Shaders
    Shaders shaders;

    // Modelos
    Model plane;
    Model triangle;
    Model cone;
    Model cylinder;
    Model sphere;
    Model torus;
    Model tree;
    Model car;
    Model tree2;
    Model tree3;


// Imagenes (texturas)
    Texture imgNoEmissive;
    Texture imgStar;
    Texture imgHex;
    Texture imgMarbel;
    Texture imgMoon;
    Texture imgEmerald;
    Texture imgBlue;
    Texture imgTree;
    Texture imgCamino;
    Texture imgCamino_vol;
    Texture imgGrass;
    Texture imgGris;
    Texture imgAsphalt;
    Texture imgAsphaltN;
    Texture imgSkyBox1;
    Texture imgSkyBox2;
    Texture imgSkyBox3;
    Texture imgSkyBox4;
    Texture imgSkyBox5;
    Texture imgSkyBox6;
    Texture imgHerb;

    // Luces y Materiales
    #define   NLD 1
    #define   NLP 1
    #define   NLF 4
    Light     lightG;
    Light     lightD[NLD];
    Light     lightP[NLP];
    Light     lightF[NLF];
    Material  mluz;
    Material  ruby;
    Material  ruby2;
    Material  gold;
    Material  bronze;
    Material  cyan;
    Material  emerald;

    Textures  texSuelo;
    Textures  texLight;
    Textures  texTriangle;
    Textures  texSphere;
    Textures  texTree;
    Textures  texCamino;
    Textures  texGrass;
    Textures  texFarola;
    Textures  texRoad;
    Textures  texHerb;
    Textures  texSkyBox1;
    Textures  texSkyBox2;
    Textures  texSkyBox3;
    Textures  texSkyBox4;
    Textures  texSkyBox5;
    Textures  texSkyBox6;


    // Viewport
    int w = 750;
    int h = 750;

    // Animaciones
    float desZ = 0.0;
    float desX = 0.0;
    float rotY = 180.0;
    float rot_farola = 0.0;
    float rot_light = 0.0;
    float rot_count;
    float rot_count2;
    float vFovy = 50;
    float mov_l1 = 0.0;
    float mov_l2 = 0.0;

    float desFarola1 = -3.6;
    float desFarola2 = -2.4;
    float desFarola3 = -1.2;
    float desFarola4 = 0;
    float desFarola5 = 1.2;
    float desFarola6 = 2.4;

    float desArbol1 = -2.4;
    float desArbol2 = 2.5;
    float desArbol3 = -2.3;
    float desArbol4 = 3.1;
    float desArbol5 = -2.7;
    float desArbol6 = 2.7;

    float desArbolZ1 = -2.4;
    float desArbolZ2 = 0;
    float desArbolZ3 = 2.4;
    float desArbolZ4 = -3.6;
    float desArbolZ5 = -1.2;
    float desArbolZ6 = 1.2;

    float desplL1 = 2.4;
    float desplL2 = 0.8;
    float desplL3 = -0.8;
    float desplL4 = -2.4;

    float desplGrass1 = -2.4;
    float desplGrass2 =    0;
    float desplGrass3 = -1.2;
    float desplGrass4 =  1.2;

    float desplLuz1 = -3.4;
    float desplLuz2 = -1.4;

    bool pausa;
    bool mouseButtonClic = false;
    double cXpos, cYpos;
    bool ret = false;

    glm::vec3 aux_pos1 = glm::vec3(0.0,  3.0,  -2.0);
    glm::vec3 aux_pos2 = glm::vec3(0.0,  3.0,  2.0);


//    float alphaY = 25;
//    float alphaX = 60;

    float alphaY = 25;
    float alphaX = 0;

    float x = 0.0;
    float y = 0.0;
    float z = 10.0;

    glm::vec3 aux_posLP = glm::vec3(-1.5, 0.2, 0.0);

int main() {

    // Inicializamos GLFW
    if(!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Creamos la ventana
    GLFWwindow* window;
    window = glfwCreateWindow(w, h, "PracticaFinal", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

    // Configuramos los CallBacks
    glfwSetFramebufferSizeCallback(window, funFramebufferSize);
    glfwSetKeyCallback(window, funKey);
    glfwSetScrollCallback(window, funSetScroll);
    glfwSetCursorPosCallback(window, funSetCursorPos);
    glfwSetMouseButtonCallback(window, funMouseButton);

    // Entramos en el bucle de renderizado
    configScene();
    while(!glfwWindowShouldClose(window) && !ret) {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(!pausa && glfwGetTime() > 0.01f){
            movimiento();
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void configScene() {

    // Test de profundidad
    glEnable(GL_DEPTH_TEST);
    glPolygonOffset(1.0,1.0);

    // Transparencias
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

    // Modelos
    plane.initModel("resources/models/plane.obj");
    triangle.initModel("resources/models/triangle.obj");
    cone.initModel("resources/models/cone.obj");
    cylinder.initModel("resources/models/cylinder.obj");
    sphere.initModel("resources/models/sphere.obj");
    torus.initModel("resources/models/torus.obj");
    tree.initModel("resources/models/Tree.obj");
    car.initModel("resources/models/car.obj");
    tree2.initModel("resources/models/Lowpoly_tree_sample.obj");
    tree3.initModel("resources/models/Tree3.obj");



    // Imagenes (texturas)
    imgNoEmissive.initTexture("resources/textures/img1.png");
    imgStar.initTexture("resources/textures/img2.png");
    imgHex.initTexture("resources/textures/img3.png");
    imgMarbel.initTexture("resources/textures/img4.png");
    imgMoon.initTexture("resources/textures/img5.png");
    imgEmerald.initTexture("resources/textures/img6.png");
    imgBlue.initTexture("resources/textures/img7.png");
    imgTree.initTexture("resources/textures/bark_0021.jpg");
    imgCamino.initTexture("resources/textures/store.png");
    imgCamino_vol.initTexture("resources/textures/storeNM.png");
    imgGrass.initTexture("resources/textures/grass.jpg");
    imgGris.initTexture("resources/textures/grey-concrete-texture.jpg");

    imgAsphalt.initTexture("resources/textures/Asphalt_002_COLOR.jpg");
    imgAsphaltN.initTexture("resources/textures/Asphalt_002_NORM.jpg");
    imgHerb.initTexture("resources/textures/Stylized_Grass_003_basecolor.jpg");

    imgSkyBox1.initTexture("resources/textures/skyBox/1.jpg");
    imgSkyBox2.initTexture("resources/textures/skyBox/2.jpg");
    imgSkyBox3.initTexture("resources/textures/skyBox/3.jpg");
    imgSkyBox4.initTexture("resources/textures/skyBox/4.jpg");
    imgSkyBox5.initTexture("resources/textures/skyBox/5.jpg");
    imgSkyBox6.initTexture("resources/textures/skyBox/6.jpg");

    pausa = false;
    glfwSetTime(0.0f);
    rot_count = 0;

    // Luz ambiental global
    lightG.ambient = glm::vec3(0.5, 0.5, 0.5);

    // Luces direccionales
    lightD[0].direction = glm::vec3(0.0, -1.0, 0.0);
    lightD[0].ambient   = glm::vec3( 0.1, 0.1, 0.1);
    lightD[0].diffuse   = glm::vec3( 0.7, 0.7, 0.7);
    lightD[0].specular  = glm::vec3( 0.2, 0.2, 0.2);

    // Luces posicionales
    lightP[0].position    = glm::vec3(-1.5, 0.2, 0.0);
    lightP[0].ambient     = glm::vec3(0.2, 0.2, 0.2);
    lightP[0].diffuse     = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].specular    = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].c0          = 1.00;
    lightP[0].c1          = 0.22;
    lightP[0].c2          = 0.20;

    // Luces focales
    lightF[0].position    = glm::vec3(0.0,  3.0,  -2.0);
    lightF[0].direction   = glm::vec3( 0.0, -1.0, 0.0);
    lightF[0].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[0].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].innerCutOff = 10.0;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 4.0;
    lightF[0].c0          = 1.000;
    lightF[0].c1          = 0.090;
    lightF[0].c2          = 0.032;

    lightF[1].position    = glm::vec3(0.0,  3.0,  2.0);
    lightF[1].direction   = glm::vec3( 0.0, -1.0, 0.0);
    lightF[1].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[1].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[1].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[1].innerCutOff = 10.0;
    lightF[1].outerCutOff = lightF[1].innerCutOff + 4.0;
    lightF[1].c0          = 1.000;
    lightF[1].c1          = 0.090;
    lightF[1].c2          = 0.032;

    lightF[2].position    = glm::vec3(0.0,  3.0,  -2.0);
    lightF[2].direction   = glm::vec3( 0.0, -1.0, 0.0);
    lightF[2].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[2].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[2].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[2].innerCutOff = 10.0;
    lightF[2].outerCutOff = lightF[2].innerCutOff + 4.0;
    lightF[2].c0          = 1.000;
    lightF[2].c1          = 0.090;
    lightF[2].c2          = 0.032;

    lightF[3].position    = glm::vec3(0.0,  3.0,  2.0);
    lightF[3].direction   = glm::vec3( 0.0, -1.0, 0.0);
    lightF[3].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[3].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[3].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[3].innerCutOff = 10.0;
    lightF[3].outerCutOff = lightF[3].innerCutOff + 4.0;
    lightF[3].c0          = 1.000;
    lightF[3].c1          = 0.090;
    lightF[3].c2          = 0.032;


    // Materiales
    mluz.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.emissive  = glm::vec4(1.0, 1.0, 1.0, 1.0);
    mluz.shininess = 1.0;

    ruby.ambient   = glm::vec4(0.174500, 0.011750, 0.011750, 0.55);
    ruby.diffuse   = glm::vec4(0.614240, 0.041360, 0.041360, 0.55);
    ruby.specular  = glm::vec4(0.727811, 0.626959, 0.626959, 0.55);
    ruby.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    ruby.shininess = 76.8;

    ruby2.ambient   = glm::vec4(0.174500, 0.011750, 0.011750, 1.00);
    ruby2.diffuse   = glm::vec4(0.614240, 0.041360, 0.041360, 1.00);
    ruby2.specular  = glm::vec4(0.727811, 0.626959, 0.626959, 1.00);
    ruby2.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    ruby2.shininess = 76.8;

    gold.ambient   = glm::vec4(0.247250, 0.199500, 0.074500, 1.00);
    gold.diffuse   = glm::vec4(0.751640, 0.606480, 0.226480, 1.00);
    gold.specular  = glm::vec4(0.628281, 0.555802, 0.366065, 1.00);
    gold.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    gold.shininess = 51.2;

    bronze.ambient   = glm::vec4(0.25, 0.148, 0.06475, 1.00);
    bronze.diffuse   = glm::vec4(0.4, 0.2368, 0.1036, 1.00);
    bronze.specular  = glm::vec4(0.774597, 0.458561, 0.200621, 1.00);
    bronze.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    bronze.shininess = 76.8;

    emerald.ambient   = glm::vec4(0.0215, 0.1745, 0.0215, 0.55);
    emerald.diffuse   = glm::vec4(0.07568, 0.61424, 0.07568, 0.55);
    emerald.specular  = glm::vec4(0.633, 0.727811, 0.633, 0.55);
    emerald.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    emerald.shininess = 76.8;

    cyan.ambient   = glm::vec4(0.0, 0.1, 0.06, 1.00);
    cyan.diffuse   = glm::vec4(0.0, 0.50980392, 0.50980392, 1.00);
    cyan.specular  = glm::vec4(0.50196078, 0.50196078, 0.50196078, 1.00);
    cyan.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    cyan.shininess = 32.0;

    texSuelo.diffuse   = imgMoon.getTexture();
    texSuelo.specular  = imgHex.getTexture();
    texSuelo.emissive  = imgStar.getTexture();
    texSuelo.normal    = 0;
    texSuelo.shininess = 10.0;

    texLight.diffuse   = 0;
    texLight.specular  = 0;
    texLight.emissive  = imgMarbel.getTexture();
    texLight.normal    = 0;
    texLight.shininess = 10.0;

    texSphere.diffuse   = imgEmerald.getTexture();
    texSphere.specular  = imgEmerald.getTexture();
    texSphere.emissive  = imgNoEmissive.getTexture();
    texSphere.normal    = imgBlue.getTexture();
    texSphere.shininess = 10.0;

    texTriangle.diffuse   = imgMarbel.getTexture();
    texTriangle.specular  = 0;
    texTriangle.emissive  = 0;
    texTriangle.normal    = imgMarbel.getTexture();
    texTriangle.shininess = 10.0;

    texTree.diffuse   = imgTree.getTexture();
    texTree.specular  = imgTree.getTexture();
    texTree.emissive  = imgTree.getTexture();
    texTree.normal    = 0;
    texTree.shininess = 10.0;

    texCamino.diffuse   = imgCamino.getTexture();
    texCamino.specular  = imgCamino.getTexture();
    texCamino.emissive  = imgCamino.getTexture();
    texCamino.normal    = imgCamino_vol.getTexture();
    texCamino.shininess = 10.0;

    texGrass.diffuse   = imgGrass.getTexture();
    texGrass.specular  = imgGrass.getTexture();
    texGrass.emissive  = imgGrass.getTexture();
    texGrass.normal    = 0;
    texGrass.shininess = 10.0;

    texFarola.diffuse   = imgGris.getTexture();
    texFarola.specular  = imgGris.getTexture();
    texFarola.emissive  = imgGris.getTexture();
    texFarola.normal    = 0;
    texFarola.shininess = 10.0;

    texRoad.diffuse   = imgAsphalt.getTexture();
    texRoad.specular  = imgAsphalt.getTexture();
    texRoad.emissive  = imgAsphalt.getTexture();
    texRoad.normal    = imgAsphaltN.getTexture();;
    texRoad.shininess = 10.0;

    texHerb.diffuse   = imgHerb.getTexture();
    texHerb.specular  = imgHerb.getTexture();
    texHerb.emissive  = imgHerb.getTexture();
    texHerb.normal    = imgHerb.getTexture();;
    texHerb.shininess = 10.0;

    texSkyBox1.diffuse   = imgSkyBox1.getTexture();
    texSkyBox1.specular  = imgSkyBox1.getTexture();
    texSkyBox1.emissive  = imgSkyBox1.getTexture();
    texSkyBox1.normal    = 0;
    texSkyBox1.shininess = 10.0;

    texSkyBox2.diffuse   = imgSkyBox2.getTexture();
    texSkyBox2.specular  = imgSkyBox2.getTexture();
    texSkyBox2.emissive  = imgSkyBox2.getTexture();
    texSkyBox2.normal    = 0;
    texSkyBox2.shininess = 10.0;

    texSkyBox3.diffuse   = imgSkyBox3.getTexture();
    texSkyBox3.specular  = imgSkyBox3.getTexture();
    texSkyBox3.emissive  = imgSkyBox3.getTexture();
    texSkyBox3.normal    = 0;
    texSkyBox3.shininess = 10.0;

    texSkyBox4.diffuse   = imgSkyBox4.getTexture();
    texSkyBox4.specular  = imgSkyBox4.getTexture();
    texSkyBox4.emissive  = imgSkyBox4.getTexture();
    texSkyBox4.normal    = 0;
    texSkyBox4.shininess = 10.0;

    texSkyBox5.diffuse   = imgSkyBox5.getTexture();
    texSkyBox5.specular  = imgSkyBox5.getTexture();
    texSkyBox5.emissive  = imgSkyBox5.getTexture();
    texSkyBox5.normal    = 0;
    texSkyBox5.shininess = 10.0;

    texSkyBox6.diffuse   = imgSkyBox6.getTexture();
    texSkyBox6.specular  = imgSkyBox6.getTexture();
    texSkyBox6.emissive  = imgSkyBox6.getTexture();
    texSkyBox6.normal    = 0;
    texSkyBox6.shininess = 10.0;
}

void renderScene() {

    // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Indicamos los shaders a utilizar
    shaders.useShaders();

    // Matriz P
//    float fovy   = 40.0;
    float fovy   = vFovy;
    float nplane =  0.1;
    float fplane = 50.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

    // Matriz V
    float x = 10.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = 10.0f*glm::sin(glm::radians(alphaY));
    float z = 10.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 eye   (  x,   y,   z);
    glm::vec3 center(0.0, 0.0,  0.0);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(eye, center, up);
    shaders.setVec3("ueye",eye);

    // Fijamos las luces
    setLights(P,V);

    // Dibujamos la escena
    drawSuelo(P,V,I);

    glm::mat4 T_cutre = glm::translate(I, glm::vec3(0.0,-0.01,0.0));
    glm::mat4 R_cutre = glm::rotate(I, glm::radians(180.f), glm::vec3(0,0,1));
    drawSuelo(P,V,T_cutre*R_cutre);

    drawArboles(P, V, I);

    drawCar(P, V, I);

    drawFarolas(P, V, I);

    drawCamino(P, V, I);

    drawSkyBox(P, V, I);

    drawCrystalBall(P, V, I);

}


//#TODO documentar
void drawSkyBox(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I, glm::vec3(20, 1.0, 20));

    glm::mat4 R1 = glm::rotate(I, glm::radians(180.f), glm::vec3(1,0,0));
    glm::mat4 T1 = glm::translate(I, glm::vec3(0.0, 20, 0));
    drawObjectTex(plane, texSkyBox1, P, V, M*T1*R1*S);

    glm::mat4 R2 = glm::rotate(I, glm::radians(90.f), glm::vec3(1,0,0));
    glm::mat4 R21 = glm::rotate(I, glm::radians(-90.f), glm::vec3(0,0,1));
    glm::mat4 T2 = glm::translate(I, glm::vec3(0.0, 0.0, -20));
    drawObjectTex(plane, texSkyBox2, P, V, M*T2*R21*R2*S);

    glm::mat4 R3 = glm::rotate(I, glm::radians(90.f), glm::vec3(0,0,1));
    glm::mat4 T3 = glm::translate(I, glm::vec3(20.0, 0.0, 0.0));
    glm::mat4 R31 = glm::rotate(I, glm::radians(180.f), glm::vec3(1,0,0));
    drawObjectTex(plane, texSkyBox3, P, V, M*T3*R31*R3*S);

    glm::mat4 R4 = glm::rotate(I, glm::radians(90.f), glm::vec3(1,0,0));
    glm::mat4 T4 = glm::translate(I, glm::vec3(0.0, 0.0, 20.0));
    glm::mat4 R41 = glm::rotate(I, glm::radians(-90.f), glm::vec3(0,0,1));
    glm::mat4 R42 = glm::rotate(I, glm::radians(180.f), glm::vec3(0,1,0));
    drawObjectTex(plane, texSkyBox4, P, V, M*T4*R42*R41*R4*S);

    glm::mat4 T5 = glm::translate(I, glm::vec3(-20.0, 0.0, 0.0));
    glm::mat4 R5 = glm::rotate(I, glm::radians(180.f), glm::vec3(0,0,1));
    drawObjectTex(plane, texSkyBox5, P, V, M*T5*R3*R5*S);

    glm::mat4 R6 = glm::rotate(I, glm::radians(180.f), glm::vec3(0,1,0));
    glm::mat4 T6 = glm::translate(I, glm::vec3(0.0, -20, 0));
    drawObjectTex(plane, texSkyBox6, P, V, M*T6*R6*S);
}

//#TODO documentar
void drawCrystalBall(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    // Dibujar esfera transparente
    glDepthMask(GL_FALSE);
    glm::mat4 Ss = glm::scale(I, glm::vec3(3.0, 3.0, 3.0));
    drawObjectMat(sphere, ruby, P, V, M*Ss);
    glDepthMask(GL_TRUE);

    // Dibujar la base de la bola de cristal
    glm::mat4 St = glm::scale(I, glm::vec3(4.5, 4.5, 4.5));
    glm::mat4 Tt = glm::translate(I, glm::vec3(0.0, -5.0, 0.0));
    drawObjectTex(torus, texFarola, P, V, M*Tt*St);

    glm::mat4 Sc = glm::scale(I, glm::vec3(4.5, 0.5, 4.5));
    glm::mat4 Tc = glm::translate(I, glm::vec3(0.0, -5.0, 0.0));
    drawObjectTex(cylinder, texFarola, P, V, M*Tc*Sc);
}


/**
 * Función que añade las luces definidas previamente a la escena.
 * @param P
 * @param V
 */
void setLights(glm::mat4 P, glm::mat4 V) {

    shaders.setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

    for(int i=0; i<NLP; i++) {
        glm::mat4 R = glm::rotate(I, glm::radians(rot_light), glm::vec3(0,1,0));
        lightP[i].position = glm::vec3(R*glm::vec4(1.0))*glm::vec3(-1.0,1.0,-1.0);
        glm::mat4 M = glm::translate(I,aux_posLP) * glm::scale(I,glm::vec3(0.05*0.5));
        drawObjectTex(sphere, texLight, P, V, R*M);
    }

    glm::mat4 T1 = glm::translate(I, glm::vec3(0.0,0.0,mov_l1));
    glm::mat4 M1 = glm::translate(I,aux_pos1) * glm::scale(I,glm::vec3(0.05*0.5));
    drawObjectTex(sphere, texLight, P, V, T1*M1);

    glm::mat4 T2 = glm::translate(I, glm::vec3(0.0,0.0,mov_l2));
//    glm::mat4 M2 = glm::translate(I,lightF[1].position) * glm::scale(I,glm::vec3(0.05*0.5));
    glm::mat4 M2 = glm::translate(I,aux_pos2) * glm::scale(I,glm::vec3(0.05*0.5));
    drawObjectTex(sphere, texLight, P, V, T2*M2);

}

/**
 * Función que dibuja un objeto, indicado por parámetro, en la escena aplicándole un material.
 * @param model
 * @param material
 * @param P
 * @param V
 * @param M
 */
void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",true);
    shaders.setMaterial("umaterial",material);
    model.renderModel(GL_FILL);
}

/**
 * Función que dibuja un objeto, indicado por parámetro, en la escena aplicándole una textura.
 * @param model
 * @param textures
 * @param P
 * @param V
 * @param M
 */
void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",false);
    shaders.setTextures("utextures",textures);
    if(textures.normal!=0) shaders.setBool("uWithNormals",true);
    else                   shaders.setBool("uWithNormals",false);
    model.renderModel(GL_FILL);
}

/**
 * Función que dibuja el coche en la escena, aplicando el material que corresponda e
 * implementando la lógica de movimiento del coche.
 * @param P
 * @param V
 * @param M
 */
void drawCar(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S_car = glm::scale(I, glm::vec3(0.03, 0.03, 0.03));
    glm::mat4 T_car = glm::translate(I, glm::vec3(desX,0.0,desZ));
    glm::mat4 R = glm::rotate(I, glm::radians(rotY), glm::vec3(0,1,0));
    drawObjectMat(car, gold, P, V, T_car*S_car*R);
}

/**
 * Función que dibuja los árboles en la escena, aplicando las texturas y materiales
 * adecuados además de situar cada árbol en la posición que le corresponda.
 * @param P
 * @param V
 * @param M
 */
void drawArboles(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    glm::mat4 S_tree = glm::scale(I, glm::vec3(0.4, 0.4, 0.4));
    glm::mat4 T_tree1 = glm::translate(I, glm::vec3(desArbol1,0.0,desArbolZ1));
    glm::mat4 T_tree2 = glm::translate(I, glm::vec3(desArbol2,0.0,desArbolZ2));
    glm::mat4 T_tree3 = glm::translate(I, glm::vec3(desArbol3,0.0,desArbolZ3));
    drawObjectTex(tree, texTree, P, V, T_tree1*S_tree);
    drawObjectTex(tree, texTree, P, V, T_tree2*S_tree);
    drawObjectTex(tree, texTree, P, V, T_tree3*S_tree);

    glm::mat4 S_tree3 = glm::scale(I, glm::vec3(0.2, 0.2, 0.2));
    glm::mat4 T_tree4 = glm::translate(I, glm::vec3(desArbol4,0.0,desArbolZ4));
    glm::mat4 T_tree5 = glm::translate(I, glm::vec3(desArbol5,0.0,desArbolZ5));
    glm::mat4 T_tree6 = glm::translate(I, glm::vec3(desArbol6,0.0,desArbolZ6));
    drawObjectMat(tree3, cyan, P, V, T_tree4*S_tree3);
    drawObjectMat(tree3, cyan, P, V, T_tree5*S_tree3);
    drawObjectMat(tree3, cyan, P, V, T_tree6*S_tree3);
}

/**
 * Función que encapsula la representación de las farolas en el plano. Calcula la posición
 * de cada farola e implementa la lógica para rotar las farolas.
 * @param P
 * @param V
 * @param M
 */
void drawFarolas(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 R = glm::rotate(I, glm::radians(rot_farola), glm::vec3(0,1,0));

    glm::mat4 T1 = glm::translate(I, glm::vec3(-1.0,0.0,desFarola1));
    glm::mat4 T2 = glm::translate(I, glm::vec3(1.0,0.0, desFarola2));
    glm::mat4 T3 = glm::translate(I, glm::vec3(-1.0,0.0,desFarola3));
    glm::mat4 T4 = glm::translate(I, glm::vec3(1.0,0.0, desFarola4));
    glm::mat4 T5 = glm::translate(I, glm::vec3(-1.0,0.0,desFarola5));
    glm::mat4 T6 = glm::translate(I, glm::vec3(1.0,0.0, desFarola6));

    drawFarola(P,V,T1*M*R);
    drawFarola(P,V,T2*M*R);
    drawFarola(P,V,T3*M*R);
    drawFarola(P,V,T4*M*R);
    drawFarola(P,V,T5*M*R);
    drawFarola(P,V,T6*M*R);
}

/**
 * Función que dibuja las farolas en la escena, incluyendo las dos esferas
 * a las que se aplica el material emisivo para simular luz.
 * @param P
 * @param V
 * @param M
 */
void drawFarola(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    // Dibujar pie farola
    glm::mat4 S = glm::scale(I, glm::vec3(0.04,0.4,0.04));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0,0.4,0.0));
    drawObjectTex(cylinder, texFarola, P, V, M*T*S);

    // Dibujar brazo farola
    glm::mat4 S1 = glm::scale(I, glm::vec3(0.03,0.2,0.03));
    glm::mat4 T1 = glm::translate(I, glm::vec3(0.0,0.8,0.0));
    glm::mat4 R1 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0,0,1));
    glm::mat4 R2 = glm::rotate(I, glm::radians(0.0f), glm::vec3(0,1,0));

    drawObjectTex(cylinder, texFarola, P, V, M*R2*T1*R1*S1);

    // Dibujar luz farola
    glm::mat4 S_sp = glm::scale(I, glm::vec3(0.04,0.04,0.04));
    glm::mat4 T_sp1 = glm::translate(I, glm::vec3(0.2,0.8,0.0));
    glm::mat4 T_sp2 = glm::translate(I, glm::vec3(-0.2,0.8,0.0));

    drawObjectMat(sphere, mluz, P, V, M*T_sp1*S_sp);
    drawObjectMat(sphere, mluz, P, V, M*T_sp2*S_sp);
}

/**
 * Función que dibuja un plano encima del plano general, al cual aplicaremos una textura
 * específica para simular un camino de piedras.
 * @param P
 * @param V
 * @param M
 */
void drawCamino(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    glm::mat4 S = glm::scale(I, glm::vec3(1.0,1.0,3.6));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0,0.01,0.0));
    drawObjectTex(plane, texRoad, P, V, M * T * S);

    glm::mat4 SL = glm::scale(I, glm::vec3(0.1,1.0,0.4));

    glm::mat4 TL1 = glm::translate(I, glm::vec3(0.0,0.015,desplL1));
    drawObjectTex(plane, texLight, P, V, M * TL1 * SL);

    glm::mat4 TL2 = glm::translate(I, glm::vec3(0.0,0.015,desplL2));
    drawObjectTex(plane, texLight, P, V, M * TL2 * SL);

    glm::mat4 TL3 = glm::translate(I, glm::vec3(0.0,0.015,desplL3));
    drawObjectTex(plane, texLight, P, V, M * TL3 * SL);

    glm::mat4 TL4 = glm::translate(I, glm::vec3(0.0,0.015,desplL4));
    drawObjectTex(plane, texLight, P, V, M * TL4 * SL);
}

/**
 * Función que dibuja el plano de nuestra escena, sobre el que irán colocados
 * el resto de elementos.
 * @param P
 * @param V
 * @param M
 */
void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Sg = glm::scale(I, glm::vec3(3.6, 1.0, 3.6));
    glm::mat4 Tg = glm::translate(I, glm::vec3(0.0,0.0,0));
    //drawObjectTex(plane, texGrass, P, V, M*S);
    drawObjectTex(plane, texHerb, P, V, M*Tg*Sg);


    glm::mat4 S = glm::scale(I, glm::vec3(3.6, 1.0, 1.2));
    glm::mat4 TS1 = glm::translate(I, glm::vec3(0.0,0.008,-2.4));
    glm::mat4 TS2 = glm::translate(I, glm::vec3(0.0,0.008,   0));
    glm::mat4 TS3 = glm::translate(I, glm::vec3(0.0,0.008, 2.4));

    drawObjectTex(plane, texHerb, P, V, M*TS1*S);
    drawObjectTex(plane, texHerb, P, V, M*TS2*S);
    drawObjectTex(plane, texHerb, P, V, M*TS3*S);


    glm::mat4 S1 = glm::scale(I, glm::vec3(1.8, 1.0, 1.2));

    glm::mat4 T1 = glm::translate(I, glm::vec3(1.8,0.009,desplGrass1));
    drawObjectTex(plane, texHerb, P, V, M*T1*S1);

    glm::mat4 T2 = glm::translate(I, glm::vec3(1.8,0.009,desplGrass2));
    drawObjectTex(plane, texHerb, P, V, M*T2*S1);

    glm::mat4 T3 = glm::translate(I, glm::vec3(-1.8,0.009,desplGrass3));
    drawObjectTex(plane, texHerb, P, V, M*T3*S1);

    glm::mat4 T4 = glm::translate(I, glm::vec3(-1.8,0.009,desplGrass4));
    drawObjectTex(plane, texHerb, P, V, M*T4*S1);
}

/**
 * Función que dibuja uncilindro, aplicándole el material correspondiente.
 * @param P
 * @param V
 * @param M
 * @param mat
 */
void drawCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M, Material mat){

    drawObjectMat(cylinder, mat, P, V, M);
}

/**
 * Función callback que calcula el tamaño de la ventana cada vez que esta es
 * redimensionada.
 * @param window
 * @param width
 * @param height
 */
void funFramebufferSize(GLFWwindow* window, int width, int height) {

    // Configuracion del Viewport
    glViewport(0, 0, width, height);

    // Actualizacion de w y h
    w = width;
    h = height;
}

/**
 * Función callback que implementa la interacción con la escena mediante teclado.
 * @param window
 * @param key
 * @param scancode
 * @param action
 * @param mods
 */
void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods) {

    switch(key) {
        /// Movimiento eje X
        case GLFW_KEY_X:
            if (mods == GLFW_MOD_SHIFT){
                if(desX <= -3.5){desX -= 0.0;} else {desX -= 0.1;}
            } else {
                if(desX >= 3.5){desX += 0.0;} else {desX += 0.1;}
            }
            break;

        /// Movimiento eje Z
        case GLFW_KEY_Z:
            if (mods == GLFW_MOD_SHIFT){
                if(desZ <= -3.5){desZ -= 0.0;} else {desZ -= 0.1;}
            } else {
                if(desZ >= 3.5){desZ += 0.0;} else {desZ += 0.1;}
            }
            break;

        /// Rotacion en Y
        case GLFW_KEY_R: if (mods == GLFW_MOD_SHIFT){rotY -= 5;} else {rotY += 5;} break;

        /// Pausa helices
        case GLFW_KEY_P:
            if (action == GLFW_PRESS && mods != GLFW_MOD_SHIFT)       {pausa = (pausa)? false:true;}
            if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK) {rot_light += 5.0;}
            break;

        /// Plegado patas
        case GLFW_KEY_UP:
            if(mov_l1 >= 5.0){
                mov_l1 += 0.0;
            } else {
                mov_l1 += 0.1;
                lightF[0].position[2] = lightF[0].position[2] + mov_l1/20;
            }
            if(mov_l2 <= -5.0){
                mov_l2 -= 0.0;
            } else {
                mov_l2 -= 0.1;
                lightF[1].position[2] = lightF[1].position[2] + mov_l2/20;
            }
            break;
        case GLFW_KEY_DOWN:
            if(mov_l1 <= -1.0){
                mov_l1 -= 0.0;
            } else {
                mov_l1 -= 0.1;
                lightF[0].position[2] = lightF[0].position[2] - mov_l1/20;
            }
            if(mov_l2 >= 1.0){
                mov_l2 += 0.0;
            } else {
                mov_l2 += 0.1;
                lightF[1].position[2] = lightF[1].position[2] - mov_l2/20;
            }
            break;

        /// Rotacion coche
        case GLFW_KEY_RIGHT: rot_farola += 5; break;
        case GLFW_KEY_LEFT: rot_farola -= 5; break;

        /// Reset escena a situación inicial
        /// Reset helicoptero a posicion inicial
        case GLFW_KEY_Q:
            desX = 0.0;
            desZ = 0.0;
            rotY = 180.0;
            rot_farola = 0.0;
            rot_light = 0.0;
            mov_l1 = 0.0;
            lightF[0].position[2] = -2.0;
            mov_l2 = 0.0;
            lightF[1].position[2] = 2.0;
            desArbol1 = -2.4;
            desArbol2 = 2.5;
            desArbol3 = -2.3;
            desArbol4 = 3.1;
            desArbol5 = -2.7;
            desArbol6 = 2.7;
            desArbolZ1 = -2.4;
            desArbolZ2 = 0;
            desArbolZ3 = 2.4;
            desArbolZ4 = -3.6;
            desArbolZ5 = -1.2;
            desArbolZ6 = 1.2;
            desplLuz1 = -3.4;
            desplLuz2 = -1.4;
            lightD->ambient = glm::vec3(0.1,0.1,0.1);
            break;

        /// Cerrar ventana
        case GLFW_KEY_ESCAPE: ret = true; break;

        /// Intensidad luz direccional
        case GLFW_KEY_D:
            if(mods == GLFW_MOD_SHIFT) {
                lightD->ambient += glm::vec3(0.1,0.1,0.1);
            }else{
                lightD->ambient -= glm::vec3(0.1,0.1,0.1);
            }
            break;

        /// On/Off luz focal
        case GLFW_KEY_F:
            if( action == GLFW_PRESS){
                if(lightF[0].c0 == 1.0){
                    lightF[0].c0 = 0.0;
                    lightF[1].c0 = 0.0;
                    lightF[2].c0 = 0.0;
                    lightF[3].c0 = 0.0;
                } else {
                    lightF[0].c0 = 1.000;
                    lightF[1].c0 = 1.000;
                    lightF[2].c0 = 1.000;
                    lightF[3].c0 = 1.000;
                }
            }
            break;

        default: break;
    }
}

/**
 * Función callback que regula la interacción de la rueda del ratón con la escena.
 * @param window
 * @param xoffset
 * @param yoffset
 */
void funSetScroll(GLFWwindow* window, double xoffset, double yoffset) {

    float aux = vFovy;
    if (yoffset > 0)
        aux += 5;
    else
        aux -= 5;
    if (aux > 90)
        aux = 90;
    else if (aux < 10)
        aux = 10;
    vFovy = aux;
}

void funSetCursorPos(GLFWwindow* window, double xpos, double ypos){

    if (mouseButtonClic) {
        alphaX = (cXpos - xpos)/ 2;
        float aux = (cYpos - ypos) / 2;
        if (aux > 89)
            aux = 89;
        else if (aux < -89)
            aux = -89;
        alphaY = - aux;

        x = 10.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
        y = 10.0f*glm::sin(glm::radians(alphaY));
        z = 10.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    }
}

void funMouseButton(GLFWwindow* _window, int button, int action, int mods) {

    /// Activado y desactivado en funcion de clic
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mouseButtonClic = true;
            glfwGetCursorPos(_window, &cXpos, &cYpos);
        } else if (action == GLFW_RELEASE) {
            mouseButtonClic = false;
        }
    }
}

void movimiento() {
    float varianza = 0.1;

    desFarola1 += varianza;
    desFarola2 += varianza;
    desFarola3 += varianza;
    desFarola4 += varianza;
    desFarola5 += varianza;
    desFarola6 += varianza;

    desArbolZ1 += varianza;
    desArbolZ2 += varianza;
    desArbolZ3 += varianza;
    desArbolZ4 += varianza;
    desArbolZ5 += varianza;
    desArbolZ6 += varianza;

    if (desFarola1 > 3.6)
        desFarola1 = -3.6;
    else if (desFarola2 > 3.6)
        desFarola2 = -3.6;
    else if (desFarola3 > 3.6)
        desFarola3 = -3.6;
    else if(desFarola4 > 3.6)
        desFarola4 = -3.6;
    else if(desFarola5 > 3.6)
        desFarola5 = -3.6;
    else if(desFarola6 > 3.6)
        desFarola6 = -3.6;

    desplL1 += varianza;
    desplL2 += varianza;
    desplL3 += varianza;
    desplL4 += varianza;

    if(desplL1 > 3.2)
        desplL1 = -3.2;
    else if(desplL2 > 3.2)
        desplL2 = -3.2;
    else if(desplL3 > 3.2)
        desplL3 = -3.2;
    else if(desplL4 > 3.2)
        desplL4 = -3.2;


    desplLuz1 += varianza;
    desplLuz2 += varianza;

    lightF[2].position[2] = lightF[2].position[2] + desplLuz1/20;
    lightF[3].position[2] = lightF[3].position[2] + desplLuz2/20;


    if(desplLuz1 > 3.6){
        desplLuz1 = -3.6;
    }
    if(desplLuz2 > 3.6) {
        desplLuz2 = -3.6;
    }

    if (desArbolZ1 > 3.6) {
        desArbolZ1 = -3.6;
        desArbol1 = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (-2.9)));
    } else if (desArbolZ2 > 3.6) {
        desArbolZ2 = -3.6;
        desArbol2 = 1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.3 - 1.0)));
    } else if (desArbolZ3 > 3.6){
        desArbolZ3 = -3.6;
        desArbol3 = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (-2.9)));
    } else if(desArbolZ4 > 3.6){
        desArbolZ4 = -3.6;
        desArbol4 = 1.0 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(3.3-1.0)));
    } else if(desArbolZ5 > 3.6){
        desArbolZ5 = -3.6;
        desArbol5 = -1.0 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(-2.9)));
    } else if(desArbolZ6 > 3.6){
        desArbolZ6 = -3.6;
        desArbol6 = 1.0 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(3.3-1.0)));
    }

    desplGrass1 += varianza;
    desplGrass2 += varianza;
    desplGrass3 += varianza;
    desplGrass4 += varianza;

    if(desplGrass1 > 2.4)
        desplGrass1 = -2.4;
    else if(desplGrass2 > 2.4)
        desplGrass2 = -2.4;
    else if(desplGrass3 > 2.4)
        desplGrass3 = -2.4;
    else if(desplGrass4 > 2.4)
        desplGrass4 = -2.4;
}