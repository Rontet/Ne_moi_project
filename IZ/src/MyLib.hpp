#pragma once
using namespace std;

struct _tax
{
	char name[6];
	int price,
		k;
};

struct _s
{
	_tax* mas;
	int n;
};

struct _flat
{
	int** ipu;
	double s;
	int ppl;
};


_s gettaxes();

_flat* gethome();