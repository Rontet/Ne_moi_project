#define ZERO_MONTH 202109
#include"MyLib.hpp"
#include"MineLib.hpp"
#include<fstream>
#include<cstdio>
#include<iomanip>
using namespace std;

int main()
{
	//объявления, ввод, динамическая обработка---------------
	_s flats = gethome();
	_s taxes = gettaxes();
	flats.mas = (_flat*)flats.mas;
	taxes.mas = (_tax*)taxes.mas;
	ifstream indet("details.txt");
	int L;
	indet >> L;
	double live_s = 0.0;
	for (int i = 0; i < flats.n; i++)
	{
		live_s += flats.mas[i].s;
		flats.mas[i].ipu = new int*[L];
		flats.mas[i].MonExp = new int*[L];
		flats.mas[i].MonOdn = new int*[L];
		for (int j = 0; j < L; j++)
		{
			flats.mas[i].ipu[j] = new int[taxes.n];
			flats.mas[i].MonExp[j] = new int[taxes.n];
			flats.mas[i].MonOdn[j] = new int[taxes.n];
		}

	}
	int **odpu = new int*[L],
		**ExpFlats = new int*[L];
	for (int i = 0; i < L; i++)
	{
		odpu[i] = new int[taxes.n];
		ExpFlats[i] = new int[taxes.n];
	}
	int date;
	char dat[8];
	for (int i = 0; i < L; i++)
	{
		indet >> dat;
		date = dtoi(dat);
		for (int j = 0; j < taxes.n; j++)
		{
			indet >> odpu[date - ZERO_MONTH][j];
			ExpFlats[i][j] = 0;
		}
	}
	int M;
	indet >> M;
	for (int i = 0, flatnum; i < M; i++)
	{
		indet >> dat >> flatnum;
		date = dtoi(dat);
		for (int j = 0; j < taxes.n; j++)
		{
			indet >> flats.mas[flatnum].ipu[date - ZERO_MONTH][j];
			flats.mas[flatnum].MonExp[date - ZERO_MONTH][j] =
			(date - ZERO_MONTH == 0) ? 0 :
			flats.mas[flatnum].ipu[date - ZERO_MONTH][j] - 
			flats.mas[flatnum].ipu[date - ZERO_MONTH - 1][j]
				;
			ExpFlats[date - ZERO_MONTH][j] +=
				flats.mas[flatnum].MonExp[date - ZERO_MONTH][j];
		}
	}
	indet.close();
	freopen("input.txt", "r", stdin);
	int N_flat;
	char taxname[6],
		 period_L[8],
		 period_R[8];
	scanf_s("%d %s %s-%s", &N_flat, taxname, period_L, period_R);
	fclose(stdin);
	//обработка и вывод---------------------------------------
	int per_L = dtoi(period_L) - ZERO_MONTH,
		per_R = dtoi(period_R) - ZERO_MONTH,
		N_tax = -1,
		f = 1;
	flats.mas[N_flat].s /= live_s;
	do
	{
		N_tax++;
		if (strcmp(taxes.mas[N_tax].name, taxname) == 0)
		{
			f = 0;
		}
	} while (N_tax < taxes.n && f);
	ofstream out("output.txt");
	int SumExpFlat = 0,
		SumExpOdn = 0,
		Cost = taxes.mas[N_tax].pay;
	out << per_R - per_L + 1 << '\n';
	for (int i = per_L; i <= per_R; i++)
	{
		int ExpFlat = flats.mas[N_flat].MonExp[i][N_tax],
			ExpOdn = (odpu[i][N_tax] - ExpFlats[i][N_tax]) *
					  flats.mas[N_flat].s
			;
		SumExpFlat += ExpFlat;
		SumExpOdn += ExpOdn;
		out << itod(i + ZERO_MONTH) << ' '
			<< ExpFlat * Cost << ' ' << ExpOdn * Cost << '\n';
	}
	out.seekp(0);
	int SumCostFlat = SumExpFlat * Cost,
		SumCostOdn = SumExpOdn * Cost;
	out << SumExpFlat << ' '
		<< SumCostFlat << ' '
		<< SumExpOdn << ' '
		<< SumCostOdn << '\n';
	out.close();
	//очистка-----------------------------------------------------
	for (int i = 0; i < L; i++)
	{
		delete[]odpu[i];
		delete[]ExpFlats[i];
	}
	for (int i = 0; i < flats.n; i++)
	{
		for (int j = 0; j < L; j++)
		{
			delete[]flats.mas[i].ipu[j];
			delete[]flats.mas[i].MonExp[j];
			delete[]flats.mas[i].MonOdn[j];
		}
		delete[]flats.mas[i].ipu;
		delete[]flats.mas[i].MonExp;
		delete[]flats.mas[i].MonOdn;
	}
	delete[]odpu;
	delete[]ExpFlats;
	delete[]flats.mas;
	delete[]taxes.mas;
	return 0;
}