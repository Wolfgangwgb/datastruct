#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<iostream>
#include<string>
using namespace std;
#include<assert.h>
#include"HuffmanTree.cpp"
#include<stdlib.h>
template <class T>
struct CompressLess
{
	bool operator()(const T* left, const T* right)
	{
		return left->_weight._strcount < right->_weight. _strcount;
	}
};

struct FileInfo
{
	FileInfo()
	:_strcount(0)
	{}

	FileInfo operator+(const FileInfo& x)
	{
		FileInfo tmp;
		tmp._strcount = _strcount + x._strcount;
		return tmp;
	}

	char _str;
	long long _strcount;
	string _strcode;
};
bool operator<(const FileInfo& left, const FileInfo& right)
{
	return left._strcount<right._strcount;;
}

class FileCompress
{
public:
	FileCompress()
	{
		for (size_t idx = 0; idx < 256; ++idx)
		{
			_file[idx]._str = idx;
		}
	}

	void Compress(const string Pathname)
	{
		//���ļ���ͳ�Ƹ���
		FILE* fp = fopen(Pathname.c_str(),"rb");
		assert(fp);
		char pRead[1024];
		while (true)
		{
			size_t tmp = fread(pRead, 1, 1024, fp);
			if (0 == tmp)
				break;

			for (size_t idx = 0; idx < tmp; ++idx)
			{
				_file[pRead[idx]]._strcount++;
			}
		}
		//����HuffmanTree
		HuffmanTree<FileInfo> ht(_file,256);
		
		//��ȡ����������
		HuffmanTreeNode<FileInfo>* root = ht.GetRoot();
		_GetHuffmanCode(root);


		//���¶�ȡ�ļ������б���
		fseek(fp,0,SEEK_SET);//���ļ�ָ�������ļ���ͷ
		char readfile[1024] = {0};
		char writefile[1024] = {0};//??????????δ��ʼ����������
		size_t writesize = 0;
		char value = 0;
		int pos = 0;
		string strcode;
		size_t n = Pathname.find_first_of('.',0);
		string Compressfilename = Pathname.substr(0,n) + ".hzp";

		FILE* fin = fopen(Compressfilename.c_str(),"wb");
		assert(fin);
		////////////////�ļ���׺//////////////
		string Head = Pathname.substr(Pathname.find_last_of('.'), Pathname.size() - 1);
		fwrite(Head.c_str(),1,Head.size(),fin);
		fputc('\n',fin);

		//ͳ���кźͱ���������Ϣ
 		string Info;
		size_t Line = 0;
		char pitoa[128] = {0};
		for (size_t idx = 0; idx < 256; ++idx)
		{
			if (_file[idx]._strcount != 0)
			{
				Info += _file[idx]._str;
				Info += ',';
				Info += _itoa(_file[idx]._strcount, pitoa, 10);
				Line++;
			}
		}
		//����Ϣ
		
		string LineH = _itoa(Line, pitoa, 10);
		fwrite(LineH.c_str(),1,LineH.size(),fin);
		fputc('\n', fin);
		//������Ϣ
		size_t head = 0;
		size_t tail = 3;
		while (head != Info.size())
		{
			//substr(��ʼλ�ã���ȡ����)
			string str = Info.substr(head, tail);
			fwrite(Info.substr(head, tail).c_str(), 1, Info.substr(head, tail).size(), fin);
			fputc('\n', fin);
			head += 3;
		}
		
		while (true)
		{
			size_t ret = fread(readfile,1,1024,fp);
			if (0 == ret)
				break;
			for (size_t idx = 0; idx < ret; ++idx)
			{
				strcode = _file[readfile[idx]]._strcode;
				for (size_t idx = 0; idx < strcode.size(); )
				{
					value <<= 1;
					if (strcode[idx] == '1')
					{
						value |= 1;
					}
					else
					{
						value |= 0;
					}
					pos++;
					if (pos == 8)
					{
						writefile[writesize++] = value;
						pos = 0;
						value = 0;
					}
					++idx;///????
				}
			}
			if (pos)/////???
			{
				value <<= (8 - pos);
				writefile[writesize++] = value;
			}
			fwrite(writefile,1,writesize,fin);//????д���ļ�ʱ��С����
			writesize = 0;
		}
		//delete[] readfile;
		//delete[] writefile;
		fclose(fp);
		fclose(fin);
	}

	void UnCompress(const string PathName)
	{
		//���ļ�
		FILE* fp = fopen(PathName.c_str(),"rb");
		assert(fp);
		//string DataInfo;

		//��ȡ��׺
		string Path;
		ReadLine(fp, Path);
		string UnCompressPathName = PathName.substr(0,PathName.find_first_of('.',0))+Path;
		//��ȡ�к�
		string Line;
		ReadLine(fp,Line);
		size_t line = atoi(Line.c_str());
		//��ȡ������Ϣ
		for (size_t idx = 0; idx < 4; ++idx)
		{
			string CodeInfo;
			ReadLine(fp, CodeInfo);
			_file[CodeInfo[0]]._strcount = atoi(CodeInfo.substr(2).c_str());
			CodeInfo.clear();
		}
		//��ԭHuffmanTree
		HuffmanTree<FileInfo> ht(_file, 256);
		//����
		
		char readfile[1024] = {0};
		char writefile[1024] = {0};
		size_t writesize = 0;
		size_t pos = 8;
		HuffmanTreeNode<FileInfo>* Root = ht.GetRoot();
		HuffmanTreeNode<FileInfo>* pCur = Root;
		FILE* fin = fopen(UnCompressPathName.c_str(), "wb");
		assert(fin);
		while (feof(fp)==0)
		{
				size_t readsize = fread(readfile,1,1024,fp);
			if (readsize == 0)
				break;
			for (size_t idx = 0; idx < readsize; ++idx)
			{
				char ch = readfile[idx];
				pos = 8;
				while (true)
				{
					if ((ch & 0x80) == 0)
					{
						pCur = pCur->_pLeft;
					}
					else
					{
						pCur = pCur->_pRight;
					}
					if (NULL == pCur->_pLeft && NULL == pCur->_pRight)
					{
						writefile[writesize++] = pCur->_weight._str;
						pCur = Root;
					}
					ch <<= 1;
					--pos;
					if (0 == pos)
						break;
				}
			}
			if (writesize)
				fwrite(writefile,1,1024,fin);
		}

	}
private:
	void _GetHuffmanCode(HuffmanTreeNode<FileInfo>* Root)//��ȡ����������������ˣ�����û���޸�
	{
		if (NULL == Root)
			return;
		if (Root)
		{
			_GetHuffmanCode(Root->_pLeft);
			_GetHuffmanCode(Root->_pRight);
			if (NULL == Root->_pLeft && NULL == Root->_pRight)
			{
				HuffmanTreeNode<FileInfo>* pCur = Root;
				HuffmanTreeNode<FileInfo>* parent = Root->_pParent;//??
				string code = _file[pCur->_weight._str]._strcode;//??
				while (parent)
				{
					if (parent->_pLeft == pCur)
					{
						code += "0";
					}
					else
					{
						code += "1";
					}
					pCur = pCur->_pParent;
					parent = pCur->_pParent;
				}
				reverse(code.begin(),code.end());
				_file[Root->_weight._str]._strcode = code;//???
			}
		}
	}

	void ReadLine(FILE* f,string& DataInfo)
	{
		while (feof(f) == 0)
		{
			char ch = fgetc(f);
			if (ch == '\n')
				break;
			DataInfo += ch;
		}
	}
private:
	FileInfo _file[256];
};

void Test()
{
	FileCompress f;
	f.Compress("1.txt");
	//f.UnCompress("1.hzp");
}