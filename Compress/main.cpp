/*
* ���룺��ѹ����ͼ���ļ�
* �����ѹ�����ͼ���ļ�
*/
#include <iostream>
#include <Windows.h>
#include <fstream>
#include "HuffmanTree.h"
#include "BinaryWriter.h"
#include "BinaryReader.h"
#include "BmpHandler.h"
#include <string>
#include <unordered_map>
using namespace std;

//����ѹ���ļ�ͷ�ṹ ��СΪ16Byte
struct CMPHeader
{
	DWORD sn;			//Ԥ���ļ�����
	DWORD width;		//ͼ����
	DWORD height;		//ͼ��߶�
	BYTE colorCount;	//��ɫ��
	BYTE reserved1;		//������
	BYTE reserved2;		//������
	BYTE reserved3;		//������
};

//������ɫ���ֶ� ��СΪ20Byte
struct CMPColorTableSegment
{
	/*
		���ַ����͵�huffman��ת����Ϊ�������ͣ������Ա����code�����С�
		������ĵ�0��Ԫ�ؿ�ʼ�����λ��ʼ����ӦstrCode����ĵ�һ��Ԫ��
	*/
	void setCode(string strCode)
	{
		char box = 0;
		int boxPos = 0;
		int codeIndex = 0;
		for(int i = 0; i < strCode.length(); i++)
		{
			int tmp = (1 << (7 - boxPos)) * (strCode[i] - '0');
			box += tmp;
			boxPos++;
			if(boxPos == 8) //box�Ѿ�װ��
			{
				code[codeIndex++] = box;
				box = 0;
				boxPos = 0;
			}
		}
		code[codeIndex] = box;
	}

	/*
		����Ա����code�����Ӧ�Ķ�����λ���ַ�����ʽ����
		��setCode�����������
	*/
	string codeToString()
	{
		char box = 0;
		int boxPos = 0;
		int codeIndex = 0;
		string strCode = "";
		int currentLength = 0;
		if(this->length == 0)
		{
			return strCode;
		}
		for(codeIndex = 0; codeIndex <= (this->length - 1) / 8; codeIndex++)
		{
			box = code[codeIndex];
			for(int i = 7; i >= 0; i--)
			{
				if((box >> i) & 1 == 1)
				{
					strCode += '1';
				}
				else
				{
					strCode += '0';
				}
				currentLength++;
				if(currentLength == this->length)
				{
					return strCode;
				}
			}
		}
	}

	/*
		���캯��
	*/
	CMPColorTableSegment()
	{
		greyValue = 0;
		length = 0;
		for(int i = 0; i < 16; i++)
		{
			code[i] = 0;
		}
	}
	WORD greyValue;		//��ɫ�Ҷ�ֵ
	WORD length;		//huffman�볤��
	BYTE code[16];		//huffman�룬��󳤶�Ϊ128
};

/*
	�������ܣ�
		����ָ���ļ���file����ͳ�����е�byte����Ƶ��
		����huffman�����õ�huffman��������

	������
		char * file  ��������ļ���

	����ֵ��
		huffman�������飬�ռ�����ڶ��У���Ҫ�ⲿ����delete
*/
string* getHuffmanCode(char* file)
{
	ifstream fin(file, ios::binary);
	ofstream fout("D:\\abc", ios::binary);
	if(!fin)
	{
		cout << "Can't open it" << endl;
	}
	bool flag = false;					//����Ƿ�����ļ�ĩβ
	int count = 0;
	int freqArray[256] = {0};			//Ƶ�����飬ͳ��ÿ���ֽ�ȡֵ���ֵĴ���
	const int BUFSIZE = 1024 * 1024;	//���ö�ȡ��������С��1M
	char* buffer = new char[BUFSIZE];	//����������
	//��ʼ��Ƶ�����飬ȫ����0
	for(int i = 0; i < 256; i++)
	{
		freqArray[i] = 0;
	}
	do
	{
		flag = (bool) fin.read(buffer, BUFSIZE);	//��ȡBUFSIZE�ֽ�����
		count = fin.gcount();	//ʵ�ʶ�ȡ���ֽ���
		for(int i = 0; i < count; i++)
		{
			freqArray[(unsigned char)buffer[i]]++;	//ͳ��Ƶ��
		}
		fout.write(buffer, count);
	}while(flag);
	HuffmanTree<int> ht((int*)freqArray,256);	//ͨ���Ҷ�ֵƵ�����飬����huffman��
	string* codes = new string[256];	//Huffman��������
	cout << codes[3];
	ht.coding(codes);	//����huffman�����õ�huffman����
	fin.close();
	fout.flush();
	fout.close();
	delete[] buffer;
	return codes;
}

/*
	�������ܣ�
		�������ĻҶ�ͼ�����ݲ���ѹ����Ŀ���ļ�

	������
		char* originFile	��ѹ���ĻҶ�ͼ�����ݲ����ļ�·��+�ļ���
		int width			ͼ�����ݵĿ�ȣ�������������ÿ���ֽ�������
		int height			ͼ�����ݵĸ߶�
		char* objFile		ѹ���ļ���Ŀ��·��+�ļ���
*/
void compress(char* originFile, int width, int height, char* objFile)
{
	cout << "Compressing..." << endl;
	//���ȸ���ԭʼ�ļ��������ÿ���Ҷ�ֵ��Ӧ�ĺ�������
	string* codes = getHuffmanCode(originFile);

	ifstream fin(originFile, ios::binary);
	ofstream fout(objFile, ios::binary);
	if(!fin)
	{
		cout << "Can't open file: " << originFile << endl;
	}
	if(!fout)
	{
		cout << "Can't open file: " << objFile << endl;
	}
	const int RBUFSIZE = 1024 * 1024;	//���ö�ȡ��������С��1M
	char* rbuffer = new char[RBUFSIZE];	//������������
	bool flag = false;					//����Ƿ�����ļ�ĩβ
	int count = 0;						//���˶����ֽ�

	const int WBUFSIZE = 1024 * 1024;	//����д��������С��1M
	char* wbuffer = new char[WBUFSIZE + 20]; //д����������
	int bufferPos = 0;					//���д��������Ч�����ֽ���
	char box = 0;						//ÿ8λװһ��
	int boxPos = 0;						//��װ��λ��
	string code = "";

	CMPColorTableSegment colorTable[256];//��ɫ��

	//д�ļ�ͷ
	CMPHeader header;
	//��SCNHд��ǰ�ļ�ǰ�ĸ��ֽ�
	//�����ļ��洢ʱ�ǵ�λ��ǰ����λ�ں�����Ҫ����ĸ˳�򵹹���
	header.sn = 'H';
	header.sn = header.sn << 8;
	header.sn += 'N';
	header.sn = header.sn << 8;
	header.sn += 'C';
	header.sn = header.sn << 8;
	header.sn += 'S';

	header.width = width;
	header.height = height;
	header.colorCount = 8;			//8λ��ɫ������2^8=128����ɫȡֵ
	fout.write((char *) &header, sizeof(header));

	//д��ɫ��
	for(int i = 0; i < 256; i++)
	{
		colorTable[i].greyValue = i;
		colorTable[i].length = codes[i].length();
		colorTable[i].setCode(codes[i]);
	}
	fout.write((char *) &colorTable, sizeof(colorTable));

	BinaryWriter bw(fout, WBUFSIZE);		//�ر��Ƶ�д�������ļ���
	int index = 0;
	do
	{
		flag = (bool) fin.read(rbuffer, RBUFSIZE);
		count = fin.gcount();
		for(int i = 0; i < count; i++)
		{
			//ÿ�ν�һ���ֽڱ���Ϊhuffman�룬д�뵽�ļ�����
			index = (unsigned char)rbuffer[i];
			bw.write(codes[index]);
		}
	}while(flag);
	bw.lastFlush();//ǿ��д��ʣ�����ݣ����һ���ֽ�������8λ����0�����λ
	delete[] rbuffer;
	delete[] wbuffer;
	fin.close();
	fout.close();
}

/*
	�������ܣ�
		����ѹ�����ļ���ѹ������ԭ�ɻҶ�ͼ�����ݲ���

	������
		char* originFile	//��ѹ���ļ�·��+�ļ���
		char* objectFile	//��ѹ��·��+�ļ���
*/

void decompress(char* originFile, char* objectFile)
{
	cout << "Decompressing..." << endl;
	ifstream fin(originFile, ios::binary);
	if(!fin)
	{
		cout << "Can't open file: " << originFile << endl;
	}
	ofstream fout(objectFile, ios::binary);
	if(!fout)
	{
		cout << "Can't open file: " << objectFile << endl;
	}
	CMPHeader header;	//ѹ���ļ�ͷ
	CMPColorTableSegment colorTable[256];	//ѹ���ļ���ɫ��

	//��ȡ�ļ�ͷ����ɫ�����
	fin.read((char*) &header, sizeof(header));
	fin.read((char*) &colorTable, sizeof(colorTable));

	unordered_map<string, int> ctMap;		//�Ӻ�������ӳ�䵽�Ҷ�ֵ
	unordered_map<string, int>::iterator iter;	//hash_map��ʱ������
	string tmpCode;
	BinaryReader br(fin, 1024 * 1024);			//�������ļ���ȡ��
	char c;
	char value;
	int wbufferSize = 1024 * 1024;
	char* wbuffer = new char[wbufferSize];
	int wbufferIndex = 0;

	//���������*������ÿ�а�˫�ֶ��룬����ȡ�����õ�ÿ���ֽ���
	int pointCount = header.height *
		((header.width * 8 + 31) / 32 * 4);

	//������ɫ����huffman��ת�����ַ�����ʽ������hash_map
	for(int i = 0; i < 256; i++)
	{
		tmpCode = colorTable[i].codeToString();
		ctMap[tmpCode] = colorTable[i].greyValue;
	}
	tmpCode = "";

	//ÿ�δ��ļ��ж�ȡһ��bit�����ַ���ʽ��ʾ
	while((c = br.getBit()) != '-')
	{
		tmpCode += c;	//���¶�����bit�ַ����ӵ���ʱhuffman�����
		iter = ctMap.find(tmpCode);	//��hashӳ����в��Ҹ����Ƿ��������Чhuffman��
		if(iter != ctMap.end())	//����Чcode
		{
			value = (char) iter->second;
			//fout.write((char *) &value, 1); //���Ҷ�ֵд���ѹ���ļ���û��ʹ�û�������д�������
			if(wbufferIndex == wbufferSize)		//����������д���ļ�
			{
				fout.write(wbuffer, wbufferIndex);
				wbufferIndex = 0;
			}
			wbuffer[wbufferIndex++] = value;
			tmpCode = "";
			pointCount--;
			if(pointCount == 0)	//ʹ��ͼ���ֽ�������ѹ�����ȣ���ֹ����ĩβ���ڲ����ֽڵ�0
			{
				break;	//���лҶ�ֵ����ѹ����ɣ�ǿ������ѭ��
			}
		}
	}
	if(wbufferIndex != 0)	//�������������ݣ�������д���ļ�
	{
		fout.write(wbuffer, wbufferIndex);
		wbufferIndex = 0;
	}
	fin.close();
	fout.flush();
	fout.close();
	delete[] wbuffer;

}


int main()
{
	char greyFile[] = "D:\\bmpExp\\grey.bmp";		//ԭʼ�Ҷ�ͼ��
	
	char headFile[] = "D:\\bmpExp\\head.dat";		//ͼ��ͷ����
	char tailFile[] = "D:\\bmpExp\\tail.dat";		//ͼ��β����
	char dataFilePath[] = "D:\\bmpExp\\data.dat";	//ͼ�����ݲ��֣���ѹ���ļ���
	
	char cFilePath[] = "D:\\bmpExp\\compress";		//ѹ���ļ�
	char dcFilePath[] = "D:\\bmpExp\\decompressed";	//��ѹ���ļ�
	char newBmpFile[] = "D:\\bmpExp\\new.bmp";	//�»Ҷ�ͼ��

	BmpHandler bh;

	//��ԭʼ�Ҷ�ͼ���Ϊͷ�����ݡ�β������
	BITMAPINFOHEADER infoHeader = bh.split(greyFile, headFile, dataFilePath, tailFile);
	//��ȡͼ��Ŀ�͸�
	int width = infoHeader.biWidth;
	int height = infoHeader.biHeight;


	//ѹ�����ѹ�����ݲ���
	compress(dataFilePath, width, height, cFilePath);	//ѹ��
	decompress(cFilePath, dcFilePath);					//��ѹ��

	//����ѹ������ļ���ͷ�ļ���β�ļ����ϣ��õ���ͼ������ԭ�����Ҷ�ͼ����ͬ�����ѹ��ȷ
	bh.merge(headFile, dcFilePath, tailFile, "D:\\bmpExp\\new.bmp");


	return 0;
}
