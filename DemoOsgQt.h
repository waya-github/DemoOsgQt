#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DemoOsgQt.h"
#include <stdio.h>
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


class geosGeometryTriUserData
{
public:
	int indexV0;
	int indexV1;
	int indexV2;
	int indexUV0;
	int indexUV1;
	int indexUV2;
	int indexPic;
};

class VertexIndexClass
{
public:
	int indexOfVertexCoordinates0;
	int indexOfVertexCoordinates1;
	int indexOfVertexCoordinates2;
};

class TextureIndexClass
{
public:
	int indexOfTextureCoordinates0;
	int indexOfTextureCoordinates1;
	int indexOfTextureCoordinates2;
	int indexOfPicture;
};

class SegmentPointIndexClass
{
public:
	int indexStart;
	int indexEnd;
};

class VertexTextureIndexClass
{
public:
	int vertexIndex;
	int textureIndex;
};


class DemoOsgQt : public QMainWindow
{
	Q_OBJECT

public:
	DemoOsgQt(QWidget *parent = Q_NULLPTR);

public:
	Ui::DemoOsgQtClass getUI() { return ui; };
	osgGA::EventHandler* getEH() { return eh; };

public slots:
	//000 ��ջ�ԭ����
	void on_pbt_000_InitializeOsgViewer_clicked();

	//001 ��ȡosgb��obj�ļ�
	void on_pbt_001_ReadOsgbAndObjToViewer_clicked();

	//002 ������ά�����
	void on_pbt_002_DrawPolygon_clicked();

private:


	Ui::DemoOsgQtClass ui;
	osgGA::EventHandler *eh;
	clock_t startTime, endTime;

};
