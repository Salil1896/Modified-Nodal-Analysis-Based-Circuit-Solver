/// Code created by : Salil Mamodiya
// Roll No.: 120102053
// Dept. :    ECE


#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <iomanip>  
using namespace std;


int main(void)
{
	vector <string> v1; // v1 will save all the id of the voltage sources
	
	// reading net list 
	// reading two times first to count total number of nodes and voltage sources.
	std::ifstream dfile("sampletest.net");
	std::ifstream d1file("sampletest.net");
	
	int nline;
	string s,a,b,c,d1,d2;
	
	// first line in file denote the number of the rows.
    dfile>>nline;
    d1file>>nline;
    
    int n = 0;
    int vc = 0;
    
    /// counting number of nodes and voltage sources.
    for(int i=0;i<nline;i++)
    {dfile>>s;
     if(s[0]=='V' || s[0] == 'I' || s[0]=='R')
     {dfile>>a>>b>>c;}
     else
     {dfile>>a>>b>>d1>>d2>>c;}
     n = max(max(atoi(a.c_str()),n),atoi(b.c_str()));
     
     // three types of voltage source V: independent  E: voltage controlled  H: current controlled
     if(s[0]=='V' || s[0] == 'E' || s[0]=='H') 
	 {vc++;v1.push_back(s);}
    }
    
    
    int rn = n; // total resistance node
    n = n+vc;  // dimension of A. node and voltage sources
    cout<<n<<"\n";
    //n denotes the dimension of the modified nodal analysis matrix.
    
    float A[n][n],L[n][n],U[n][n],B[n],X[n],Y[n];
    float R[rn][rn];
    
    //initializing A and B
    for(int i=0;i<n;i++)
	{
	    for(int j=0;j<n;j++)
	    {A[i][j] = 0;}
	    B[i] = 0;
	}
	
	int vid = rn;
	
	//reading file
    for(int i=0;i<nline;i++)
    {
	    d1file>>s;
	    
		// Resistance row
	    if(s[0]=='R')
	    {
	    	d1file>>a>>b>>c;
	    	int x1 = atoi(a.c_str());
	        int x2 = atoi(b.c_str());
	        float f1 = atof(c.c_str());
	    	if(x1!=0){	
		    A[x1-1][x1-1] = A[x1-1][x1-1]+ (1.0/f1);
			R[x1-1][x1-1] = R[x1-1][x1-1]+ (1.0/f1);
			}
		    
		    if(x2!=0)
		    {A[x2-1][x2-1] = A[x2-1][x2-1]+ (1.0/f1);
			 R[x2-1][x2-1] = R[x2-1][x2-1]+ (1.0/f1);
			}
		    
			if(x1!=0 && x2!=0)
		    {
		       A[x1-1][x2-1]  = A[x1-1][x2-1]-(1.0/f1); 
		       R[x1-1][x2-1]  = R[x1-1][x2-1]-(1.0/f1);
		       A[x2-1][x1-1]  = A[x2-1][x1-1]-(1.0/f1); 
		       R[x2-1][x1-1]  = R[x2-1][x1-1]-(1.0/f1);
			}
			
		
	    
	    }
	    
	    
	    // independent current source
	    if(s[0]=='I')
	    {
	     d1file>>a>>b>>c;
	     int x1 = atoi(a.c_str());
	     int x2 = atoi(b.c_str());
	     float f1 = atof(c.c_str());	
		 if(x1>0)
		 B[x1-1] = -f1; 
		
		 if(x2>0)
		 B[x2-1] = f1;
		}
	    
	    //independent voltage source
	    if(s[0]=='V')
	    {
	      d1file>>a>>b>>c;
	      int x1 = atoi(a.c_str());
	      int x2 = atoi(b.c_str());
	      float f1 = atof(c.c_str());
	      if(x1>0) {A[vid][x1-1] = 1; A[x1-1][vid] = 1; }
	      if(x2>0) {A[vid][x2-1] = -1; A[x2-1][vid] = -1; }
	      B[vid] = B[vid] + f1;
	      vid = vid + 1;
	    }
	    
	    // VCCS
	    if(s[0]=='G')
	    {
	    	d1file>>a>>b>>d1>>d2>>c;
	        int x1 = atoi(a.c_str());
	        int x2 = atoi(b.c_str());
	        int y1 = atoi(d1.c_str());
	        int y2 = atoi(d2.c_str());
	        float f1 = atof(c.c_str());
	        if(y1>0)
	        {
	        	if(x1>0) A[x1-1][y1-1] = A[x1-1][y1-1]  + f1;
	        	if(x2>0) A[x2-1][y1-1] = A[x2-1][y1-1] + (-f1);
	        	
			}
			
			if(y2>0)
			{
				if(x1>0) A[x1-1][y2-1] = A[x1-1][y2-1]  + (-f1);
	        	if(x2>0) A[x2-1][y2-1] = A[x2-1][y2-1] + f1;
			}
	    }
	    
	    
		//VCVS
		if(s[0]=='E')
		{
			d1file>>a>>b>>d1>>d2>>c;
	        int x1 = atoi(a.c_str());
	        int x2 = atoi(b.c_str());
	        int y1 = atoi(d1.c_str());
	        int y2 = atoi(d2.c_str());
	        float f1 = atof(c.c_str());
	        
	        if(x1>0)
			{
				A[vid][x1-1] = 1;
				A[x1-1][vid] = 1; 
			}
				
			if(x2>0)
			{
				A[vid][x2-1] = -1;
				A[x2-1][vid] = -1;
			}
	        
			if(y1>0)
			{A[vid][y1-1] = A[vid][y1-1] + (-f1);}
			
			if(y2>0)
			{A[vid][y2-1] = A[vid][y2-1] + f1;} 
	       
		     vid++;
		}
		
		//CCCS
		if(s[0]=='F')
		{
			d1file>>a>>b>>d1>>d2>>c;
	        int x1 = atoi(a.c_str());
	        int x2 = atoi(b.c_str());
	        int y1 = atoi(d1.c_str());
	        int y2 = atoi(d2.c_str());
	        float f1 = atof(c.c_str());
	       
	        if(y1>0)
	        {
	        	if(x1>0)
	        	{
	        		
	        	  A[x1-1][y1-1] = A[x1-1][y1-1] + f1*(-R[y1-1][y2-1]);	
				}
				if(x2>0)
				{
				   A[x2-1][y1-1] = A[x2-1][y1-1] + f1*R[y1-1][y2-1];
				}
	        	
			}
			
			if(y2>0)
			{
				if(x1>0)
	        	{
	        	  A[x1-1][y2-1] = A[x1-1][y2-1]  + f1*R[y1-1][y2-1];	
				}
				if(x2>0)
				{
				   A[x2-1][y2-1] = A[x2-1][y2-1] +  f1*(-R[y1-1][y2-1]);
				}
			}
			 
		}
		
		
	    // CCVS
	    if(s[0]=='H')
	    {
	    	d1file>>a>>b>>d1>>d2>>c;
	        int x1 = atoi(a.c_str());
	        int x2 = atoi(b.c_str());
	        int y1 = atoi(d1.c_str());
	        int y2 = atoi(d2.c_str());
	        float f1 = atof(c.c_str());
	        
	        if(x1>0)
	        {
	        	A[vid][x1-1] = 1;
	        	A[x1-1][vid] = 1;
			}
			if(x2>0)
			{
				A[vid][x2-1] = -1;
				A[x2-1][vid] = -1;
			}
	        if(y1>0) {A[vid][y1-1] =  A[vid][y1-1] +  f1*R[y1-1][y2-1];}
			if(y2>0) {A[vid][y2-1] = A[vid][y2-1] + (-f1*R[y1-1][y2-1]);}
			vid++;
	    	
		}
	    
	    
	    
    }
    
    //cout<<"\n"<<A[1][1]<<"\n";
    
    //printing A
    cout<<"\n\nA Matrix\n\n";
    for(int i=0;i<n;i++)
    {
    	for(int j=0;j<n;j++)
    	{
    		cout<<std::fixed<<std::setprecision(2)<<A[i][j]<<" ";
		}
		cout<<std::setprecision(2)<<"  "<<B[i]<<"\n";
	}
   
    // finding LU decomposition
   	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(i==j)
				L[i][j]=1;
			else if(i<j)
				L[i][j]=0;
			U[i][j]=A[i][j];
		}
	}
	for(int j=0;j<n;j++)
	{
		for(int i=j+1;i<n;i++)
		{
			// Find Coeff
			double r = U[i][j]/U[j][j];
			for(int p=0;p<n;p++)
				U[i][p] = U[i][p] - r*U[j][p];
			L[i][j] = r;
		}
	}
    
    // printing L and U
    cout<<"\n\nL Matrix\n\n";
    for(int i=0;i<n;i++)
    {
    	for(int j=0;j<n;j++)
    	{
    		cout<<L[i][j]<<" ";
		}
		cout<<"\n";
	}
  
    for(int i=0;i<n;i++)
    {
    	for(int j=0;j<n;j++)
    	{
    		cout<<U[i][j]<<" ";
		}
		cout<<"\n";
	}
    
    //finding solution using Forward and Backward method
	for(int i=0; i<n; i++)
    {
        Y[i]=B[i];
        for(int j=0; j<i; j++)
        {
            Y[i]-=L[i][j]*Y[j];
        }
    }
    
    for(int i=n-1; i>=0; i--)
    {
        X[i]= Y[i];
        for(int j=i+1; j<n; j++)
        {
            X[i]-=U[i][j]*X[j];
        }
        X[i]/=U[i][i];
    }

    
    //printing solution
    int cnt = 1;
    
    cout<<"\n"<<"Solution From LU Decomposition"<<"\n";
    for(int i=0;i<n;i++)
    {
    	if(i<rn)
    	{cout<<"V_N"<<i+1<<"  "<<X[i]<<"\n";}
    	
    	else
    	{
    	 
    	 cout<<"I_"<<v1.at(cnt-1)<<"  "<<X[i]<<"\n";
		 cnt++;	
		}
	}
    

}
