#ifndef CREATEPOLYGON3DHANDLER_H
#define CREATEPOLYGON3DHANDLER_H

#include <Windows.h>
#include <string>

#include <QOpenGLWidget>
#include <QDate>
#include <QTableWidget>
#include <QMenu>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QVector>

#include <osg/Group>
#include <osg/Geode>
#include <osg/Drawable>
#include <osg/ShapeDrawable>
#include <osg/Plane>
#include <osgdb/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/Renderer>
#include <osgFX/Scribe>
#include <osgFX/Outline>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/SceneView>
#include <osg/ValueObject>
#include <osgGA/GUIEventHandler>
#include <osgGA/CameraManipulator>
#include <osgGA/StandardManipulator>


class CreatePolygon3DHandler :public osgGA::GUIEventHandler
{
public:
	CreatePolygon3DHandler(osgViewer::Viewer *viewer);
	//virtual bool handle(osgGA::Event *event, osg::Object *object, osg::NodeVisitor *nv);
	//virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, osg::Object *, osg::NodeVisitor *);
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

public:
	std::vector<osg::Vec3> getVecResultPoint3d() { return vecResultPoint3d; };
	osg::ref_ptr<osg::Vec3Array> getArrResultPoint3d() { return arrResultOsgPoint3d; }

private:
	osgViewer::Viewer* mViewer;

private:
	//创建m_FirstPushPointX，m_FirstPushPointY用于存储鼠标左键按下时的坐标
	float m_FirstPushPointX, m_FirstPushPointY;

	//创建m_FirstReleasePointY，m_FirstReleasePointY用于存储鼠标左键弹起时的坐标
	float m_FirstReleasePointX, m_FirstReleasePointY;

	//创建数组vecPoint3d，用于存储点选到的三维点
	std::vector<osg::Vec3> vecDrawOsgPoint3d;

	std::vector<osg::Vec3> vecResultPoint3d;
	osg::ref_ptr<osg::Vec3Array> arrResultOsgPoint3d;

	osg::Group* pGroupUnitSphere = nullptr;
	osg::Geode* pLineStringGeode = nullptr;
	osg::Geode* pLineStringGeodeMove = nullptr;


	bool isOverFlag;


protected:
	void Pick(float x, float y);
	void PickMove(float x, float y);
	void PickFinished();
private:
	//bool GetNodeFromXY(osg::Node* &outNode, std::string& outNodeName, osg::Vec3d& point, float x, float y);
	void CreateTempPlaneGeode(osg::Geode& planeGeode, osg::Vec3 osgPoint3dCenter, double widthPlane, double heightPlane, double zValue);
	bool GetInsectionPointFromXY(osg::Vec3d &point, float x, float y);
	osg::Vec3 screen2World(float x, float y);
};
extern CreatePolygon3DHandler *createPolygon3DHandler;
#endif // CREATEPOLYGON3DHANDLER_H
