#ifndef FINDGEOMETRYVISTOR_H
#define FINDGEOMETRYVISTOR_H

//#include <osg/PolygonMode>
//#include <osg/ShapeDrawable>
//#include <osg/Geometry>
#include <osg/Geode>
//#include <osg/LOD>
//#include <osgDB/ReadFile>
//#include <osgUtil/PrintVisitor>
//#include <osgViewer/ViewerEventHandlers>
//#include <osgViewer/Viewer>
//#include <osg/ArgumentParser>
//#include <osg/ApplicationUsage>
//#include <osg/Group>
//#include <osg/Notify>
//#include <osg/Vec3>
//#include <osg/ProxyNode>
//#include <osg/Texture2D>
//#include <osg/Texture3D>
//#include <osg/BlendFunc>
//#include <osg/Timer>
//#include <osg/ComputeBoundsVisitor>
//#include <osg/BlendColor>
//#include <osg/BlendFunc>
//#include <osg/io_utils>
//#include <osg/TriangleFunctor>
#include <osg/Drawable>
//#include <osgDB/Registry>
//#include <osgDB/WriteFile>
//#include <osgDB/FileNameUtils>
//#include <osgDB/ReaderWriter>
//#include <osgDB/PluginQuery>
//#include <osg/Material>
//#include <osgUtil/Optimizer>
//#include <osgUtil/Simplifier>
//#include <osgUtil/SmoothingVisitor>
//#include <osgUtil/DelaunayTriangulator>
//#include <osgViewer/GraphicsWindow>
//#include <osgViewer/Version>
//#include <osgText/Font>
//#include <osgText/Text>
//#include <osgWidget/Util>
//#include <osgWidget/WindowManager>
//#include <osgWidget/Box>
//#include <osgWidget/Canvas>
//#include <osgWidget/Label>
//#include <osg/io_utils>
//#include <osgGA/TrackballManipulator>
//#include <osgGA/StateSetManipulator>
//#include <osg/AnimationPath>
//
//#include <QMainWindow>
//#include <QFileDialog>
//#include <QMessagebox>
//#include <QApplication>
//#include <QImageReader>
//#include <QImage>
//#include <QPixmap>
//#include <QScreen>
//#include <QGuiApplication>
//#include <QSize>
//#include <QDesktopWidget>
//#include <QProgressDialog>
//#include <QThread>
//#include <QWidget>
//
//#include <sys/types.h>  
//#include <sys/stat.h> 
//
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <atlimage.h>
#include <string>
//#include <vector>
#include <stdio.h>
//#include <stdlib.h>
//#include <io.h>
//#include <direct.h>   
//#include <gdiplus.h>
//#include <time.h>
//#include <math.h>
//#include <utility>
//#include <thread>
//#include <chrono>
//#include <functional>
//#include <atomic>
//#include <algorithm>
//#include <numeric>
//#include <ctime>
//#include <atlpath.h>
//#include <shlwapi.h>
//
using namespace std;
//using namespace Gdiplus;

class  AttributePrinter :public osg::Drawable::AttributeFunctor
{
public:
	typedef osg::Drawable::AttributeType AttributeType;

	int m_SizeVertices;
	int m_SizeNormals;
	int m_SizeTexture;

	osg::Vec3* m_Vec3Vertices;
	osg::Vec3* m_Vec3Normals;
	osg::Vec2* m_Vec2Texture;
	inline const char* getTypeName(AttributeType type)
	{
		static const char* typeName[] =
		{
			"Vertices",
			"Weights",
			"Normals",
			"Colors",
			"Secondary Colors",
			"Fog Coords",
			"Attribute 6",
			"Attribute 7",
			"Texture Coords 0",
			"Texture Coords 1",
			"Texture Coords 2",
			"Texture Coords 3",
			"Texture Coords 4",
			"Texture Coords 5",
			"Texture Coords 6",
			"Texture Coords 7"
		};
		return typeName[type];
	}
	template<typename T>

	void printInfo(AttributeType type, unsigned int size, T* front)
	{
		//std::cout << "******" << getTypeName(type) << ":" << size << std::endl;
#pragma region ÔÝÊ±×¢ÊÍ
		//string typeName(getTypeName(type));
		//ofstream write;
		//string txtFileName = "WAYA" + typeName + ".txt";
		//write.open(txtFileName);
		//for (unsigned int i = 0; i < size; ++i)
		//{
		//	write << "(" << *(front + i) << ")" << std::endl;
		//}
		//write.close();
#pragma endregion
	}
	virtual void  apply(AttributeType type, unsigned int size, osg::Vec2* front)
	{
		string typeName(getTypeName(type));
		if (typeName == "Texture Coords 0")
		{
			m_SizeTexture = size;
			m_Vec2Texture = front;
		}
		else
		{
			return;
		}
		printInfo(type, size, front);
	}
	virtual void  apply(AttributeType type, unsigned int size, osg::Vec3* front)
	{
		string typeName(getTypeName(type));
		if (typeName == "Vertices")
		{
			m_SizeVertices = size;
			m_Vec3Vertices = front;
		}
		else if (typeName == "Normals")
		{
			m_SizeNormals = size;
			m_Vec3Normals = front;
		}
		else
		{
			return;
		}
		printInfo(type, size, front);
	}
};
struct TrianglePrinter
{
	TrianglePrinter()
	{
		//std::cout << "*** Triangles ***" << std::endl;
	}
	osg::Vec3Array* arrList;
	void operator()(const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3, bool)const
	{
		arrList->push_back(v1);
		arrList->push_back(v2);
		arrList->push_back(v3);
	}
};

class FindGeometryVistor :public osg::NodeVisitor
{
public:
	FindGeometryVistor() :osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) {}

	int VerticesSize;

	osg::ref_ptr<osg::Vec2Array> arrTexture = new osg::Vec2Array();
	osg::ref_ptr<osg::Vec3Array> arrVertices = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec3Array> arrNormals = new osg::Vec3Array();

	osg::ref_ptr<osg::Vec2Array> arr1Texture = new osg::Vec2Array();
	osg::ref_ptr<osg::Vec3Array> arr1Vertices = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec3Array> arr1Normals = new osg::Vec3Array();


	osg::ref_ptr<osg::Vec2Array> arr2Texture = new osg::Vec2Array();
	osg::ref_ptr<osg::Vec3Array> arr2Vertices = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec3Array> arr2Normals = new osg::Vec3Array();


	osg::ref_ptr<osg::Vec2Array> arr3Texture = new osg::Vec2Array();
	osg::ref_ptr<osg::Vec3Array> arr3Vertices = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec3Array> arr3Normals = new osg::Vec3Array();


	virtual void apply(osg::Node& node)
	{
		traverse(node);
	}
	virtual void apply(osg::Geode& node)
	{
		//printf("node.getNumDrawables():%d\n", node.getNumDrawables());
		for (unsigned int i = 0; i < node.getNumDrawables(); ++i)
		{
			osg::Drawable* drawable = node.getDrawable(i);
			if (!drawable)
			{
				continue;
			}
			//std::cout << "[" << drawable->libraryName() << "::" << drawable->className() << "]" << std::endl;

			AttributePrinter attrPrinter;
			drawable->accept(attrPrinter);

			//osg::TriangleFunctor<TrianglePrinter> triPrinter;
			//triPrinter.arrList = new osg::Vec3Array;
			//drawable->accept(triPrinter);

			//ofstream write;
			//string txtFileName = "WAYATriangle.txt";
			//write.open(txtFileName);
			//for (size_t j = 0; j < triPrinter.arrList->size(); j++)
			//{
			//	write << "(" << triPrinter.arrList->at(j) << ")" << std::endl;
			//}
			//write.close();

			//printf("attrPrinter.m_SizeVertices   %d\n", attrPrinter.m_SizeVertices);
			for (int j = 0; j < attrPrinter.m_SizeVertices; j = j + 3)
			{
				arrVertices->push_back(*(attrPrinter.m_Vec3Vertices + j));
				arrVertices->push_back(*(attrPrinter.m_Vec3Vertices + j + 1));
				arrVertices->push_back(*(attrPrinter.m_Vec3Vertices + j + 2));
				arr1Vertices->push_back(*(attrPrinter.m_Vec3Vertices + j));
				arr2Vertices->push_back(*(attrPrinter.m_Vec3Vertices + j + 1));
				arr3Vertices->push_back(*(attrPrinter.m_Vec3Vertices + j + 2));

				arrNormals->push_back(*(attrPrinter.m_Vec3Normals + j));
				arrNormals->push_back(*(attrPrinter.m_Vec3Normals + j + 1));
				arrNormals->push_back(*(attrPrinter.m_Vec3Normals + j + 2));
				arr1Normals->push_back(*(attrPrinter.m_Vec3Normals + j));
				arr2Normals->push_back(*(attrPrinter.m_Vec3Normals + j + 1));
				arr3Normals->push_back(*(attrPrinter.m_Vec3Normals + j + 2));

				arrTexture->push_back(*(attrPrinter.m_Vec2Texture + j));
				arrTexture->push_back(*(attrPrinter.m_Vec2Texture + j + 1));
				arrTexture->push_back(*(attrPrinter.m_Vec2Texture + j + 2));
				arr1Texture->push_back(*(attrPrinter.m_Vec2Texture + j));
				arr2Texture->push_back(*(attrPrinter.m_Vec2Texture + j + 1));
				arr3Texture->push_back(*(attrPrinter.m_Vec2Texture + j + 2));
			}


		}
		//printf("arr1Vertices:%d\n", (int)arr1Vertices->size());
		//printf("arr1Normals:%d\n", (int)arr1Normals->size());
		//printf("arr1Texture:%d\n", (int)arr1Texture->size());
		traverse(node);
	}
};
#endif //FINDGEOMETRYVISTOR_H
