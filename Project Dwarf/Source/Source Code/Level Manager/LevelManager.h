#pragma once

//STL includes 
#include <string>

//Header includes


//Namespaces
using namespace std;

//Forward Declares.
class Core;

class CLevelManager
{

public:
	CLevelManager();
	~CLevelManager();

	bool Initialize();
	void Shutdown();

	void LoadLevel(string _filename, Core* _core);

	void LoadTestLevel(Core* _core);
};