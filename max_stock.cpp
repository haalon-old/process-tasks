#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

void arithm(int * arr, int len)
{
	if( len <= 2)
	{
		std::cout << "None";
		return;
	}

	int start = 0, end = -1, diff = arr[1] - arr[0];
	int istart = 0, iend = -1;
	for (int i = 1; i < len-1; ++i)
	{
		if(diff == arr[i+1] - arr[i] && diff > 0)
			iend = i+1;
		else
			istart = i;

		if( iend - istart > end - start)
		{
			end = iend;
			start = istart;
		}
		std::cout<<i<<' '<<istart<<' '<<iend<<'\n';
		diff = arr[i+1] - arr[i];
	}

	for (int i = start; i <= end; ++i)
		std::cout<<arr[i]<<' ';
}

void stock(int * arr, int len)
{
	int imax = 0, imin = 0, diff = 0;
	int start = 0, end = -1;
	for (int i = 0; i < len; ++i)
	{

		if(arr[i] < arr[imin])
			imin = i;
		if(i > imin)
			if(arr[i] - arr[imin] > diff)
			{
				diff = arr[i] - arr[imin];
				start = imin;
				end = i;
			}
	}

	for (int i = start; i <= end; ++i)
		std::cout<<arr[i]<<' ';

}

void test()
{
	int pid;
	if(pid = fork())
	{
		std::cout << "child: "<<pid << " I: " << (int)getpid() << '\n';
		int status;
		int p = wait(&status);
		std::cout << p << ' '<<WEXITSTATUS(status)<<'\n';
	}
	else
	{
		// execlp("ls", "ls", NULL);
		exit(1);
	}
}

int main(int argc, char const *argv[])
{
	test();
}