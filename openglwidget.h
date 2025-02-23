#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class OpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions{
    Q_OBJECT
public:
    OpenGLWidget(QWidget *parent = nullptr);
    QRect _viewport = QRect(0,0,100,100);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};

#endif // OPENGLWIDGET_H
