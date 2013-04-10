#include "StdAfx.h"
#include "Model3DSPhraser.h"
using namespace modelphraser;

modelphraser::Model3DSPhraser::Model3DSPhraser(void):m_pFile(0)
{
}


modelphraser::Model3DSPhraser::~Model3DSPhraser(void)
{
}

bool Model3DSPhraser::loadFile(wchar_t *filename){
	_wfopen_s(&m_pFile,filename,L"rb");
	if(!m_pFile)	return false;
	CHUNK chunk=readChunk();
	if(chunk.id!=PRIM)	return false;
	phrasePrim(chunk);
	fclose(m_pFile);
	return true;
}

void Model3DSPhraser::phrasePrim(CHUNK& prim){
	m_currentStack.push(prim);
	DWORD count=0;
	DWORD length=CHUNKL(prim);
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case PRIM_EDIT:
			phraseEdit(chunk);
			break;
			
		case PRIM_KEY:
			phraseKey(chunk);
			break;

		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}
	m_currentStack.pop();
}
void Model3DSPhraser::phraseKey(CHUNK& key){
	m_currentStack.push(key);
	DWORD count=0;
	DWORD length=CHUNKL(key);
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case KEY_INFO:
			{
				DWORD kf1,kf2;
				kf1=readDWord();
				kf2=readDWord();
				int i=0;
			}
			break;
		case KEY_MESH:
			phraseKeyMesh(chunk);
			break;
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}

	m_currentStack.pop();
}
void Model3DSPhraser::phraseKeyMesh(CHUNK& keymesh){
	m_currentStack.push(keymesh);
	DWORD count=0;
	DWORD length=CHUNKL(keymesh);
	keyMeshStart();
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case KEYF_HIERARCY:
			{
				char name[64];
				readString(name);
				readWord();
				readWord();
				int t=readWord();
				keyMeshName(name,t);
			}
			break;
			
		case KEYF_DUMMY:
			{
				char name[64];
				readString(name);
				keyDummyMeshName(name);
			}
			//skipChunk(chunk);
			break;
		case KEYF_PIVOT:
			{
				float pf1,pf2,pf3;
				pf1=readFloat();
				pf2=readFloat();
				pf3=readFloat();
				keyFramePivot(pf1,pf2,pf3);
			}
			break;
		case TRACK_POS:
			phraseKeyPos(chunk);
			break;
		case TRACK_ROT:
			phraseKeyRotate(chunk);
			break;
		case TRACK_SCL:
			phraseKeyScale(chunk);
			break;
		case KEYF_NODEID:
			skipChunk(chunk);
			break;
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}
	keyMeshEnd();

	m_currentStack.pop();
}
void Model3DSPhraser::phraseKeyPos(CHUNK& poschunk){
	SKIP5WORDS();
	WORD num=readWord();
	readWord();
	while(num>0)
	{
		WORD framenum=readWord();
		readDWord();
		float x,y,z;
		x=readFloat();
		y=readFloat();
		z=readFloat();
		keyFramePos(framenum,x,y,z);
		num--;
	}
}
void Model3DSPhraser::phraseKeyRotate(CHUNK& rotatechunk){
	skipChunk(rotatechunk);
}
void Model3DSPhraser::phraseKeyScale(CHUNK& scalechunk){
	skipChunk(scalechunk);
}

void Model3DSPhraser::phraseEdit(CHUNK& edit){
	m_currentStack.push(edit);
	DWORD count=0;
	DWORD length=CHUNKL(edit);
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case EDIT_MESH:
			phraseMesh(chunk);
			break;
		case EDIT_MAT:
			phraseMatrial(chunk);
			break;
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}

	m_currentStack.pop();
}

void Model3DSPhraser::phraseMatrial(CHUNK& mat){
	m_currentStack.push(mat);
	DWORD count=0;
	DWORD length=CHUNKL(mat);
	matrialBegin();
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case MAT_NAME:
			{
				char name[128];
				readString(name);
				matrialName(name);
				int i=0;
			}
			break;
		case MAT_MAP:
			phraseMatMap(chunk);
			break;
		case MAT_AMB:
			phraseMatAbm(chunk);
			break;
		case MAT_DIF:
			phraseMatDif(chunk);
			break;
		case MAT_SPE:
			phraseMatSpec(chunk);
			break;
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}
	matrialEnd();
	m_currentStack.pop();
}
void Model3DSPhraser::phraseMatAbm(CHUNK& abm){
	DWORD count=0;
	DWORD length=CHUNKL(abm);
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case GLOBAL_RGB_BYTE:
			BYTE r,g,b;
			r=readByte();
			g=readByte();
			b=readByte();
			matrialAmb3b(r,g,b);
			break;
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}

}
void Model3DSPhraser::phraseMatDif(CHUNK& dif){
	DWORD count=0;
	DWORD length=CHUNKL(dif);
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case GLOBAL_RGB_BYTE:
			BYTE r,g,b;
			r=readByte();
			g=readByte();
			b=readByte();
			matrialDif3b(r,g,b);
			break;
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}

}
void Model3DSPhraser::phraseMatSpec(CHUNK& spec){
	DWORD count=0;
	DWORD length=CHUNKL(spec);
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case GLOBAL_RGB_BYTE:
			BYTE r,g,b;
			r=readByte();
			g=readByte();
			b=readByte();
			matrialSpec3b(r,g,b);
			break;
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}

}
void Model3DSPhraser::phraseMatMap(CHUNK& mat){
	m_currentStack.push(mat);
	DWORD count=0;
	DWORD length=CHUNKL(mat);
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case MAP_NAME:
			{
				char name[128];
				readString(name);
				matrialtextureName(name);
				int i=0;
			}
			break;
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}

	m_currentStack.pop();
}
void Model3DSPhraser::phraseMesh(CHUNK& mesh){
	m_currentStack.push(mesh);
	DWORD count=0;
	DWORD length=CHUNKL(mesh);
	char name[128];
	int l=readString(name);
	count+=l;
	meshGroupStart(name);
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case MESH_INFO:
			phraseMeshInfo(chunk);
			break;
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}
	meshGroupEnd(name);
	m_currentStack.pop();
}

void Model3DSPhraser::phraseMeshInfo(CHUNK& meshInfo){
	m_currentStack.push(meshInfo);	
	DWORD count=0;
	DWORD length=CHUNKL(meshInfo);
	
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case MESH_VERTEX:
			phraseVertex(chunk);
			break;
		case MESH_MAPCOORD:
			phraseTexcoord(chunk);
			break;
		case MESH_FACET:
			phraseFaceInfo(chunk);
			break;
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}
	m_currentStack.pop();

}

void Model3DSPhraser::phraseVertex(CHUNK& vertex){
	m_currentStack.push(vertex);	
	WORD numV=readWord();
	float x,y,z;
	for(int i=0;i<numV;i++)
	{
		x=readFloat();
		y=readFloat();
		z=readFloat();
		vertex3f(x,y,z);
	}
	m_currentStack.pop();

}

void Model3DSPhraser::phraseTexcoord(CHUNK& tex){
	m_currentStack.push(tex);	
	WORD numV=readWord();
	float u,v;
	for(int i=0;i<numV;i++)
	{
		u=readFloat();
		v=readFloat();
		texCoord2f(u,v);
	}
	m_currentStack.pop();

}
void Model3DSPhraser::phraseFaceInfo(CHUNK& info){
	m_currentStack.push(info);	
	DWORD count=0;
	DWORD length=CHUNKL(info);
	WORD num=readWord();
	count+=2;
	WORD i1,i2,i3;
	for(int i=0;i<num;i++)
	{
		i1=readWord();
		i2=readWord();
		i3=readWord();
		readWord();
		count+=8;
		face(i1,i2,i3);
	}
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case FACET_MAT:
			char mat[128];
			readString(mat);
			meshMatBegin(mat);
			num=readWord();
			while(num>0){
				WORD f=readWord();
				usingMatFace(f);
				num--;
			}
			meshMatEnd(mat);
			break;
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}
	m_currentStack.pop();

}