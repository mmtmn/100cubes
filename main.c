#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "pandaFramework.h"
#include "pandaSystem.h"
#include "genericAsyncTask.h"
#include "asyncTaskManager.h"
#include "cLerpNodePathInterval.h"
#include "auto_bind.h"
#include "keyboardButton.h"
#include "mouseButton.h"
#include "collisionTraverser.h"
#include "collisionHandlerPusher.h"
#include "collisionNode.h"
#include "collisionSphere.h"
#include "geomVertexFormat.h"
#include "geomVertexData.h"
#include "geomVertexWriter.h"
#include "geomTriangles.h"
#include "geomNode.h"
#include "pandaNode.h"
#include "nodePathCollection.h"
#include "textNode.h"
#include "virtualFileSystem.h"

struct MyApp {
    // App-specific variables and objects
    WindowFramework* framework;
    AsyncTaskManager* taskMgr;
    CollisionTraverser* cTrav;
    CollisionHandlerPusher* pusher;
    PT(CollisionNode) colliderNode;
    NodePath camera;
    NodePath render;
    bool keys[256];
};

// Function prototypes
void init(MyApp* app);
void cleanup(MyApp* app);
AsyncTask::DoneStatus moveTask(GenericAsyncTask* task, void* data);
PT(GeomNode) createCubeModel();
void updateKey(const Event* event, void* data);

int main(int argc, char* argv[]) {
    MyApp app;
    init(&app);
    app.framework->main_loop();
    cleanup(&app);
    return 0;
}

void init(MyApp* app) {
    app->framework = new WindowFramework("MyApp", 800, 600);
    app->taskMgr = AsyncTaskManager::get_global_ptr();
    app->render = NodePath(app->framework->get_render());
    app->camera = app->framework->get_camera_group();

    // Set up camera
    app->camera.set_pos(0, -20, 5);
    app->camera.look_at(0, 0, 0);

    // Set up collision detection for the camera
    app->cTrav = new CollisionTraverser();
    app->pusher = new CollisionHandlerPusher();
    app->pusher->set_horizontal(true);

    // Create a collision sphere for the camera
    app->colliderNode = new CollisionNode("cam_collider");
    app->colliderNode->add_solid(new CollisionSphere(0, 0, 0, 1));
    NodePath collider = app->camera.attach_new_node(app->colliderNode);
    app->pusher->add_collider(collider, app->camera);
    app->cTrav->add_collider(collider, app->pusher);

    // Create a custom cube model
    PT(GeomNode) cubeModel = createCubeModel();

    // Create cubes
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            for (int k = 0; k < 20; k++) {
                NodePath cubeNode = app->render.attach_new_node("cube_node");
                cubeNode.set_scale(0.5);
                cubeNode.set_pos(i - 4.5, j - 4.5, k);
                cubeNode.attach_new_node(cubeModel);
            }
        }
    }

    // Set up keyboard controls
    memset(app->keys, 0, sizeof(app->keys));
    app->framework->define_key("w", "Move Forward", updateKey, app);
    app->framework->define_key("s", "Move Backward", updateKey, app);
    app->framework->define_key("a", "Move Left", updateKey, app);
    app->framework->define_key("d", "Move Right", updateKey, app);
    app->framework->define_key("q", "Move Down", updateKey, app);
    app->framework->define_key("e", "Move Up", updateKey, app);
    app->framework->define_key("arrow_left", "Rotate Left", updateKey, app);
    app->framework->define_key("arrow_right", "Rotate Right", updateKey, app);
    app->framework->define_key("arrow_up", "Rotate Up", updateKey, app);
    app->framework->define_key("arrow_down", "Rotate Down", updateKey, app);

    // Enable wireframe mode
    app->framework->enable_wireframe(true);

    // Hide the cursor
    app->framework->set_cursor_hidden(true);

    // Start the movement task
    PT(GenericAsyncTask) task = new GenericAsyncTask("Move Task", moveTask, app);
    app->taskMgr->add(task);
}

void cleanup(MyApp* app) {
    delete app->framework;
}

AsyncTask::DoneStatus moveTask(GenericAsyncTask* task, void* data) {
    MyApp* app = (MyApp*)data;
    float speed = 0.1f;
    float rotSpeed = 1.0f;

    if (app->keys['w']) {
        app->camera.set_y(app->camera, speed);
    }
    if (app->keys['s']) {
        app->camera.set_y(app->camera, -speed);
    }
    if (app->keys['a']) {
        app->camera.set_x(app->camera, -speed);
    }
    if (app->keys['d']) {
        app->camera.set_x(app->camera, speed);
    }
    if (app->keys['q']) {
        app->camera.set_z(app->camera, -speed);
    }
    if (app->keys['e']) {
        app->camera.set_z(app->camera, speed);
    }
    if (app->keys[KeyboardButton::arrow_left()]) {
        app->camera.set_h(app->camera, app->camera.get_h() + rotSpeed);
    }
    if (app->keys[KeyboardButton::arrow_right()]) {
        app->camera.set_h(app->camera, app->camera.get_h() - rotSpeed);
    }
    if (app->keys[KeyboardButton::arrow_up()]) {
        app->camera.set_p(app->camera, app->camera.get_p() + rotSpeed);
    }
    if (app->keys[KeyboardButton::arrow_down()]) {
        app->camera.set_p(app->camera, app->camera.get_p() - rotSpeed);
    }

    return AsyncTask::DS_cont;
}

PT(GeomNode) createCubeModel() {
    PT(GeomVertexData) vdata = new GeomVertexData("cube_data", GeomVertexFormat::get_v3n3cpt2(), Geom::UH_static);

    GeomVertexWriter vertex(vdata, InternalName::get_vertex());
    GeomVertexWriter normal(vdata, InternalName::get_normal());
    GeomVertexWriter color(vdata, InternalName::get_color());
    GeomVertexWriter texcoord(vdata, InternalName::get_texcoord());

    // Vertices of a unit cube
    LVecBase3 vertices[] = {
        {-0.5, -0.5, -0.5}, {+0.5, -0.5, -0.5}, {-0.5, +0.5, -0.5}, {+0.5, +0.5, -0.5},
        {-0.5, -0.5, +0.5}, {+0.5, -0.5, +0.5}, {-0.5, +0.5, +0.5}, {+0.5, +0.5, +0.5}
    };

    // Normals for each face
    LVecBase3 normals[] = {
        {0, 0, -1}, {0, 0, 1}, {0, -1, 0},
        {0, 1, 0}, {-1, 0, 0}, {1, 0, 0}
    };

    // Vertex indices for each face
    int indices[][4] = {
        {0, 2, 3, 1}, {4, 5, 7, 6}, {0, 1, 5, 4},
        {2, 6, 7, 3}, {0, 4, 6, 2}, {1, 3, 7, 5}
    };

    // Add vertices, normals, colors, and texture coordinates
    for (int i = 0; i < 8; i++) {
        vertex.add_data3f(vertices[i]);
        normal.add_data3f(normals[i / 4]);
        color.add_data4f(1, 1, 1, 1);
        texcoord.add_data2f(0, 0);
    }

    // Create geometry and primitive
    PT(Geom) geom = new Geom(vdata);
    PT(GeomTriangles) prim = new GeomTriangles(Geom::UH_static);

    // Add vertex indices for each face
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            prim->add_vertex(indices[i][j]);
        }
        prim->close_primitive();
    }

    geom->add_primitive(prim);

    // Create a GeomNode and attach the geometry
    PT(GeomNode) cubeNode = new GeomNode("cube_node");
    cubeNode->add_geom(geom);

    return cubeNode;
}

void updateKey(const Event* event, void* data) {
    MyApp* app = (MyApp*)data;
    int key = event->get_key_code();
    app->keys[key] = event->get_num_parameters() == 1;
}
