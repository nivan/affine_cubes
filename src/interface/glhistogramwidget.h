#ifndef GLHISTOGRAMWIDGET_H
#define GLHISTOGRAMWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLTexture>

class GLHistogramWidget : public QOpenGLWidget
{
public:
    GLHistogramWidget();
    ~GLHistogramWidget();
private:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram* m_program;
    QOpenGLShader*        m_shader;
    QOpenGLTexture*       m_texture;
protected:
    void initializeGL();
    void paintGL();
    void resizeGL();
};

#endif // GLHISTOGRAMWIDGET_H
