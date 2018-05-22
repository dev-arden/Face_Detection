#pragma once

template<typename T>
class IppImage
{
protected:
	int width; //영상의 가로크기(픽셀단위)
	int height;//영상의 세로크기(픽셀단위)
	T** pixels;

public:
	IppImage();
	Ippimage(int w, int h);
	IppImage(const IppImage<T>& img);
	~IppImage();

	void CreateImage(int w, int h);
	void 
};