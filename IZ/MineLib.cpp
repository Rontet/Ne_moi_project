#include"MineLib.hpp"
#include "MyLib.hpp"

int dtoi(string d)
{
	d.append(d, 0, 2);
	d.erase(0, 3);
	return stoi(d);
}

string itod(int i)
{
	string d = to_string(i);
	d.insert(2, ".");
	return d;
}

int Vodn(_flats flats, int N_flat, _report odpu)
{
	
}