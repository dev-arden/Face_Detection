#pragma once

template<typename T>
class IppImage
{
protected:
	int width; //������ ����ũ��(�ȼ�����)
	int height;//������ ����ũ��(�ȼ�����)
	T** pixels;

public:
	IppImage();
	Ippimage(int w, int h);
	IppImage(const IppImage<T>& img);
	~IppImage();

	void CreateImage(int w, int h);
	void 
};