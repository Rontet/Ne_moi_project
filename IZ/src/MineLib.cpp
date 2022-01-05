#include"MineLib.hpp"
#include "MyLib.hpp"
#include<cstring>
#include<cstdlib>
int dtoi(char d[])
{
	char i[] = { d[3], d[4], d[5], d[6], d[0], d[1], '\0' };
	return atoi(i);
}

char* itod(int i)
{
	char tmp[10];
	itoa(i, tmp, 10);
	char* d = new char[]{ tmp[4], tmp[5], '.', tmp[0], tmp[1], tmp[2], tmp[3], '\0'};
	return d;
}