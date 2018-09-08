#pragma once

#include<iostream>
#include<map>
#include<boost/shared_ptr.hpp>
#include<boost/make_shared.hpp>
#include<boost/thread.hpp>
#include <boost/typeof/typeof.hpp>
#include<boost/foreach.hpp>
#include<boost/lexical_cast.hpp>
#include<boost/make_shared.hpp>
#include<fstream>
#include<windows.h>
#include<string>
#include<vector>
using namespace std;
using namespace boost;

#define SUCESS 0
#define INPUTDOESNOTEXIST -926

typedef boost::shared_ptr<boost::thread> PtrThread;