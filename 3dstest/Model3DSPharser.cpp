#include "StdAfx.h"
#include "Model3DSPharser.h"
using namespace modelpharser;

modelpharser::Model3DSPharser::Model3DSPharser(void):m_pFile(0)
{
}


modelpharser::Model3DSPharser::~Model3DSPharser(void)
{
}

bool Model3DSPharser::loadFile(wchar_t *filename){
	_wfopen_s(&m_pFile,filename,L"rb");
	if(!m_pFile)	return false;
	CHUNK chunk=readChunk();
	if(chunk.id!=PRIM)	return false;
	pharsePrim(chunk);
	fclose(m_pFile);
	return true;
}

void Model3DSPharser::pharsePrim(CHUNK& prim){
	m_currentStack.push(prim);
	DWORD count=0;
	DWORD length=CHUNKL(prim);
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case PRIM_EDIT:
			pharseEdit(chunk);
			break;
			
		case PRIM_KEY:
			pharseKey(chunk);
			break;

		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}
	m_currentStack.pop();
}
void Model3DSPharser::pharseKey(CHUNK& key){
	m_currentStack.push(key);
	skipChunk(key);

	m_currentStack.pop();
}

void Model3DSPharser::pharseEdit(CHUNK& edit){
	m_currentStack.push(edit);
	DWORD count=0;
	DWORD length=CHUNKL(edit);
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case EDIT_MESH:
			pharseMesh(chunk);
			break;
		case EDIT_MAT:
			pharseMatrial(chunk);
			break;
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}

	m_currentStack.pop();
}

void Model3DSPharser::pharseMatrial(CHUNK& mat){
	m_currentStack.push(mat);
	DWORD count=0;
	DWORD length=CHUNKL(mat);
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case MAT_NAME:
			{
				char name[128];
				readString(name);
				int i=0;
			}
			break;
		case MAT_MAP:
			phraseMatMap(chunk);
			break;
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}

	m_currentStack.pop();
}
void Model3DSPharser::phraseMatMap(CHUNK& mat){
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
void Model3DSPharser::pharseMesh(CHUNK& mesh){
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
			pharseMeshInfo(chunk);
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

void Model3DSPharser::pharseMeshInfo(CHUNK& meshInfo){
	m_currentStack.push(meshInfo);	
	DWORD count=0;
	DWORD length=CHUNKL(meshInfo);
	
	while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		case MESH_VERTEX:
			pharseVertex(chunk);
			break;
		case MESH_MAPCOORD:
			pharseTexcoord(chunk);
			break;
		case MESH_FACET:
			pharseFaceInfo(chunk);
			break;
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}
	m_currentStack.pop();

}

void Model3DSPharser::pharseVertex(CHUNK& vertex){
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

void Model3DSPharser::pharseTexcoord(CHUNK& tex){
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
void Model3DSPharser::pharseFaceInfo(CHUNK& info){
	m_currentStack.push(info);	
	DWORD count=0;
	DWORD length=CHUNKL(info);
	WORD num=readWord();
	count+=2;
	/*while(count<length){
		CHUNK chunk=readChunk();
		switch(chunk.id){
		default:
			skipChunk(chunk);
			break;
		}
		count+=chunk.length;
	}*/
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