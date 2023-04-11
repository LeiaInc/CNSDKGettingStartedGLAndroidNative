package com.leia.cnsdkgettingstartedglandroidnative;

import android.animation.IntArrayEvaluator;
import android.app.Activity;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import com.leia.sdk.LeiaSDK;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainRenderer implements GLSurfaceView.Renderer {

    MainActivity activity = null;
    private FloatBuffer vertexBuffer = null;
    private int program = 0;
    private boolean internalInitOk = false;

    private final int vertexCount = 3;
    private final int vertexStride = 3 * 4;// xyz * sizeof(float)

    private final String vertexShaderCode =
            "attribute vec4 vPosition;" +
            "void main() {" +
            "  gl_Position = vPosition;" +
            "}";

    private final String fragmentShaderCode =
            "precision mediump float;" +
            "uniform vec4 vColor;" +
            "void main() {" +
            "  gl_FragColor = vColor;" +
            "}";

    static float triangleCoords[] = { // todo: convert to cube once
        0.0f,  0.5f, 0.0f, // top
       -0.5f, -0.5f, 0.0f, // bottom left
        0.5f, -0.5f, 0.0f  // bottom right
    };

    MainRenderer(MainActivity activity) {
        this.activity = activity;
    }

    boolean doInternalInit() {

        if (!internalInitOk) {
            ByteBuffer bb = ByteBuffer.allocateDirect(triangleCoords.length * 4);
            bb.order(ByteOrder.nativeOrder());

            vertexBuffer = bb.asFloatBuffer();
            vertexBuffer.put(triangleCoords);
            vertexBuffer.position(0);

            int vertexShader = GLES20.glCreateShader(GLES20.GL_VERTEX_SHADER);
            GLES20.glShaderSource(vertexShader, vertexShaderCode);
            GLES20.glCompileShader(vertexShader);

            int fragmentShader = GLES20.glCreateShader(GLES20.GL_FRAGMENT_SHADER);
            GLES20.glShaderSource(fragmentShader, fragmentShaderCode);
            GLES20.glCompileShader(fragmentShader);

            program = GLES20.glCreateProgram();
            GLES20.glAttachShader(program, vertexShader);
            GLES20.glAttachShader(program, fragmentShader);
            GLES20.glLinkProgram(program);
            GLES20.glDeleteShader(vertexShader);
            GLES20.glDeleteShader(fragmentShader);

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

        // Render frame.
        float green = (float)((System.currentTimeMillis() / 1000.0) % 1.0);

        int renderTarget = activity.getRenderTargetForView(0);
        int renderTargetWidth = 1280; // todo: add JNI call to get this
        int renderTargetHeight = 720; // todo: add JNI call to get this
        int convergenceDistance = 500; // todo: add JNI call to get this
        int windowWidth = 2560;
        int windowHeight = 1600;

        // Clear backbuffer to black
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);

        // Clear render-target to dark blue.
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, renderTarget);
        GLES20.glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

        IntBuffer Viewport = IntBuffer.allocate(4);
        GLES20.glGetIntegerv(GLES20.GL_VIEWPORT, Viewport);

		for (int viewIndex = 0; viewIndex < 2; viewIndex++)
		{
			// Get camera position and projection matrix.
            /*
            glm::vec3 cameraPos = glm::vec3(0,0,0);
            glm::vec3 cameraFwd = glm::vec3(0,1,0);
            glm::vec3 cameraUp = glm::vec3(0,0,1);
            float FOV = 90.0f * 3.14159 / 180.0f;
            float aspectRatio = (float)windowWidth / (float)windowHeight;
            glm::vec3 viewPos;
            glm::mat4 projectionPersp;
            pLeiaInterlacer->GetConvergedPerspectiveViewInfo // todo: add JNI call for this
            (
                i,
                cameraPos,
                cameraFwd,
                cameraUp,
                glm::radians(FOV),
                aspectRatio,
                0.01f,
                10000.0f,
                &viewPos,
                &projectionPersp
            );
            */

            // todo: create translate matrix for geometry with convergenceDistance
            // todo: compute final transform matrix and push into vertex shader input to do geometry transform

            // Render view into left or right of render-target.
            GLES20.glViewport(renderTargetWidth * viewIndex, 0, renderTargetWidth, renderTargetHeight);
            GLES20.glUseProgram(program);
            int positionHandle = GLES20.glGetAttribLocation(program, "vPosition");
            GLES20.glEnableVertexAttribArray(positionHandle);
            GLES20.glVertexAttribPointer(positionHandle, 3, GLES20.GL_FLOAT, false, vertexStride, vertexBuffer);
            int colorHandle = GLES20.glGetUniformLocation(program, "vColor");
            float triangleColor[] = { 1.0f, green, 0.0f, 1.0f };
            GLES20.glUniform4fv(colorHandle, 1, triangleColor, 0);
            GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, vertexCount);
            GLES20.glDisableVertexAttribArray(positionHandle);
		}

        GLES20.glViewport(Viewport.get(0), Viewport.get(1), Viewport.get(2), Viewport.get(3));

        activity.doPostProcess(windowWidth, windowHeight);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
    }
}
