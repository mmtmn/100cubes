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
                for k in range(100):
                    cube_node = self.render.attachNewNode('cube_node')
                    cube_node.setScale(0.5)
                    cube_node.setPos(i - 4.5, j - 4.5, k)
                    cube_node.attachNewNode(cube_model)

        # Set up keyboard controls
        self.accept('escape', self.quit)
        self.accept('w', self.move_forward)
        self.accept('s', self.move_backward)
        self.accept('a', self.move_left)
        self.accept('d', self.move_right)
        self.accept('q', self.move_down)
        self.accept('r', self.move_up)
        self.accept('arrow_left', self.rotate_left)
        self.accept('arrow_right', self.rotate_right)
        self.accept('arrow_up', self.rotate_up)
        self.accept('arrow_down', self.rotate_down)

        # Enable wireframe mode
        base.toggleWireframe()

        # Hide the cursor
        props = WindowProperties()
        props.setCursorHidden(True)
        self.win.requestProperties(props)

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

    def move_forward(self):
        self.camera.setY(self.camera, 1)

    def move_backward(self):
        self.camera.setY(self.camera, -1)

    def move_left(self):
        self.camera.setX(self.camera, -1)

    def move_right(self):
        self.camera.setX(self.camera, 1)

    def move_down(self):
        self.camera.setZ(self.camera, -1)

    def move_up(self):
        self.camera.setZ(self.camera, 1)

    def rotate_left(self):
        self.camera.setH(self.camera.getH() + 5)

    def rotate_right(self):
        self.camera.setH(self.camera.getH() - 5)

    def rotate_up(self):
        self.camera.setP(self.camera.getP() + 5)

    def rotate_down(self):
        self.camera.setP(self.camera.getP() - 5)

    def quit(self):
        self.userExit()

app = MyApp()
app.run()
