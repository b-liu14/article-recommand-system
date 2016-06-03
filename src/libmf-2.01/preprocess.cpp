#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <ctime>

#include "mf.h"

using namespace std;
using namespace mf;

#define num_user (5551)
#define num_book (16980)
#define num_test (251)

struct Option
{
    Option() : ratio(0.4) {}
    string tr_path, tr2_path, te_path, an_path;
    mf_float ratio;
};

bool is_numerical(char *str)
{
    int c = 0;
    while(*str != '\0')
    {
        if(isdigit(*str))
            c++;
        str++;
    }
    return c > 0;
}

Option parse_option(int argc, char **argv)
{
    vector<string> args;
    for(int i = 0; i < argc; i++)
        args.push_back(string(argv[i]));

    Option option;

    mf_int i;
    for(i = 1; i < argc; i ++)
    {
    	if(args[i].compare("-r") == 0)
    	{
    		if((i+1) >= argc)
                throw invalid_argument("need to specify ration after -r");
            i++;
            if(!is_numerical(argv[i]))
                throw invalid_argument("-r should be followed by a number");
            option.ratio = (mf_float)atof(argv[i]);
    	}
    	else
    		break;
    }
    if(i >= argc)
        throw invalid_argument("input training data not specified");
    option.tr_path = string(args[i++]);

	if(i >= argc)
        throw invalid_argument("output training data not specified");
    option.tr2_path = string(args[i++]);    

    if(i >= argc)
        throw invalid_argument("testing data not specified");
    option.te_path = string(args[i++]);

    if(i >= argc)
        throw invalid_argument("answer data not specified");
    option.an_path = string(args[i++]);

    return option;
}

void preprocess(string tr_path, string tr2_path, string te_path, string an_path, mf_float ratio)
{
	srand(unsigned(time(NULL)));
	mf_problem prob = read_problem(tr_path);

	ofstream f_tr(tr2_path);
    if(!f_tr.is_open())
        throw runtime_error("cannot open " + tr_path);

	ofstream f_te(te_path);
    if(!f_te.is_open())
        throw runtime_error("cannot open " + te_path);

    ofstream f_an(an_path);
    if(!f_an.is_open())
        throw runtime_error("cannot open " + an_path);

    vector< vector<mf_int> > tr(num_user + 1);
	for(mf_int i = 0; i < prob.nnz; i ++)
	{
		tr[ prob.R[i].u ].push_back( prob.R[i].v );
	}
	vector<mf_int> te(num_user + 1);
	vector<mf_int> an(num_user + 1);
	for(mf_int j = 1; j <= num_user; j ++)
	{
		te.clear();
		an.clear();
		auto& v = tr[j];
		mf_int num_an = floor(v.size() * ratio);
		for(mf_int i = 0; i < num_an; i ++)
		{
			mf_int pos = random() % v.size();
			te.push_back(v[pos]);
			an.push_back(v[pos]);
			v.erase(v.begin() + pos);
		}
		for(mf_int i = num_an; i < num_test; i ++)
		{
			mf_int book_id = random() % num_book + 1;
			while(  find(v.begin(), v.end(), book_id) != v.end() ||
					find(te.begin(), te.end(), book_id) != te.end() )
				book_id = random() % num_book + 1;
			te.push_back(book_id);
		}

		for(unsigned i = 0; i < v.size(); i ++)
		{
			f_tr << j << " " << v[i] << " " << 1 << endl;
		}

		sort(an.begin(), an.end());
		for(unsigned i = 0; i < an.size(); i ++)
		{
			f_an << j << " " << an[i] << " " << 1 << endl;
		}

		sort(te.begin(), te.end());
		for(unsigned i = 0; i < te.size(); i ++)
		{
			f_te << j << " " << te[i] << " " << 1 << endl;
		}
	}
	f_tr.close();
	f_te.close();
	f_an.close();
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

    preprocess(option.tr_path, option.tr2_path, option.te_path, option.an_path, option.ratio);

    return 0;
}
