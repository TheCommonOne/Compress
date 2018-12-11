#ifndef BINARYREADER_H
#define BINARYREADER_H
#include <fstream>
#include <iostream>
using namespace std;

/*
	�������ļ���ȡ������ȡ����Ϊbit
	�����˻��������ܹ���߶�ȡЧ��
*/
class BinaryReader
{
public:

	/*
		���캯����
		ifstream& fi	Ϊ�ļ�������������
		int bufSize		���ö�ȡ��������С
	*/
	BinaryReader(ifstream& fi, int bufSize)
	{
		if(!fi || !fi.good())
		{
			cout << "error" << endl;
		}
		this->fin = &fi;
		this->bufSize = bufSize;
		this->buffer = new char[bufSize];
		box = 0;
		bufIndex = 0;
		boxPos = 0;
		reload();	//Ԥ��һ��������
		if(gcount == 0)	//�ļ�Ϊ��
		{
			boxPos = 8;
		}
		else
		{
			box = buffer[bufIndex++];
		}
	}

	/*
		ÿ�δ��ļ��ж�ȡ���ݵ�������
	*/
	void reload()
	{
		fin->read(buffer, this->bufSize);
		gcount = fin->gcount();
	}

	/*
		���η����ļ��е�ÿ��bit
		�������ļ�ĩβ������'-'
	*/
	char getBit()
	{
		if(boxPos == 8)	//��ǰbox���Ѿ���ȡ��ϣ���ӻ�������ȡ��һ�ֽ�
		{
			if(bufIndex == gcount)	//�������Ѿ���ȡ�꣬���ٶ��ļ����ݵ�������
			{
				if(gcount < bufSize)	//�ϴ��Ѿ������ļ����򷵻�'-'֪ʶ��ȡ���
				{
					return '-';
				}
				reload();
				if(gcount == 0)
				{
					return '-';
				}
				bufIndex = 0;
			}
			box = buffer[bufIndex++];
			boxPos = 0;
		}
		if((box >> (7 - boxPos++)) & 1 == 1)
		{
			return '1';
		}
		else
		{
			return '0';
		}
	}

private:
	ifstream* fin;	//�ļ���ȡ��
	char* buffer;	//������
	char box;		//��ǰ��ȡ���ֽ�
	int bufSize;	//��������С
	int bufIndex;	//��������ǰλ��
	int boxPos;		//��ǰ��ȡ�ֽ��У�����bit��λ�ã����λΪ0�����λΪ7
	int gcount;		//��ǰ���ļ��������ֽ���
};

#endif