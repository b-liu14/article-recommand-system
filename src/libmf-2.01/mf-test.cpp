#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include "mf.h"

using namespace std;
using namespace mf;
struct Option
{
    Option() : eval(RMSE) {}
    string test_path, model_path, recommend_path, output_path, list_path;
    mf_int eval, recommend_number, output_gap;
};

string predict_help()
{
    return string(
"usage: mf-predict [options] test_file model_file [output_file]\n"
"\n"
"options:\n"
"-e <eval>: specify the evaluation criterion (default 0)\n"
"\t 0 -- root mean square error\n"
"\t 1 -- mean absolute error\n"
"\t 2 -- generalized KL-divergence\n"
"\t 5 -- logarithmic error\n"
"\t 6 -- accuracy\n"
"\t10 -- row-wise mean percentile rank\n"
"\t11 -- column-wise mean percentile rank\n"
"\t12 -- row-wise area under the curve\n"
"\t13 -- column-wise area under the curve\n");
}

Option parse_option(int argc, char **argv)
{
    vector<string> args;
    for(int i = 0; i < argc; i++)
        args.push_back(string(argv[i]));

    if(argc == 1)
        throw invalid_argument(predict_help());

    Option option;

    mf_int i;
    for(i = 1; i < argc; i++)
    {
        if(args[i].compare("-e") == 0)
        {
            if((i+1) >= argc)
                throw invalid_argument("need to specify evaluation criterion after -e");
            i++;
            option.eval = atoi(argv[i]);
            if(option.eval != RMSE &&
               option.eval != MAE &&
               option.eval != GKL &&
               option.eval != LOGLOSS &&
               option.eval != ACC &&
               option.eval != ROW_AUC &&
               option.eval != COL_AUC &&
               option.eval != ROW_MPR &&
               option.eval != COL_MPR)
                throw invalid_argument("unknown evaluation criterion");
        }
	else if(args[i].compare("-k") == 0)
        {
            if((i+1) >= argc)
                throw invalid_argument("need to specify recommend number after -k");
            i++;
            option.recommend_number = atoi(argv[i]);
            if(option.recommend_number < 0)
                throw invalid_argument("unknown recommend number");
        }
	else if(args[i].compare("-g") == 0)
        {
            if((i+1) >= argc)
                throw invalid_argument("need to specify output gap after -g");
            i++;
            option.output_gap = atoi(argv[i]);
            if(option.output_gap <= -2 || option.output_gap == 0)
                throw invalid_argument("unknown output gap");
        }
        else
            break;
    }
    if(i >= argc-4)
        throw invalid_argument("testing data, model file and recommend file not specified");
    option.test_path = string(args[i++]);
    option.model_path = string(args[i++]);
    option.recommend_path = string(args[i++]);
    option.list_path = string(args[i++]);

    if(i < argc)
    {
        option.output_path = string(args[i]);
    }
    else if(i == argc)
    {
        const char *ptr = strrchr(&*option.test_path.begin(), '/');
        if(!ptr)
            ptr = option.test_path.c_str();
        else
            ++ptr;
        option.output_path = string(ptr) + ".out";
    }
    else
    {
        throw invalid_argument("invalid argument");
    }

    return option;
}

void test(string test_path, string model_path, string recommend_path, string list_path, string output_path, mf_int eval, mf_int recommend_number, mf_int output_gap)
{
	ofstream f_out(output_path);
    	if(!f_out.is_open())
        	throw runtime_error("cannot open " + output_path);

    	mf_model *model = mf_load_model(model_path.c_str());
    	if(model == nullptr)
        	throw runtime_error("cannot load model from " + model_path);
	mf_int testNum = 251;

	ifstream recommendFile(recommend_path);
	ifstream testFile(test_path);
	ofstream calFile(output_path);
	ofstream listFile(list_path, ios::app);
	vector<int> recommend;
	vector<int> test;
	double precision;
	double recall;
	double average_precision = 0.0;
	double average_recall = 0.0;
	for (mf_int i = 0; i < model->m; ++i)
	{
		if(output_gap >= 0 && i % output_gap == 0)
			cout << "measure: " << i << endl;
		recommend.clear();
		test.clear();
		int cal = 0;
		for (mf_int j = 0;j < testNum;j++)
		{
			mf_int a,b;
			double temp;
			if (j < recommend_number)
			{
				recommendFile >> a;
				recommendFile >> b;
				recommendFile >> temp;
				recommend.push_back(b);
			}
			testFile >> a;
			testFile >> b;
			testFile >> temp;
			test.push_back(b);
		}
		for (mf_int j = 0; j < recommend_number;j++)
		{
			if (find(test.begin(),test.end(),recommend[j]) != test.end())
			{
				cal++;
			}
		}
		precision = (double)cal/recommend_number;
		recall = (double)cal/testNum;
		average_precision += precision;
		average_recall += recall;
		calFile << i+1 << ' ' << precision << ' ' << recall << endl;
	}
	average_precision = average_precision / model->m;
	average_recall = average_recall / model->m;
    cout << "AveragePrecision: " << average_precision << endl;
    cout << "AverageRecall: " << average_recall << endl;
    listFile << "AveragePrecision: " << average_precision << "	";
    listFile << "AverageRecall: " << average_recall << endl;

    mf_destroy_model(&model);
}

int main(int argc, char **argv)
{
    Option option;
    try
    {
        option = parse_option(argc, argv);
    }
    catch(invalid_argument &e)
    {
        cout << e.what() << endl;
        return 1;
    }

    try
    {
        test(option.test_path, option.model_path, option.recommend_path, option.list_path, option.output_path, option.eval, option.recommend_number, option.output_gap);
    }
    catch(runtime_error &e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}
