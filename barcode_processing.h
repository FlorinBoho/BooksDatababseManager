#include<Python.h>
#include<iostream>
using namespace std;

#include<fstream>
#include<vector>
#include<sstream>
#include<string>

void process_string(char* str)
//{'ISBN-13': '9781476753836', 'Title' : 'Salt, Fat, Acid, Heat', 'Authors' : ['Samin Nosrat'], 'Publisher' : 'Simon and Schuster', 'Year' : '2017', 'Language' : 'en'}
{
	strcpy_s(str, strlen(str), str + 1);
	str[strlen(str) - 1] = '\0';
	char* pch = strtok_s(0, "\'", &str);
	while (pch != NULL)
	{
		if (!strcmp(pch, "ISBN-13") || !strcmp(pch, "Title") || !strcmp(pch, "Authors") || !strcmp(pch, "Publisher") || !strcmp(pch, "Year") || !strcmp(pch, "Language"))
		{

			cout << endl;
		}
		cout << pch;
		pch = strtok_s(0, "\'", &str);
	}
	cout << endl;
}

int process_barcode(char* barcode)
{
	//PyObject* pInt;

	Py_Initialize();

	//PyObject* PyInit_modulename(void);


	PyObject* pModule = PyImport_ImportModule("isbnlib");


	if (pModule)
	{
		PyObject* pFunc = PyObject_GetAttrString(pModule, "meta");

		if (pFunc && PyCallable_Check(pFunc))
		{
			PyObject* pArgs = PyTuple_New(3);
			PyObject* pValue;

			//if (!(pValue = PyUnicode_FromString("9780000000000"))) //barcode that does not work
			//if (!(pValue = PyUnicode_FromString("9781476753836"))) //barcode that works
			//if (!(pValue = PyUnicode_FromString("9783161484100"))) //barcode that works
			if (!(pValue = PyUnicode_FromString(barcode)))
				return NULL;
			PyTuple_SetItem(pArgs, 0, pValue); //ISBN number argument

			if (!(pValue = PyUnicode_FromString("default"))) //API to use (default - google books api) and cache method
				return NULL;
			PyTuple_SetItem(pArgs, 1, pValue); //API used
			PyTuple_SetItem(pArgs, 2, pValue); //cache method

			pValue = PyObject_CallObject(pFunc, pArgs);//de introdus argumente in locul lui NULL

			if (pValue != NULL)
			{
				PyObject* repr = PyObject_Repr(pValue);
				PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
				char *bytes = PyBytes_AS_STRING(str);

				//printf("%s\n", bytes); //book details output
				process_string(bytes);

				Py_XDECREF(repr);
				Py_XDECREF(str);

				//printf_s("C: meta() = %ld\n", PyLong_AsLong(pValue)); //nu e buna asta
				Py_DECREF(pValue);
			}
			else
			{
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				PyErr_Print();
				cout << "ISBN not found\n";
				return 0;
			}
		}
		else
		{
			if (PyErr_Occurred())
				PyErr_Print();
			cout << "Cannot find function meta()";
		}

		Py_XDECREF(pFunc);
		Py_DECREF(pModule);

	}
	else
	{
		PyErr_Print();
		cout << "Failed to load module isbnlib";
		return 0;
	}

	if (Py_FinalizeEx() < 0) {
		return 120;
	}

	//system("PAUSE");
	return 1;
}

int menu()
{
	int option = 0;
	cout  << "Choose an option from below to do something with the book:" << endl;
	do {
		cout << "\t1. Add the book to my to read list." << endl;
		cout << "\t2. Update the last read page." << endl;
		cout << "\t3. Mark the book as already read." << endl;
		cout << "\t4. Do nothing." << endl;
		cout << "\nOption: "; cin >> option;
	} while (option <= 1 && option >= 4 );
	return option;

}

//isbn was not previously in the database
/*void insert_new_barcode(char* barcode)
{
	fstream file;
	file.open("db.dat", ios::app);
	if (file.fail())
	{
		cout << "Error opening the file db.dat!" << endl;
		exit(1);
	}

	switch (menu())
	{
		case 1:
			file << barcode << " " << "1";
			break;
		case 2:
			int page, total_pages;
			cout << "Insert reached page number: "; cin >> page;
			cout << "Insert total number of pages: "; cin >> total_pages;
			file << barcode << " " << "2" << " " << page << "/" << total_pages;
			break;
		case 3:
			file << barcode << " " << "3";
			break;
		case 4:
			break;
	}
	file.close();
}
*/

/*
void modify_existing_barcode(char* barcode)
{
	char bcd[20];
	fstream file;
	file.open("db.dat", ios::in);
	if (file.fail())
	{
		cout << "Error opening the file db.dat!" << endl;
		exit(1);
	}
	while (!file.eof())
	{
		file >> bcd;
		if (!strcmp(barcode, bcd))
		{
			cout << "The barcode exists in the database already and it is marked as: ";
			//...
		}
	}
}
*/

/*
void search_barcode_in_file(char* barcode)
{
	int k = 0;
	int option;
	char bcd[20];
	int pagenr, totalpagenr = 0;
	
	fstream file;
	file.open("db.dat",ios::in | ios::out);
	if (file.fail())
	{
		cout << "Error opening the file db.dat!" << endl;
		exit(1);
	}

	string line;
	string deleteline=barcode;
	while (getline(file,line))
	{
		cout << line << endl;
		if (line.find(deleteline,0)!=-1) //if found barcode in the database
		{
			cout << "The barcode exists in the database already and it is marked as: "; 
			file >> option;
			switch (option)
			{
			case 1:
				cout << "Book to be read" << endl;
				break;
			case 2:
				cout << "Currently reading book, reached page: ";
				file >> pagenr >> totalpagenr;
				cout << pagenr << " out of " << totalpagenr << endl;
				break;
			case 3:
				cout << "The Book has already been read" << endl;
				break;
			}
			option = menu();

			//work on this, it doesnt work
			ofstream temp;
			temp.open("temp.db");

			switch (option)
			{
			case 1:
				break;
			case 2:
				file.seekp(-(int)strlen(barcode), ios::cur);
				file << barcode << " " << "2";
				cout << "Reached page: "; cin >> pagenr;
				if (totalpagenr == 0)
				{
					cout << " out of the total of: "; cin >> totalpagenr;
				}
				file << " " << pagenr << "/" << totalpagenr << endl;
				break;
			case 3:
				file << " " << "3";
				break;
			case 4:
				break;
			}
			k = 1;
		}
	}
	file.close();
	if (k == 0)
	{
		insert_new_barcode(barcode);
	}
}
*/


vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}

string edit_existing_line(string line)
{
	string to_return;
	string pageread, totalnr;
	vector<string> case2;
	//vector<string> sep = split(line, '\\s+');
	vector<string> sep = split(line, ' ');
	//String splited[] = line.split("\\s+"); //splits string by space to get the barcode and the previous option
	to_return = sep[0];

	cout << "The barcode exists in the database already and it is marked as: ";

	switch (stoi(sep[1]))
	{
	case 1:
		cout << "Book to be read" << endl;
		break;
	case 2:
		cout << "Currently reading book, page: "; cout << sep[2]<<endl;
		case2 = split(sep[2], '/');
		break;
	case 3:
		cout << "The Book has already been read" << endl;
		break;
	}

	int option = menu();
	switch (option)
	{
	case 1:
		to_return += " 1";
		break;
	case 2:
		cout << "Insert number of the last read page "; cin >> pageread;
		to_return += " 2 ";
		to_return += pageread;
		to_return += "/";
		if (case2.empty())
		{
			cout << "Total number of pages: "; cin >> totalnr;
		}
		else
		{
			totalnr = case2[1];
		}
		to_return += totalnr;
		break;
	case 3:
		to_return += " 3";
		break;
	case 4:
		to_return = line;
		break;
	}
	return to_return;
}



void search_barcode_in_file_new(char* barcode)
{
	int check = 0, check2 = 0;
	string deleteline=barcode;
	string line,new_line;

	ifstream fin; fin.open("db.dat");
	ofstream temp; temp.open("temp.dat");

	while (getline(fin, line))
	{
		if (line.find(deleteline, 0) != -1)//if barcode was found in the database
		{
			string line2=edit_existing_line(line);
			temp << line2 << endl; //printing out the new line
			check = 1;
		}
		else
		{
			temp << line << endl; //printing out the rest of the lines from the database
		}
	}

	if (check == 0) //if barcode was not found in the whole database
	{
		new_line = barcode;
		switch (menu())
		{
		case 1:
			new_line += " 1";
			break;
		case 2:
			int page, total_pages;
			cout << "Insert reached page number: "; cin >> page;
			cout << "Insert total number of pages: "; cin >> total_pages;
			new_line += " 2";
			new_line += " ";
			new_line += page;
			new_line += "/";
			new_line += total_pages;
			break;
		case 3:
			new_line += " 3";
			break;
		case 4:
			check2 = 1;
			break;
		}
		if(check2==0)
			temp << new_line << endl;
	}

	temp.close();
	fin.close();
	remove("db.dat");
	rename("temp.dat", "db.dat");
}
