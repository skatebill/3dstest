// 3dstest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <malloc.h>
#include "Model3DSPharser.h"
using namespace std;
#define BYTE   unsigned char
#define WORD	unsigned short
#define DWORD	unsigned int
FILE* pFile=0;
struct CHUNK{
	WORD	id;
	DWORD	length;
};
BYTE readByte()
{
	BYTE r=0;
	fread(&r,1,1,pFile);
	return r;
}
WORD readWord()
{
	return readByte()+(readByte()<<8);
}
DWORD readDWord()
{
	return readWord() + (readWord()<<16);
}
float readFloat()
{
	float r=0;
	fread(&r,4,1,pFile);
	return r;
}
CHUNK readChunk()
{
	CHUNK r;
	r.id=readWord();
	r.length=readDWord();
	return r;
}
void skipChunk(CHUNK& t)
{
	fseek(pFile,t.length-6,SEEK_CUR);
}
int readString(char* out)
{
	int i=0;
	char p=1;
	while(p)
	{
		p=readByte();
		out[i++]=p;
	}
	return i;
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*pFile=fopen("man.3DS","rb");
	if(!pFile)
	{
		cout<<"error load file!"<<endl;
	}
	CHUNK t;
	t=readChunk();
	t=readChunk();
	while(t.id!=0xB000)
	{
		skipChunk(t);
		t=readChunk();
	}
	t=readChunk();
/*	while(t.id!=0xB002)
	{
		skipChunk(t);
		t=readChunk();
	}
	t=readChunk();
	int id = readWord();
	t=readChunk();
	while(t.id!=0xB010)
	{
		skipChunk(t);
		t=readChunk();
	}
	char s[128];
	readString(s);
	readDWord();
	WORD i=readWord();
	t=readChunk();
	float f1=readFloat(),f2=readFloat(),f3=readFloat();
	t=readChunk();
	readWord();readWord();readWord();readWord();readWord();
	int num=readWord();
	int num2=readWord();
	for(int i=0;i<num;i++){
		int w=readWord();
		readDWord();
		float f1=readFloat(),f2=readFloat(),f3=readFloat();
			continue;
	};
	t=readChunk();
	readWord();readWord();readWord();readWord();readWord();
	num=readWord();
	num2=readWord();
	for(int i=0;i<num;i++){
		int w=readWord();
		readDWord();
		float f1=readFloat(),f2=readFloat(),f3=readFloat(),f4=readFloat();
			continue;
	};
	t=readChunk();
	readWord();readWord();readWord();readWord();readWord();
	num=readWord();
	num2=readWord();
	for(int i=0;i<num;i++){
		int w=readWord();
		readDWord();
		float f1=readFloat(),f2=readFloat(),f3=readFloat();
			continue;
	};
	t=readChunk();
	t=readChunk();
	num = readWord();
	t=readChunk();
	readString(s);
	readDWord();
	i=readWord();*/
	/*int it=0;
	char name[128];
	while(it<16)
	{
		while(t.id!=0xB002)
		{
			skipChunk(t);
			t=readChunk();
		}
		t=readChunk();
		while(t.id!=0xB010){
			skipChunk(t);
			t=readChunk();
		}
		readString(name);
		readDWord();
		WORD i=readWord();
		t=readChunk();
		cout<<name<<" location:"<<i<<endl;
		it++;
	}
	fclose(pFile);*/
	modelpharser::Model3DSPharser t;
	bool r=t.loadFile(L"test.3DS");
	return 0;
}

