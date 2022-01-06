#pragma once
#include"MineLib.hpp"
using namespace std;

struct _tax
{
	char name[6];
	int cost,
		k;
};

struct _flat
{
	int **ipu,
		**MonExp;
	double s;
	int ppl;
};

_s gettaxes();

_s gethome();