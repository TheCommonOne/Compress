#ifndef BINARYWRITER_H
#define BINARYWRITER_H

#include <iostream>
#include <fstream>
using namespace std;

/*
	�������ļ�д������д������Ϊbit
	�����˻��������ܹ����д��Ч��
*/
class BinaryWriter
{
public:
	BinaryWriter(ofstream& fout, int bufSize)
	{
		this->fout = &fout;
		this->bufSize = bufSize;
		buffer = new char[bufSize + 20];
		box = 0;
		bufIndex = 0;
		boxPos = 0;
	}
	~BinaryWriter()
	{
		this->lastFlush();
		delete[] buffer;
	}

	/*
		��str�������huffman��д���ļ�
		���ﵽ����������ʱ���Ż�����д�룬�����ݴ��ڻ�����
	*/
	void write(string str)
	{
		int tmp = 0;
		for(int i = 0; i < str.length(); i++)
		{
			box += (1 << (7 - boxPos)) * (str[i] - '0');
			boxPos++;
			if(boxPos == 8) //box�Ѿ�װ��
			{
				buffer[bufIndex] = box;
				bufIndex++;
				box = 0;
				boxPos = 0;
				if(bufIndex == bufSize)	//buffer�Ѿ�װ����д���ļ�
				{
					fout->write(buffer, bufSize);
					fout->flush();
					bufIndex = 0;
				}
			}
		}
	}

	/*
		ÿ��д��һ��bit��δ�ã�Ч���Եͣ�
	*/
	void writeBit(char c)
	{
		box += (1 << (7 - boxPos)) * (c - '0');
		boxPos++;
		if(boxPos == 8) //box�Ѿ�װ��
		{
			buffer[bufIndex] = box;
			bufIndex++;
			box = 0;
			boxPos = 0;
			if(bufIndex == bufSize)	//buffer�Ѿ�װ����д���ļ�
			{
				fout->write(buffer, bufSize);
				fout->flush();
				bufIndex = 0;
			}
		}
	}

	/*
		ǿ�ƽ�������������д�뵽�ļ�����ʹ������δ����
		���һ���ֽ�����������λ��0���롣
	*/
	void lastFlush()
	{
		//��������box��û�����ݣ���д��
		if(boxPos == 0 && bufIndex == 0)	
		{
			return;
		}
		else if(boxPos == 0)//box�е������Զ�д�뻺�����������������ݻ�δд�뵽�ļ�
		{
			fout->write(buffer, bufIndex);
			fout->flush();
			bufIndex = 0;
		}
		else //��������box�ж�������
		{
			buffer[bufIndex] = box;
			bufIndex++;
			fout->write(buffer, bufIndex);
			fout->flush();
			bufIndex = 0;
			box = 0;
			boxPos = 0;
		}
	}

private:
	ofstream* fout;
	char* buffer;
	char box;
	int bufSize;
	int bufIndex;
	int boxPos;
};

#endif