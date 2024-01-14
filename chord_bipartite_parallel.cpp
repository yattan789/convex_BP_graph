/*Logic: Generate bipartite graph by adding edges between the partition sets randomly in parallel. Check whether there exists a edge such that N(u) U N(v) forms a biclique. Keep removing such edges in parallel. When there are no such vertices in the updated graph, if the graph is empty, then it is chordal bipartite. Else it is not chordal bipartite. */

#include<iostream>
#include<cstdlib>
#include<stdio.h>
#include<ctime>
#include<omp.h>
using namespace std;
long int compdeg(long int i,long int a[500][500]);                                //Declaring all functions and global variables
bool chordal_bipartite(long int a[500][500]);
bool edge_check(long int u,long int v,long int a[500][500]);
void gen_chordal_bipartite(long int n,long int m);                                                   
long int m,n,storemat[1000][1000],adj[500][500];
int main()
{
	cout<<"Enter number of vertices\n";
   	cin>>n;
	cout<<"Enter number of chordal bipartite graphs to be returned\n";                  //Taking inputs
	cin>>m;
	freopen("choral_bipartite_parallel.txt","w",stdout);
	gen_chordal_bipartite(n,m);
}
void gen_chordal_bipartite(long int n,long int m)
{
	long int j,i,u,x,d,flag=0,f,g,q,z,h,counter4=0,p,cop[500][500],counter5=1;
	srand(time(0));
	while(counter5<=m)
	{	
		for(u=1;u<=n/2;u++)                                                  //Size of 1st partition set
		{
			#pragma omp parallel for
			for(i=1;i<=n;i++)                                           
			{
				#pragma omp parallel for
				for(j=1;j<=n;j++)
				{
					adj[i][j]=0;                                  //Initialising the matrix                          
				}
			}
			#pragma omp parallel for
			for(i=1;i<=u;i++)
			{
				#pragma omp parallel for
				for(j=u+1;j<=n;j++)
				{
					adj[i][j]=0;
					adj[i][j]=(rand()%2)+0;                     //Adding edges between the partition sets randomly
					adj[j][i]=adj[i][j];
					cop[i][j]=adj[i][j];
					adj[i][i]=0;
					cop[j][i]=cop[i][j];
					cop[i][i]=0;
				}
			}
			#pragma omp parallel for
			for(p=2;p<=counter5;p++)                           //Checking if matrix is same as previous matrices
			{
				for(q=1;q<=n;q++)
				{
					for(z=1,x=((p-2)*n)+1;z<=n,x<=(p-1)*n;z++,x++)
					{
						if(storemat[q][x]==adj[q][z])    //Checking from stored matrices
						{
							counter4++;
						}
					}
				}
				if(counter4==n*n)                               
				{
				flag=1;
				}
				else
				flag=0;
				counter4=0;
			}
			if(flag!=1 && chordal_bipartite(cop))          //Displaying matrix if it is not displayed already
			{
				cout<<"Chordal bipartite Graph:"<<counter5<<'\n';
				for(d=1;d<=n;d++)
				{
					for(f=((counter5-1)*n)+1,g=1;f<=counter5*n,g<=n;f++,g++)
					{
						storemat[d][f]=adj[d][g]; 
                				cout<<adj[d][g]<<' '; 
					}  
					cout<<'\n';                                                  
				}
				counter5++;                                  //Incrementing counter for matrices
			}
			if(counter5>m)
			break;
		}	
	}	
}
bool chordal_bipartite(long int a[500][500])                                   //Checks whether graph is chordal bipartite
{
	long int i,j,x=0;
	#pragma omp parallel for
	for(i=1;i<=n;i++)
	{
		#pragma omp parallel for
		for(j=1;j<=n;j++)
		{
			if(edge_check(i,j,a))                    //Checking if edge satisfies edge check and removing it 
			{
				a[i][j]=0;
				a[j][i]=0;             
			}
		}
	}
	#pragma omp parallel for
	for(i=n;i>=1;i--)
	{
		#pragma omp parallel for
		for(j=n;j>=1;j--)
		{
			if(edge_check(i,j,a))
			{
				a[i][j]=0;
				a[j][i]=0;             //Checking if edge satisfies edge check and removing it
			}
		}
	}
	#pragma omp parallel for
	for(i=1;i<=n;i++)
	{
		#pragma omp parallel for
		for(j=1;j<=n;j++)
		{
			if(a[i][j]==1)             //Checking if the graph is empty
			{
				return false;
			}
		}
	}
	return true;
}
bool edge_check(long int edge1,long int edge2,long int a[500][500])            
{
	long int i,j,counter1=0,counter2=0,counter3=0,counter4=0,f[500],k,l,m;
	#pragma omp parallel for
	for(i=1;i<=n;i++)
	{
		if(a[edge1][i]==1)
		{
			f[i]=1;
			counter1++;                             //Counting the no. of neighbours of edge1 and assigning them 1 in an array
		}
		else if(a[edge2][i]==1)
		{
			f[i]=2;
			counter2++;                             //Counting the no. of neighbours of edge1 and assigning them 2 in an array
		}
		else
		{
			f[i]=0;                                 //Assigning 0 else
		}
	}
	#pragma omp parallel for
	for(i=1;i<=n;i++)
	{
		#pragma omp parallel for
		for(j=1;j<=n;j++)
		{
			if(f[i]==1 && f[j]==0 || f[i]==2 && f[j]==0)
			{
				a[i][j]=0;
				a[j][i]=0;                //Removing all vertices and their edges if they are not neighbours of edge1 or 2
			}
		}
	}
	#pragma omp parallel for
	for(i=1;i<=n;i++)
	{
		if(f[i]==1)
		{
			if(compdeg(i,a)==counter2)        //Checking if degree of all vertices assigned 1=No. of vertices assigned 2 
			{
				counter3++;
			}
		}
	}
	#pragma omp parallel for
	for(i=1;i<=n;i++)
	{
		if(f[i]==2)
		{
			if(compdeg(i,a)==counter1)        //Checking if degree of all vertices assigned 2=No. of vertices assigned 1
			{
				counter4++;
			}
		}
	}
	if(counter1==counter3 && counter2==counter4)      //Checking if above conditions satisfy for both edge1 and edge2
		return true;
	else
		return false;
}
long int compdeg(long int i,long int a[500][500])                                           //Finding out degree of each vertex
{
	long int counter=0,j;
	#pragma omp parallel for
	for(j=1;j<=n;j++)
	{
		if(a[i][j]==1)
		{
			counter++;                                          //Incrementing counter
		}
	}
	return counter;
}
