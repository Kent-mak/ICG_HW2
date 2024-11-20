#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <bits/stdc++.h>

#include "./header/Object.h"
#include "./header/stb_image.h"

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
unsigned int createShader(const string &filename, const string &type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
unsigned int modelVAO(Object &model);
unsigned int loadTexture(const char *tFileName);
void init();
glm::vec3 HSVtoRGB(const glm::vec3& hsv);

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

// Shader
unsigned int vertexShader, fragmentShader, shaderProgram;

// Texture
unsigned int airplaneTexture, earthTexture;

// VAO, VBO
unsigned int airplaneVAO, earthVAO;

// Objects to display
Object *airplaneObject, *earthObject;

// Constants you may need
const int airplaneHeight = 27;
const int rotateEarthSpeed = 30;
const int rotateAirplaneSpeed = 90;
const int squeezeSpeed = 90;
const int rainbowSpeed = 72;

// You can use these parameters
float rotateEarthDegree = 0;
float rotateAirplaneDegree = 0;
int rotateAxisDegree = 180;
bool useSqueeze = false;
float squeezeFactor = 0;
bool useRainbowColor = false;
float rainbowDegree = 0;
glm::vec3 rainbowColor = glm::vec3(1.0f, 1.0f, 1.0f);

int main() {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    /* TODO#0: Change window title to "HW2 - [your student id]"
     *        Ex. HW2 - 312550000
     */

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HW2 - 111550026", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSwapInterval(1);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // TODO#1: Finish function createShader
    // TODO#2: Finish function createProgram
    // TODO#3: Finish function modelVAO
    // TODO#4: Finish function loadTexture
    // You can find the above functions right below the main function

    // Initialize Object, Shader, Texture, VAO, VBO
    init();

    // Enable depth test, face culling
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // Set viewport
    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // render loop variables
    double dt;
    double lastTime = glfwGetTime();
    double currentTime;

    /* TODO#5: Data connection - Retrieve uniform variable locations
     *    1. Retrieve locations for model, view, and projection matrices.
     *    2. Retrieve locations for squeezeFactor, rainbowColor, and other parameters.
     * Hint:
     *    glGetUniformLocation
     */
    
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    GLuint squeezeFactorLoc = glGetUniformLocation(shaderProgram, "squeezeFactor");
    GLuint rainbowColorLoc = glGetUniformLocation(shaderProgram, "rainbowColor");
    // GLuint useSqueezeLoc = glGetUniformLocation(shaderProgram, "useSqueeze");
    GLuint useRainbowLoc = glGetUniformLocation(shaderProgram, "useRainbow");
    // GLuint ourTextureLoc = glGetUniformLocation(shaderProgram, "ourTexture");


    // render loop
    while (!glfwWindowShouldClose(window)) {
        // render
        glClearColor(0 / 255.0, 0 / 255.0, 0 / 255.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 50.0f, 90.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

        glm::mat4 base(1.0f), airplaneModel(1.0f), earthModel(1.0f);

        /* TODO#6-1: Render Airplane
         *    1. Set up airplane model matrix.
         *    2. Send model, view, and projection matrices to the program.
         *    3. Send squeezeFactor, rainbowColor, or other parameters to the program.
         *    4. Apply the texture, and render the airplane.
         * Hint:
         *	  rotate, translate, scale
         *    glUniformMatrix4fv, glUniform1f, glUniform3fv
         *    glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
         */

        // // 1. Set up the airplane model matrix
        airplaneModel = glm::rotate(airplaneModel, glm::radians((float)rotateAxisDegree), glm::vec3(0.0f, 1.0f, 0.0f)); 
        airplaneModel = glm::rotate(airplaneModel, rotateAirplaneDegree, glm::vec3(-1.0f, 0.0f, 0.0f)); // Rotate over time
        airplaneModel = glm::translate(airplaneModel, glm::vec3(0.0f, airplaneHeight, 0.0f)); // Position the airplane
        
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(airplaneModel));  // Send the model matrix
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));            // Send the view matrix
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)); // Send the projection matrix
        
        glUniform1f(squeezeFactorLoc, squeezeFactor);
        glUniform1i(useRainbowLoc, useRainbowColor);
        glm::vec3 rainbowColorRGB;
        rainbowColorRGB = HSVtoRGB(rainbowColor);
        
        glUniform3fv(rainbowColorLoc, 1, &rainbowColorRGB[0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, airplaneTexture); 

        glBindVertexArray(airplaneVAO); 
        glDrawArrays(GL_TRIANGLES, 0, airplaneObject->numFace * 3); // 3 vertices per triangle
        /* TODO#6-2: Render Earth
         *    1. Set up earth model matrix.
         *    2. Send model, view, and projection matrices to the program.
         *    3. Send squeezeFactor, rainbowColor, or other parameters to the program.
         *    4. Apply the texture, and render the earth.
         * Hint:
         *	  rotate, translate, scale
         *    glUniformMatrix4fv, glUniform1f, glUniform3fv
         *    glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
         */
  
        earthModel = glm::rotate(earthModel, rotateEarthDegree, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate over time
        earthModel = glm::scale(earthModel, glm::vec3(10.0f, 10.0f, 10.0f)); 

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(earthModel));  // Send the model matrix
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));            // Send the view matrix
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)); // Send the projection matrix
        glUniform1f(squeezeFactorLoc, squeezeFactor);
        glUniform1i(useRainbowLoc, false);

        // std::cout << "texture:" << earthTexture << '\n';
        glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
        glBindTexture(GL_TEXTURE_2D, earthTexture); 

        glBindVertexArray(earthVAO);
        glDrawArrays(GL_TRIANGLES, 0, earthObject->numFace * 3);

        GLenum error = glGetError();
        error = glGetError();
        if (error == GL_INVALID_ENUM) {
            std::cout << "Error: " << error << std::endl;
        }
        
        // Status update
        currentTime = glfwGetTime();
        dt = currentTime - lastTime;
        lastTime = currentTime;

        /* TODO#7: Update "rotateEarthDegree", "rotateAirplaneDegree", "rotateAxisDegree",
         *          "squeezeFactor", "rainbowColor"
         */
        if (useSqueeze) {
            squeezeFactor += dt * glm::radians(90.0f);
            // std::cout << "squeezeFactor: " << squeezeFactor <<'\n';
        }
        
        rainbowColor.x += dt * rainbowSpeed; // Increment hue by rainbowSpeed per second
        if (rainbowColor.x > 360.0f) {
            rainbowColor.x -= 360.0f; // Wrap around when exceeding 360 degrees
        }
        rotateAirplaneDegree = glm::radians(currentTime * rotateAirplaneSpeed);
        rotateEarthDegree = glm::radians(currentTime * rotateEarthSpeed);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

glm::vec3 HSVtoRGB(const glm::vec3& hsv) {
    float H = hsv.x; // Convert normalized H to degrees
    float S = hsv.y;
    float V = hsv.z;

    float C = V * S;          // Chroma
    float X = C * (1.0f - fabs(fmod(H / 60.0f, 2.0f) - 1.0f));
    float m = V - C;

    float r, g, b;

    if (H >= 0.0f && H < 60.0f) {
        r = C; g = X; b = 0;
    } else if (H >= 60.0f && H < 120.0f) {
        r = X; g = C; b = 0;
    } else if (H >= 120.0f && H < 180.0f) {
        r = 0; g = C; b = X;
    } else if (H >= 180.0f && H < 240.0f) {
        r = 0; g = X; b = C;
    } else if (H >= 240.0f && H < 300.0f) {
        r = X; g = 0; b = C;
    } else {
        r = C; g = 0; b = X;
    }

    return glm::vec3(r + m, g + m, b + m);
}

/* TODO#1: createShader
 * input:
 *      filename: shader file name
 *      type: shader type, "vert" means vertex shader, "frag" means fragment shader
 * output: shader object
 * Hint:
 *      glCreateShader, glShaderSource, glCompileShader
 */
unsigned int createShader(const string &filename, const string &type) {
    // cout << "create shader entered: "<< filename << "\n";
    GLenum shaderType;
    if (type == "vert") {
        shaderType = GL_VERTEX_SHADER;
    } else if (type == "frag") {
        shaderType = GL_FRAGMENT_SHADER;
    } else {
        cerr << "Unknown shader type: " << type << endl;
        return 0;
    }

    ifstream shaderFile(filename);
    if (!shaderFile.is_open()) {
        cerr << "Failed to open shader file: " << filename << endl;
        return 0;
    }

    stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    string shaderCode = shaderStream.str();
    const char* shaderSource = shaderCode.c_str();

    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        cerr << "Shader compilation failed:\n" << infoLog << endl;
        return 0;
    }

    return shader;
}

/* TODO#2: createProgram
 * input:
 *      vertexShader: vertex shader object
 *      fragmentShader: fragment shader object
 * output: shader program
 * Hint:
 *      glCreateProgram, glAttachShader, glLinkProgram, glDetachShader
 */
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    unsigned int program = glCreateProgram();

    // Attach the vertex and fragment shaders
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link the program
    glLinkProgram(program);

    // Check for linking errors
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program linking failed:\n" << infoLog << std::endl;
        return 0;
    }

    // Detach the shaders after linking
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    return program;
}

/* TODO#3: modelVAO
 * input:
 *      model: Object you want to render
 * output: VAO
 * Hint:
 *      glGenVertexArrays, glBindVertexArray, glGenBuffers, glBindBuffer, glBufferData,
 *      glVertexAttribPointer, glEnableVertexAFttribArray,
 */
unsigned int modelVAO(Object &model) {
    unsigned int VAO, VBO, EBO;

    // Generate and bind the Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate a Vertex Buffer Object (VBO) for vertex positions
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(float), model.positions.data(), GL_STATIC_DRAW);

    // Configure vertex position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Check if normals are provided and create a VBO for them
    if (!model.normals.empty()) {
        unsigned int normalVBO;
        glGenBuffers(1, &normalVBO);
        glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
        glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(float), model.normals.data(), GL_STATIC_DRAW);

        // Configure normal attribute (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
    }

    // Check if texture coordinates are provided and create a VBO for them
    if (!model.texcoords.empty()) {
        unsigned int texCoordVBO;
        glGenBuffers(1, &texCoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, model.texcoords.size() * sizeof(float), model.texcoords.data(), GL_STATIC_DRAW);

        // Configure texture coordinate attribute (location = 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
    }

    // Unbind the VAO to prevent accidental modifications
    glBindVertexArray(0);

    // Optionally, unbind VBOs to clean up (not necessary if VAO is unbound)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // cout << "modelVAO:" << VAO << '\n';
    return VAO;
}

/* TODO#4: loadTexture
 * input:
 *      filename: texture file name
 * output: texture object
 * Hint:
 *      glEnable, glGenTextures, glBindTexture, glTexParameteri, glTexImage2D
 */
unsigned int loadTexture(const string &filename) {
    // cout << "loading texture ...\n";
    unsigned int textureID;
    glGenTextures(1, &textureID);
    // Load image data
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        // Determine the image format based on the number of channels
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else {
            std::cerr << "Unsupported number of channels: " << nrChannels << std::endl;
            stbi_image_free(data);
            return 0;
        }
        // Bind the texture and set texture parameters
        glBindTexture(GL_TEXTURE_2D, textureID);
        // Upload the texture data
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        // Generate mipmaps for the texture
        glGenerateMipmap(GL_TEXTURE_2D);
        // Set texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Repeat wrapping on the S axis
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Repeat wrapping on the T axis
        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Linear filtering with mipmaps
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear filtering
        // Free the image memory
        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        stbi_image_free(data);
        return 0;
    }

    return textureID;
}

/* TODO#8: Key callback
 *    1. Press 'd' to increase the "rotateAxisDegree" by 1.
 *    2. Press 'a' to decrease the "rotateAxisDegree" by 1.
 *    3. Press 's' to squeeze the earth.
 *    4. Press 'r' to make the color of the airplane rainbow.
 * Hint:
 *      GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_R
 *      GLFW_PRESS, GLFW_REPEAT
 */


void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_D:
                // std::cout << "Pressed D: " << rotateAxisDegree<<'\n';
                rotateAxisDegree += 1;
                break;

            case GLFW_KEY_A:
                // std::cout << "Pressed A: " << rotateAxisDegree<<'\n';
                rotateAxisDegree -= 1;
                break;

            case GLFW_KEY_S:
                
                if (useSqueeze) {
                    useSqueeze = false;
  
                }else {
                    useSqueeze = true;
                }
                // std::cout << "Pressed S: " << useSqueeze <<'\n';
                
                break;

            case GLFW_KEY_R:
                if (useRainbowColor) {
                    useRainbowColor = false;
                }else {
                    useRainbowColor = true;
                }
                // std::cout << "Pressed R: " << useRainbowColor <<'\n';
                
                break;

            default:
                break;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void init() {
#if defined(__linux__) || defined(__APPLE__)
    string dirShader = "../../src/shaders/";
    string dirAsset = "../../src/asset/obj/";
    string dirTexture = "../../src/asset/texture/";
#else
    string dirShader = "..\\..\\src\\shaders\\";
    string dirAsset = "..\\..\\src\\asset\\obj\\";
    string dirTexture = "..\\..\\src\\asset\\texture\\";
#endif
    // cout << "initing\n";
    // Object
    airplaneObject = new Object(dirAsset + "airplane.obj");
    earthObject = new Object(dirAsset + "earth.obj");

    // Shader
    vertexShader = createShader(dirShader + "vertexShader.vert", "vert");
    fragmentShader = createShader(dirShader + "fragmentShader.frag", "frag");
    shaderProgram = createProgram(vertexShader, fragmentShader);
    glUseProgram(shaderProgram);

    // Texture
    airplaneTexture = loadTexture(dirTexture + "airplane.jpg");
    earthTexture = loadTexture(dirTexture + "earth.jpg");

    // VAO, VBO
    airplaneVAO = modelVAO(*airplaneObject);
    earthVAO = modelVAO(*earthObject);
}
