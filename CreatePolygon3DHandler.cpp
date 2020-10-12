#include "CreatePolygon3DHandler.h"
#include <QMessageBox>

CreatePolygon3DHandler::CreatePolygon3DHandler(osgViewer::Viewer *viewer) :mViewer(viewer),isOverFlag(false)
{

};

bool CreatePolygon3DHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	//�������Ĳ������ͣ�������Ӧ���¼���
	switch (ea.getEventType())
	{
		case osgGA::GUIEventAdapter::PUSH:
		{
			printf("�������\n");
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				printf("������������\n");

				m_FirstPushPointX = ea.getX();
				m_FirstPushPointY = ea.getY();
				isOverFlag = false;
			}
			if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{
				printf("����������Ҽ�\n");
			}
			if (ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON)
			{
				printf("����������м�\n");
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
			printf("��굯��\n");
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				printf("����������\n");
				m_FirstReleasePointX = ea.getX();
				m_FirstReleasePointY = ea.getY();

				if (std::abs(m_FirstReleasePointX - m_FirstPushPointX) < 1 && std::abs(m_FirstReleasePointY - m_FirstPushPointY) < 1)
				{
					Pick(ea.getX(), ea.getY());
				}
			}
			if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{
				printf("��������Ҽ�\n");
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
				printf("���µ��Ǽ���Z��\n");
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

		//�ж�ͨ��xy�����Ƿ��ܹ����node
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
	else//1.2 �ӵ�ѡ�ڶ����㿪ʼ������ѡ�ĵ���Բ���ģ���ϣ��������ģ���ϣ���ĵ�ѡ��ĸ̲߳�ȡǰһ����ĸ߳�
	{
		osg::Vec3d pickedPoint;

		//�ж�ͨ��xy�����Ƿ��ܹ����node��������ԣ�˵����ʱ��ѡ���������ģ����
		if (GetInsectionPointFromXY(pickedPoint, x, y))
		{
			//vecPoint3d.push_back(pickedPoint);
			vecDrawOsgPoint3d.push_back(pickedPoint);
		}
		else
		{
			//˵����ʱ��ѡ������㲻��ģ����

			//����һ����ʱƽ��
			//����һ��Ҷ�ڵ����
			osg::ref_ptr<osg::Geode> tempPlaneGeode = new osg::Geode();
			osg::Vec3 tempOsgPoint3dCenter(0, 0, 0);
			double tempWidth = 999999;
			double tempheight = 999999;
			//double tempZValue = vecPoint3d.back().z();
			double tempZValue = vecDrawOsgPoint3d.back().z();

			//��Ⱥ͸߶ȣ���ʱ��д���������������Χ�ڣ�Ҳ���������е���бģ��
			//��������ֱ��ͨ�����ߺ���֪�߳�ˮƽ��Ľ��㣬����ѧ����ֱ����������꣬�����Ļ�Ч�����
			CreateTempPlaneGeode(*tempPlaneGeode, tempOsgPoint3dCenter, tempWidth, tempheight, tempZValue);

			//��ʱ���
			mViewer->getSceneData()->asGroup()->addChild(tempPlaneGeode.get());

			//��������ѡ��������ʱƽ��Ľ���
			if (GetInsectionPointFromXY(pickedPoint, x, y))
			{
				//vecPoint3d.push_back(pickedPoint);
				vecDrawOsgPoint3d.push_back(pickedPoint);
			}

			//�����pickedPoint���Ƴ�tempPlaneGeode
			mViewer->getSceneData()->asGroup()->removeChild(tempPlaneGeode.get());
		}
	}

	//2 ���½ڵ���
	//2.1 ���Ƴ�
	mViewer->getSceneData()->asGroup()->removeChild(pGroupUnitSphere);

	//2.2 �����
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

	//3 ���²ü��߽���
	//3.1 ���Ƴ�
	mViewer->getSceneData()->asGroup()->removeChild(pLineStringGeodeMove);
	//mViewer->getSceneData()->asGroup()->removeChild(pLineStringGeode);

	//3.2 �����
	pLineStringGeode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
	////������������
	osg::ref_ptr<osg::Vec3Array> arrDrawOsgPoint3d = new osg::Vec3Array;

	//������ɫ����
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

	//�����߿�
	osg::ref_ptr<osg::LineWidth> lw = new osg::LineWidth(6.0f);
	geometry->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON);

	pLineStringGeode->addDrawable(geometry);
	pLineStringGeode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	mViewer->getSceneData()->asGroup()->addChild(pLineStringGeode);
}

void CreatePolygon3DHandler::PickMove(float x, float y)
{
	//��ѡ��һ���㣬������ģ����
	if (vecDrawOsgPoint3d.size() > 0)
	{
		osg::Vec3d pickedPoint;

		//�ж�ͨ��xy�����Ƿ��ܹ����node
		if (GetInsectionPointFromXY(pickedPoint, x, y))
		{
			vecDrawOsgPoint3d.push_back(pickedPoint);
		}
		else
		{
			//����һ��999999*999999����ʱƽ��
			osg::ref_ptr<osg::Geode> tempGeode = new osg::Geode();
			osg::Vec3 tempOsgPoint3dCenter(0, 0, 0);
			double tempWidth = 999999;
			double tempheight = 999999;
			double tempZValue = vecDrawOsgPoint3d.back().z();

			CreateTempPlaneGeode(*tempGeode, tempOsgPoint3dCenter, tempWidth, tempheight, tempZValue);

			//��ʱ���
			mViewer->getSceneData()->asGroup()->addChild(tempGeode.get());

			if (GetInsectionPointFromXY(pickedPoint, x, y))
			{
				vecDrawOsgPoint3d.push_back(pickedPoint);
			}
			//�����pickedPoint���Ƴ�tempGeode
			mViewer->getSceneData()->asGroup()->removeChild(tempGeode);
		}

		mViewer->getSceneData()->asGroup()->removeChild(pLineStringGeodeMove);
		mViewer->getSceneData()->asGroup()->removeChild(pLineStringGeode);
		pLineStringGeodeMove = new osg::Geode;

		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
		////��������
		osg::ref_ptr<osg::Vec3Array> arrDrawOsgPoint3d = new osg::Vec3Array;
		//������ɫ����
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

		//�����߿�
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
	//�Ƴ��������ӵ�
	mViewer->getSceneData()->asGroup()->removeChild(pGroupUnitSphere);
	mViewer->getSceneData()->asGroup()->removeChild(pLineStringGeodeMove);

	//��ʼ��pGroupUnitSphere
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
		////��������
		osg::ref_ptr<osg::Vec3Array> arrDrawOsgPoint3d = new osg::Vec3Array;
		//������ɫ����
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

		//�����߿�
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
	//����һ�����������
	osg::ref_ptr<osg::Geometry> tempGeom = new osg::Geometry();

	//�����������飬ע�ⶥ������˳������ʱ���
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

	//����tempGeom�Ķ�������setVertexArray(Array *array)
	tempGeom->setVertexArray(tempVec3Array.get());

	tempGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	//��ӵ�Ҷ�ڵ�

	planeGeode.addDrawable(tempGeom.get());

}

bool CreatePolygon3DHandler::GetInsectionPointFromXY(osg::Vec3d &point, float x, float y)
{
	//����һ�����߽���������ش���һ��ͨ���ӵ�����������ߣ���ʵ�壩���洢������������������ģ����ɵļ��ϵ�������
	osgUtil::LineSegmentIntersector::Intersections intersections;

	//��ȡ���ý���
	if (mViewer->computeIntersections(x, y, intersections))
	{
		//��ǰ���������ӵ㡱�Ĳ�����ֻ��Ҫ���ǻ�ȡ�����ߴ����ĵ�һ��ģ��
		osgUtil::LineSegmentIntersector::Intersection intersection = *intersections.begin();

		//��ȡ��ģ�͵�nodePath
		osg::NodePath& nodePath = intersection.nodePath;
		//����//printf���ĳ�����nodePath��Σ�ѡȡ��Ҫ��node
		//nodePath[0]			osg::Camera
		//nodePath[1]			osg::Group
		//nodePath[2]			osg::PagedLOD		���൱�ڵ�������һϵ��osgb�ļ��е�Tile_+050_+192.osgb����ʱ��ô��⣩

		//��ȡ��������
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


