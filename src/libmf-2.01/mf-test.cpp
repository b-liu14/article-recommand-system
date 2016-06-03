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
    Option() :output_gap(1) {}
    string answer_path, model_path, recommend_path, output_path, list_path;
    mf_int output_gap;
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
        if(args[i].compare("-g") == 0)
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
    option.answer_path = string(args[i++]);
    option.model_path = string(args[i++]);
    option.recommend_path = string(args[i++]);
    option.list_path = string(args[i++]);
    option.output_path = string(args[i++]);

    return option;
}

void test(string answer_path, string model_path, string recommend_path, string list_path, string output_path, mf_int output_gap)
{
	ofstream f_out(output_path);
    	if(!f_out.is_open())
        	throw runtime_error("cannot open " + output_path);

    	mf_model *model = mf_load_model(model_path.c_str());
    	if(model == nullptr)
        	throw runtime_error("cannot load model from " + model_path);

	ifstream recommendFile(recommend_path);
    mf_problem ans = read_problem(answer_path);
	ofstream calFile(output_path);
	ofstream listFile(list_path, ios::app);
	vector<int> recommend;
	vector< vector<int> > test(model->m);
	double precision;
	double recall;
	double average_precision = 0.0;
	double average_recall = 0.0;
    for (mf_int i = 0; i < ans.nnz; ++i)
    {
        test[ans.R[i].u].push_back(ans.R[i].v);
    }
	for (mf_int i = 1; i < model->m; ++i)
	{
		if(output_gap >= 0 && i % output_gap == 0)
			cout << "measure: " << i << endl;
		recommend.clear();
		int cal = 0;
        mf_int a,b;
        char tmp;
        recommendFile >> a >> tmp;
        for (mf_int j = 0;j < 5;j++)
        {
            if (j < 4)
                recommendFile >> b >> tmp;
            else
                recommendFile >> b;
            recommend.push_back(b);
        }
		for (mf_int j = 0; j < 5;j++)
		{
			if (find(test[i].begin(),test[i].end(),recommend[j]) != test[i].end())
			{
				cal++;
			}
		}
		precision = (double)cal/5;
		recall = (double)cal/test[i].size();
		average_precision += precision;
		average_recall += recall;
		calFile << i << ' ' << precision << ' ' << recall << endl;
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
        test(option.answer_path, option.model_path, option.recommend_path, option.list_path, option.output_path, option.output_gap);
    }
    catch(runtime_error &e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}
