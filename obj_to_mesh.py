import json

def obj_to_json_mesh(obj_filename):
    vertices = []
    triangles = []
    with open(obj_filename, 'r') as file:
        for line in file:
            parts = line.strip().split()
            if not parts:
                continue
            if parts[0] == 'v':
                vertex = list(map(float, parts[1:4]))
                vertices.append(vertex)
            elif parts[0] == 'f':
                # OBJ indices are 1-based
                idx = [int(part.split('/')[0]) - 1 for part in parts[1:4]]
                triangle = {
                    "v0": vertices[idx[0]],
                    "v1": vertices[idx[1]],
                    "v2": vertices[idx[2]]
                }
                triangles.append(triangle)
    
    mesh = {
        "type": "mesh",
        "material": {
            "ks": 0.2,
            "kd": 0.8,
            "specularexponent": 30,
            "diffusecolor": [0.8, 0.3, 0.3],
            "specularcolor": [1.0, 1.0, 1.0],
            "isreflective": true,
            "reflectivity": 0.3,
            "isrefractive": false,
            "refractiveindex": 1.0
        },
        "triangles": triangles
    }
    
    return mesh

# Usage
obj_filename = 'teapot.obj'
mesh = obj_to_json_mesh(obj_filename)

# Incorporate into your scene
scene = {
    "nbounces": 8,
    "rendermode": "phong",
    "camera": { /* your camera settings */ },
    "scene": {
        "backgroundcolor": [0.1, 0.1, 0.1],
        "lightsources": [ /* your light sources */ ],
        "shapes": [
            /* other shapes */,
            mesh
        ]
    }
}

# Save to JSON file
with open('scene_with_teapot.json', 'w') as outfile:
    json.dump(scene, outfile, indent=2)
