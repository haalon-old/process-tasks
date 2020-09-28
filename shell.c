#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <signal.h>
#define DBG 0

// ./ftst < servd | ./ftst | ./ftst & ./ftst < servout &
// ls | ./pipe & ./serv < servd ; echo 42
// ls > servaut ; echo 42 | ./pipe & ./ftst < servd

int main(int argc, char const *argv[])
{
	char *str,*str1,*buf,a, *parameters [10],pipeflag=0,fwflag=0,frflag=0;
	int i=20,j=0,n,fd[2],savefd[2], rfile,wfile, bitarr=0;
	str = (char*)malloc(20);
	if(DBG) printf("SRATR\n" );
	while((a=getchar())!='\n')
	{
		if(j<i)
		{
			*(str+j)=a;
		}
		else
		{
			i+=20;
			//str1=str;
			str1=(char*)malloc(i);			
			for(n=0;n<j;n++)
				*(str1+n)=*(str+n);
			free(str);
			str=str1;
			*(str+j)=a;
			


		}
		j++;
		
	} 

	if(j<i)
	{
		*(str+j)=' ';
	}
	else
	{
		i+=20;
		//str1=str;
		str1=(char*)malloc(i);			
		for(n=0;n<j;n++)
			*(str1+n)=*(str+n);
		free(str);
		str=str1;
		*(str+j)=' ';
	}

	//write(1,str,j);
	putchar('\n');
	n=1;
	a=0;
	buf=str;
	
 	while( ( str1 = (  ( ( strstr(buf," & ")!=NULL && strstr(buf," ; ")==NULL )  || ( strstr(buf," & ") < strstr(buf," ; ") ) ) && !( strstr(buf," & ")==NULL && strstr(buf," ; ")!=NULL ) ? strstr(buf," & ") : strstr(buf," ; "))) != NULL)
	{
		if( *(buf = str1+1) == '&' ) 
			bitarr+=n;
		n*=2;
		
	}
	if(DBG) printf("%d\n",bitarr );
	//printf("%d\n",bitarr >> 1);
	//str1=strtok(str," ");
	
	str1=strtok(str," ");
	for(i=0;i<10;i++)
	{
		parameters[i]=NULL;
	}
	//printf("%s\n",str1 );

	//if(fork()) exit(0);

	pipe(savefd);
	close(savefd[1]);

	while(str1!=NULL)
	{
		if(DBG) printf("#%s\n",str1 );
		if(  strcmp(str1,";") && strcmp(str1,"|") && strcmp(str1,">") && strcmp(str1,"<") && strcmp(str1,"&")) //operands + parameters
		{
			if(a)
			{
				parameters[a]=str1;

			}
			else
			{					
				buf=str1;
				parameters[0]=buf; 		
			}	
			a+=1;
			

		}
		else																			// operations
		{
			if(!a){ printf("INVALID INPUT"); break;}
			a=0;
			
			if(!strcmp(str1,"<"))
			{
				str1=strtok(NULL," ");
				if(DBG) printf("#%s\n",str1 );
				rfile = open(str1 , O_RDONLY , 0777);
				frflag=1;
				a=1;

			}

			if(!strcmp(str1,">"))
			{
				str1=strtok(NULL," ");
				if(DBG) printf("#%s\n",str1 );
				unlink(str1);
				wfile = creat(str1 ,  0777);
				fwflag=1;
				a=1;

			}

			if(!strcmp(str1,"&"))
			{

				if(!fork())
				{
						if(fork()) exit(0);
						signal(SIGINT,SIG_IGN);
						close(0);			

						if(pipeflag)
						{							
							dup2(fd[0],0);
							close(fd[0]);
						}
						if(fwflag)	
						{
							dup2(wfile,1);
							close(wfile);
						}			

						if(frflag)	
						{
							dup2(rfile,0);
							close(rfile);
						}

						if (DBG) { printf("%s ", buf ); printf("*%d*%d*%d*%d\n",bitarr, pipeflag, frflag, fwflag );	}

						execvp(buf,parameters);
						printf("ERROR!");
						exit(0);
				}
				else
				{		
					//sleep(1);			
					bitarr=bitarr >> 1;
					for(i=0;i<10;i++)
					{
						parameters[i]=NULL;
					}
					pipeflag=0;				
					
				}

				if(fwflag)
				{
					close(wfile);
					fwflag=0;
				}
				if(frflag)
				{
					close(rfile);
					frflag=0;
				}
				//if(fork()) exit(0);
				
			
				
			}		


			if(!strcmp(str1,";"))
			{		
					

				//pipeflag=0;
				if(fork())
				{
					//waitflag=1;
					while(wait(NULL)>0)
					{;}

					for(i=0;i<10;i++)
					{
						parameters[i]=NULL;
					}
					bitarr=bitarr >> 1;
				}
				else
				{
					

					if(pipeflag)
					{
						dup2(fd[0],0);
						close(fd[0]);
					}
					if(fwflag)	
					{
						dup2(wfile,1);
						close(wfile);
					}			

					if(frflag)	
					{
						dup2(rfile,0);
						close(rfile);
					}	

					if (DBG) { printf("%s ", buf ); printf("*%d*%d*%d*%d\n",bitarr, pipeflag, frflag, fwflag );	}		

					execvp(buf,parameters);
					printf("ERROR!");
					exit(0);
				}
				pipeflag=0;
				if(fwflag)
				{
					close(wfile);
					fwflag=0;
				}
				if(frflag)
				{
					close(rfile);
					frflag=0;
				}

			}

			if(!strcmp(str1,"|"))
			{
				
				if(pipeflag)
				{
					dup2(0,savefd[0]);
					dup2(fd[0],0);
					close(fd[0]);
				}

				pipe(fd);


				if(fork())
				{
					close(fd[1]);
					if(pipeflag)
						dup2(savefd[0],0);
					for(i=0;i<10;i++)
					{
						parameters[i]=NULL;
					}
					pipeflag=1;
					
				}
				else
				{
					if (DBG) { printf("%s ", buf ); printf("*%d*%d*%d*%d\n",bitarr, pipeflag, frflag, fwflag );	}
					if(frflag)	
					{
						dup2(rfile,0);
						close(rfile);
					}				
					
					dup2(fd[1],1);
					close(fd[1]);

					if(fwflag)	
					{
						dup2(wfile,1);
						close(wfile);
					}
					//write(1,"#",1);
					if(bitarr & 1)
					{
						//write(1,"#",1);
						if(fork()) exit(0);
					}
					
					
					execvp(buf,parameters);
					printf("ERROR!");
					exit(0);

				}
				//waitflag=0;
				if(frflag)
				{
					close(rfile);
					frflag=0;
				}
				if(fwflag)
				{
					close(wfile);
					fwflag=0;
				}

			}


			
		}
		
		str1=strtok(NULL," ");


	}
	
if(a)
{

	if(fork())
	{
		while(wait(NULL)>0)
					{
						//sleep(2);
						//printf("HELP!\n");
					}
	}
	else
	{
		

		if(pipeflag)
		{
			dup2(fd[0],0);
			close(fd[0]);
		}
		if(frflag)	
		{
			dup2(rfile,0);
			close(rfile);
		}	

		if(fwflag)	
		{
			dup2(wfile,1);
			close(wfile);
		}		
		if (DBG) { printf("%s ", buf ); printf("*%d*%d*%d*%d\n",bitarr, pipeflag, frflag, fwflag );	}

		execvp(buf,parameters);
		printf("ERROR!");
		exit(0);
	}
}
	if(frflag)
	{
		close(frflag);
		frflag=0;
	}
	if(fwflag)
	{
		close(fwflag);
		fwflag=0;
	}
	close(savefd[0]);
//	sleep(1);
	free(str);
	return 0;
}