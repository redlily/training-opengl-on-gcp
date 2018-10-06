using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class ExportMesh : MonoBehaviour
{
    void Start()
    {
        var meshComponent = GetComponentInChildren<SkinnedMeshRenderer>();
        var mesh = meshComponent.sharedMesh;

        using (var writer = new StreamWriter(Application.dataPath + "/model.h", false))
        {
            // write vertices.
            writer.WriteLine("const size_t modelVerticesSize = sizeof(GLfloat) * 5 * {0};", mesh.vertexCount);
            writer.WriteLine("const GLfloat modelVertices[] = {");
            
            for (var i = 0; i < mesh.vertexCount; ++i)
            {
                var position = mesh.vertices[i];
                var texCoord = mesh.uv[i];
                writer.WriteLine("{0}, {1}, {2}, {3}, {4},",
                    position.x, position.y, position.z, texCoord.x, 1.0 - texCoord.y);
            }

            writer.WriteLine("};");
            writer.WriteLine();
            
            // write indices.
            writer.WriteLine("const size_t modelIndicesCount = {0};", mesh.triangles.Length);
            writer.WriteLine("const size_t modelIndicesSize = sizeof(GLushort) * {0};", mesh.triangles.Length);
            writer.WriteLine("const GLushort modelIndices[] = {");

            foreach (var triangle in mesh.triangles)
            {
                writer.WriteLine("{0},", triangle);
            }

            writer.WriteLine("};");
            
            writer.Flush();
            writer.Close();
        }
    }
}