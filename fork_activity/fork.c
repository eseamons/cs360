#include <iostream>
using namespace std;
main() {
	int pid = fork();
	cout << "PID is "<<pid<<endl;
}