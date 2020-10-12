#include "OsgViewerWidget.h"
#include <QApplication>
#include <osg/PolygonMode>
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>

using namespace std;

OsgViewerWidget::OsgViewerWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	init3D();
	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
}

OsgViewerWidget::~OsgViewerWidget()
{
}

//重写QOpenGLWidget相关
void OsgViewerWidget::paintGL()
{
	if (isVisibleTo(QApplication::activeWindow()))
	{
		frame();
	}
}

//重写QWidget相关（QOpenGLWidget继承于QWidget）
#pragma region 键盘
void OsgViewerWidget::keyPressEvent(QKeyEvent *event)
{
	//按键	1	显示网格，网格模式与纹理模式切换
	//按键	2	显示点云，点云模式与纹理模式切换

	switch (event->key())
	{
	case Qt::Key_1:
	{
		osg::ref_ptr<osg::StateSet> stateset = m_root->getOrCreateStateSet();

		osg::PolygonMode* polyModeObj = dynamic_cast<osg::PolygonMode*>(m_root->getOrCreateStateSet()->getAttribute(osg::StateAttribute::POLYGONMODE));

		osg::PolygonMode::Mode currentMode;

		if (polyModeObj)
		{
			currentMode = polyModeObj->getMode(osg::PolygonMode::FRONT_AND_BACK);
		}
		else
		{
			currentMode = osg::PolygonMode::FILL;
		}

		if (currentMode == osg::PolygonMode::FILL)
		{
			polyModeObj = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
		}
		else if (currentMode == osg::PolygonMode::LINE)
		{
			polyModeObj = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
		}
		else if (currentMode == osg::PolygonMode::POINT)
		{
			polyModeObj = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
		}

		////线宽
		//osg::LineWidth* lineObj = dynamic_cast<osg::LineWidth*>(m_root->getOrCreateStateSet()->getAttribute(osg::StateAttribute::LINEWIDTH));
		//lineObj = new osg::LineWidth(10.0f);

		stateset->setAttribute(polyModeObj);

		break;
	}
	case Qt::Key_2:
	{
		osg::ref_ptr<osg::StateSet> stateset = m_root->getOrCreateStateSet();

		osg::PolygonMode* polyModeObj = dynamic_cast<osg::PolygonMode*>(m_root->getOrCreateStateSet()->getAttribute(osg::StateAttribute::POLYGONMODE));

		osg::PolygonMode::Mode currentMode;

		if (polyModeObj)
		{
			currentMode = polyModeObj->getMode(osg::PolygonMode::FRONT_AND_BACK);
		}
		else
		{
			currentMode = osg::PolygonMode::FILL;
		}

		if (currentMode == osg::PolygonMode::FILL)
		{
			polyModeObj = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::POINT);
		}
		else if (currentMode == osg::PolygonMode::POINT)
		{
			polyModeObj = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
		}
		else if (currentMode == osg::PolygonMode::LINE)
		{
			polyModeObj = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::POINT);
		}

		stateset->setAttribute(polyModeObj);

		break;
	}

	case Qt::Key_Space:
	{
		//方向向上
		osg::Vec3d up(0.0, 0.0, 1.0);//osg右x,前y,上z,opengl由osg绕x轴向外旋转90°,
		//
		osg::Vec3d viewDirection(0.0, 0, 1.0);//

		double centerX = m_root->getBound().center().x();
		double centerY = m_root->getBound().center().y();
		double centerZ = m_root->getBound().center().z();
		//相机盯着的那个点（目标点，相当于靶心），不一定就是包围球的中点，也可以是场景的原点，这个看情况
		osg::Vec3d center(centerX, centerY, centerZ);

		//眼睛的位置=目标点+离目标点的距离
		double viewDistance = 3 * m_root->getBound().radius();
		osg::Vec3d eye = center + viewDirection * viewDistance;

		//相机设置矩阵
		this->getCameraManipulator()->setHomePosition(eye, center, up);
		this->home();
		break;
	}
	case Qt::Key_F12:
	{
		osg::ref_ptr<osg::Group> pGroup = this->getSceneData()->asGroup();
		int numChildren = pGroup->getNumChildren();
		pGroup->removeChildren(0, numChildren);
	}
	case Qt::Key_V:
	{
		//osg::ref_ptr<osg::Vec3d> eye = new osg::Vec3d;
		//osg::ref_ptr<osg::Vec3d> center = new osg::Vec3d;
		//osg::ref_ptr<osg::Vec3d> up = new osg::Vec3d;
		//this->getCameraManipulator()->getHomePosition(*eye, *center, *up);

		//printf("eye:%f   %f   %f\n", eye->x(), eye->y(), eye->z());
		break;
	}
	default:
		break;
	}
}
void OsgViewerWidget::keyReleaseEvent(QKeyEvent *event)
{
	////printf("2\n");
	setKeyboardModifiers(event);
	m_window->getEventQueue()->keyRelease(event->key());
	QOpenGLWidget::keyReleaseEvent(event);
	update();
}
#pragma endregion
#pragma region 鼠标
void OsgViewerWidget::mousePressEvent(QMouseEvent *event)
{
	////printf("3\n");
	int button = 0;
	switch (event->button()) {
	case Qt::LeftButton: button = 1; break;
	case Qt::MidButton: button = 2; break;
	case Qt::RightButton: button = 3; break;
	case Qt::NoButton: button = 0; break;
	default: button = 0; break;
	}
	setKeyboardModifiers(event);
	m_window->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
	update();
}
void OsgViewerWidget::mouseReleaseEvent(QMouseEvent *event)
{
	////printf("4\n");
	int button = 0;
	switch (event->button()) {
	case Qt::LeftButton: button = 1; break;
	case Qt::MidButton: button = 2; break;
	case Qt::RightButton: button = 3; break;
	case Qt::NoButton: button = 0; break;
	default: button = 0; break;
	}
	setKeyboardModifiers(event);
	m_window->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);

	QOpenGLWidget::mouseReleaseEvent(event);
	update();
}
void OsgViewerWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	////printf("5\n");
	int button = 0;
	switch (event->button()) {
	case Qt::LeftButton: button = 1; break;
	case Qt::MidButton: button = 2; break;
	case Qt::RightButton: button = 3; break;
	case Qt::NoButton: button = 0; break;
	default: button = 0; break;
	}
	setKeyboardModifiers(event);
	m_window->getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), button);

	QOpenGLWidget::mouseDoubleClickEvent(event);
	update();
}
void OsgViewerWidget::mouseMoveEvent(QMouseEvent *event)
{
	////printf("6\n");
	setKeyboardModifiers(event);
	m_window->getEventQueue()->mouseMotion(event->x(), event->y());
	QOpenGLWidget::mouseMoveEvent(event);
	update();
}
void OsgViewerWidget::wheelEvent(QWheelEvent *event)
{
	////printf("7\n");
	setKeyboardModifiers(event);
	m_window->getEventQueue()->mouseScroll(
		event->orientation() == Qt::Vertical ?
		(event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN) :
		(event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT));
	QOpenGLWidget::wheelEvent(event);
	update();
}
#pragma endregion
void OsgViewerWidget::resizeEvent(QResizeEvent *event)
{
	////printf("8\n");
	const QSize &size = event->size();
	m_window->resized(x(), y(), size.width(), size.height());
	m_window->getEventQueue()->windowResize(x(), y(), size.width(), size.height());
	m_window->requestRedraw();

	//    const QSize& oldSize = event->oldSize();
	//    int oldWidth = oldSize.width();
	//    int oldHeight = oldSize.height();

	//    int newWidth = size.width();
	//    int newHeight = size.height();

	//    double widthChangeRatio = double(newWidth) / double(oldWidth);
	//    double heigtChangeRatio = double(newHeight) / double(oldHeight);
	//    double aspectRatioChange = widthChangeRatio / heigtChangeRatio;
	QOpenGLWidget::resizeEvent(event);
}
void OsgViewerWidget::moveEvent(QMoveEvent *event)
{
	////printf("9\n");
	const QPoint &pos = event->pos();
	m_window->resized(pos.x(), pos.y(), width(), height());
	m_window->getEventQueue()->windowResize(pos.x(), pos.y(), width(), height());

	QOpenGLWidget::moveEvent(event);
}

//重写QObject相关(QWidget继承于QObject)
void OsgViewerWidget::timerEvent(QTimerEvent *)
{
	////printf("10\n");
	update();
}

//私有成员
void OsgViewerWidget::setKeyboardModifiers(QInputEvent *event)
{
	////printf("11\n");
	//int modkey = event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier);
	//unsigned int mask = 0;
	//if (modkey & Qt::ShiftModifier) {
	//	mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
	//}
	//if (modkey & Qt::ControlModifier) {
	//	mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
	//}
	//if (modkey & Qt::AltModifier) {
	//	mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
	//}

	//m_window->getEventQueue()->getCurrentEventState()->setModKeyMask(mask);
	update();
}
osg::ref_ptr<osg::Camera> OsgViewerWidget::createCamera(int x, int y, int w, int h)
{
	////printf("12\n");
	m_window = new osgViewer::GraphicsWindowEmbedded(x, y, w, h);
	//    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->windowDecoration = true;
	traits->x = x;
	traits->y = y;
	traits->width = w;
	traits->height = h;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext(m_window);
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera->setProjectionMatrixAsPerspective(30.0f, double(traits->width) / double(traits->height), 1.0f, 10000.0f);
	camera->setClearColor(osg::Vec4(0.4, 0.4, 0.4, 0.4));

	return camera.release();
}
void OsgViewerWidget::init3D()
{
	m_root = new osg::Group;
	m_root->setName("Root");

	setCamera(createCamera(0, 0, width(), height()));
	osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;
	manipulator->setAllowThrow(false);
	setCameraManipulator(manipulator);
	addEventHandler(new osgViewer::StatsHandler);
	addEventHandler(new osgViewer::ThreadingHandler());
	addEventHandler(new osgViewer::HelpHandler);

	addEventHandler(new osgGA::StateSetManipulator(this->getCamera()->getOrCreateStateSet()));
	// add the window size toggle handler
	addEventHandler(new osgViewer::WindowSizeHandler);
	// add the record camera path handler
	addEventHandler(new osgViewer::RecordCameraPathHandler);
	// add the LOD Scale handler
	addEventHandler(new osgViewer::LODScaleHandler);
	// add the screen capture handler
	addEventHandler(new osgViewer::ScreenCaptureHandler);

	setThreadingModel(osgViewer::Viewer::SingleThreaded);

	m_root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	m_root->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

	setSceneData(m_root);
	startTimer(10);
}