#include <iostream>
#include <Windows.h>
#include <time.h>

using namespace std;

int main()
{
	string* intro1 = new string("Rapid Fire...");
	cout<<*intro1<<endl;
		
	string* intro2 = new string("By: Kevin Siraki...");
	cout<<*intro2<<endl;
		
	cout<<endl<<endl;
		
	string* opt = new string("Enter 1 for MMB, 2 for M4, or 3 for M5...");
	cout<<*opt<<endl;
		
	int* key = new int[0];
	cin>>*key;
		
	cout<<endl<<endl;
		
	string* latency = new string("Delay time (in MS)?...");
	cout<<*latency<<endl;
		
	int* x = new int[0];
	cin>>*x;
		
	cout<<endl<<endl;
		
	string* success = new string("Success: Keep this Window Open!!!");
	cout<<*success<<endl;
		
	switch(*key)
	{
		case 1 : 
		{
			while(1)
			{
				if(GetAsyncKeyState(VK_MBUTTON)&0x8000)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 10, 0, 0);
					//Sleep(60);
					mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
					Sleep(*x);
				}
				Sleep(10);
			}
				
		break;
		}
			
		case 2 :
		{
			while(1)
			{
				Sleep(10);
				
				if(GetAsyncKeyState(5)&0x8000)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 10, 0, 0);
					//Sleep(60);
					mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
					Sleep(*x);
				}
				Sleep(10);
			}
				
		break;
		}
			
		case 3 :
		{
			while(1)
			{
				if(GetAsyncKeyState(6)&0x8000)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 10, 0, 0);
					//Sleep(60);
					mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
					Sleep(*x);
				}
				Sleep(10);
			}
				
		break;
		}
			
		default : 
			cout << "Invalid input...";
			return 0;
	}
	
	delete intro1;
	delete intro2;
	delete opt;
	delete latency;
	delete success;
	delete x;
	delete key;
	intro1 = NULL;
	intro2 = NULL;
	opt = NULL;
	latency = NULL;
	success = NULL;
	x = NULL;
	key = NULL;
	
}
