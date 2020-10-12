#include "CreatePolygon3DHandler.h"
#include <QMessageBox>

CreatePolygon3DHandler::CreatePolygon3DHandler(osgViewer::Viewer *viewer) :mViewer(viewer),isOverFlag(false)
{

};

bool CreatePolygon3DHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	//根据鼠标的操作类型，进行相应的事件绑定
	switch (ea.getEventType())
	{
		case osgGA::GUIEventAdapter::PUSH:
		{
			printf("按下鼠标\n");
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				printf("按的是鼠标左键\n");

				m_FirstPushPointX = ea.getX();
				m_FirstPushPointY = ea.getY();
				isOverFlag = false;
			}
			if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{
				printf("按的是鼠标右键\n");
			}
			if (ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON)
			{
				printf("按的是鼠标中键\n");
			}
			break;
		}
		case osgGA::GUIEventAdapter::MOVE:
		{
			if (!isOverFlag)
			{
				PickMove(ea.getX(), ea.getY());
			}
			break;
		}
		case osgGA::GUIEventAdapter::RELEASE:
		{
			printf("鼠标弹起\n");
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				printf("弹起的是左键\n");
				m_FirstReleasePointX = ea.getX();
				m_FirstReleasePointY = ea.getY();

				if (std::abs(m_FirstReleasePointX - m_FirstPushPointX) < 1 && std::abs(m_FirstReleasePointY - m_FirstPushPointY) < 1)
				{
					Pick(ea.getX(), ea.getY());
				}
			}
			if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{
				printf("弹起的是右键\n");
				if (vecDrawOsgPoint3d.size() < 3)
				{
					return false;
				}
				else
				{
					PickFinished();
				}
			}
			break;
		}
		case osgGA::GUIEventAdapter::KEYDOWN:
		{
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Z)
			{
				printf("按下的是键盘Z键\n");
				//printf("&d\n", ea.getKey());
				//QMessageBox::information(NULL, "1", "1", QMessageBox::Ok, QMessageBox::Ok);
			}
			break;
		}
	}
	return false;
}

void CreatePolygon3DHandler::Pick(float x, float y)
{
	if (vecDrawOsgPoint3d.size() == 0)
	{
		osg::Vec3d pickedPoint;

		//判断通过xy坐标是否能够获得node
		if (GetInsectionPointFromXY(pickedPoint, x, y))
		{
			//vecPoint3d.push_back(pickedPoint);
			vecDrawOsgPoint3d.push_back(pickedPoint);
		}
		else
		{
			QMessageBox::information(NULL, "Info", "first must on model", QMessageBox::Ok, QMessageBox::Ok);
			return;
		}
	}
	else//1.2 从点选第二个点开始，所点选的点可以不在模型上，如果不在模型上，则改点选点的高程采取前一个点的高程
	{
		osg::Vec3d pickedPoint;

		//判断通过xy坐标是否能够获得node，如果可以，说明此时点选的坐标点在模型上
		if (GetInsectionPointFromXY(pickedPoint, x, y))
		{
			//vecPoint3d.push_back(pickedPoint);
			vecDrawOsgPoint3d.push_back(pickedPoint);
		}
		else
		{
			//说明此时点选的坐标点不在模型上

			//创建一个临时平面
			//创建一个叶节点对象
			osg::ref_ptr<osg::Geode> tempPlaneGeode = new osg::Geode();
			osg::Vec3 tempOsgPoint3dCenter(0, 0, 0);
			double tempWidth = 999999;
			double tempheight = 999999;
			//double tempZValue = vecPoint3d.back().z();
			double tempZValue = vecDrawOsgPoint3d.back().z();

			//宽度和高度，暂时先写死，基本上这个范围内，也包含了所有的倾斜模型
			//后续考虑直接通过射线和已知高程水平面的交点，用数学方法直接求得其坐标，这样的话效果最好
			CreateTempPlaneGeode(*tempPlaneGeode, tempOsgPoint3dCenter, tempWidth, tempheight, tempZValue);

			//临时添加
			mViewer->getSceneData()->asGroup()->addChild(tempPlaneGeode.get());

			//计算鼠标点选射线与临时平面的交点
			if (GetInsectionPointFromXY(pickedPoint, x, y))
			{
				//vecPoint3d.push_back(pickedPoint);
				vecDrawOsgPoint3d.push_back(pickedPoint);
			}

			//计算出pickedPoint后，移除tempPlaneGeode
			mViewer->getSceneData()->asGroup()->removeChild(tempPlaneGeode.get());
		}
	}

	//2 更新节点球
	//2.1 先移除
	mViewer->getSceneData()->asGroup()->removeChild(pGroupUnitSphere);

	//2.2 再添加
	pGroupUnitSphere = new osg::Group;
	for (size_t indexPoint3d = 0; indexPoint3d < vecDrawOsgPoint3d.size(); indexPoint3d++)
	{
		osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(vecDrawOsgPoint3d.at(indexPoint3d), 0.01f);
		osg::ref_ptr<osg::ShapeDrawable> shapeDrawable = new osg::ShapeDrawable(sphere.get());
		shapeDrawable->setColor(osg::Vec4(1.0f, 1.0f, 0.0f, 0.5f));
		osg::ref_ptr<osg::Geode> unitSphere = new osg::Geode;
		unitSphere->addDrawable(shapeDrawable.get());
		unitSphere->setName("unitSphere_" + std::to_string((int)indexPoint3d));
		pGroupUnitSphere->addChild(unitSphere.get());
	}
	mViewer->getSceneData()->asGroup()->addChild(pGroupUnitSphere);

	//3 更新裁剪边界线
	//3.1 先移除
	mViewer->getSceneData()->asGroup()->removeChild(pLineStringGeodeMove);
	//mViewer->getSceneData()->asGroup()->removeChild(pLineStringGeode);

	//3.2 再添加
	pLineStringGeode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
	////创建顶点数组
	osg::ref_ptr<osg::Vec3Array> arrDrawOsgPoint3d = new osg::Vec3Array;

	//创建颜色数组
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	//
	for (size_t i = 0; i < vecDrawOsgPoint3d.size(); i++)
	{
		arrDrawOsgPoint3d->push_back(vecDrawOsgPoint3d[i]);
		colors->push_back(osg::Vec4(1.0, 1.0, 0.0, 0.5));
	}

	geometry->setVertexArray(arrDrawOsgPoint3d);
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::ref_ptr<osg::PrimitiveSet> primitiveSet = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, vecDrawOsgPoint3d.size());
	geometry->addPrimitiveSet(primitiveSet);

	//设置线宽
	osg::ref_ptr<osg::LineWidth> lw = new osg::LineWidth(6.0f);
	geometry->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON);

	pLineStringGeode->addDrawable(geometry);
	pLineStringGeode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	mViewer->getSceneData()->asGroup()->addChild(pLineStringGeode);
}

void CreatePolygon3DHandler::PickMove(float x, float y)
{
	//点选第一个点，必须在模型上
	if (vecDrawOsgPoint3d.size() > 0)
	{
		osg::Vec3d pickedPoint;

		//判断通过xy坐标是否能够获得node
		if (GetInsectionPointFromXY(pickedPoint, x, y))
		{
			vecDrawOsgPoint3d.push_back(pickedPoint);
		}
		else
		{
			//构建一个999999*999999的临时平面
			osg::ref_ptr<osg::Geode> tempGeode = new osg::Geode();
			osg::Vec3 tempOsgPoint3dCenter(0, 0, 0);
			double tempWidth = 999999;
			double tempheight = 999999;
			double tempZValue = vecDrawOsgPoint3d.back().z();

			CreateTempPlaneGeode(*tempGeode, tempOsgPoint3dCenter, tempWidth, tempheight, tempZValue);

			//临时添加
			mViewer->getSceneData()->asGroup()->addChild(tempGeode.get());

			if (GetInsectionPointFromXY(pickedPoint, x, y))
			{
				vecDrawOsgPoint3d.push_back(pickedPoint);
			}
			//计算出pickedPoint后，移除tempGeode
			mViewer->getSceneData()->asGroup()->removeChild(tempGeode);
		}

		mViewer->getSceneData()->asGroup()->removeChild(pLineStringGeodeMove);
		mViewer->getSceneData()->asGroup()->removeChild(pLineStringGeode);
		pLineStringGeodeMove = new osg::Geode;

		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
		////顶点数组
		osg::ref_ptr<osg::Vec3Array> arrDrawOsgPoint3d = new osg::Vec3Array;
		//设置颜色数组
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

		for (size_t i = 0; i < vecDrawOsgPoint3d.size(); i++)
		{
			arrDrawOsgPoint3d->push_back(vecDrawOsgPoint3d[i]);
			colors->push_back(osg::Vec4(1.0, 1.0, 0.0, 0.5));
		}

		geometry->setVertexArray(arrDrawOsgPoint3d);
		geometry->setColorArray(colors);
		geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

		osg::ref_ptr<osg::PrimitiveSet> primitiveSet = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, vecDrawOsgPoint3d.size());
		geometry->addPrimitiveSet(primitiveSet);

		//设置线宽
		osg::ref_ptr<osg::LineWidth> lw = new osg::LineWidth(6.0f);
		geometry->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON);

		pLineStringGeodeMove->addDrawable(geometry);
		pLineStringGeodeMove->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
		mViewer->getSceneData()->asGroup()->addChild(pLineStringGeodeMove);

		vecDrawOsgPoint3d.pop_back();
	}
}

void CreatePolygon3DHandler::PickFinished()
{
	//移除所有种子点
	mViewer->getSceneData()->asGroup()->removeChild(pGroupUnitSphere);
	mViewer->getSceneData()->asGroup()->removeChild(pLineStringGeodeMove);

	//初始化pGroupUnitSphere
	pGroupUnitSphere = new osg::Group;

	for (size_t indexPoint3d = 0; indexPoint3d < vecDrawOsgPoint3d.size(); indexPoint3d++)
	{
		osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(vecDrawOsgPoint3d.at(indexPoint3d), 0.01f);
		osg::ref_ptr<osg::ShapeDrawable> shapeDrawable = new osg::ShapeDrawable(sphere.get());
		shapeDrawable->setColor(osg::Vec4(1.0f, 1.0f, 0.0f, 0.5f));
		osg::ref_ptr<osg::Geode> unitSphere = new osg::Geode;
		unitSphere->addDrawable(shapeDrawable.get());
		unitSphere->setName("unitSphere_" + std::to_string((int)indexPoint3d));
		pGroupUnitSphere->addChild(unitSphere.get());
	}
	mViewer->getSceneData()->asGroup()->addChild(pGroupUnitSphere);

	if (vecDrawOsgPoint3d.size() > 1)
	{
		mViewer->getSceneData()->asGroup()->removeChild(pLineStringGeode);
		mViewer->getSceneData()->asGroup()->removeChild(pLineStringGeodeMove);
		pLineStringGeode = new osg::Geode;

		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
		////顶点数组
		osg::ref_ptr<osg::Vec3Array> arrDrawOsgPoint3d = new osg::Vec3Array;
		//设置颜色数组
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

		for (size_t i = 0; i < vecDrawOsgPoint3d.size(); i++)
		{
			arrDrawOsgPoint3d->push_back(vecDrawOsgPoint3d[i]);
			colors->push_back(osg::Vec4(1.0, 1.0, 0.0, 0.5));
		}

		geometry->setVertexArray(arrDrawOsgPoint3d);
		geometry->setColorArray(colors);
		geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

		osg::ref_ptr<osg::PrimitiveSet> primitiveSet = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, vecDrawOsgPoint3d.size());
		geometry->addPrimitiveSet(primitiveSet);

		//设置线宽
		osg::ref_ptr<osg::LineWidth> lw = new osg::LineWidth(6.0f);
		geometry->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON);

		pLineStringGeode->addDrawable(geometry);
		pLineStringGeode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
		mViewer->getSceneData()->asGroup()->addChild(pLineStringGeode);
		arrResultOsgPoint3d = arrDrawOsgPoint3d;
	}
	vecResultPoint3d = vecDrawOsgPoint3d;
	isOverFlag = true;

	for (size_t i = 0; i < vecDrawOsgPoint3d.size(); i++)
	{
		printf("vecDrawOsgPoint3d[%d]:%f    %f    %f\n", (int)i, vecDrawOsgPoint3d[i].x(), vecDrawOsgPoint3d[i].y(), vecDrawOsgPoint3d[i].z());
	}

	vecDrawOsgPoint3d.clear();
}


void CreatePolygon3DHandler::CreateTempPlaneGeode(osg::Geode& planeGeode, osg::Vec3 osgPoint3dCenter, double widthPlane, double heightPlane, double zValue)
{
	//创建一个几何体对象
	osg::ref_ptr<osg::Geometry> tempGeom = new osg::Geometry();

	//创建顶点数组，注意顶点的添加顺序是逆时针的
	osg::ref_ptr<osg::Vec3Array> tempVec3Array = new osg::Vec3Array();
	osg::Vec3d pt1 = screen2World(osgPoint3dCenter.x() - 0.5*widthPlane, osgPoint3dCenter.y() - 0.5*heightPlane);
	osg::Vec3d pt2 = screen2World(osgPoint3dCenter.x() + 0.5*widthPlane, osgPoint3dCenter.y() - 0.5*heightPlane);
	osg::Vec3d pt3 = screen2World(osgPoint3dCenter.x() + 0.5*widthPlane, osgPoint3dCenter.y() + 0.5*heightPlane);
	osg::Vec3d pt4 = screen2World(osgPoint3dCenter.x() - 0.5*widthPlane, osgPoint3dCenter.y() + 0.5*heightPlane);

	pt1.z() = zValue;
	pt2.z() = zValue;
	pt3.z() = zValue;
	pt4.z() = zValue;

	tempVec3Array->push_back(pt1);
	tempVec3Array->push_back(pt2);
	tempVec3Array->push_back(pt3);
	tempVec3Array->push_back(pt4);

	//设置tempGeom的顶点数据setVertexArray(Array *array)
	tempGeom->setVertexArray(tempVec3Array.get());

	tempGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	//添加到叶节点

	planeGeode.addDrawable(tempGeom.get());

}

bool CreatePolygon3DHandler::GetInsectionPointFromXY(osg::Vec3d &point, float x, float y)
{
	//创建一个射线交集（抽象地创建一条通过视点与鼠标点的射线（非实体），存储该射线所穿过的所有模型组成的集合的容器）
	osgUtil::LineSegmentIntersector::Intersections intersections;

	//获取到该交集
	if (mViewer->computeIntersections(x, y, intersections))
	{
		//当前“插入种子点”的操作，只需要我们获取到射线穿过的第一个模型
		osgUtil::LineSegmentIntersector::Intersection intersection = *intersections.begin();

		//获取该模型的nodePath
		osg::NodePath& nodePath = intersection.nodePath;
		//根据//printf出的出来的nodePath层次，选取想要的node
		//nodePath[0]			osg::Camera
		//nodePath[1]			osg::Group
		//nodePath[2]			osg::PagedLOD		其相当于单击到的一系列osgb文件中的Tile_+050_+192.osgb（暂时这么理解）

		//获取交点坐标
		point = intersection.getWorldIntersectPoint();
		return true;
	}
	else
	{
		return false;
	}
}

osg::Vec3 CreatePolygon3DHandler::screen2World(float x, float y)
{
	osg::Vec3 vec3;
	osg::ref_ptr<osg::Camera> camera = mViewer->getCamera();
	osg::Vec3 vScreen(x, y, 0);
	//printf("vScreen     x:%f     y:%f      z:%f", vScreen.x(), vScreen.y(), vScreen.z());

	osg::Matrix mVPW = camera->getViewMatrix() * camera->getProjectionMatrix() * camera->getViewport()->computeWindowMatrix();
	osg::Matrix invertVPW;
	invertVPW.invert(mVPW);
	vec3 = vScreen * invertVPW;
	//printf("vec3     x:%f     y:%f      z:%f", vec3.x(), vec3.y(), vec3.z());
	return vec3;
}


