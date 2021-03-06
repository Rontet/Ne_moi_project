#include"MyLib.hpp"
#include"MineLib.hpp"
#include<fstream>

_s gettaxes()
{
	ifstream intax("res\\taxes.txt");
	int N_Taxes;
	intax >> N_Taxes;
	_tax* taxes = new _tax[N_Taxes];
	for (int i = 0; i < N_Taxes; i++)
	{
		intax >> taxes[i].name >> taxes[i].cost >> taxes[i].k;
	}
	intax.close();
	return { taxes, N_Taxes };
}

_s gethome()
{
	ifstream inhome("res\\home.txt");
	int N;
	double S_home;
	inhome >> N >> S_home;
	_flat* flats = new _flat[N + 1];
	for (int i = 1; i <= N; i++)
	{
		inhome >> flats[i].s >> flats[i].ppl;
		flats[i].s /= S_home;
	}
	inhome.close();
	return { flats, N };
}