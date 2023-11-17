package com.leia.cnsdkgettingstartedglandroidnative;

import android.opengl.Matrix;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;

import com.leia.core.Vector3;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainRenderer implements GLSurfaceView.Renderer {

    private MainActivity activity       = null;
    private int          program        = 0;
    private boolean      internalInitOk = false;
    private int[]        vao            = new int[1];
    private long         startTime      = 0;
    private float        geometryDist   = 500.0f;
    private float        baseline       = 20.0f;
    private float        cubeSize       = 200.0f;
    private Vector3      cameraPos      = new Vector3(0,0,0);
    private Vector3      cameraDir      = new Vector3(0,1,0);
    private Vector3      cameraUp       = new Vector3(0,0,1);
    private float        fov            = 90.0f * 3.14159f / 180.0f;
    private float        nearz          = 1.0f;
    private float        farz           = 10000.0f;
    private int          windowWidth    = 0;
    private int          windowHeight   = 0;

    private final String vertexShaderCode =
            "#version 310 es\n" +
            "in vec3 inPos;\n" +
            "in vec3 inColor;\n" +
            "out vec3 color;\n" +
            "uniform mat4 transform;\n" +
            "void main() {\n" +
            "  gl_Position = transform * vec4(inPos, 1.0);\n" +
            "  color = inColor;\n" +
            "}";

    private final String fragmentShaderCode =
            "#version 310 es\n" +
            "precision highp float;\n" +
            "in vec3 color;\n" +
            "out vec4 frag_color;\n" +
            "void main() {\n" +
            "    frag_color = vec4(color, 1.0);\n" +
            "}";

    MainRenderer(MainActivity activity) {
        this.activity = activity;
    }

    boolean doInternalInit() {

        if (!internalInitOk) {

            float l = -cubeSize  / 2.0f;
            float r = l + cubeSize;
            float b = -cubeSize / 2.0f;
            float t = b + cubeSize;
            float n = -cubeSize / 2.0f;
            float f = n + cubeSize;

            float cubeVerts[][] =
            {
                {l, n, b}, // Left Near Bottom
                {l, f, b}, // Left Far Bottom
                {r, f, b}, // Right Far Bottom
                {r, n, b}, // Right Near Bottom
                {l, n, t}, // Left Near Top
                {l, f, t}, // Left Far Top
                {r, f, t}, // Right Far Top
                {r, n, t}  // Right Near Top
            };

            int faces[][] =
            {
                {0,1,2,3}, // bottom
                {1,0,4,5}, // left
                {0,3,7,4}, // front
                {3,2,6,7}, // right
                {2,1,5,6}, // back
                {4,7,6,5}  // top
            };

            float faceColors[][] =
            {
                {1,0,0},
                {0,1,0},
                {0,0,1},
                {1,1,0},
                {0,1,1},
                {1,0,1}
            };

            FloatBuffer verts = FloatBuffer.allocate(6*12);
            FloatBuffer colors = FloatBuffer.allocate(6*12);
            ShortBuffer indices= ShortBuffer.allocate(6*6);
            for (int i = 0; i < 6; i++)
            {
                int i0 = faces[i][0];
                int i1 = faces[i][1];
                int i2 = faces[i][2];
                int i3 = faces[i][3];

                // Add indices.
                int startIndex = (int)verts.position()/3;
                indices.put((short)(startIndex + 0));
                indices.put((short)(startIndex + 1));
                indices.put((short)(startIndex + 2));
                indices.put((short)(startIndex + 0));
                indices.put((short)(startIndex + 2));
                indices.put((short)(startIndex + 3));

                verts.put(cubeVerts[i0][0]);
                verts.put(cubeVerts[i0][1]);
                verts.put(cubeVerts[i0][2]);

                verts.put(cubeVerts[i1][0]);
                verts.put(cubeVerts[i1][1]);
                verts.put(cubeVerts[i1][2]);

                verts.put(cubeVerts[i2][0]);
                verts.put(cubeVerts[i2][1]);
                verts.put(cubeVerts[i2][2]);

                verts.put(cubeVerts[i3][0]);
                verts.put(cubeVerts[i3][1]);
                verts.put(cubeVerts[i3][2]);

                colors.put(faceColors[i][0]);
                colors.put(faceColors[i][1]);
                colors.put(faceColors[i][2]);

                colors.put(faceColors[i][0]);
                colors.put(faceColors[i][1]);
                colors.put(faceColors[i][2]);

                colors.put(faceColors[i][0]);
                colors.put(faceColors[i][1]);
                colors.put(faceColors[i][2]);

                colors.put(faceColors[i][0]);
                colors.put(faceColors[i][1]);
                colors.put(faceColors[i][2]);
            }

            indices.position(0);
            verts.position(0);
            colors.position(0);

            int vertexPositionsAttributeIndex  = 0;
            int vertexColorsAttributeIndex     = 1;

            GLES30.glGenVertexArrays(1, IntBuffer.wrap(vao));
            GLES30.glBindVertexArray(vao[0]);

            // Create index buffer.
            int[] indexBuffer = new int[1];
            GLES30.glGenBuffers(1, IntBuffer.wrap(indexBuffer));
            GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, indexBuffer[0]);
            GLES30.glBufferData(GLES30.GL_ELEMENT_ARRAY_BUFFER, indices.capacity() * 2, indices, GLES30.GL_STATIC_DRAW);

            // Create vertex positions buffer.
            int[] vertexPositionsBuffer = new int[1];
            GLES30.glGenBuffers(1, IntBuffer.wrap(vertexPositionsBuffer));
            GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, vertexPositionsBuffer[0]);
            GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, verts.capacity() * 4, verts, GLES30.GL_STATIC_DRAW);
            GLES30.glVertexAttribPointer(vertexPositionsAttributeIndex, 3, GLES30.GL_FLOAT, false, 0, 0);
            GLES30.glEnableVertexAttribArray(vertexPositionsAttributeIndex);

            // Create vertex colors buffer.
            int[] vertexColorsBuffer = new int[1];
            GLES30.glGenBuffers(1, IntBuffer.wrap(vertexColorsBuffer));
            GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, vertexColorsBuffer[0]);
            GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, colors.capacity() * 4, colors, GLES30.GL_STATIC_DRAW);
            GLES30.glVertexAttribPointer(vertexColorsAttributeIndex, 3, GLES30.GL_FLOAT, false, 0, 0);
            GLES30.glEnableVertexAttribArray(vertexColorsAttributeIndex);

            // Unbind buffers
            GLES30.glBindVertexArray(0);
            GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, 0);
            GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, 0);

            // Create vertex shader.
            int vertexShader = GLES30.glCreateShader(GLES30.GL_VERTEX_SHADER);
            GLES30.glShaderSource(vertexShader, vertexShaderCode);
            GLES30.glCompileShader(vertexShader);

            // Verify vertex shader compile succeeded.
            {
                int[] success = new int[1];
                GLES30.glGetShaderiv(vertexShader, GLES30.GL_COMPILE_STATUS, IntBuffer.wrap(success));
                if(success[0] == 0)
                {
                    String infoLog = GLES30.glGetShaderInfoLog(vertexShader);
                    return false;
                }
            }

            // Create fragment shader.
            int fragmentShader = GLES30.glCreateShader(GLES30.GL_FRAGMENT_SHADER);
            GLES30.glShaderSource(fragmentShader, fragmentShaderCode);
            GLES30.glCompileShader(fragmentShader);

            // Verify fragment shader compile succeeded.
            {
                int[] success = new int[1];
                GLES30.glGetShaderiv(vertexShader, GLES30.GL_COMPILE_STATUS, IntBuffer.wrap(success));
                if(success[0] == 0)
                {
                    String infoLog = GLES30.glGetShaderInfoLog(vertexShader);
                    return false;
                }
            }

            // Create program.
            program = GLES30.glCreateProgram();
            GLES30.glAttachShader(program, vertexShader);
            GLES30.glAttachShader(program, fragmentShader);
            GLES30.glLinkProgram(program);

            // Verify program link succeeded.
            {
                int[] success = new int[1];
                GLES30.glGetProgramiv(program, GLES30.GL_LINK_STATUS, IntBuffer.wrap(success));
                if(success[0] == 0)
                {
                    String infoLog = GLES30.glGetProgramInfoLog(program);
                    return false;
                }
            }

            // Delete non-needed shaders since the program has them already.
            GLES30.glDeleteShader(vertexShader);
            GLES30.glDeleteShader(fragmentShader);

            // Enable depth-test and face culling.
            GLES30.glEnable(GLES30.GL_DEPTH_TEST);
            GLES30.glEnable(GLES30.GL_CULL_FACE);

            // Set convergence distance to be exactly at the rendered cube.
            activity.setConvergenceDistance(geometryDist);
            activity.setBaselineScaling(baseline);

            internalInitOk = true;
        }

        return internalInitOk;
    }

    boolean doDeferredInitialization()
    {
        // Initialize CNSDK.
        if (!activity.doCNSDKInit())
            return false;

        // Initialize graphics used with CNSDK.
        if (!activity.doGraphicsInit())
            return false;

        // Initialize our internal resources.
        if (!doInternalInit())
            return false;

        // Ready to use CNSDK.
        return true;
    }

    @Override
    public void onDrawFrame(GL10 gl) {

        if (!doDeferredInitialization())
            return;

        // Compute elapsed time.
        float elapsedTime = 0;
        {
            long curTime = System.currentTimeMillis();
            if (startTime == 0.0)
                startTime = curTime;
            elapsedTime = (float) (curTime - startTime) / 1000.0f;
        }

        int   renderTarget        = activity.getRenderTargetForView(0);
        int   renderTargetWidth   = activity.getViewWidth();
        int   renderTargetHeight  = activity.getViewHeight();
        float aspectRatio         = (float)renderTargetWidth / (float)renderTargetHeight;

        // Clear backbuffer to black
        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, 0);
        GLES30.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT | GLES30.GL_DEPTH_BUFFER_BIT);

        // Clear render-target to dark blue.
        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, renderTarget);
        GLES30.glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT | GLES30.GL_DEPTH_BUFFER_BIT);

        // Remember viewport values to restore them later.
        IntBuffer Viewport = IntBuffer.allocate(4);
        GLES30.glGetIntegerv(GLES30.GL_VIEWPORT, Viewport);

        // Create geometry transform to rotate the cube and place it at specified distance.
        float geometryTransform[] = new float[16];
        {
            float rotateMatrix[] = new float[16];
            Matrix.setRotateEulerM(rotateMatrix, 0, 20.0f * elapsedTime, 0.0f * elapsedTime, 40.0f * elapsedTime);

            float translateMatrix[] = new float[16];
            Matrix.setIdentityM(translateMatrix, 0);
            translateMatrix[13] = geometryDist;

            Matrix.multiplyMM(geometryTransform, 0, translateMatrix, 0, rotateMatrix, 0);
        }

        for (int viewIndex = 0; viewIndex < 2; viewIndex++)
        {
            // Compute per-view values.
            activity.calculateConvergedPerspectiveViewInfo(
                    viewIndex,
                    cameraPos.x, cameraPos.y, cameraPos.z,
                    cameraDir.x, cameraDir.y, cameraDir.z,
                    cameraUp.x, cameraUp.y, cameraUp.z,
                    fov,
                    aspectRatio,
                    nearz,
                    farz);

            // Check we are within the comfort zone.
            /*float comfortNear = activity.getViewComfortZoneNear(nearz, farz);
            float comfortFar = activity.getViewComfortZoneFar(nearz, farz);
            float minGeomDist = geometryDist - cubeSize/2.0f;
            float maxGeomDist = geometryDist + cubeSize/2.0f;
            assert(minGeomDist >= comfortNear);
            assert(maxGeomDist <= comfortFar);*/

            // Get the camera position for the current view.
            Vector3 viewCameraPos = new Vector3();
            viewCameraPos.x = activity.getConvergedPerspectiveViewPosition(0);
            viewCameraPos.y = activity.getConvergedPerspectiveViewPosition(1);
            viewCameraPos.z = activity.getConvergedPerspectiveViewPosition(2);

            // Get the camera target for the current view.
            Vector3 viewTargetPos = new Vector3();
            viewTargetPos.x = viewCameraPos.x + cameraDir.x;
            viewTargetPos.y = viewCameraPos.y + cameraDir.y;
            viewTargetPos.z = viewCameraPos.z + cameraDir.z;

            // Get the projection matrix for the current view.
            float viewProjectionMatrix[] = new float[16];
            for (int i=0; i<16; i++)
                viewProjectionMatrix[i] = activity.getConvergedPerspectiveViewProjectionMatrix(i);

            // Get camera transform.
            float cameraTransform[] = new float[16];
            Matrix.setLookAtM(cameraTransform, 0, viewCameraPos.x, viewCameraPos.y, viewCameraPos.z, viewTargetPos.x, viewTargetPos.y, viewTargetPos.z, cameraUp.x, cameraUp.y, cameraUp.z);

            // Compute combined matrix.
            float temp[] = new float[16];
            float mvp[] = new float[16];
            Matrix.multiplyMM(temp, 0, viewProjectionMatrix, 0, cameraTransform, 0);
            Matrix.multiplyMM(mvp, 0, temp, 0, geometryTransform, 0);

            // Render view into left or right of render-target.
            GLES30.glViewport(renderTargetWidth * viewIndex, 0, renderTargetWidth, renderTargetHeight);
            GLES30.glUseProgram(program);
            int tranformHandle = GLES30.glGetUniformLocation(program, "transform");
            GLES30.glUniformMatrix4fv(tranformHandle, 1, false, FloatBuffer.wrap(mvp));
            GLES30.glBindVertexArray(vao[0]);
            GLES30.glEnableVertexAttribArray(0);
            GLES30.glEnableVertexAttribArray(1);
            int triangles = 6 * 2;
            GLES30.glDrawElements(GLES30.GL_TRIANGLES, triangles * 3, GLES30.GL_UNSIGNED_SHORT, 0);
        }

        // Restore previous viewport.
        GLES30.glViewport(Viewport.get(0), Viewport.get(1), Viewport.get(2), Viewport.get(3));

        // Perform interlacing and sharpening.
        activity.doPostProcess(windowWidth, windowHeight);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        windowWidth = width;
        windowHeight = height;
    }
}
