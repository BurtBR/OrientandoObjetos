#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class OpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions{
    Q_OBJECT
private:
    QRect _viewPort = QRect(0,0,100,100);
    GLfloat *_bufferData = nullptr;
    GLuint _bufferIndex = -1;
    qsizetype _bufferSize = 0;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

public:
    OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();

    bool SetVertexData(const float buffer[], qsizetype size);
};

#endif // OPENGLWIDGET_H
