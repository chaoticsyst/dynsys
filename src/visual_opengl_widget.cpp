#include <vector>
#include <cmath>
#include "visual_opengl_widget.h"
#include "Model.h"

// Timer constants
constexpr int TIMER_INTERVAL = 1;

// Window size constants
constexpr QSize MIN_WINDOW_SIZE = QSize(640, 480);
constexpr QSize INIT_WINDOW_SIZE = QSize(1080, 720);

// Size of cubes (points)
constexpr double R = 0.005;

// Painting
constexpr size_t POINTS_PER_ITERATION = 10;


VisualOpenGLWidget::VisualOpenGLWidget(QWidget *parent) :
    QGLWidget{QGLFormat(), parent}, lastPoint{0} {

    pointsTimer = new QTimer(this);
    pointsTimer->setInterval(TIMER_INTERVAL);
    connect(pointsTimer, SIGNAL(timeout()), this, SLOT(updatePoints()));
    pointsTimer->start();

    keysTimer = new QTimer(this);
    keysTimer->setInterval(TIMER_INTERVAL);
    connect(keysTimer, SIGNAL(timeout()), this, SLOT(updateKeys()));
    keysTimer->start();
}

QSize VisualOpenGLWidget::minimumSizeHint() const {
    return MIN_WINDOW_SIZE;
}

QSize VisualOpenGLWidget::sizeHint() const {
    return INIT_WINDOW_SIZE;
}

//TODO: rewrite this function
void addCube(QVector<QVector3D> &vertices, const QVector3D &center) {

    //cubes
    vertices << QVector3D(center.x() - R, center.y() - R, center.z() + R)
             << QVector3D(center.x() + R, center.y() - R, center.z() + R)
             << QVector3D(center.x() + R, center.y() + R, center.z() + R) // Front
             << QVector3D(center.x() + R, center.y() + R, center.z() + R)
             << QVector3D(center.x() - R, center.y() + R, center.z() + R)
             << QVector3D(center.x() - R, center.y() - R, center.z() + R)
             << QVector3D(center.x() + R, center.y() - R, center.z() - R)
             << QVector3D(center.x() - R, center.y() - R, center.z() - R)
             << QVector3D(center.x() - R, center.y() + R, center.z() - R) // Back
             << QVector3D(center.x() - R, center.y() + R, center.z() - R)
             << QVector3D(center.x() + R, center.y() + R, center.z() - R)
             << QVector3D(center.x() + R, center.y() - R, center.z() - R)
             << QVector3D(center.x() - R, center.y() - R, center.z() - R)
             << QVector3D(center.x() - R, center.y() - R, center.z() + R)
             << QVector3D(center.x() - R, center.y() + R, center.z() + R) // Left
             << QVector3D(center.x() - R, center.y() + R, center.z() + R)
             << QVector3D(center.x() - R, center.y() + R, center.z() - R)
             << QVector3D(center.x() - R, center.y() - R, center.z() - R)
             << QVector3D(center.x() + R, center.y() - R, center.z() + R)
             << QVector3D(center.x() + R, center.y() - R, center.z() - R)
             << QVector3D(center.x() + R, center.y() + R, center.z() - R) // Right
             << QVector3D(center.x() + R, center.y() + R, center.z() - R)
             << QVector3D(center.x() + R, center.y() + R, center.z() + R)
             << QVector3D(center.x() + R, center.y() - R, center.z() + R)
             << QVector3D(center.x() - R, center.y() + R, center.z() + R)
             << QVector3D(center.x() + R, center.y() + R, center.z() + R)
             << QVector3D(center.x() + R, center.y() + R, center.z() - R) // Top
             << QVector3D(center.x() + R, center.y() + R, center.z() - R)
             << QVector3D(center.x() - R, center.y() + R, center.z() - R)
             << QVector3D(center.x() - R, center.y() + R, center.z() + R)
             << QVector3D(center.x() - R, center.y() - R, center.z() - R)
             << QVector3D(center.x() + R, center.y() - R, center.z() - R)
             << QVector3D(center.x() + R, center.y() - R, center.z() + R) // Bottom
             << QVector3D(center.x() + R, center.y() - R, center.z() + R)
             << QVector3D(center.x() - R, center.y() - R, center.z() + R)
             << QVector3D(center.x() - R, center.y() - R, center.z() - R);

    //tetrahedrons
    /*vertices << QVector3D(center.x() + R, center.y() + R, center.z() + R) <<
                QVector3D(center.x() - R, center.y() - R, center.z() + R) <<
                QVector3D(center.x() + R, center.y() - R, center.z() - R) << //front
                QVector3D(center.x() + R, center.y() + R, center.z() + R) <<
                QVector3D(center.x() - R, center.y() + R, center.z() - R) <<
                QVector3D(center.x() - R, center.y() - R, center.z() + R) << //right
                QVector3D(center.x() + R, center.y() + R, center.z() + R) <<
                QVector3D(center.x() + R, center.y() - R, center.z() - R) <<
                QVector3D(center.x() - R, center.y() + R, center.z() - R) << //left
                QVector3D(center.x() - R, center.y() + R, center.z() - R) <<
                QVector3D(center.x() - R, center.y() - R, center.z() + R) <<
                QVector3D(center.x() + R, center.y() - R, center.z() - R);*/ //bottom
}

void VisualOpenGLWidget::pushBackToPaint(const QVector<QVector3D>& v) {
    std::for_each(
        v.begin(),
        v.end(),
        [this](const QVector3D& vec) {
            this->pointsToPaint.push_back(vec);
        }
    );
}

void VisualOpenGLWidget::clearPoints() {
    vertices.clear();
    pointsToPaint.clear();
    lastPoint = 0;
}

void VisualOpenGLWidget::updatePoints() {
    if (lastPoint == static_cast<size_t>(pointsToPaint.size())) {
        return;
    }
    for (size_t i = 0; i < POINTS_PER_ITERATION; i++) {
        addCube(vertices, pointsToPaint[lastPoint++]);
    }
    repaint();
}

void VisualOpenGLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    qglClearColor(QColor(Qt::black));

    shaderProgram.addShaderFromSourceCode(QGLShader::Vertex,
                                          "attribute highp vec4 vertex;\n"
                                          "uniform highp mat4 matrix;\n"
                                          "void main(void)\n"
                                          "{\n"
                                          "   gl_Position = matrix * vertex;\n"
                                          "}");
    shaderProgram.addShaderFromSourceCode(QGLShader::Fragment,
                                          "uniform mediump vec4 color;\n"
                                          "void main(void)\n"
                                          "{\n"
                                          "   gl_FragColor = color;\n"
                                          "}");
    shaderProgram.link();
}

void VisualOpenGLWidget::resizeGL(int width, int height) {
    camera.recalculatePerspective(width, height);
}

void VisualOpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram.bind();

    shaderProgram.setUniformValue("matrix", camera.getMatrix());
    shaderProgram.setUniformValue("color", QColor(Qt::white));
    shaderProgram.setAttributeArray("vertex", vertices.constData());
    shaderProgram.enableAttributeArray("vertex");
    glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // У меня тут постоянно вылетает warning про устаршевий метод TODO: Заменить на более новый
    shaderProgram.disableAttributeArray("vertex");
    shaderProgram.release();
}

void VisualOpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    camera.recalculateTarget(event->pos());
    event->accept();
}

void VisualOpenGLWidget::mousePressEvent(QMouseEvent *event) {
    camera.resetMousePosition(event->pos());
    event->accept();
}

void VisualOpenGLWidget::keyPressEvent(QKeyEvent *event) {
    keys.insert(event->key());
}

void VisualOpenGLWidget::keyReleaseEvent(QKeyEvent *event) {
    keys.remove(event->key());
}

void VisualOpenGLWidget::updateKeys() {
    if (keys.contains(Qt::Key_W)) {
        camera.moveForward(1);
    }
    if (keys.contains(Qt::Key_S)) {
        camera.moveForward(-1);
    }
    if (keys.contains(Qt::Key_D)) {
        camera.moveRight(1);
    }
    if (keys.contains(Qt::Key_A)) {
        camera.moveRight(-1);
    }
    if (keys.contains(Qt::Key_Q)) {
        camera.moveUp(1);
    }
    if (keys.contains(Qt::Key_E)) {
        camera.moveUp(-1);
    }
}
