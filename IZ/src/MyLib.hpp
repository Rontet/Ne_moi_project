#pragma once
using namespace std;

struct _tax
{
	char name[6];
	int price,
		k;
};

struct _flat
{
	double s;
	int ppl;
};

struct _s
{
	void* s;
	int n;
};

_s gettaxes();

_s gethome();