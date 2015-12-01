#include "glhistogramwidget.h"
#include <QOpenGLFunctions>

GLHistogramWidget::GLHistogramWidget()
    : m_program(0), m_shader(0), m_texture(0) {

}

GLHistogramWidget::~GLHistogramWidget(){
    // Make sure the context is current and then explicitly
    // destroy all underlying OpenGL resources.
//    makeCurrent();

//    if(m_texture) delete m_texture;
//    if(m_shader)  delete m_shader;
//    if(m_program) delete m_program;

//    m_vbo.destroy();
//    m_vao.destroy();

//    doneCurrent();

}

void GLHistogramWidget::initializeGL(){
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

//    this->vao.create();
//     QOpenGLVertexArrayObject::Binder binder(&this->vao);
//     this->bufVertex.generate(GL_ARRAY_BUFFER);
//     this->bufIndex.generate(GL_ELEMENT_ARRAY_BUFFER);


    //
    m_vao.create();
    if (m_vao.isCreated())
        m_vao.bind();

//    //
//    m_vbo.create();
//    m_vbo.bind();
//    //m_vbo.allocate(...);

//    //
//    //m_texture = new QOpenGLTexture(QImage(...));

////    //
////    m_shader = new QOpenGLShader(...);
////    m_program = new QOpenGLShaderProgram(...);
}

void GLHistogramWidget::paintGL(){
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT);

    //

}

void GLHistogramWidget::resizeGL()
{

}

