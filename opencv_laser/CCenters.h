#pragma once
class CCenters
{
public:
	CCenters();
	~CCenters();
	
	void Add(double x, double y, double value);
	void Empty();
	bool FindCenters(unsigned char* m_pImage);
	void Caculate(unsigned char* m_pImageCaculate, int xpos, int ypos);

};

