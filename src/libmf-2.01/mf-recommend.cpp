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
    Option() : output_gap(1){}
    string test_path, model_path, output_path;
    mf_int recommend_number, output_gap;
};

struct Rate
{
    mf_int u;
    mf_int v;
    mf_float r;
    Rate(mf_int _u, mf_int _v, mf_float _r) : u(_u), v(_v), r(_r) {}
    bool operator <(const Rate& rate) const
    {
        return r > rate.r;
    }
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
        if(args[i].compare("-k") == 0)
        {
	    if((i+1) >= argc)
                throw invalid_argument("need to specify evaluation criterion after -e");
            i++;
	    option.recommend_number = atoi(argv[i]);
	    if(option.recommend_number <= 0)
		throw invalid_argument("unknown evaluation criterion");
	}
	else if(args[i].compare("-g") == 0)
        {
	    if((i+1) >= argc)
                throw invalid_argument("need to specify evaluation criterion after -e");
            i++;
	    option.output_gap = atoi(argv[i]);
	    if(option.recommend_number <= -2 || option.recommend_number == 0)
		throw invalid_argument("unknown evaluation criterion");
	}
        else
            break;
    }
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

void recommend(string test_path, string model_path, string output_path, mf_int recommend_number, mf_int output_gap)
{
    ofstream f_out(output_path);
    if(!f_out.is_open())
        throw runtime_error("cannot open " + output_path);

    mf_model *model = mf_load_model(model_path.c_str());
    if(model == nullptr)
        throw runtime_error("cannot load model from " + model_path);

    if(recommend_number > model->n)
	throw runtime_error("recommend_number is too big!");

    vector<Rate> rate;
    for(mf_int u = 1; u < model->m; u ++)
    {
	if(output_gap > 0 && u % output_gap == 0)
		cout << "recommend: " << u << endl;
        rate.clear();
        for(mf_int v = 1; v < model->n; v ++)
        {
            mf_float r = mf_predict(model, u, v);
            rate.push_back(Rate(u, v, r));
        }
        sort(rate.begin(), rate.end());
        for(int i = 0; i < recommend_number; i ++)
        {
            f_out << rate[i].u << " " << rate[i].v << " " << rate[i].r << endl;
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
        recommend(option.test_path, option.model_path, option.output_path, option.recommend_number, option.output_gap);
    }
    catch(runtime_error &e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}
