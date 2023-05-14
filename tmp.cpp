/*#Ôóíêöèè ñîðòèðîâêè äîëæíû ïðèíèìàòü std::vector<int> -ñîðòèðóåìûé íàáîð ýëåìåíòîâ.
#https://docs.google.com/document/d/1L-doO_8pcQJ2gBpHUw7w7zEiB3auwPjt/edit
#. Ñîðòèðîâêà âûáîðîì
#   Ñîðòèðîâêà Øåëëà
#0. Ñîðòèðîâêà åñòåñòâåííûì
äâóõïóòåâûì ñëèÿíèåì

1 000, 2 000, 3 000, …, 10 000,  25000, 50000, 100000:
*/
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
using namespace std::chrono;

struct stats {
	long comparison_count = 0;
	long copy_count = 0;
	double time = 0;
};
size_t lcg() {
	static size_t x = 0;
	x = (1021 * x + 24631) % 116640;
	return x;
}

std::vector<int> create_vector(int N, int chose) {
	std::vector<int> TMP;
	if (chose == 1)
	{
		for (int i = 0; i < N; i++)
		{
			TMP.push_back(lcg());
		}
	}
	else if (chose == 2)
	{
		for (int i = 0; i < N; i++)
		{
			TMP.push_back(i);
		}

	}
	else {
		for (int i = N; i > 0; i--)
		{
			TMP.push_back(i);
		}

	}
	return TMP;
}

stats sorting_choice(std::vector<int> Vector) {
	auto start = high_resolution_clock::now();
	bool needing_restart = false;
	stats A;
	int min = 0; // äëÿ çàïèñè ìèíèìàëüíîãî çíà÷åíèÿ
	int buf = 0; // äëÿ îáìåíà çíà÷åíèÿìè 

	/*********** Íà÷àëî ñîðòèðîâêè **************/
	for (int i = 0; i < Vector.size(); i++)
	{

		min = i; // çàïîìíèì íîìåð òåêóùåé ÿ÷åéêè, êàê ÿ÷åéêè ñ ìèíèìàëüíûì çíà÷åíèåì
		// â öèêëå íàéäåì ðåàëüíûé íîìåð ÿ÷åéêè ñ ìèíèìàëüíûì çíà÷åíèåì
		for (int j = i + 1; j < Vector.size(); j++)
			min = (Vector[j] < Vector[min]) ? j : min;
			A.copy_count +=1;
			A.comparison_count += 1;
		// cäåëàåì ïåðåñòàíîâêó ýòîãî ýëåìåíòà, ïîìåíÿâ åãî ìåñòàìè ñ òåêóùèì
		if (i != min)
		{
			buf = Vector[i];
			Vector[i] = Vector[min];
			Vector[min] = buf;
			A.copy_count += 3;
		}
		A.comparison_count += 1;
	}
	auto stop = high_resolution_clock::now();
	duration<double> duration = stop - start;
	A.time = (double)duration.count();
	return A;
}
stats sorting_shell(std::vector<int> Vector) {
	auto start = high_resolution_clock::now();
	stats A;
	for (int s = Vector.size() / 2; s > 0; s /= 2) {
		for (int i = s; i < Vector.size(); ++i) {
			for (int j = i - s; j >= 0 && Vector[j] > Vector[j + s]; j -= s) {
				int temp = Vector[j];
				Vector[j] = Vector[j + s];
				Vector[j + s] = temp;
				A.comparison_count += 1;
				A.copy_count += 3;
			}
		}
	}
	auto stop = high_resolution_clock::now();
	duration<double> duration = stop - start;
	A.time = (double)duration.count();

	return A;
}

void testing(int N, int chosing, int number_circle, int variant_create_vector)
{

	stats AVG;

	for (int i = 0; i < number_circle; i++)
	{
		std::vector<int> TMP = create_vector(N, variant_create_vector);
		stats tmp;
		if (chosing == 1) { // choice
			tmp = sorting_choice(TMP);

			AVG.comparison_count += tmp.comparison_count;
			AVG.copy_count += tmp.copy_count;
			AVG.time += tmp.time;
		}
		else  { // shell 
			tmp = sorting_shell(TMP);

			AVG.comparison_count += tmp.comparison_count;
			AVG.copy_count += tmp.copy_count;
			AVG.time += tmp.time;
		}

		//AVG.comparison_count += tmp->comparison_count ;
		//AVG.copy_count += tmp->copy_count / 100;
		//AVG.time += tmp->time / 100;
	}
	std::ofstream out("log.txt", std::ios::app);
	if (chosing == 1) { // ÁÀÁË
		std::cout << "The  choice sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
		if (out.is_open())
		{
			out << "The  choice sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
		}
	}
	else { // shake 
		std::cout << "The shell sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
		if (out.is_open())
		{
			out << "The shell sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
		}
	}
	
	out.close();


}
int main() {
	//testing (ðàçìåð âåêòîðà,âûáîð ñîðòèðîâêè, êîëè÷åñòâî öèêëîâ,âûáîð ãåíåðàöèè âåêòîðà)

	std::cout << "\nAverage value on 100 vectors\n";
	for (int i = 1; i < 3; i++)
	{
		testing(1000, i, 100, 1);
		testing(2000, i, 100, 1);
		testing(3000, i, 100, 1);
		testing(10000, i, 100, 1);
		testing(25000, i, 100, 1);
		testing(50000, i, 100, 1);
		testing(100000, i, 100, 1);
	}

	std::cout << "\nValue on sorted vector\n";
	for (int i = 1; i <3; i++)
	{
		testing(1000, i, 1, 2);
		testing(2000, i, 1, 2);
		testing(3000, i, 1, 2);
		testing(10000, i, 1, 2);
		testing(25000, i, 1, 2);
		testing(50000, i, 1, 2);
		testing(100000, i, 1, 2);
	}

	std::cout << "\nValue on reverse-sorted vector\n";
	for (int i = 1; i < 3; i++)
	{
		testing(1000, i, 1, 3);
		testing(2000, i, 1, 3);
		testing(3000, i, 1, 3);
		testing(10000, i, 1, 3);
		testing(25000, i, 1, 3);
		testing(50000, i, 1, 3);
		testing(100000, i, 1, 3);
	}



}
