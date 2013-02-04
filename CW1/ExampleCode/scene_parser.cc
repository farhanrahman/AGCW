#include "part2.h"


// ====================================================================
// ====================================================================
// CONSTRUCTORS, DESTRUCTOR & INITIALIZE

SceneParser::SceneParser() {
  initialize();
}

SceneParser::SceneParser(const char* filename) {
  initialize();

  // open the file
  assert(filename != NULL);
  const char *ext = &filename[strlen(filename)-4];
  assert(strcmp(ext,".txt") == 0);
  file = fopen(filename,"r");
  assert (file != NULL);

  // parse the scene
  parseFile();

  // close the file
  fclose(file);
  file = NULL;
}

SceneParser::~SceneParser() {
  if (group != NULL)
    delete group;
  if (camera != NULL)
    delete camera;
}

void SceneParser::initialize() {
  // initialize some reasonable default values
  group = NULL;
  camera = NULL;
  background_color = Vec3f(0.5,0.5,0.5);
  current_object_color = Vec3f(1,1,1);
  file = NULL;
}

// ====================================================================
// ====================================================================

void SceneParser::parseFile() {
  //
  // at the top level, the scene can have a camera,
  // background color and a group of objects
  //
  char token[MAX_PARSER_TOKEN_LENGTH];
  while (getToken(token)) {
    if (strcmp(token, "OrthographicCamera") == 0) {
      camera = parseOrthographicCamera();
    } else if (strcmp(token, "Background") == 0) {
      parseBackground();
    } else if (strcmp(token, "Group") == 0) {
      group = parseGroup();
    } else {
      printf ("Unknown token in parseFile: '%s'\n", token);
      exit(0);
    }
  }
}

Group* SceneParser::parseGroup() {
  //
  // each group starts with an integer that specifies
  // the number of objects in the group
  //
  // the material node sets the color of all objects which
  // follow, until the next material node
  //
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (strcmp(token, "{") == 0);

  // read in the number of objects
  int num_objects, countRead;
  getToken(token); assert (strcmp(token, "num_objects") == 0);
  countRead = fscanf(file,"%d", &num_objects);

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++
  // create a new Group with the right number of objects
  Group *answer = new Group(num_objects);
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++

  // read in the objects
  int count = 0;
  while (num_objects > count) {
    getToken(token);
    if (strcmp(token, "Material") == 0) {
      parseMaterial();
    } else if (strcmp(token, "Sphere") == 0) {
      Sphere *sphere = parseSphere();

      // ++++++++++++++++++++++++++++++++++++++++++++++++
      // add this object to the group
      answer->addObject(count,sphere);
      // ++++++++++++++++++++++++++++++++++++++++++++++++

      count++;
    } else {
      printf ("Unknown token in parseGroup: '%s'\n", token);
      exit(0);
    }
  }
  getToken(token); assert (strcmp(token, "}") == 0);

  // return the group
  return answer;
}

// ====================================================================
// ====================================================================

Camera* SceneParser::parseOrthographicCamera() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  float c0,c1,c2;
  float d0,d1,d2;
  float u0,u1,u2;
  float size;
  int countRead;

  // read in the camera parameters
  getToken(token); assert (strcmp(token, "{") == 0);
  getToken(token); assert (strcmp(token, "center") == 0);
  countRead = fscanf(file,"%f %f %f", &c0,&c1,&c2);
  getToken(token); assert (strcmp(token, "direction") == 0);
  countRead = fscanf(file,"%f %f %f", &d0,&d1,&d2);
  getToken(token); assert (strcmp(token, "up") == 0);
  countRead = fscanf(file,"%f %f %f", &u0,&u1,&u2);
  getToken(token); assert (strcmp(token, "size") == 0);
  countRead = fscanf(file,"%f", &size);
  getToken(token); assert (strcmp(token, "}") == 0);

  Vec3f center = Vec3f(c0,c1,c2);
  Vec3f direction = Vec3f (d0,d1,d2);
  Vec3f up = Vec3f(u0,u1,u2);

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++
  // create a new orthographic camera and return it
  return new OrthographicCamera(center,direction,up,size);
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++
}


void SceneParser::parseBackground() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  float c0,c1,c2;
  int countRead;

  // read in the background color
  getToken(token); assert (strcmp(token, "{") == 0);
  getToken(token); assert (strcmp(token, "color") == 0);

  countRead = fscanf(file,"%f %f %f",&c0,&c1,&c2);
  getToken(token); assert (strcmp(token, "}") == 0);

  // set the background color
  background_color = Vec3f(c0,c1,c2);
}

// ====================================================================
// ====================================================================

void SceneParser::parseMaterial() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  float c0,c1,c2;
  int countRead;

  // read in the material color
  getToken(token); assert (strcmp(token, "{") == 0);
  getToken(token); assert (strcmp(token, "diffuseColor") == 0);
  countRead = fscanf(file,"%f %f %f", &c0,&c1,&c2);
  getToken(token); assert (strcmp(token, "}") == 0);

  // change the current object color
  // scoping for the materials is very simplistic,
  // and essentially ignores any tree hierarchy
  current_object_color = Vec3f(c0,c1,c2);
}


Sphere* SceneParser::parseSphere() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  float c0,c1,c2;
  float radius;
  int countRead;

  // read in the sphere parameters
  getToken(token); assert (strcmp(token, "{") == 0);
  getToken(token); assert (strcmp(token, "center") == 0);
  countRead = fscanf(file,"%f %f %f", &c0,&c1,&c2);
  getToken(token); assert (strcmp(token, "radius") == 0);
  countRead = fscanf(file,"%f", &radius);
  getToken(token); assert (strcmp(token, "}") == 0);

  Vec3f center = Vec3f(c0,c1,c2);
  Vec3f color = current_object_color;

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++
  // create a new sphere object and return it
  return new Sphere(center,radius,color);
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ====================================================================
// ====================================================================

int SceneParser::getToken(char token[MAX_PARSER_TOKEN_LENGTH]) {
  // for simplicity, tokens must be separated by whitespace
  assert (file != NULL);
  int success = fscanf(file,"%s ",token);
  if (success == EOF) {
    token[0] = '\0';
    return 0;
  }
  return 1;
}

// ====================================================================
// ====================================================================

