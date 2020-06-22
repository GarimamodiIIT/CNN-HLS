#include "baseline.h"



void prefetchH(int h, int local_image[C][K+Tl][W+K-1], int input[C][H][W])
{
	//printf("\n%d", h);
	if(h==0)
				{
					for(int p=0;p<C;p++)
						{
							for(int q=0;q<K;q++)
							{
								for(int r=0;r<W;r++)
								{
									local_image[p][q][r]=input[p][q][r];
								}
							}
						}
				}
LPFC:	for(int p=0;p<C;p++)
	{
		LPFQ:	for(int q=0;q<Tl;q++)
			{
				//printf("\n%d", (h+K+q)%(K+Tl));
				if(h+K+q>=H)
				{
					LPF1:for(int r=0;r<W;r++)
					{

						local_image[p][((h+K+q)%(K+Tl))][r]=0;
					}
				}
				else
				{
					LPF2:for(int r=0;r<W;r++)
					{

						local_image[p][((h+K+q)%(K+Tl))][r]=input[p][h+K+q][r];
					}
				}
			}
	}
}

void compute1(int mm, int m, int h, int local_kernel[Tm][K][K][C], int local_image[C][K+Tl][W+K-1], int hwin[C][K][K], int local_output[Tm][W])
{

	LW:for(int w=0;w<W+K-1;w++)
			{
				int in_val[C][K];
				IV1:for(int p=0;p<C;p++)
				{
					IV2:for(int q=0;q<K;q++)
					{
						in_val[p][q]=local_image[p][(h+q)%(K+Tl)][w];
					}
				}
				int out_val[C][K][K];
				//int out_val=0;
				LC:for(int c=0;c<C;c++)
				{
					LK1:for(int x=0;x<K;x++)
					{
						LK2:for(int y=0;y<K;y++)
						{
							hwin[c][x][y]=(y<K-1)?hwin[c][x][y+1]:in_val[c][x];
							out_val[c][x][y]=hwin[c][x][y]*local_kernel[mm][x][y][c];
							//out_val+=hwin[c][x][y]*local_kernel[mm][x][y][c];
							//out_val+=local_image[c][((h+x)%(K+Tl))][w+y]*local_kernel[x][y][c];
						}
					}
				}


				if((w>=K-1))
				{
					//output[m][h][w]=out_val;
					//output[m+mm][h][w-K+1]=out_val;
					//local_output[mm][w-K+1]=out_val;   uncomment
					//printf("\n%d, %d, %d", m, h, w-K+1);
					local_output[mm][w-K+1]=0;
					Lout1:for(int u=0;u<C;u++)
					{
						Lout2:for(int v=0;v<K;v++)
						{
							Lout3:for(int z=0;z<K;z++)
							{
								local_output[mm][w-K+1]+=out_val[u][v][z];
								//out_val_total+=out_val[u][v][z];

								//local_output[mm][w-K+1]=out_val;
							}
						}
					}

				}
			}

				/*for(int u=0;u<W-K+1;u++)
				{
					output[m][h][u]=local_output[h][u];
				}*/
}

void baseline(int input[C][H][W], int kernel[M][K][K][C], int output[M][H][W])
{
#pragma HLS INTERFACE m_axi depth=196608 port=output offset=slave
#pragma HLS INTERFACE m_axi depth=540 port=kernel offset=slave
#pragma HLS INTERFACE m_axi depth=196608 port=input offset=slave


	int local_image[C][K+Tl][W+K-1];
	int local_kernel[Tm][K][K][C];
	int local_output[Tm][W];
	
	//initializations
	
	P1:for(int i=0;i<C;i++)
	{
		P2:for(int j=0;j<K+Tl;j++)
		{
			P3:for(int k=W;k<W+K-1;k++)
			{
				local_image[i][j][k]=0;
			}
		}
	}

	/*II1:for(int p=0;p<C;p++)
	{
		II2:for(int q=0;q<K;q++)
		{
			II3:for(int r=0;r<W;r++)
			{
				local_image[p][q][r]=input[p][q][r];
			}
		}
	}*/

	//load kernel
	LM:for(int m=0;m<M;m+=Tm)
	{	

		IK0:for(int mmm=0;mmm<Tm;mmm++)
		{
		IK1:for(int p=0;p<K;p++)
		{
			IK2:for(int q=0;q<K;q++)
			{
				IK3:for(int r=0;r<C;r++)
				{
					local_kernel[mmm][p][q][r]=kernel[m+mmm][p][q][r];
				}
			}
		}
		}

		LMM:for(int mm=0;mm<Tm;mm++) //before h
		{
	
	//PE storage element

	int hwin[C][K][K];
	LH:for(int h=0;h<H;h++)
	{


		prefetchH(h, local_image, input);
		compute1(mm, m, h, local_kernel, local_image, hwin, local_output);
		/*for(int w=0;w<W;w++)
		{
			int in_val[C][K];
			for(int p=0;p<C;p++)
			{
				for(int q=0;q<K;q++)
				{
					in_val[p][q]=local_image[p][(h+q)%(K+Tl)][w];
				}
			}
			int out_val=0;
			for(int c=0;c<C;c++)
			{
				for(int x=0;x<K;x++)
				{
					for(int y=0;y<K;y++)
					{
						hwin[c][x][y]=(y<K-1)?hwin[c][x][y+1]:in_val[c][x];
						out_val+=hwin[c][x][y]*local_kernel[x][y][c];
					}
				}
			}
			if((w>=K-1))
			{
				output[m][h][w-K+1]=out_val;
				//printf("\n%d, %d, %d", m, h, w-K+1);

			}
		}*/

			/*for(int u=0;u<W-K+1;u++)
			{
				output[m][h][u]=local_output[h][u];
			}*/

		Lout:for(int u=0;u<W-K+1;u++)
		{
			output[m+mm][h][u]=local_output[mm][u];
		}
	}//LH loop
	}
	}
}
