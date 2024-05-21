from direct.showbase.ShowBase import ShowBase
from panda3d.core import Vec3, Point3, Geom, GeomNode, GeomVertexData, GeomVertexFormat, GeomVertexWriter, GeomTriangles
from panda3d.core import CollisionTraverser, CollisionHandlerPusher, CollisionSphere, CollisionNode
from panda3d.core import WindowProperties

class MyApp(ShowBase):
    def __init__(self):
        ShowBase.__init__(self)

        # Set up camera
        self.disableMouse()
        self.camera.setPos(0, -20, 5)
        self.camera.lookAt(0, 0, 0)

        # Set up collision detection for the camera
        self.cTrav = CollisionTraverser()
        self.pusher = CollisionHandlerPusher()
        self.pusher.setHorizontal(True)

        # Create a collision sphere for the camera
        colliderNode = CollisionNode('cam_collider')
        colliderNode.addSolid(CollisionSphere(0, 0, 0, 1))
        collider = self.camera.attachNewNode(colliderNode)
        self.pusher.addCollider(collider, self.camera)
        self.cTrav.addCollider(collider, self.pusher)

        # Create a custom cube model
        cube_model = self.create_cube_model()

        # Create cubes
        for i in range(10):
            for j in range(10):
                for k in range(10):
                    cube_node = self.render.attachNewNode('cube_node')
                    cube_node.setScale(0.5)
                    cube_node.setPos(i - 4.5, j - 4.5, k)
                    cube_node.attachNewNode(cube_model)

        # Set up keyboard controls
        self.keys = {'w': False, 's': False, 'a': False, 'd': False, 'q': False, 'e': False,
                     'arrow_left': False, 'arrow_right': False, 'arrow_up': False, 'arrow_down': False}
        for key in self.keys:
            self.accept(key, self.update_key, [key, True])
            self.accept(key+'-up', self.update_key, [key, False])

        # Enable wireframe mode
        base.toggleWireframe()

        # Hide the cursor
        props = WindowProperties()
        props.setCursorHidden(True)
        self.win.requestProperties(props)

        # Start the movement task
        self.taskMgr.add(self.move_task, 'Move Task')

    def create_cube_model(self):
        format = GeomVertexFormat.getV3n3cpt2()
        vdata = GeomVertexData('cube_data', format, Geom.UHStatic)

        vertex = GeomVertexWriter(vdata, 'vertex')
        normal = GeomVertexWriter(vdata, 'normal')
        color = GeomVertexWriter(vdata, 'color')
        texcoord = GeomVertexWriter(vdata, 'texcoord')

        # Vertices of a unit cube
        vertices = [
            (-0.5, -0.5, -0.5), (+0.5, -0.5, -0.5), (-0.5, +0.5, -0.5), (+0.5, +0.5, -0.5),
            (-0.5, -0.5, +0.5), (+0.5, -0.5, +0.5), (-0.5, +0.5, +0.5), (+0.5, +0.5, +0.5)
        ]

        # Normals for each face
        normals = [
            (0, 0, -1), (0, 0, 1), (0, -1, 0),
            (0, 1, 0), (-1, 0, 0), (1, 0, 0)
        ]

        # Vertex indices for each face
        indices = [
            (0, 2, 3, 1), (4, 5, 7, 6), (0, 1, 5, 4),
            (2, 6, 7, 3), (0, 4, 6, 2), (1, 3, 7, 5)
        ]

        # Add vertices, normals, colors, and texture coordinates
        for i in range(len(vertices)):
            vertex.addData3f(*vertices[i])
            normal.addData3f(*normals[i // 4])
            color.addData4f(1, 1, 1, 1)
            texcoord.addData2f(0, 0)

        # Create geometry and primitive
        geom = Geom(vdata)
        prim = GeomTriangles(Geom.UHStatic)

        # Add vertex indices for each face
        for face_indices in indices:
            for index in face_indices:
                prim.addVertex(index)
        prim.closePrimitive()

        geom.addPrimitive(prim)

        # Create a GeomNode and attach the geometry
        cube_node = GeomNode('cube_node')
        cube_node.addGeom(geom)

        return cube_node

    def update_key(self, key, value):
        self.keys[key] = value

    def move_task(self, task):
        speed = 0.1
        rot_speed = 1.0

        if self.keys['w']:
            self.camera.setY(self.camera, speed)
        if self.keys['s']:
            self.camera.setY(self.camera, -speed)
        if self.keys['a']:
            self.camera.setX(self.camera, -speed)
        if self.keys['d']:
            self.camera.setX(self.camera, speed)
        if self.keys['q']:
            self.camera.setZ(self.camera, -speed)
        if self.keys['e']:
            self.camera.setZ(self.camera, speed)
        if self.keys['arrow_left']:
            self.camera.setH(self.camera.getH() + rot_speed)
        if self.keys['arrow_right']:
            self.camera.setH(self.camera.getH() - rot_speed)
        if self.keys['arrow_up']:
            self.camera.setP(self.camera.getP() + rot_speed)
        if self.keys['arrow_down']:
            self.camera.setP(self.camera.getP() - rot_speed)

        return task.cont

app = MyApp()
app.run()
