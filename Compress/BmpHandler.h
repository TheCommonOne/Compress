#ifndef BMPHANDLER_H
#define BMPHANDLER_H

#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

class BmpHandler
{
public:
	
	/*
		���Ҷ�ͼ���ͷ�����ݲ��֡�β�ֿ���д�������ļ���
		����ͼ����Ϣ����
	*/
	BITMAPINFOHEADER split(char* originFile, char* headFile, char* dataFile, char* tailFile)
	{
		cout << "Splitting..." << endl;
		ifstream fin(originFile, ios::binary);
		ofstream foutHead(headFile, ios::binary);
		ofstream foutData(dataFile, ios::binary);
		ofstream foutTail(tailFile, ios::binary);
		
		if(!fin)
		{
			cout << "Can't open it" << endl;
		}
	
		BITMAPFILEHEADER fileHeader;
		BITMAPINFOHEADER infoHeader;
		//��ȡ�ļ�ͷ��λͼ��Ϣͷ
		fin.read((char *) &fileHeader, sizeof(fileHeader));
		fin.read((char *) &infoHeader, sizeof(infoHeader));

		int width = infoHeader.biWidth;
		int height = infoHeader.biHeight;
		int bpp = infoHeader.biBitCount;
	
		char* headBuffer = new char[fileHeader.bfOffBits];	//����head������

		//���ļ�ָ�����õ���ͷ����ȡͼ�����ݲ���֮ǰ��������Ϣ��д��headFile��
		fin.seekg(0);
		fin.read(headBuffer, fileHeader.bfOffBits);
		foutHead.write(headBuffer, fileHeader.bfOffBits);
		foutHead.flush();
		
		//����ͼ��ÿ�е��ֽ���
		//����RGB����Ҫ��ÿ�б��밴4�ֽڶ���
		int lineWidth = (width * bpp + 31) / 32 * 4;
	
		//����ͼ�����ݻ�����
		byte * dataBuffer = new byte[lineWidth * height];

		// ����ƫ������bmp���ݲ���
		fin.seekg(fileHeader.bfOffBits);

		for(int i = 0; i < height; i++)
		{
			fin.read((char *) &dataBuffer[i * lineWidth], lineWidth);
		}

		//�����ݲ���д���ļ�dataFile
		foutData.write((char *) & dataBuffer[0], lineWidth * height);
		foutData.flush();

		//�ļ�β�������ֽڵ�0
		char zeroData[2] = {0};
		foutTail.write(zeroData, 2);
		foutTail.flush();

		fin.close();
		foutHead.close();
		foutData.close();
		foutTail.close();
		delete[] headBuffer;
		delete[] dataBuffer;

		return infoHeader;
	}

	/*
		��ͼ���ͷ�����ݲ��֡�β���ϳ�һ����Ч��bmpͼ��
	*/
	void merge(const char* headFile, const char* dataFile,const char* tailFile, const char* objectFile)
	{
		cout << "Merging..." << endl;

		ifstream fhead(headFile, ios::binary);
		ifstream fdata(dataFile, ios::binary);
		ifstream ftail(tailFile, ios::binary);

		ofstream outfile(objectFile, ios::binary);
		char c;
		fhead.read(&c, 1);
		while(fhead.good())
		{
			outfile.write(&c, 1);
			fhead.read(&c, 1);
		}

		fdata.read(&c, 1);
		while(fdata.good())
		{
			outfile.write(&c, 1);
			fdata.read(&c, 1);
		}

		ftail.read(&c, 1);
		while(ftail.good())
		{
			outfile.write(&c, 1);
			ftail.read(&c, 1);
		}

		outfile.flush();
		outfile.close();
	}

};

#endif