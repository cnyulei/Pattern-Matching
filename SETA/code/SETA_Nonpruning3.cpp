//SETA_Nonpruning3 
//����ʹ�ü��ý��Ĳ��ԣ�����ʹ�ü��������Ĳ���

#include <iostream.h>
#include <string.h>
#include <afxwin.h>


#define N 100000
char disp_all;
struct store_char  
{
	char ch;
	int len;  //��Ӧch�ַ����ֵĴ���
	int position[N];  //��Ӧch�ַ��ڴ��е�λ��
};


struct gapnit
{
	char start,end; //
	int min,max; //
};

struct arc
{
	int position;	//��ʾλ��
	int flag;		//��ʾ״̬
};

struct node
{
	int name;		//name of NODE 
	int positon;	//����������λ��
	bool flag;		//�Ƿ���Ĵ��� flag=true�����к��ӻ�˫�ף�flag=false��
	__int64 *trpn,*frpn;	//true root path number, false root path number
	__int64 *tlpn,*flpn;	//true leaf path number, false leaf path number
};

struct row
{
	node *r;
	int start,end;
	int num;
};

#define MAXLEN 100

row *snt;			//subnettree

gapnit gap[N];  //pattern p[i]
int gap_len=0;  //the length of pattern
int seq_len;
int maxp_c=-1;



void dealrange(char *p,int len)      //��a[1,3]b��ʽ��ģʽ���д�����a����start�У�b����end�У�1��min��3��max
{
	gap[gap_len].start =p[0];
	gap[gap_len].end =p[len];
	if (len==1)
	{
		gap[gap_len].max =gap[gap_len].min=0;
	}
	else
	{
		int value=0;
		int i;
		int flag=0;
		for ( i=2;p[i]!=',';i++)
			if (p[i]=='-')
				flag=1;
			else
				value=value*10+p[i]-48;
			if (flag==0)
				gap[gap_len].min=value;
			else
				gap[gap_len].min=-value;
			value=0;
			flag=0;
			for (int j=i+1;p[j]!=']';j++)
				if (p[j]=='-')
					flag=1;
				else
					value=value*10+p[j]-48;
				if (flag==0)
					gap[gap_len].max=value;
				else
					gap[gap_len].max=-value;
	}
	if (gap[gap_len].max-gap[gap_len].min+1>maxp_c)
		maxp_c=gap[gap_len].max-gap[gap_len].min+1;
	gap_len++;
}

void convert_p_to_ruler(char *p)     //��pת��Ϊ��������
{
	char st,en;
	int l,r ;
	int len=strlen(p);
	st=p[0];
	en=p[len-1];
	if (isalpha(st) && isalpha(en))
	{
		l=0;
		for (int i=1;i<len;i++)
		{
			if (isalpha(p[i]))
			{
				r=i;
				dealrange(p+l,r-l);
				l=i;
			}
		}
	}
	else
		printf("irregular pattern");
}
void disp_pattern()         //��ʾ��������
{
	for (int i=0;i<gap_len;i++)
	{
		printf("%c\t%d\t%d\t%c\n",gap[i].start ,gap[i].min , gap[i].max ,gap[i].end );
	}
}
void create_subnettree_approx(int maxlen,int T)
{
	snt=new row [gap_len+1];
	for (int i=0;i<gap_len+1;i++)
	{
		snt[i].r =new node [maxlen];
		for (int j=0;j<maxlen;j++)
		{
			snt[i].r [j].tlpn =new __int64 [T+1];
			snt[i].r [j].flpn =new __int64 [T+1];
			snt[i].r [j].frpn =new __int64 [T+1];
			snt[i].r [j].trpn =new __int64 [T+1];
		}
	}
}
void release_subnettree(int maxlen)
{
	for (int i=gap_len;i>=0;i--)
	{
		for (int j=maxlen-1;j>=0;j--)
		{
			delete []snt[i].r [j].tlpn;
			delete []snt[i].r [j].flpn;
			delete []snt[i].r [j].frpn;
			delete []snt[i].r [j].trpn;
		}
		delete snt[i].r ;
	}
	delete []snt;
}
void searchup(char *s,int minlen,int maxlen,int pos,int level,__int64 *in,__int64 *out, int T)
{
	int i,j,k;
	for (int tt=0;tt<=T;tt++)
		in[tt]=out[tt]=0;
	//in=out=0;
	for (i=level-1;i>=0;i--)
	{
		//�������
		snt[i].start=snt[i+1].start - gap[i].max-1;
		if (gap[i].max <0)
			snt[i].start ++;
		
		if (snt[i].start<pos-maxlen+1)
			snt[i].start=pos-maxlen+1;
		if (snt[i].start<0)
			snt[i].start=0;
		
		//�����յ�
		snt[i].end =snt[i+1].end -gap[i].min -1;
		if (gap[i].min <0)
			snt[i].end++;
		
		if (snt[i].end>=pos)
			snt[i].end=pos;
		
		snt[i].num=0;
		for (j=0,k=snt[i].start;k<=snt[i].end;k++)//Ҫ���㵽���һ�����
		{
			int flag=0;
			int pn;
			if (s[k]==gap[i].start)
				pn=0;
			else
				pn=1;
			{
				for (int tt=0;tt<=T;tt++)
				{
					snt[i].r [j].frpn[tt] =snt[i].r [j].trpn[tt] =0;
				}
				snt[i].r [j].name =k;
				snt[i].r [j].flag =false;
				for (int l=0;l<snt[i+1].num ;l++)
				{
					//˵����ʵ�ʵĽ��
					__int64 frpn=0,trpn=0;
					/*for (int tt=0;tt<=T;tt++)
					{
						frpn+=snt[i+1].r [l].frpn[tt];
						trpn+=snt[i+1].r [l].trpn[tt];
						if (frpn +trpn>0)
							break;
					}*/
					if (snt[i+1].r [l].flag ==true)
						//(frpn +trpn>0)
						//if (snt[i+1].r [l].frpn +snt[i+1].r [l].trpn>0)
					{
						int p=snt[i+1].r [l].name;
						if(p==k)
							continue;
						int tmp=p-k;
						if (tmp>0)
							tmp--;
						if (gap[i].min <=tmp&&tmp<=gap[i].max )
						{
							flag=1;
							if (pn==0)
							{
								int tt;
								__int64 sum=0;
								for (tt=0;tt<=T;tt++)
								{
									snt[i].r [j].frpn[tt] +=snt[i+1].r [l].frpn[tt] ;
									snt[i].r [j].trpn[tt] +=snt[i+1].r [l].trpn[tt] ;
									sum+=snt[i].r [j].frpn[tt];
									sum+=snt[i].r [j].trpn[tt];
								}
								if (minlen<=pos-k+1&&pos-k+1<=maxlen)
								{
									for (int tt=0;tt<=T;tt++)
									{
										snt[i].r [j].trpn[tt]+=snt[i].r [j].frpn[tt];
										snt[i].r [j].frpn[tt]=0;
									}
								}
								if (sum>0)//(snt[i].r [j].frpn[tt]+snt[i].r [j].trpn[tt]>0)
									snt[i].r [j].flag= true;
							}
							else //pn==1
							{
								int tt;
								__int64 sum=0;
								for (tt=1;tt<=T;tt++)
								{
									snt[i].r [j].frpn[tt] +=snt[i+1].r [l].frpn[tt-1] ;
									snt[i].r [j].trpn[tt] +=snt[i+1].r [l].trpn[tt-1] ;
									sum+=snt[i].r [j].frpn[tt];
									sum+=snt[i].r [j].trpn[tt];
								}
								if (minlen<=pos-k+1&&pos-k+1<=maxlen)
								{
									for (tt=0;tt<=T;tt++)
									{
										snt[i].r [j].trpn[tt]+=snt[i].r [j].frpn[tt];
										snt[i].r [j].frpn[tt]=0;
									}
								}
								if (sum>0)//(snt[i].r [j].frpn[tt]+snt[i].r [j].trpn[tt]>0)
									snt[i].r [j].flag= true;
							}							
						}
					}
				}
			}
			if (flag==1)
			{
				j++;
				snt[i].num=j;
			}
		}
		if (snt[i].num==0)
			return ;
		else
		{
			snt[i].start=snt[i].r [0].name;
			snt[i].end=snt[i].r [snt[i].num-1].name;
		}
		
	}
	for (j=0;j<snt[0].num;j++  )
	{
		for (int tt=0;tt<=T;tt++)
		{
			in[tt]+=snt[0].r[j].trpn[tt];	
			out[tt]+=snt[0].r[j].frpn[tt];	//false root path number
		}
	}
}
void searchdown(char *s,int minlen,int maxlen,int pos,int level,__int64 *in,__int64 *out, int T)
{
	int i,j,k;
	for (int tt=0;tt<=T;tt++)
		in[tt]=out[tt]=0;
	//in=out=0;
	if (gap[level].min >=0)
	{
		return ;
	}
	for (i=level+1;i<=gap_len;i++)
	{
		snt[i].num=0;
		//�������
		snt[i].start=snt[i-1].start + gap[i-1].min;
		if (gap[i-1].min >=0)
			snt[i].start++;
		if (snt[i].start<pos-maxlen+1)
			snt[i].start=pos-maxlen+1;
		
		//�����յ�
		snt[i].end =snt[i-1].end +gap[i-1].max ;
		if (gap[i-1].max >=0)				
			snt[i].end ++;
		if (snt[i].end>=pos-1)				//���²���ʱ�������ҵ�posλ��
			snt[i].end=pos-1;
		
		for (j=0,k=snt[i].start;k<=snt[i].end;k++)//Ҫ���㵽���һ�����
		{
			int flag=0;
			int pn;
			if (s[k]==gap[i-1].end)
				pn=0;
			else
				pn=1;
			{
				for (int tt=0;tt<=T;tt++)
				{
					snt[i].r [j].flpn[tt] =snt[i].r [j].tlpn[tt] =0;
				}
				snt[i].r [j].name =k;
				snt[i].r [j].flag =false;
				for (int l=0;l<snt[i-1].num ;l++)
				{
					//˵����ʵ�ʵĽ��
					/*__int64 flpn=0,tlpn=0;
					for (int tt=0;tt<=T;tt++)
					{
						flpn+=snt[i-1].r [l].flpn[tt];
						tlpn+=snt[i-1].r [l].tlpn[tt];
						if (flpn +tlpn>0)
							break;
					}*/
					if (snt[i-1].r [l].flag ==true)
						//(flpn +tlpn>0)
						//if (snt[i-1].r [l].frpn +snt[i-1].r [l].trpn>0)
					{
						int p=snt[i-1].r [l].name;
						if(p==k)
							continue;
						int tmp=k-p;
						if (tmp>0)
							tmp--;
						if (gap[i-1].min <=tmp&&tmp<=gap[i-1].max )
						{
							flag=1;
							if (pn==0)
							{
								int tt;
								__int64 sum=0;
								for (tt=0;tt<=T;tt++)
								{
									snt[i].r [j].flpn[tt] +=snt[i-1].r [l].flpn[tt] ;
									snt[i].r [j].tlpn[tt] +=snt[i-1].r [l].tlpn[tt] ;
									sum+=snt[i].r [j].flpn[tt];
									sum+=snt[i].r [j].tlpn[tt];
								}
								if (minlen<=pos-k+1&&pos-k+1<=maxlen)
								{
									for ( tt=0;tt<=T;tt++)
									{
										snt[i].r [j].tlpn[tt]+=snt[i].r [j].flpn[tt];
										snt[i].r [j].flpn[tt]=0;
									}
								}
								if (sum>0)//(snt[i].r [j].frpn[tt]+snt[i].r [j].trpn[tt]>0)
									snt[i].r [j].flag= true;
							}
							else	//pn==1
							{
								int tt;
								__int64 sum=0;
								for (tt=1;tt<=T;tt++)
								{
									snt[i].r [j].flpn[tt] +=snt[i-1].r [l].flpn[tt-1] ;
									snt[i].r [j].tlpn[tt] +=snt[i-1].r [l].tlpn[tt-1] ;
									sum+=snt[i].r [j].flpn[tt];
									sum+=snt[i].r [j].tlpn[tt];
								}
								if (minlen<=pos-k+1&&pos-k+1<=maxlen)
								{
									for (tt=0;tt<=T;tt++)
									{
										snt[i].r [j].tlpn[tt]+=snt[i].r [j].flpn[tt];
										snt[i].r [j].flpn[tt]=0;
									}
								}
								if (sum>0)//(snt[i].r [j].frpn[tt]+snt[i].r [j].trpn[tt]>0)
									snt[i].r [j].flag= true;
							}							
						}
					}
				}
			}
			if (flag==1)
			{
				j++;
				snt[i].num=j;
			}
		}
		if (snt[i].num==0)
			return ;
		else
		{
			snt[i].start=snt[i].r [0].name;
			snt[i].end=snt[i].r [snt[i].num-1].name;
		}
		
	}
	for (j=0;j<snt[gap_len].num;j++  )
	{
		for (int tt=0;tt<=T;tt++)
		{
			in[tt]+=snt[gap_len].r[j].tlpn[tt];
			out[tt]+=snt[gap_len].r[j].flpn[tt];
		}
	}
}


__int64 deal_subnettree_approx(char *s,int minlen,int maxlen,int pos,int level,
							   __int64 *din, __int64 *dout, __int64 *uin, __int64 *uout, int T)
{
	__int64 stsp=0;
	int mm=gap_len;
	char pc;
	if (level==mm)
		pc=gap[level-1].end;
	else
		pc=gap[level].start;
	snt[level].num =1;
	
	snt[level].start =snt[level].end =pos;
	snt[level].r[0].name=pos;
	for (int tt=0;tt<=T;tt++)
	{
		snt[level].r[0].flpn[tt] =0;
		snt[level].r[0].tlpn[tt] =0;
		snt[level].r[0].frpn[tt] =0;
		snt[level].r[0].trpn[tt] =0;
	}
	snt[level].r[0].flpn[0] =1;
	snt[level].r[0].frpn[0] =1;
	snt[level].r[0].flag =true;
	if (level==gap_len)
	{	//�����һ�㣬�����һ�㵹�ƽ���������
		//ȷ����Χ
		//__int64 *in,*out;			//in ��ʾ��ȫ��Լ���ڵģ�out��ʾ����ȫ��Լ���ڵġ�	
		//in= new __int64 [T+1];
		//out=new __int64 [T+1];
//		if (T==0&&pc!=s[pos])
//			stsp=0;
//		else
//		{
			searchup(s,minlen,maxlen,pos,level,uin,uout,T);
			if (pc==s[pos])
			{
				for (int tt=0;tt<=T;tt++)
					stsp+=uin[tt];
			}
			else
			{
				for (int tt=0;tt<T;tt++)
					stsp+=uin[tt];
			}
//		}
		//delete []in;
		//delete []out;
		
	}
	else
	{	//���м��
		//__int64 *din,*dout;
		//din= new __int64 [T+1];
		//dout=new __int64 [T+1];
		searchdown(s,minlen,maxlen,pos,level,din,dout,T);
		//__int64 *uin,*uout;			//in ��ʾ��ȫ��Լ���ڵģ�out��ʾ����ȫ��Լ���ڵġ�	
		//uin= new __int64 [T+1];
		//uout=new __int64 [T+1];
		searchup(s,minlen,maxlen,pos,level,uin,uout,T);
		//stsp=uin*dout+din*uout+uin*din;
		if (s[pos]==pc)
		{
			for (int t1=0;t1<=T;t1++)
			{
				for (int t2=0;t2<=T-t1;t2++)
				{
					stsp+=uin[t1]*dout[t2]+din[t1]*uout[t2]+uin[t1]*din[t2];
				}
			}
		}
		else
		{
			for (int t1=0;t1<T;t1++)
			{
				for (int t2=0;t2<T-t1;t2++)
				{
					stsp+=uin[t1]*dout[t2]+din[t1]*uout[t2]+uin[t1]*din[t2];
				}
			}
		}
		//delete []din;
		//delete []dout;
		//delete []uin;
		//delete []uout;
		
	}
	return stsp;
}

__int64 calc_tsp_approx(char *s,int minlen,int maxlen,int T)
{
	int len=strlen(s);
	int i,j=gap_len;
	create_subnettree_approx(maxlen,T);
	//init_subnettree();
	__int64 tsp=0;
	__int64 *din,*dout;
	__int64 *uin,*uout;
	din= new __int64 [T+1];
	dout=new __int64 [T+1];
	uin= new __int64 [T+1];
	uout=new __int64 [T+1];
	//for (i=4;i<len;i++)
	for (i=minlen-1;i<len;i++)
	{
		for (j=gap_len;j>=0;j--)
		{
			/*if((j==gap_len&&gap[gap_len-1].max>=0)||
				(j>0&&j<gap_len&&gap[j-1].max >=0&&gap[j].min <0)
				||(j==0&&gap[j].min <0))*/
			{
				__int64 t=deal_subnettree_approx(s,minlen,maxlen,i,j,din,dout,uin,uout,T);
				tsp+=t;
				//printf("%d,%d:=%I64d\n",i,j,t);
			}
		}
		//getchar();
		//printf("%d:=%I64d\n",i,tsp);
		//int aa=tsp;
		//cout <<i<<":"<<aa<<endl;
	}
	release_subnettree(maxlen);
	delete []din;
	delete []dout;
	delete []uin;
	delete []uout;
	return tsp;
}
void Inputstr(char *fn,char *str)
{
	FILE* fp=fopen(fn,"r+");
	if (NULL==fp)
	{
		cout<<"Cannot find the file.\n";
		return;
	}
	fseek(fp,0,SEEK_END); 
	int len =ftell(fp);
	fseek(fp,0,0); 
	fscanf(fp, "%s",str);
}

void main()
{
	char fsn[]="s1.txt";
	char s[10000]="atggaggaagadag";//"CACAGG";//="aabbaa";
	Inputstr(fsn,s);
	//char p[1000]="A[-1,1]B";
	char p[1000]="G[0,2]A[0,1]C[1,2]G[0,2]T[0,1]C[0,1]C[0,2]A[0,1]C";
	//char p[1000]="G[-2,2]A[-2,1]C[1,2]G[0,2]T[0,1]C[0,1]C[0,2]A[0,1]C";
	//char p[1000]="G[-2,2]A[-2,1]C[-2,2]G[-2,2]T[0,1]C[0,1]C[0,2]A[0,1]C";	
	//char p[1000]="G[-2,2]A[-2,1]C[-2,2]G[-2,2]T[-2,1]C[-2,1]C[0,2]A[0,1]C";	
	//char p[1000]="G[-2,2]A[-2,1]C[-2,2]G[-2,2]T[-2,1]C[-2,1]C[-2,2]A[-2,1]C";	
	//char p[1000]="a[-2,1]g[0,1]a[-2,1]g[-2,1]a";//"C[-1,2]A[-5,-2]G";
	//char p[1000]="A[-5,6]C[-4,7]G[-3,8]T";
	//char s[10000]="aggga";
	//char p[1000]="a[-2,1]g[0,1]a[-2,1]g";
	int minlen,maxlen,T;
	cout <<"Please input the sequence:\n";
	//cin>>s;
	cout <<"Please input the pattern:\n";
	//cin>>p;
	convert_p_to_ruler(p);
	cout<<"Please input minlen, maxlen:";
	cin>>minlen>>maxlen;
	cout <<"Please input the Hamming distance:\n";
	cin>>T;
	disp_pattern();
	
	//cout<<"Ϊ��ȷ���㷨����ʱ�䣬��˶���ͬģʽ������У����������б���";
	//int lp;
	//cin>>lp;
	__int64 tsp;
	DWORD   dwBeginTime=GetTickCount();
	for (int i=0;i<10;i++)
	{
		tsp=calc_tsp_approx(s,minlen,maxlen,T);
	}
	
	DWORD   dwEndTime=GetTickCount();
	printf("TSP=%I64d\n",tsp);
	cout <<"Time cost:"<<(dwEndTime-dwBeginTime)/10.0<<endl;
}
