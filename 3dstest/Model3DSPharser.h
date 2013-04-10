#pragma once

#include <stdio.h>
#include <iostream>
#include <wchar.h>
#include <stack>
using namespace std;

#define BYTE   unsigned char
#define WORD	unsigned short
#define DWORD	unsigned int
#define CHUNK_HEAD_L 6
#define CHUNKL(x)	x.length-6

namespace modelpharser{
	
	//begin primary chunk
	const WORD PRIM =0x4D4D;
		//begin edit chunk
		const WORD PRIM_EDIT =0x3D3D;
			//begin mesh chunk
			 const WORD EDIT_MESH =0x4000;
				 const WORD MESH_INFO =0x4100;
					 const WORD MESH_VERTEX =0x4110;
					 const WORD MESH_FACET  =0x4120;
						 const WORD FACET_MAT=0x4130;
						 const WORD FACET_SMOOTH =0x4150;
					 const WORD MESH_MAPCOORD =0x4140;
					 const WORD MESH_LOCAL    =0x4160;
			//end mesh chunk

			//begin material chunk
			const WORD EDIT_MAT =0xafff;
				const WORD MAT_NAME =0xa000;
				const WORD MAT_AMB  =0xa010;
				const WORD MAT_DIF  =0xa020;
				const WORD MAT_SPE  =0xa030;
				const WORD MAT_MAP  =0xa200;
					const WORD MAP_NAME =0xa300;
			//end material chunk

		//end edit chunk

		//begin keyframe chunk
		const WORD PRIM_KEY =0xB000;
			 const WORD KEY_INFO =0xB008;
			 const WORD KEY_MESH =0xB002;
				 const WORD KEYF_HIERARCY =0xB010;
				 const WORD KEYF_DUMMY    =0xB011;
				 const WORD KEYF_PIVOT    =0xB013;
				 const WORD TRACK_POS     =0xB020;
				 const WORD TRACK_ROT     =0xB021;
				 const WORD TRACK_SCL     =0xB022;
				 const WORD KEYF_NODEID   =0xB030;
		//end keyframe chunk

		//some chunks in all others
		const WORD GLOBAL_RGB_FLOAT       =0x0010;
		const WORD GLOBAL_RGB_BYTE        =0x0011;
		const WORD GLOBAL_RGB_BYTE_GAMMA  =0x0012;
		const WORD GLOBAL_RGB_FLOAT_GAMMA =0x0013;
		const WORD GLOBAL_PER_WORD        =0x0030;
		const WORD GLOBAL_PER_FLOAT       =0x0031;
	//end primary chunk

	struct CHUNK{
		WORD	id;
		DWORD	length;
	};
	class Model3DSPharser
	{
	private:
		FILE*	m_pFile;
		stack<CHUNK> m_currentStack;
	
		BYTE readByte()
		{
			BYTE r=0;
			fread(&r,1,1,m_pFile);
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
			fread(&r,4,1,m_pFile);
			return r;
		}
		CHUNK readChunk()
		{
			CHUNK r;
			r.id=readWord();
			r.length=readDWord();
			return r;
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
		
		void skipChunk(CHUNK& t)
		{
			fseek(m_pFile,t.length-6,SEEK_CUR);
		}
	protected:
		virtual void meshGroupStart(char* meshName)
		{

		}
		virtual void meshGroupEnd(char* meshName)
		{

		}
		virtual void vertex3f(float x,float y,float z)
		{

		}
		virtual void texCoord2f(float u, float v){

		}
		virtual void face(int a,int b,int c){
			
		}
		virtual void meshMatBegin(char* matName){

		}
		virtual void meshMatEnd(char* matName){

		}
		virtual void usingMatFace(int index){

		}

	public:
		Model3DSPharser(void);
		virtual ~Model3DSPharser(void);

		bool loadFile(wchar_t* filename);
		void pharsePrim(CHUNK&);
		void pharseEdit(CHUNK&);
		void pharseKey(CHUNK&);
		void pharseMesh(CHUNK&);
		void pharseMeshInfo(CHUNK&);
		void pharseVertex(CHUNK&);
		void pharseTexcoord(CHUNK&);
		void pharseFaceInfo(CHUNK&);
		void pharseFace(CHUNK&);
	};

}