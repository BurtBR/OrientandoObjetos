#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget{parent}{

}

void OpenGLWidget::initializeGL(){
    initializeOpenGLFunctions();

    // Background Color
    glClearColor(0.5,0.5,0.5,1.0);
}

void OpenGLWidget::paintGL(){

    glColor3f(0.5, 0.0 ,0.0);

    static const GLfloat g_vertex_buffer_data[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.25f, 0.5f, 0.0f,
        0.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.0f,
        -0.25f, 0.5f, 0.0f,
    };

    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // Specifies the index of the generic vertex attribute to be modified.
        3,                  // Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // Specifies the byte offset between consecutive generic vertex attributes.
                            // If stride is 0, the generic vertex attributes are understood to be tightly packed in the array.
        (void*)0            // Specifies a offset of the first component of the first generic vertex attribute in the array
        );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
}

void OpenGLWidget::resizeGL(int w, int h){

}
