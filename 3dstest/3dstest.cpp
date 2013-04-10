// 3dstest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <malloc.h>
#include <list>
#include <vector>
#include "Model3DSPharser.h"
using namespace modelpharser;
struct Point3f{
	float x,y,z;
	Point3f(float _x=0,float _y=0,float _z=0){x=_x;y=_y;z=_z;}
};
struct Texcoord2f{
	float u,v;
	Texcoord2f(float _u=0,float _v=0){u=_u;v=_v;}
};
struct triangleface{
	WORD a,b,c;
	triangleface(WORD _a=0,WORD _b=0,WORD _c=0){a=_a;b=_b;c=_c;}
};
struct faceWmat{	
	char name[128];
	vector<WORD> triangleList;
};
struct meshgroup{
	char name[128];
	vector<Point3f*> pointList;
	vector<Texcoord2f*> texList;
	vector<triangleface*> faceList;
	vector<faceWmat*> fwmList;
};
class test:public Model3DSPharser{
private:
	list<meshgroup*> m_GroupList;
	meshgroup* mCurGroup;
	faceWmat* mCurMat;
protected:	
		virtual void meshGroupStart(char* meshName)
		{
			mCurGroup=new meshgroup;
			strcpy(mCurGroup->name,meshName);
		}
		virtual void meshGroupEnd(char* meshName)
		{
			m_GroupList.push_back(mCurGroup);
			mCurGroup=0;
		}
		virtual void vertex3f(float x,float y,float z)
		{
			mCurGroup->pointList.push_back(new Point3f(x,y,z));
		}
		virtual void texCoord2f(float u, float v){
			mCurGroup->texList.push_back(new Texcoord2f(u,v));
		}
		virtual void face(int a,int b,int c){
			mCurGroup->faceList.push_back(new triangleface(a,b,c));
		}
		virtual void meshMatBegin(char* matName){
			mCurMat = new faceWmat;
			strcpy(mCurMat->name,matName);

		}
		virtual void meshMatEnd(char* matName){
			mCurGroup->fwmList.push_back(mCurMat);
			mCurMat=0;
		}
		virtual void usingMatFace(int index){
			mCurMat->triangleList.push_back(index);
		}
public:
	test(){};
	virtual ~test(){}
	void yeld(){
		list<meshgroup*>::iterator ite=m_GroupList.begin();
		while(ite!=m_GroupList.end())
		{
			cout<<"GROUP name: "<<(*ite)->name<<endl;
			cout<<"\tpoint num: "<<(*ite)->pointList.size()<<endl;
			cout<<"\ttexcoord num: "<<(*ite)->texList.size()<<endl;
			cout<<"\ttriangle num: "<<(*ite)->faceList.size()<<endl;
			cout<<"\tmaterial num: "<<(*ite)->fwmList.size()<<endl;
			vector<faceWmat*>::iterator mite=(*ite)->fwmList.begin();
			int n=0;
			while(mite!=(*ite)->fwmList.end())
			{
				cout<<"\t\tmatrial #"<<n<<"\t name: "<<(*mite)->name<<endl;
				cout<<"\t\tmatrial #"<<n<<"\t trianleNum: "<<(*mite)->triangleList.size()<<endl;
				mite++;n++;
			}
			ite++;
		}
	}

};
int _tmain(int argc, _TCHAR* argv[])
{
	test t;
	bool r=t.loadFile(L"test2.3DS");
	t.yeld();
	return 0;
}


