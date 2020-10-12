#ifndef OSGVIEWERWIDGET_H
#define OSGVIEWERWIDGET_H

#include <QOpenGLWidget>
#include <osgViewer/Viewer>
#include <QInputEvent>


class OsgViewerWidget : public QOpenGLWidget, public osgViewer::Viewer
{
	Q_OBJECT

public:
	OsgViewerWidget(QWidget *parent = 0);
	~OsgViewerWidget();

public:
	osgViewer::Viewer* getOSGViewer() { return this; }
	osg::Group* getRoot() { return m_root; }

protected:
	//virtual void initializeGL();
	virtual void paintGL();
	//virtual void resizeGL(int w, int h);


	//键盘相关
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);


	//鼠标相关
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);


	virtual void resizeEvent(QResizeEvent *event);
	virtual void moveEvent(QMoveEvent *event);
	virtual void timerEvent(QTimerEvent *);

private:
	osg::ref_ptr<osg::Group> m_root;
	osgViewer::GraphicsWindow* m_window;

	void setKeyboardModifiers(QInputEvent *event);
	osg::ref_ptr<osg::Camera> createCamera(int x, int y, int w, int h);
	void init3D();//特有

};
#endif // OSGVIEWERWIDGET_H






