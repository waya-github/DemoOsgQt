#include "DemoOsgQt.h"
#include "CreatePolygon3DHandler.h"

#include <xmemory>
#include <stdexcept>
#include <stdarg.h>
#include <tchar.h>
#include <stdio.h>
#include <wx/dir.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "FindGeometryVistor.h"

#include <QLine>

#include <QMessageBox>
#include <osgDB/ReaderWriter>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/DelaunayTriangulator>
#include <osgUtil/SmoothingVisitor>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgUtil/Simplifier>
#include <osg/GraphicsContext>
#include <osgGA/TrackballManipulator>

#include <geos/geom/Point.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/Triangle.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/LineString.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/IntersectionMatrix.h>

#include <windows.h>
#include <shellapi.h>
extern "C"
{
#include <triangle/triangle.h>
}

Q_DECLARE_METATYPE(geosGeometryTriUserData)
Q_DECLARE_METATYPE(VertexIndexClass)
Q_DECLARE_METATYPE(TextureIndexClass)
Q_DECLARE_METATYPE(SegmentPointIndexClass)
Q_DECLARE_METATYPE(VertexTextureIndexClass)


DemoOsgQt::DemoOsgQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void DemoOsgQt::on_pbt_000_InitializeOsgViewer_clicked()
{
	QKeyEvent *QKeyF12 = new QKeyEvent(QEvent::KeyPress, Qt::Key_F12, Qt::NoModifier);
	QCoreApplication::postEvent(ui.openGLWidgetOSGB, QKeyF12);
}

//001 读取OSGB和OBJ文件
void DemoOsgQt::on_pbt_001_ReadOsgbAndObjToViewer_clicked()
{
	//OSGB
	osgViewer::Viewer* pOsgbViewer = ui.openGLWidgetOSGB->getOSGViewer();
	osg::ref_ptr<osg::Group> pOsgbGroup = pOsgbViewer->getSceneData()->asGroup();

	printf("\n\n读取Osgb文件至模型控件.....\n");
	printf("初始child数量：%d\n", pOsgbGroup->getNumChildren());

	std::string osgbFileName = "C:\\TestData\\osgb\\Tile_+051_+187\\Tile_+051_+187.osgb";
	printf("fileName：%s\n", osgbFileName.c_str());

	osg::ref_ptr<osgDB::Options> option = new osgDB::Options(std::string("noTriStripPolygons"));
	osg::Node* pOsgbNode = osgDB::readNodeFile(osgbFileName, option);

	pOsgbGroup->addChild(pOsgbNode);
	printf("加载osgb文件后的child数量：%d\n\n", pOsgbGroup->getNumChildren());

	//通过调用osgviewer的自定义的快捷键，实现居中显示。
	QKeyEvent *QKeySpace = new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
	ui.openGLWidgetOSGB->focusWidget();
	QCoreApplication::postEvent(ui.openGLWidgetOSGB, QKeySpace);
	QCoreApplication::processEvents();

	//OBJ
	osgViewer::Viewer* pObjViewer = ui.openGLWidgetOBJ->getOSGViewer();
	osg::ref_ptr<osg::Group> pObjGroup = pObjViewer->getSceneData()->asGroup();

	printf("\n\n读取Obj文件至模型控件.....\n");
	printf("初始child数量：%d\n", pObjGroup->getNumChildren());

	std::string objFileName = "C:\\TestData\\OBJ\\Tile_+051_+187\\Tile_+051_+187.obj";
	printf("fileName：%s\n", objFileName.c_str());

	osg::Node* pObjNode = osgDB::readNodeFile(objFileName, option);

	pObjGroup->addChild(pObjNode);
	printf("加载Obj文件后的child数量：%d\n\n", pObjGroup->getNumChildren());


	//通过调用osgviewer的自定义的快捷键，实现居中显示。
	QKeyEvent *QKeySpace2 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);//如果此处直接用QKeySpace的话，容易报错
	ui.openGLWidgetOBJ->focusWidget();
	QCoreApplication::postEvent(ui.openGLWidgetOBJ, QKeySpace2);
	QCoreApplication::processEvents();

	QMessageBox::information(NULL, "Info", "finished", QMessageBox::Ok, QMessageBox::Ok);
}

//002 绘制三维多边形
void DemoOsgQt::on_pbt_002_DrawPolygon_clicked()
{
	printf("on_pbt_002_DrawPolygon_clicked\n");

	CreatePolygon3DHandler* createPolygon3DHandler = new CreatePolygon3DHandler(ui.openGLWidgetOSGB->getOSGViewer());

	ui.openGLWidgetOSGB->getOSGViewer()->addEventHandler(createPolygon3DHandler);
}