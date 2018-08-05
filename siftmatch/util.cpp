#include"util.h"

void aywait()
{
	cout << endl << "Ay waitting a character to continue..." << endl;
	getchar();
	return;
}


void print_some_pairs(vector<Point2f>& ref_coor, vector<Point2f>& tar_coor, int num)
{
	cout << "head" << endl;
	cout << "---------------------------------------------------------------" << endl;
	for (int i = 0; i < num; i++)
	{
		if (i < ref_coor.size())
		{
			cout << ref_coor[i] << " <-> " << tar_coor[i] << endl;
		}
	}
	cout << endl;

	cout << "tail" << endl;
	cout << "---------------------------------------------------------------" << endl;
	for (int i = (int)ref_coor.size() - num; i < ref_coor.size(); i++)
	{
		if (i >= 0 && i < ref_coor.size())
		{
			cout << ref_coor[i] << " <-> " << tar_coor[i] << endl;
		}
	}


	cout << endl;
	cout << "---------------------------------------------------------------" << endl << endl;

}


void print_matrix(Mat& mat)
{
	int rows = mat.rows;
	int cols = mat.cols;
	cout << endl;
	for (int i = 0; i < rows; i++)
	{
		float*pf = (float*)mat.ptr<float>(i);
		for (int j = 0; j < cols; j++)
		{
			cout << pf[j] << "\t";
		}
		cout << endl;
	}
	cout << endl;

}
