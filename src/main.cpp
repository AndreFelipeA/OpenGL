#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Renderer.h"
#include "TileSet.h"
#include "Texture.h"
#include "Wfc.h"
#include "ScalarField.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool firstMouse = true;
float yaw   = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

static int edgeTable[256] = {
 0x0 , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
 0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
 0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
 0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
 0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
 0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
 0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
 0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
 0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
 0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
 0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
 0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
 0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
 0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
 0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
 0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
 0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
 0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
 0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
 0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
 0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
 0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
 0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
 0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
 0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
 0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
 0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
 0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
 0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
 0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
 0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
 0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0
};
 
static int triTable[256][16] = {
 {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {0,8,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {0,1,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {1,8,3,9,8,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {1,2,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {0,8,3,1,2,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {9,2,10,0,2,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {2,8,3,2,10,8,10,9,8,-1,-1,-1,-1,-1,-1,-1},
 {3,11,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {0,11,2,8,11,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {1,9,0,2,3,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {1,11,2,1,9,11,9,8,11,-1,-1,-1,-1,-1,-1,-1},
 {3,10,1,11,10,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {0,10,1,0,8,10,8,11,10,-1,-1,-1,-1,-1,-1,-1},
 {3,9,0,3,11,9,11,10,9,-1,-1,-1,-1,-1,-1,-1},
 {9,8,10,10,8,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {4,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {4,3,0,7,3,4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {0,1,9,8,4,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {4,1,9,4,7,1,7,3,1,-1,-1,-1,-1,-1,-1,-1},
 {1,2,10,8,4,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {3,4,7,3,0,4,1,2,10,-1,-1,-1,-1,-1,-1,-1},
 {9,2,10,9,0,2,8,4,7,-1,-1,-1,-1,-1,-1,-1},
 {2,10,9,2,9,7,2,7,3,7,9,4,-1,-1,-1,-1},
 {8,4,7,3,11,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {11,4,7,11,2,4,2,0,4,-1,-1,-1,-1,-1,-1,-1},
 {9,0,1,8,4,7,2,3,11,-1,-1,-1,-1,-1,-1,-1},
 {4,7,11,9,4,11,9,11,2,9,2,1,-1,-1,-1,-1},
 {3,10,1,3,11,10,7,8,4,-1,-1,-1,-1,-1,-1,-1},
 {1,11,10,1,4,11,1,0,4,7,11,4,-1,-1,-1,-1},
 {4,7,8,9,0,11,9,11,10,11,0,3,-1,-1,-1,-1},
 {4,7,11,4,11,9,9,11,10,-1,-1,-1,-1,-1,-1,-1},
 {9,5,4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {9,5,4,0,8,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {0,5,4,1,5,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {8,5,4,8,3,5,3,1,5,-1,-1,-1,-1,-1,-1,-1},
 {1,2,10,9,5,4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {3,0,8,1,2,10,4,9,5,-1,-1,-1,-1,-1,-1,-1},
 {5,2,10,5,4,2,4,0,2,-1,-1,-1,-1,-1,-1,-1},
 {2,10,5,3,2,5,3,5,4,3,4,8,-1,-1,-1,-1},
 {9,5,4,2,3,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {0,11,2,0,8,11,4,9,5,-1,-1,-1,-1,-1,-1,-1},
 {0,5,4,0,1,5,2,3,11,-1,-1,-1,-1,-1,-1,-1},
 {2,1,5,2,5,8,2,8,11,4,8,5,-1,-1,-1,-1},
 {10,3,11,10,1,3,9,5,4,-1,-1,-1,-1,-1,-1,-1},
 {4,9,5,0,8,1,8,10,1,8,11,10,-1,-1,-1,-1},
 {5,4,0,5,0,11,5,11,10,11,0,3,-1,-1,-1,-1},
 {5,4,8,5,8,10,10,8,11,-1,-1,-1,-1,-1,-1,-1},
 {9,7,8,5,7,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {9,3,0,9,5,3,5,7,3,-1,-1,-1,-1,-1,-1,-1},
 {0,7,8,0,1,7,1,5,7,-1,-1,-1,-1,-1,-1,-1},
 {1,5,3,3,5,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {9,7,8,9,5,7,10,1,2,-1,-1,-1,-1,-1,-1,-1},
 {10,1,2,9,5,0,5,3,0,5,7,3,-1,-1,-1,-1},
 {8,0,2,8,2,5,8,5,7,10,5,2,-1,-1,-1,-1},
 {2,10,5,2,5,3,3,5,7,-1,-1,-1,-1,-1,-1,-1},
 {7,9,5,7,8,9,3,11,2,-1,-1,-1,-1,-1,-1,-1},
 {9,5,7,9,7,2,9,2,0,2,7,11,-1,-1,-1,-1},
 {2,3,11,0,1,8,1,7,8,1,5,7,-1,-1,-1,-1},
 {11,2,1,11,1,7,7,1,5,-1,-1,-1,-1,-1,-1,-1},
 {9,5,8,8,5,7,10,1,3,10,3,11,-1,-1,-1,-1},
 {5,7,0,5,0,9,7,11,0,1,0,10,11,10,0,-1},
 {11,10,0,11,0,3,10,5,0,8,0,7,5,7,0,-1},
 {11,10,5,7,11,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {10,6,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {0,8,3,5,10,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {9,0,1,5,10,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {1,8,3,1,9,8,5,10,6,-1,-1,-1,-1,-1,-1,-1},
 {1,6,5,2,6,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {1,6,5,1,2,6,3,0,8,-1,-1,-1,-1,-1,-1,-1},
 {9,6,5,9,0,6,0,2,6,-1,-1,-1,-1,-1,-1,-1},
 {5,9,8,5,8,2,5,2,6,3,2,8,-1,-1,-1,-1},
 {2,3,11,10,6,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {11,0,8,11,2,0,10,6,5,-1,-1,-1,-1,-1,-1,-1},
 {0,1,9,2,3,11,5,10,6,-1,-1,-1,-1,-1,-1,-1},
 {5,10,6,1,9,2,9,11,2,9,8,11,-1,-1,-1,-1},
 {6,3,11,6,5,3,5,1,3,-1,-1,-1,-1,-1,-1,-1},
 {0,8,11,0,11,5,0,5,1,5,11,6,-1,-1,-1,-1},
 {3,11,6,0,3,6,0,6,5,0,5,9,-1,-1,-1,-1},
 {6,5,9,6,9,11,11,9,8,-1,-1,-1,-1,-1,-1,-1},
 {5,10,6,4,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {4,3,0,4,7,3,6,5,10,-1,-1,-1,-1,-1,-1,-1},
 {1,9,0,5,10,6,8,4,7,-1,-1,-1,-1,-1,-1,-1},
 {10,6,5,1,9,7,1,7,3,7,9,4,-1,-1,-1,-1},
 {6,1,2,6,5,1,4,7,8,-1,-1,-1,-1,-1,-1,-1},
 {1,2,5,5,2,6,3,0,4,3,4,7,-1,-1,-1,-1},
 {8,4,7,9,0,5,0,6,5,0,2,6,-1,-1,-1,-1},
 {7,3,9,7,9,4,3,2,9,5,9,6,2,6,9,-1},
 {3,11,2,7,8,4,10,6,5,-1,-1,-1,-1,-1,-1,-1},
 {5,10,6,4,7,2,4,2,0,2,7,11,-1,-1,-1,-1},
 {0,1,9,4,7,8,2,3,11,5,10,6,-1,-1,-1,-1},
 {9,2,1,9,11,2,9,4,11,7,11,4,5,10,6,-1},
 {8,4,7,3,11,5,3,5,1,5,11,6,-1,-1,-1,-1},
 {5,1,11,5,11,6,1,0,11,7,11,4,0,4,11,-1},
 {0,5,9,0,6,5,0,3,6,11,6,3,8,4,7,-1},
 {6,5,9,6,9,11,4,7,9,7,11,9,-1,-1,-1,-1},
 {10,4,9,6,4,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {4,10,6,4,9,10,0,8,3,-1,-1,-1,-1,-1,-1,-1},
 {10,0,1,10,6,0,6,4,0,-1,-1,-1,-1,-1,-1,-1},
 {8,3,1,8,1,6,8,6,4,6,1,10,-1,-1,-1,-1},
 {1,4,9,1,2,4,2,6,4,-1,-1,-1,-1,-1,-1,-1},
 {3,0,8,1,2,9,2,4,9,2,6,4,-1,-1,-1,-1},
 {0,2,4,4,2,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {8,3,2,8,2,4,4,2,6,-1,-1,-1,-1,-1,-1,-1},
 {10,4,9,10,6,4,11,2,3,-1,-1,-1,-1,-1,-1,-1},
 {0,8,2,2,8,11,4,9,10,4,10,6,-1,-1,-1,-1},
 {3,11,2,0,1,6,0,6,4,6,1,10,-1,-1,-1,-1},
 {6,4,1,6,1,10,4,8,1,2,1,11,8,11,1,-1},
 {9,6,4,9,3,6,9,1,3,11,6,3,-1,-1,-1,-1},
 {8,11,1,8,1,0,11,6,1,9,1,4,6,4,1,-1},
 {3,11,6,3,6,0,0,6,4,-1,-1,-1,-1,-1,-1,-1},
 {6,4,8,11,6,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {7,10,6,7,8,10,8,9,10,-1,-1,-1,-1,-1,-1,-1},
 {0,7,3,0,10,7,0,9,10,6,7,10,-1,-1,-1,-1},
 {10,6,7,1,10,7,1,7,8,1,8,0,-1,-1,-1,-1},
 {10,6,7,10,7,1,1,7,3,-1,-1,-1,-1,-1,-1,-1},
 {1,2,6,1,6,8,1,8,9,8,6,7,-1,-1,-1,-1},
 {2,6,9,2,9,1,6,7,9,0,9,3,7,3,9,-1},
 {7,8,0,7,0,6,6,0,2,-1,-1,-1,-1,-1,-1,-1},
 {7,3,2,6,7,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {2,3,11,10,6,8,10,8,9,8,6,7,-1,-1,-1,-1},
 {2,0,7,2,7,11,0,9,7,6,7,10,9,10,7,-1},
 {1,8,0,1,7,8,1,10,7,6,7,10,2,3,11,-1},
 {11,2,1,11,1,7,10,6,1,6,7,1,-1,-1,-1,-1},
 {8,9,6,8,6,7,9,1,6,11,6,3,1,3,6,-1},
 {0,9,1,11,6,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {7,8,0,7,0,6,3,11,0,11,6,0,-1,-1,-1,-1},
 {7,11,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {7,6,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {3,0,8,11,7,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {0,1,9,11,7,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {8,1,9,8,3,1,11,7,6,-1,-1,-1,-1,-1,-1,-1},
 {10,1,2,6,11,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {1,2,10,3,0,8,6,11,7,-1,-1,-1,-1,-1,-1,-1},
 {2,9,0,2,10,9,6,11,7,-1,-1,-1,-1,-1,-1,-1},
 {6,11,7,2,10,3,10,8,3,10,9,8,-1,-1,-1,-1},
 {7,2,3,6,2,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {7,0,8,7,6,0,6,2,0,-1,-1,-1,-1,-1,-1,-1},
 {2,7,6,2,3,7,0,1,9,-1,-1,-1,-1,-1,-1,-1},
 {1,6,2,1,8,6,1,9,8,8,7,6,-1,-1,-1,-1},
 {10,7,6,10,1,7,1,3,7,-1,-1,-1,-1,-1,-1,-1},
 {10,7,6,1,7,10,1,8,7,1,0,8,-1,-1,-1,-1},
 {0,3,7,0,7,10,0,10,9,6,10,7,-1,-1,-1,-1},
 {7,6,10,7,10,8,8,10,9,-1,-1,-1,-1,-1,-1,-1},
 {6,8,4,11,8,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {3,6,11,3,0,6,0,4,6,-1,-1,-1,-1,-1,-1,-1},
 {8,6,11,8,4,6,9,0,1,-1,-1,-1,-1,-1,-1,-1},
 {9,4,6,9,6,3,9,3,1,11,3,6,-1,-1,-1,-1},
 {6,8,4,6,11,8,2,10,1,-1,-1,-1,-1,-1,-1,-1},
 {1,2,10,3,0,11,0,6,11,0,4,6,-1,-1,-1,-1},
 {4,11,8,4,6,11,0,2,9,2,10,9,-1,-1,-1,-1},
 {10,9,3,10,3,2,9,4,3,11,3,6,4,6,3,-1},
 {8,2,3,8,4,2,4,6,2,-1,-1,-1,-1,-1,-1,-1},
 {0,4,2,4,6,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {1,9,0,2,3,4,2,4,6,4,3,8,-1,-1,-1,-1},
 {1,9,4,1,4,2,2,4,6,-1,-1,-1,-1,-1,-1,-1},
 {8,1,3,8,6,1,8,4,6,6,10,1,-1,-1,-1,-1},
 {10,1,0,10,0,6,6,0,4,-1,-1,-1,-1,-1,-1,-1},
 {4,6,3,4,3,8,6,10,3,0,3,9,10,9,3,-1},
 {10,9,4,6,10,4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {4,9,5,7,6,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {0,8,3,4,9,5,11,7,6,-1,-1,-1,-1,-1,-1,-1},
 {5,0,1,5,4,0,7,6,11,-1,-1,-1,-1,-1,-1,-1},
 {11,7,6,8,3,4,3,5,4,3,1,5,-1,-1,-1,-1},
 {9,5,4,10,1,2,7,6,11,-1,-1,-1,-1,-1,-1,-1},
 {6,11,7,1,2,10,0,8,3,4,9,5,-1,-1,-1,-1},
 {7,6,11,5,4,10,4,2,10,4,0,2,-1,-1,-1,-1},
 {3,4,8,3,5,4,3,2,5,10,5,2,11,7,6,-1},
 {7,2,3,7,6,2,5,4,9,-1,-1,-1,-1,-1,-1,-1},
 {9,5,4,0,8,6,0,6,2,6,8,7,-1,-1,-1,-1},
 {3,6,2,3,7,6,1,5,0,5,4,0,-1,-1,-1,-1},
 {6,2,8,6,8,7,2,1,8,4,8,5,1,5,8,-1},
 {9,5,4,10,1,6,1,7,6,1,3,7,-1,-1,-1,-1},
 {1,6,10,1,7,6,1,0,7,8,7,0,9,5,4,-1},
 {4,0,10,4,10,5,0,3,10,6,10,7,3,7,10,-1},
 {7,6,10,7,10,8,5,4,10,4,8,10,-1,-1,-1,-1},
 {6,9,5,6,11,9,11,8,9,-1,-1,-1,-1,-1,-1,-1},
 {3,6,11,0,6,3,0,5,6,0,9,5,-1,-1,-1,-1},
 {0,11,8,0,5,11,0,1,5,5,6,11,-1,-1,-1,-1},
 {6,11,3,6,3,5,5,3,1,-1,-1,-1,-1,-1,-1,-1},
 {1,2,10,9,5,11,9,11,8,11,5,6,-1,-1,-1,-1},
 {0,11,3,0,6,11,0,9,6,5,6,9,1,2,10,-1},
 {11,8,5,11,5,6,8,0,5,10,5,2,0,2,5,-1},
 {6,11,3,6,3,5,2,10,3,10,5,3,-1,-1,-1,-1},
 {5,8,9,5,2,8,5,6,2,3,8,2,-1,-1,-1,-1},
 {9,5,6,9,6,0,0,6,2,-1,-1,-1,-1,-1,-1,-1},
 {1,5,8,1,8,0,5,6,8,3,8,2,6,2,8,-1},
 {1,5,6,2,1,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {1,3,6,1,6,10,3,8,6,5,6,9,8,9,6,-1},
 {10,1,0,10,0,6,9,5,0,5,6,0,-1,-1,-1,-1},
 {0,3,8,5,6,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {10,5,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {11,5,10,7,5,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {11,5,10,11,7,5,8,3,0,-1,-1,-1,-1,-1,-1,-1},
 {5,11,7,5,10,11,1,9,0,-1,-1,-1,-1,-1,-1,-1},
 {10,7,5,10,11,7,9,8,1,8,3,1,-1,-1,-1,-1},
 {11,1,2,11,7,1,7,5,1,-1,-1,-1,-1,-1,-1,-1},
 {0,8,3,1,2,7,1,7,5,7,2,11,-1,-1,-1,-1},
 {9,7,5,9,2,7,9,0,2,2,11,7,-1,-1,-1,-1},
 {7,5,2,7,2,11,5,9,2,3,2,8,9,8,2,-1},
 {2,5,10,2,3,5,3,7,5,-1,-1,-1,-1,-1,-1,-1},
 {8,2,0,8,5,2,8,7,5,10,2,5,-1,-1,-1,-1},
 {9,0,1,5,10,3,5,3,7,3,10,2,-1,-1,-1,-1},
 {9,8,2,9,2,1,8,7,2,10,2,5,7,5,2,-1},
 {1,3,5,3,7,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {0,8,7,0,7,1,1,7,5,-1,-1,-1,-1,-1,-1,-1},
 {9,0,3,9,3,5,5,3,7,-1,-1,-1,-1,-1,-1,-1},
 {9,8,7,5,9,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {5,8,4,5,10,8,10,11,8,-1,-1,-1,-1,-1,-1,-1},
 {5,0,4,5,11,0,5,10,11,11,3,0,-1,-1,-1,-1},
 {0,1,9,8,4,10,8,10,11,10,4,5,-1,-1,-1,-1},
 {10,11,4,10,4,5,11,3,4,9,4,1,3,1,4,-1},
 {2,5,1,2,8,5,2,11,8,4,5,8,-1,-1,-1,-1},
 {0,4,11,0,11,3,4,5,11,2,11,1,5,1,11,-1},
 {0,2,5,0,5,9,2,11,5,4,5,8,11,8,5,-1},
 {9,4,5,2,11,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {2,5,10,3,5,2,3,4,5,3,8,4,-1,-1,-1,-1},
 {5,10,2,5,2,4,4,2,0,-1,-1,-1,-1,-1,-1,-1},
 {3,10,2,3,5,10,3,8,5,4,5,8,0,1,9,-1},
 {5,10,2,5,2,4,1,9,2,9,4,2,-1,-1,-1,-1},
 {8,4,5,8,5,3,3,5,1,-1,-1,-1,-1,-1,-1,-1},
 {0,4,5,1,0,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {8,4,5,8,5,3,9,0,5,0,3,5,-1,-1,-1,-1},
 {9,4,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {4,11,7,4,9,11,9,10,11,-1,-1,-1,-1,-1,-1,-1},
 {0,8,3,4,9,7,9,11,7,9,10,11,-1,-1,-1,-1},
 {1,10,11,1,11,4,1,4,0,7,4,11,-1,-1,-1,-1},
 {3,1,4,3,4,8,1,10,4,7,4,11,10,11,4,-1},
 {4,11,7,9,11,4,9,2,11,9,1,2,-1,-1,-1,-1},
 {9,7,4,9,11,7,9,1,11,2,11,1,0,8,3,-1},
 {11,7,4,11,4,2,2,4,0,-1,-1,-1,-1,-1,-1,-1},
 {11,7,4,11,4,2,8,3,4,3,2,4,-1,-1,-1,-1},
 {2,9,10,2,7,9,2,3,7,7,4,9,-1,-1,-1,-1},
 {9,10,7,9,7,4,10,2,7,8,7,0,2,0,7,-1},
 {3,7,10,3,10,2,7,4,10,1,10,0,4,0,10,-1},
 {1,10,2,8,7,4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {4,9,1,4,1,7,7,1,3,-1,-1,-1,-1,-1,-1,-1},
 {4,9,1,4,1,7,0,8,1,8,7,1,-1,-1,-1,-1},
 {4,0,3,7,4,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {4,8,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {9,10,8,10,11,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {3,0,9,3,9,11,11,9,10,-1,-1,-1,-1,-1,-1,-1},
 {0,1,10,0,10,8,8,10,11,-1,-1,-1,-1,-1,-1,-1},
 {3,1,10,11,3,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {1,2,11,1,11,9,9,11,8,-1,-1,-1,-1,-1,-1,-1},
 {3,0,9,3,9,11,1,2,9,2,11,9,-1,-1,-1,-1},
 {0,2,11,8,0,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {3,2,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {2,3,8,2,8,10,10,8,9,-1,-1,-1,-1,-1,-1,-1},
 {9,10,2,0,9,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {2,3,8,2,8,10,0,1,8,1,10,8,-1,-1,-1,-1},
 {1,10,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {1,3,8,9,1,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {0,9,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {0,3,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
 {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
};

double ScalarField(double x, double y, double z)
{
    double R = 1.0;
    double r = 0.3;
    double q = sqrt(x*x+z*z) - R;

    return q*q + y*y - r*r;
    // return x*x + y*y + z*z - 1.0;
}

const int n = 30;
const double ext = 1.5;
std::vector<GridCell> BuildGrid()
{
    std::vector<GridCell> cells;
    double step = (2.0*ext)/n;

    for(int ix = 0; ix < n; ix++)
    {
        for(int iy = 0; iy < n; iy++)
        {
            for(int iz = 0; iz < n; iz++)
            {
                double x0 = -ext +(ix*step);
                double y0 = -ext +(iy*step);
                double z0 = -ext +(iz*step);

                GridCell cell;

                cell.pos[0] = {x0, y0, z0};
                cell.pos[1] = {x0 + step, y0, z0};
                cell.pos[2] = {x0 + step, y0 + step, z0};
                cell.pos[3] = {x0, y0 + step, z0};
                cell.pos[4] = {x0, y0, z0 + step};
                cell.pos[5] = {x0 + step, y0, z0 + step};
                cell.pos[6] = {x0 + step, y0 + step, z0 + step};
                cell.pos[7] = {x0, y0 + step, z0 + step};

                for(int i = 0; i < 8; i++)
                {
                    cell.val[i] = ScalarField(cell.pos[i].x, cell.pos[i].y, cell.pos[i].z);
                }

                cells.push_back(cell);
            }
        }
    }

    return cells;

}

Vec3 VertexInterp(double isolevel, Vec3 p1, Vec3 p2, double val1, double val2)
{
    if(fabs(isolevel - val1) < 1e-5) return p1;
    if(fabs(isolevel - val2) < 1e-5) return p2;
    if(fabs(val1 - val2) < 1e-5) return p1;

    double mu = (isolevel - val1)/(val2 - val1);

    Vec3 p;

    p.x = p1.x + mu * (p2.x - p1.x);
    p.y = p1.y + mu * (p2.y - p1.y);
    p.z = p1.z + mu * (p2.z - p1.z);

    return p;

}


int Polygonize(GridCell cell, double isolevel, Triangle* triangles)
{
    int cubeIndex = 0;

    if(cell.val[0] < isolevel) cubeIndex |= 1;
    if(cell.val[1] < isolevel) cubeIndex |= 2;
    if(cell.val[2] < isolevel) cubeIndex |= 4;
    if(cell.val[3] < isolevel) cubeIndex |= 8;
    if(cell.val[4] < isolevel) cubeIndex |= 16;
    if(cell.val[5] < isolevel) cubeIndex |= 32;
    if(cell.val[6] < isolevel) cubeIndex |= 64;
    if(cell.val[7] < isolevel) cubeIndex |= 128;

    if(edgeTable[cubeIndex] == 0) return 0;

    Vec3 vertList[12];

    if(edgeTable[cubeIndex] & 1)
        vertList[0] = VertexInterp(isolevel, cell.pos[0], cell.pos[1], cell.val[0], cell.val[1]);
    if(edgeTable[cubeIndex] & 2)
        vertList[1] = VertexInterp(isolevel, cell.pos[1], cell.pos[2], cell.val[1], cell.val[2]);
    if(edgeTable[cubeIndex] & 4)
        vertList[2] = VertexInterp(isolevel, cell.pos[2], cell.pos[3], cell.val[2], cell.val[3]);
    if(edgeTable[cubeIndex] & 8)
        vertList[3] = VertexInterp(isolevel, cell.pos[3], cell.pos[0], cell.val[3], cell.val[0]);
    if(edgeTable[cubeIndex] & 16)
        vertList[4] = VertexInterp(isolevel, cell.pos[4], cell.pos[5], cell.val[4], cell.val[5]);
    if(edgeTable[cubeIndex] & 32)
        vertList[5] = VertexInterp(isolevel, cell.pos[5], cell.pos[6], cell.val[5], cell.val[6]);
    if(edgeTable[cubeIndex] & 64)
        vertList[6] = VertexInterp(isolevel, cell.pos[6], cell.pos[7], cell.val[6], cell.val[7]);
    if(edgeTable[cubeIndex] & 128)
        vertList[7] = VertexInterp(isolevel, cell.pos[7], cell.pos[4], cell.val[7], cell.val[4]);
    if(edgeTable[cubeIndex] & 256)
        vertList[8] = VertexInterp(isolevel, cell.pos[0], cell.pos[4], cell.val[0], cell.val[4]);
    if(edgeTable[cubeIndex] & 512)
        vertList[9] = VertexInterp(isolevel, cell.pos[1], cell.pos[5], cell.val[1], cell.val[5]);
    if(edgeTable[cubeIndex] & 1024)
        vertList[10] = VertexInterp(isolevel, cell.pos[2], cell.pos[6], cell.val[2], cell.val[6]);
    if(edgeTable[cubeIndex] & 2048)
        vertList[11] = VertexInterp(isolevel, cell.pos[3], cell.pos[7], cell.val[3], cell.val[7]);
    
    
    int n_triangles = 0;
    for(int i = 0; triTable[cubeIndex][i] != -1; i += 3)
    {
        triangles[n_triangles].p[0] = vertList[triTable[cubeIndex][i]];
        triangles[n_triangles].p[1] = vertList[triTable[cubeIndex][i+1]];
        triangles[n_triangles].p[2] = vertList[triTable[cubeIndex][i+2]];
        n_triangles++;
    }
            
    return n_triangles;

}

std::vector<Triangle> MarchingCubes()
{
    std::vector<GridCell> grid = BuildGrid();
    std::vector<Triangle> triangles;
    std::cout << "Size: " << grid.size();

    Triangle tri[5];

    for(int i = 0; i < grid.size(); i++)
    {
        int n = Polygonize(grid[i], 0.0, tri);
        for(int t = 0; t < n; t++)
            triangles.push_back(tri[t]);
    }

    std::cout << "Triangles: " << triangles.size();

    return triangles;
}

std::vector<float> TrianglesToVertexBuffer(const std::vector<Triangle>& triangles)
{
    std::vector<float> vertex;

    vertex.reserve(triangles.size() * 9);

    for(auto& tri: triangles)
    {
        for(int i = 0; i < 3; i++)
        {
            vertex.push_back(float(tri.p[i].x));
            vertex.push_back(float(tri.p[i].y));
            vertex.push_back(float(tri.p[i].z));
        }
    }
    return vertex;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= 0.001f;
    yoffset *= 0.001f;

    yaw   += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void processInput(GLFWwindow* window)
{
    const float cameraSpeed = 5.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * cameraFront;
        // std::cout << "W\n";
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cameraSpeed * cameraFront;
        // std::cout << "S\n";
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        // std::cout << "A\n";
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        // std::cout << "D\n";
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "WFC", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glfwSwapInterval(1);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float positions[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9,10,10,11, 8,
        12,13,14,14,15,12,
        16,17,18,18,19,16,
        20,21,22,22,23,20
    };

    VertexArray va;
    // VertexBuffer vb(positions, sizeof(positions));
    // VertexBufferLayout layout;
    // layout.Push<float>(3);
    // layout.Push<float>(3);
    // layout.Push<float>(2);
    // va.AddBuffer(vb, layout);
    // IndexBuffer ib(indices, 36);

    std::string shaderPath = std::string(RES_PATH) + "Basic.shader";
    Shader shader(shaderPath);

    std::string texturesPath = std::string(TEXTURES_PATH);
    TileSet tiles;
    tiles.LoadFromFolder(texturesPath);


    // Roda o WFC
    // WFC WFC(3, 3, 3, tiles, 0);
    // Grid3D grid = WFC.getGrid();

    //ScalarField
    std::vector<Triangle> tri = MarchingCubes();
    std::vector<float> TrianglesVertex = TrianglesToVertexBuffer(tri); 
    
    VertexBuffer sphereVB(TrianglesVertex.data(), TrianglesVertex.size()* sizeof(float));
    VertexBufferLayout spherelayout;
    spherelayout.Push<float>(3);
    va.AddBuffer(sphereVB, spherelayout);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 420");
    ImGui::StyleColorsDark();

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

        // // Renderiza o grid do WFC
        // for (int i = 0; i < grid.width(); i++) {
        //     for (int j = 0; j < grid.height(); j++) {
        //         for (int k = 0; k < grid.depth(); k++) {
        //             Wave* wave = grid.tryAt(i, j, k);
        //             if (!wave || wave->tileID == UNCOLLAPSED) continue;

        //             int tileID = wave->tileID;
        //             // if (tileID < 0 || tileID >= tiles.textures.size()) continue;

        //             glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i * 1.1f, j * 1.1f, k * 1.1f));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.f,1.f,1.f));            
        glm::mat4 mvp = proj * view * model;
        
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

        va.Bind();
        glDrawArrays(GL_TRIANGLES, 0, TrianglesVertex.size()/3);

                    

        //             tiles.textures[WFC.getTileIndexValue(tileID)]->Bind(0);
        //             shader.Bind();
        //             shader.SetUniformMat4f("u_MVP", mvp);
        //             shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
        //             shader.SetUniform1i("u_Texture", 0);

        //             va.Bind();
        //             ib.Bind();
        //             glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        //         }
        //     }
        // }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}