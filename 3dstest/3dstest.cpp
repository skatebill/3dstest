// 3dstest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <malloc.h>
#include <list>
#include <vector>
#include <fstream>
#include "Model3DSPhraser.h"
using namespace modelphraser;
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
struct colorByte{
	BYTE r,g,b;
	colorByte(BYTE _r=0,BYTE _g=0,BYTE _b=0){r=_r;b=_b;g=_g;}
};
struct matrial{
	colorByte dif;
	colorByte spec;
	colorByte abm;
	char name[64];
	char texcoordName[64];
};
class test:public Model3DSPhraser{
private:
	list<meshgroup*> m_GroupList;
	vector<matrial*> matriallist;
	meshgroup* mCurGroup;
	faceWmat* mCurMatFace;
	matrial* mCurMat;
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
			mCurMatFace = new faceWmat;
			strcpy(mCurMatFace->name,matName);

		}
		virtual void meshMatEnd(char* matName){
			mCurGroup->fwmList.push_back(mCurMatFace);
			mCurMatFace=0;
		}
		virtual void usingMatFace(int index){
			mCurMatFace->triangleList.push_back(index);
		}
		virtual void matrialBegin(){
			mCurMat=new matrial;
		}
		virtual void matrialEnd(){
			matriallist.push_back(mCurMat);
			mCurMat=0;
		}
		virtual void matrialName(char* name){
			strcpy(mCurMat->name,name);

		}
		virtual void matrialtextureName(char* name){
			strcpy(mCurMat->texcoordName,name);

		}
		virtual void matrialAmb3b(BYTE r,BYTE g,BYTE b){
			mCurMat->abm.r=r;
			mCurMat->abm.g=g;
			mCurMat->abm.b=b;
		}
		virtual void matrialDif3b(BYTE r,BYTE g,BYTE b){
			mCurMat->dif.r=r;
			mCurMat->dif.g=g;
			mCurMat->dif.b=b;

		}
		virtual void matrialSpec3b(BYTE r,BYTE g,BYTE b){
			mCurMat->spec.r=r;
			mCurMat->spec.g=g;
			mCurMat->spec.b=b;

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
		
		cout<<"MAT num: "<<matriallist.size()<<endl;
		vector<matrial*>::iterator matite= matriallist.begin();
			int n=0;
		while(matite!= matriallist.end())
		{
			cout<<"\tmartrial #"<<n<<" name: "<<(*matite)->name<<endl;
			cout<<"\t\t texname: "<<(*matite)->texcoordName<<endl;
			matite++;n++;
		}
	}

};
int _tmain(int argc, _TCHAR* argv[])
{
	test t;
	freopen("log.txt", "w", stdout);
	bool r=t.loadFile(L"test2.3DS");
	t.yeld();
	return 0;
}


