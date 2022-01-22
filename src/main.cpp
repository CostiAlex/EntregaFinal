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
void drawAspa(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M, Material mat);
void drawCone(glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec3 color);
void drawCola(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawToroideCola(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAspasCola(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawPatas(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCabina(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHelicoptero(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods);
void funSetScroll(GLFWwindow* window, double xoffset, double yoffset);
void funSetCursorPos(GLFWwindow* window, double xpos, double ypos);
void funMouseButton(GLFWwindow* _window, int button, int action, int mods);

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
    Texture  imgTree;

    // Luces y Materiales
    #define   NLD 1
    #define   NLP 1
    #define   NLF 1
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


    // Viewport
    int w = 500;
    int h = 500;

    // Animaciones
    float desZ = 0.0;
    float desX = 0.0;
    float rotY = 0.0;
    float rot_pata = 0.0;
    float mov_pata = 0.0;
    float rot_light = 0.0;
    bool pausa;
    bool custom_aspas;
    float rot_count;
    float rot_count2;
    int num_aspas = 4;
    float vFovy = 50;

    bool mouseButtonClic = false;
    double cXpos, cYpos;
    bool ret = false;

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

    pausa = false;
    custom_aspas = false;
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
    lightF[0].position    = glm::vec3(3.0,  3.0,  4.0);
    lightF[0].direction   = glm::vec3( -3.0, -3.0, -4.0);
    lightF[0].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[0].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].innerCutOff = 10.0;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 4.0;
    lightF[0].c0          = 1.000;
    lightF[0].c1          = 0.090;
    lightF[0].c2          = 0.032;

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

    // Dibujar plano fondo

    glm::mat4 Rp = glm::rotate(I, glm::radians(90.f), glm::vec3(0,0,1));
    glm::mat4 Sp = glm::scale(I, glm::vec3(3.6, 1.0, 3.6));
    glm::mat4 Tp = glm::translate(I, glm::vec3(-3.6, 0.0, 0.0));

    glm::mat4 R = glm::rotate(I, glm::radians(90.0f+rotY), glm::vec3(0,1,0));
    glm::mat4 T = glm::translate(I, glm::vec3(desX,0.0,desZ));
    //drawHelicoptero(P,V,T*R);


    // #TODO aniadido para la practica final
    glm::mat4 S_tree = glm::scale(I, glm::vec3(0.4, 0.4, 0.4));
    glm::mat4 T_tree1 = glm::translate(I, glm::vec3(1.0,0.0,1.0));
    glm::mat4 T_tree2 = glm::translate(I, glm::vec3(-1.0,0.0,2.0));
    glm::mat4 T_tree3 = glm::translate(I, glm::vec3(-2.0,0.0,3.0));
    //drawObjectMat(tree, cyan, P, V, T_tree1*S_tree);
    //drawObjectMat(tree, cyan, P, V, T_tree2*S_tree);
    //drawObjectMat(tree, cyan, P, V, T_tree3*S_tree);
    drawObjectTex(tree, texTree, P, V, T_tree1*S_tree);
    drawObjectTex(tree, texTree, P, V, T_tree2*S_tree);
    drawObjectTex(tree, texTree, P, V, T_tree3*S_tree);
    glm::mat4 S_car = glm::scale(I, glm::vec3(0.03, 0.03, 0.03));
    glm::mat4 T_car = glm::translate(I, glm::vec3(desX,0.0,desZ));
    drawObjectMat(car, ruby, P, V, T_car*S_car);

    glm::mat4 S_tree2 = glm::scale(I, glm::vec3(0.07, 0.07, 0.07));
    drawObjectTex(tree2, texTree, P, V, S_tree2);
    //drawObjectMat(tree2, cyan, P, V, S_tree2);

    glm::mat4 S_tree3 = glm::scale(I, glm::vec3(0.2, 0.2, 0.2));
    glm::mat4 T_tree4 = glm::translate(I, glm::vec3(3.0,0.0,2.0));
    drawObjectMat(tree3, cyan, P, V, T_tree4*S_tree3);



    glDepthMask(GL_FALSE);
    drawObjectMat(plane, emerald, P, V, Tp*Rp*Sp);
    glDepthMask(GL_TRUE);
}

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

void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",true);
    shaders.setMaterial("umaterial",material);
    model.renderModel(GL_FILL);

}

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

void drawHelicoptero(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 T1 = glm::translate(I, glm::vec3(0,-0.05,0.0));
    drawPatas(P,V,M*T1);

    drawCola(P,V,M);

    glm::mat4 RH = glm::rotate(I, glm::radians(rot_count), glm::vec3(0,1,0));
    drawHelice(P,V,M*RH);

    drawCabina(P,V,M);

}

void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(3.6, 1.0, 3.6));
    drawObjectTex(plane, texSuelo, P, V, M*S);
}

void drawCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M, Material mat){

    drawObjectMat(cylinder, mat, P, V, M);
}

void drawCone(glm::mat4 P, glm::mat4 V, glm::mat4 M, Material mat){

    drawObjectMat(cone, mat, P, V, M);

}

void drawAspa(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Rz90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    glm::mat4 S    = glm::scale(I, glm::vec3(0.025,0.185,0.07));
    glm::mat4 T1   = glm::translate(I, glm::vec3(0.0,-0.6,0.0));
    glm::mat4 T2   = glm::translate(I, glm::vec3(-0.1,0.0,0.0));

    drawObjectMat(cone, ruby2, P, V, M*T2*Rz90*T1*S);

}

void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    if(custom_aspas){
        glm::mat4 T = glm::translate(I,glm::vec3(0.0,0.9,0.0));
        glm::mat4 RyNum = glm::rotate(I, glm::radians(360.0f/num_aspas), glm::vec3(0, 1, 0));
        glm::mat4 aux;
        for(int i = 1; i < num_aspas+1; i++){
            aux = RyNum;
            if( i == 1)
                drawAspa(P,V,M*T);
            else
                for(int j = 2; j < i; j++)
                    aux *= RyNum;
                drawAspa(P,V,M*aux*T);
        }
    } else {
        glm::mat4 Ry90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 1, 0));
        glm::mat4 T = glm::translate(I,glm::vec3(0.0,0.9,0.0));
        drawAspa(P,V,M*T);
        drawAspa(P,V,M*Ry90*T);
        drawAspa(P,V,M*Ry90*Ry90*T);
        drawAspa(P,V,M*Ry90*Ry90*Ry90*T);
    }

    glm::mat4 T3 = glm::translate(I,glm::vec3(0.0,0.85,0.0));
    glm::mat4 S3 = glm::scale(I, glm::vec3(0.05, 0.05, 0.05));

    drawObjectMat(cylinder, cyan, P, V, M*T3*S3);

}

void drawPatas(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    //// Rotaciones y traslado patas
    glm::mat4 R_izq = glm::rotate(I, glm::radians(rot_pata), glm::vec3(1,0,0));
    glm::mat4 R_der = glm::rotate(I, glm::radians(-rot_pata), glm::vec3(1,0,0));
    glm::mat4 T_izq = glm::translate(I, glm::vec3(0.0,0.0,mov_pata));
    glm::mat4 T_der = glm::translate(I, glm::vec3(0.0,0.0,-mov_pata));

    glm::mat4 R_izq_Suelo = glm::rotate(I, glm::radians(rot_pata), glm::vec3(1,0,0));
    glm::mat4 R_der_Suelo = glm::rotate(I, glm::radians(-rot_pata), glm::vec3(1,0,0));


    //// Cilindros apoyados en el suelo
    glm::mat4 S_FloorCylinder = glm::scale(I, glm::vec3(0.05,0.6,0.05));
    glm::mat4 R_FloorCylinder = glm::rotate   (I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    glm::mat4 T_FloorCylinder = glm::translate(I, glm::vec3(0.1,0.0,-0.4));
    glm::mat4 T_FloorCylinder_2 = glm::translate(I, glm::vec3(0.1,0.0,0.4));

    drawCylinder(P, V, M*T_izq*R_izq_Suelo*R_FloorCylinder*T_FloorCylinder*S_FloorCylinder, bronze);
    drawCylinder(P, V, M*T_der*R_der_Suelo*R_FloorCylinder*T_FloorCylinder_2*S_FloorCylinder, bronze);

    //// Cylindros patas inclinados
    glm::mat4 S_BodyCylinder_2 = glm::scale(I, glm::vec3(0.05,0.3,0.05));

    glm::mat4 R_BodyCylinder_2 = glm::rotate   (I, glm::radians(-45.0f), glm::vec3(1, 0, 0));
    glm::mat4 R_BodyCylinder_3 = glm::rotate   (I, glm::radians(45.0f), glm::vec3(1, 0, 0));

    glm::mat4 T_BodyCylinder_3 = glm::translate(I, glm::vec3(0.3,0.3,0.2));
    glm::mat4 T_BodyCylinder_4 = glm::translate(I, glm::vec3(-0.3,0.3,0.2));
    glm::mat4 T_BodyCylinder_5 = glm::translate(I, glm::vec3(0.3,0.3,-0.2));
    glm::mat4 T_BodyCylinder_6 = glm::translate(I, glm::vec3(-0.3,0.3,-0.2));

    drawCylinder(P, V, M*T_der*T_BodyCylinder_3*R_der*R_BodyCylinder_2*S_BodyCylinder_2, bronze);
    drawCylinder(P, V, M*T_der*T_BodyCylinder_4*R_der*R_BodyCylinder_2*S_BodyCylinder_2, bronze);
    drawCylinder(P, V, M*T_izq*T_BodyCylinder_5*R_izq*R_BodyCylinder_3*S_BodyCylinder_2, bronze);
    drawCylinder(P, V, M*T_izq*T_BodyCylinder_6*R_izq*R_BodyCylinder_3*S_BodyCylinder_2, bronze);
}

void drawCola(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    //// Cono amarillo de la cola
    glm::mat4 S_RearCone = glm::scale(I, glm::vec3(0.08, 0.22, 0.05));
    glm::mat4 R_RearCone = glm::rotate(I, glm::radians(-80.0f), glm::vec3(0, 0, 1));
    glm::mat4 T_RearCone = glm::translate(I, glm::vec3(0.9, 0.6, 0));

    drawCone(P, V, M * T_RearCone * R_RearCone * S_RearCone, gold);

    //// Cilindro de la cola
    glm::mat4 S_RearCyl   = glm::scale(I, glm::vec3(0.1, 0.2, 0.05));
    glm::mat4 R_RearCyl_X = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
    glm::mat4 R_RearCyl_Z = glm::rotate(I, glm::radians(10.0f), glm::vec3(0, 0, 1));
    glm::mat4 T_RearCyl   = glm::translate(I, glm::vec3(1.1, 0.62, 0.0));

    drawCylinder(P, V, M * T_RearCyl * R_RearCyl_Z * R_RearCyl_X * S_RearCyl, cyan);

    //// Toroide de la cola
    drawToroideCola(P, V, M);
}

void drawToroideCola(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    glm::mat4 S_Torus = glm::scale(I, glm::vec3(0.166,0.2,0.166));
    glm::mat4 R_Torus = glm::rotate(I, glm::radians(90.0f), glm::vec3(1,0,0));
    glm::mat4 T_Torus = glm::translate(I, glm::vec3(1.5,0.7,0.0));

    glm::mat4 RHC = glm::rotate(I, glm::radians(rot_count2), glm::vec3(0, 0, 1));
    drawAspasCola(P, V, M*T_Torus*RHC);

    drawObjectMat(torus, cyan, P, V, M*T_Torus*R_Torus*S_Torus);
}

void drawAspasCola(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    glm::mat4 S_Triangle = glm::scale(I, glm::vec3(0.015,0.10,0.0));

    glm::mat4 R_51Z  = glm::rotate(I, glm::radians(51.0f),  glm::vec3(0,0,1));
    glm::mat4 R_102Z = glm::rotate(I, glm::radians(102.0f), glm::vec3(0,0,1));
    glm::mat4 R_153Z = glm::rotate(I, glm::radians(153.0f), glm::vec3(0,0,1));
    glm::mat4 R_204Z = glm::rotate(I, glm::radians(204.0f), glm::vec3(0,0,1));
    glm::mat4 R_255Z = glm::rotate(I, glm::radians(255.0f), glm::vec3(0,0,1));
    glm::mat4 R_306Z = glm::rotate(I, glm::radians(306.0f), glm::vec3(0,0,1));

    drawObjectTex(triangle, texTriangle, P, V, M*S_Triangle);
    drawObjectTex(triangle, texTriangle, P, V, M*R_51Z*S_Triangle);
    drawObjectTex(triangle, texTriangle, P, V, M*R_102Z*S_Triangle);
    drawObjectTex(triangle, texTriangle, P, V, M*R_153Z*S_Triangle);
    drawObjectTex(triangle, texTriangle, P, V, M*R_204Z*S_Triangle);
    drawObjectTex(triangle, texTriangle, P, V, M*R_306Z*S_Triangle);
    drawObjectTex(triangle, texTriangle, P, V, M*R_255Z*S_Triangle);

}

void drawCabina(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    //// Verde
    glm::mat4 T1 = glm::translate(I,glm::vec3(0.0,0.5,0.0));
    glm::mat4 S1 = glm::scale(I, glm::vec3(0.33, 0.15, 0.2));
    drawObjectTex(sphere, texSphere, P, V, M*T1*S1);

    //// Magenta
    glm::mat4 T2 = glm::translate(I,glm::vec3(-0.5,0.6,0.0));
    glm::mat4 S2 = glm::scale(I, glm::vec3(0.12, 0.1, 0.12));
    drawObjectMat(sphere, ruby, P, V, M*T2*S2);
}

void funFramebufferSize(GLFWwindow* window, int width, int height) {

    // Configuracion del Viewport
    glViewport(0, 0, width, height);

    // Actualizacion de w y h
    w = width;
    h = height;
}

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

        /// Rotacion patas
        case GLFW_KEY_RIGHT: if(rot_pata < 45){rot_pata += 5;} break;
        case GLFW_KEY_LEFT: if(rot_pata > 0){rot_pata -= 5;} break;

        /// Plegado patas
        case GLFW_KEY_UP: if(rot_pata == 45){mov_pata += 0.05;} break;
        case GLFW_KEY_DOWN: if(rot_pata == 45){mov_pata -= 0.05;} break;

        /// Numero de aspas helice principal
        case GLFW_KEY_1: num_aspas = 1; custom_aspas = true; break;
        case GLFW_KEY_2: num_aspas = 2; custom_aspas = true; break;
        case GLFW_KEY_3: num_aspas = 3; custom_aspas = true; break;
        case GLFW_KEY_4: num_aspas = 4; custom_aspas = true; break;
        case GLFW_KEY_5: num_aspas = 5; custom_aspas = true; break;
        case GLFW_KEY_6: num_aspas = 6; custom_aspas = true; break;
        case GLFW_KEY_7: num_aspas = 7; custom_aspas = true; break;
        case GLFW_KEY_8: num_aspas = 8; custom_aspas = true; break;
        case GLFW_KEY_9: num_aspas = 9; custom_aspas = true; break;

        /// Reset helicoptero a posicion inicial
        case GLFW_KEY_Q:
            desX = 0.0;
            desZ = 0.0;
            rotY = 0.0;
            rot_pata = 0.0;
            num_aspas = 4;
            mov_pata = 0.0;
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
