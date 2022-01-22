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

void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawFarola(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawFarolas(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCamino(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawArboles(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCar(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods);
void funSetScroll(GLFWwindow* window, double xoffset, double yoffset);
void funSetCursorPos(GLFWwindow* window, double xpos, double ypos);
void funMouseButton(GLFWwindow* _window, int button, int action, int mods);

    // Shaders
    Shaders shaders;

    // Modelos
    Model plane;
    Model cylinder;
    Model sphere;
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

    // Luces y Materiales
    #define   NLD 1
    #define   NLP 1
    #define   NLF 2
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


    // Viewport
    int w = 500;
    int h = 500;

    // Animaciones
    float desZ = 0.0;
    float desX = 0.0;
    float rotY = 0.0;
    float rot_farola = 0.0;
    float rot_light = 0.0;
    float rot_count;
    float rot_count2;
    float vFovy = 50;

    bool pausa;
    bool mouseButtonClic = false;
    bool ret = false;

    double cXpos, cYpos;

    float alphaY = 25;
    float alphaX = 60;

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
    window = glfwCreateWindow(w, h, "Practica 3", NULL, NULL);
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
            rot_count += 1.8f;
            rot_count2 += 3.6f;
            glfwSetTime(0.0f);
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
    cylinder.initModel("resources/models/cylinder.obj");
    sphere.initModel("resources/models/sphere.obj");
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
    lightF[0].position    = glm::vec3(0.0,  3.0,  -4.0);
    lightF[0].direction   = glm::vec3( 0.0, -3.0, 5.0);
    lightF[0].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[0].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].innerCutOff = 10.0;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 4.0;
    lightF[0].c0          = 1.000;
    lightF[0].c1          = 0.090;
    lightF[0].c2          = 0.032;

    lightF[1].position    = glm::vec3(0.0,  3.0,  4.0);
    lightF[1].direction   = glm::vec3( 0.0, -2.0, -4.0);
    lightF[1].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[1].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[1].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[1].innerCutOff = 10.0;
    lightF[1].outerCutOff = lightF[1].innerCutOff + 4.0;
    lightF[1].c0          = 1.000;
    lightF[1].c1          = 0.090;
    lightF[1].c2          = 0.032;

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
}

void renderScene() {

    // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Indicamos los shaders a utilizar
    shaders.useShaders();

    // Matriz P
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

    // Dibujar plano fondo
    glm::mat4 Rp = glm::rotate(I, glm::radians(90.f), glm::vec3(0,0,1));
    glm::mat4 Sp = glm::scale(I, glm::vec3(3.6, 1.0, 3.6));
    glm::mat4 Tp = glm::translate(I, glm::vec3(-3.6, 0.0, 0.0));
    glDepthMask(GL_FALSE);
    drawObjectMat(plane, emerald, P, V, Tp*Rp*Sp);
    glDepthMask(GL_TRUE);
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

    for(int i=0; i<NLF; i++) {
        glm::mat4 M = glm::translate(I,lightF[i].position) * glm::scale(I,glm::vec3(0.05*0.5));
        drawObjectTex(sphere, texLight, P, V, M);
    }
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
    glm::mat4 T_tree1 = glm::translate(I, glm::vec3(-2.6,0.0,-3.0));
    glm::mat4 T_tree2 = glm::translate(I, glm::vec3(2.5,0.0,0.0));
    glm::mat4 T_tree3 = glm::translate(I, glm::vec3(-2.3,0.0,2.4));
    drawObjectTex(tree, texTree, P, V, T_tree1*S_tree);
    drawObjectTex(tree, texTree, P, V, T_tree2*S_tree);
    drawObjectTex(tree, texTree, P, V, T_tree3*S_tree);

    glm::mat4 S_tree3 = glm::scale(I, glm::vec3(0.2, 0.2, 0.2));
    glm::mat4 T_tree4 = glm::translate(I, glm::vec3(3.1,0.0,3.5));
    glm::mat4 T_tree5 = glm::translate(I, glm::vec3(-2.7,0.0,0.0));
    glm::mat4 T_tree6 = glm::translate(I, glm::vec3(2.7,0.0,-3.1));
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

    glm::mat4 T1 = glm::translate(I, glm::vec3(-1.0,0.0,-2.0));
    glm::mat4 T2 = glm::translate(I, glm::vec3(-1.0,0.0,1.0));
    glm::mat4 T3 = glm::translate(I, glm::vec3(1.0,0.0,2.0));
    glm::mat4 T4 = glm::translate(I, glm::vec3(1.0,0.0,-1.0));
    glm::mat4 R = glm::rotate(I, glm::radians(rot_farola), glm::vec3(0,1,0));
    drawFarola(P,V,T1*M*R);
    drawFarola(P,V,T2*M*R);
    drawFarola(P,V,T3*M*R);
    drawFarola(P,V,T4*M*R);
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
    drawObjectTex(plane, texCamino, P, V, M*T*S);
}

/**
 * Función que dibuja el plano de nuestra escena, sobre el que irán colocados
 * el resto de elementos.
 * @param P
 * @param V
 * @param M
 */
void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(3.6, 1.0, 3.6));
    drawObjectTex(plane, texGrass, P, V, M*S);
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

        /// Rotacion coche
        case GLFW_KEY_RIGHT: rot_farola += 5; break;
        case GLFW_KEY_LEFT: rot_farola -= 5; break;

        /// Reset escena a situación inicial
        case GLFW_KEY_Q:
            desX = 0.0;
            desZ = 0.0;
            rotY = 0.0;
            rot_farola = 0.0;
            rot_light = 0.0;
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
                } else {
                    lightF[0].c0 = 1.000;
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
