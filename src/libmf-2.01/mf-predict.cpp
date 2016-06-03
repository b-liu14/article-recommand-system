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
    Option(){}
    string test_path, model_path, output_path;
};

struct Recommend
{
	Recommend(mf_int _item_id, mf_float _r) : item_id(_item_id), r(_r) {}
	mf_int item_id;
	mf_float r;
	bool operator <(const Recommend& recommend) const
	{
		return r > recommend.r;
	}
};

string predict_help()
{
    return string("usage: mf-predict test_file model_file [output_file]\n");
}

Option parse_option(int argc, char **argv)
{
    vector<string> args;
    for(int i = 0; i < argc; i++)
        args.push_back(string(argv[i]));

    if(argc == 1)
        throw invalid_argument(predict_help());

    Option option;

    mf_int i = 1;
    if(i >= argc-1)
        throw invalid_argument("testing data and model file not specified");
    option.test_path = string(args[i++]);
    option.model_path = string(args[i++]);

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

void predict(string test_path, string model_path, string output_path)
{
    mf_problem prob = read_problem(test_path);

    ofstream f_out(output_path);
    if(!f_out.is_open())
        throw runtime_error("cannot open " + output_path);

    mf_model *model = mf_load_model(model_path.c_str());
    if(model == nullptr)
        throw runtime_error("cannot load model from " + model_path);

	vector< vector<Recommend> > vec(model->m);
    for(mf_int i = 0; i < prob.nnz; i++)
    {
        mf_float r = mf_predict(model, prob.R[i].u, prob.R[i].v);
		vec[ prob.R[i].u ].push_back( Recommend(prob.R[i].v, r) );
    }
	for(mf_int i = 1; i < model->m; i++)
	{
		sort(vec[i].begin(), vec[i].end());
		f_out << i << ',';
		for(mf_int j = 0; j < 5; j++)
		{
			if((unsigned)j > vec[i].size())
				throw runtime_error("size of vec[i] is smaller than 5");
			f_out << vec[i][j].item_id;
			if(j < 4)
				f_out << ";";
			else
				f_out << endl;
		}
	}
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
        predict(option.test_path, option.model_path, option.output_path);
    }
    catch(runtime_error &e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}
