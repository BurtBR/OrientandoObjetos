#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget{parent}{

}

OpenGLWidget::~OpenGLWidget(){
    if(_bufferData){
        delete _bufferData;
        _bufferData = nullptr;
    }
}

void OpenGLWidget::initializeGL(){
    initializeOpenGLFunctions();

    // Background Color
    glClearColor(0.5,0.5,0.5,1.0);
}

void OpenGLWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT);

    if(!_bufferSize)
        return;

    glColor3f(0.5, 0.0 ,0.0);

    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _bufferIndex);
    glVertexAttribPointer(
        0,                  // Specifies the index of the generic vertex attribute to be modified.
        3,                  // Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // Specifies the byte offset between consecutive generic vertex attributes.
                            // If stride is 0, the generic vertex attributes are understood to be tightly packed in the array.
        (void*)0            // Specifies a offset of the first component of the first generic vertex attribute in the array
        );

    // Draw
    glDrawArrays(GL_TRIANGLES, 0, _bufferSize/sizeof(GLfloat));
    glDisableVertexAttribArray(0);
}

void OpenGLWidget::resizeGL(int w, int h){

}

bool OpenGLWidget::SetVertexData(const GLfloat buffer[], qsizetype size){
    if(_bufferIndex < 0){
        return false;
    }

    if(_bufferData){
        delete _bufferData;
        _bufferData = nullptr;
    }

    _bufferSize = size*sizeof(GLfloat);

    try{
        _bufferData = new GLfloat[size];
    }catch(...){
        return false;
    }

    memcpy(_bufferData, buffer, _bufferSize);

    // Generate 1 buffer, put the resulting identifier in _bufferIndex
    glGenBuffers(1, &_bufferIndex);
    glBindBuffer(GL_ARRAY_BUFFER, _bufferIndex);
    // Give the vertices to OpenGL
    glBufferData(GL_ARRAY_BUFFER, _bufferSize, _bufferData, GL_DYNAMIC_DRAW);

    return true;
}
