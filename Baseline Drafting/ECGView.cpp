// ECGView.cpp : implementation of the CECGView class
//

#include "stdafx.h"
#include "ECG.h"

#include "ECGDoc.h"
#include "ECGView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECGView

IMPLEMENT_DYNCREATE(CECGView, CScrollView)

BEGIN_MESSAGE_MAP(CECGView, CScrollView)
	//{{AFX_MSG_MAP(CECGView)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECGView construction/destruction

CECGView::CECGView()
{
	// TODO: add construction code here

}

CECGView::~CECGView()
{
}

BOOL CECGView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CECGView drawing

void CECGView::OnDraw(CDC* pDC)
{
	CECGDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	ECG_Plot(pDC);
}

void CECGView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	//初始化
	m_fileOpen = FALSE;
	strcpy(fname, "");

	OnFileOpen();
}

/////////////////////////////////////////////////////////////////////////////
// CECGView printing

BOOL CECGView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CECGView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CECGView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CECGView diagnostics

#ifdef _DEBUG
void CECGView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CECGView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CECGDoc* CECGView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CECGDoc)));
	return (CECGDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CECGView message handlers

void CECGView::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CFileDialog fdlg(TRUE);
	if (fdlg.DoModal () == IDOK)
	{
		//AfxMessageBox(fdlg.GetFileTitle());
		
		//fdlg.GetFileName();GetFileExt   GetFileTitle GetFileName
		//GetFileExt()获取文件扩展名为.dat的文件
		if (  strcmp (fdlg.GetFileExt(),"dat") == 0 )
		{
			strcpy(fname,fdlg.GetFileTitle());
			//AfxMessageBox( fdlg.GetFileName());
			//LPCTSTR lpsz1 ="C:\MYFILE.TXT";
			//AfxFormatString1("The title you have chosen is:%1",NULL,"aa");
			m_fileOpen = TRUE;
		}
		
		else { 
			AfxMessageBox("can't open this file! The file must be \".dat\" style.");
			CECGView::OnFileOpen();
		}
		
	}
	//GetDocument()->OnNewDocument();
	Invalidate();
	
}

/////////////////////////////////////////////////////////////////////////////
/* 本篇程序依据注释文件,打印出标准的散点图和标准的QRS位置 */
/* 再用低通滤波器检测出来的QRS位置标上，和标准的做对比  */
/*如果出现库连接的错误，project-->settings-->link-->category-->input(下拉列表)-->ignore libraries-->填上msvcrtd.lib*/

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"


#include <wfdb/wfdb.h>
#include <wfdb/ecgmap.h>
#include <wfdb/ecgcodes.h>

#pragma comment( lib, "bmiwfdbb.lib" )

void CECGView::ECG_Plot(CDC *pDC)
{

	int nsig, k,m = 0,im = 0, nm = 0;

	long int *origin;
	int  *R_num, *iR_position, *R_negativePositionNum;

	WFDB_Sample *v;
    WFDB_Siginfo *s;	
	if ((nsig = isigopen(fname, NULL, 0)) < 1) exit(2);	
    s = (WFDB_Siginfo *)malloc(nsig * sizeof(WFDB_Siginfo));
    v = (WFDB_Sample *)malloc(nsig * sizeof(WFDB_Sample));
    if (s == NULL || v == NULL)  exit(2);   
    if (isigopen(fname, s, nsig) != nsig)  
		exit(3);
	

	int freq = 360;
	//int NN = 700000 * freq / 360;
	iR_position = (int *)calloc(3000,sizeof(int));

	setifreq(360.);
	
	FILE *fp;
	CString hz= ".dat";
	int N_data;
	if ((fp = fopen(fname+hz,"rb")) != NULL)
	{
		fseek(fp,0,SEEK_END);
		N_data = ftell(fp)/3;
		fclose(fp);
		//MessageBox(Length);
	}

	origin = (long int*) malloc(N_data * sizeof(long int));

	for( k = 0; k < N_data; k++ ) {
		if(getvec(v) > 0)
			origin[k] = v[0];
		else break;
	}


//矫正基线漂移的代码范围

	int structelement1[3] = {0,2,0};

	int structelement2[54];

	for (int tt = 0; tt < 54; tt++)
	{
		structelement2[tt] = 0;
	}

	long int *f1,*f2,*f3,*ocf0k1,*cof0k1,*ocf1k2,*cof1k2;
	f1 = (long int *)malloc(N_data * sizeof(long int));
	f2 = (long int *)malloc(N_data * sizeof(long int));
	f3 = (long int *)malloc(N_data * sizeof(long int));

	ocf0k1 = (long int *)malloc(N_data * sizeof(long int));
	cof0k1 = (long int *)malloc(N_data * sizeof(long int));
	ocf1k2 = (long int *)malloc(N_data * sizeof(long int));
	cof1k2 = (long int *)malloc(N_data * sizeof(long int));

	OpeningClosing(origin,structelement1,ocf0k1,N_data,3);
	ClosingOpening(origin,structelement1,cof0k1,N_data,3);

	for (int i0 = 0; i0 < N_data; i0++)
	{
		f1[i0] = (ocf0k1[i0] + cof0k1[i0])/2;
	}

	OpeningClosing(f1,structelement2,ocf1k2,N_data,54);
	ClosingOpening(f1,structelement2,cof1k2,N_data,54);

	for(int i1 = 0; i1 < N_data; i1++)
	{
		f2[i1] = (ocf1k2[i1] + cof1k2[i1])/2;
		f3[i1] = f1[i1] - f2[i1];
	}

/*
	FILE *fp3;
	if ((fp3 = fopen("F:\\baseline100.txt", "w+")) != NULL)
	{
		for (int g = 0;g < N_data; g++)
		{
			fprintf(fp3, "%d ", f3[g]);
		}
		fclose(fp3);
	}
	else{MessageBox("写入文件100.txt失败！","错误");
	}
*/
	//
	N_data = k;
	R_num = &m;
	R_negativePositionNum = &nm;
	//iR_num = &im;

	//////
	CSize sizeTotal,sizePage,sizeLine;	
	sizeTotal.cx = N_data;
	sizeTotal.cy = 1200;
	sizePage.cx = 5000;
	sizePage.cy = 2000;
	sizeLine.cx = 1000;
	sizeLine.cy = 2000;
	SetScrollSizes(MM_TEXT, sizeTotal,sizePage,sizeLine);
	//////

	//绘图坐标的横轴为20开始
	pDC -> MoveTo(20, 20);	 pDC -> LineTo(20, 620);

	pDC -> TextOut(25, 625, "O(0,0)");
	pDC -> TextOut(100,10, "File Name:");
	pDC -> TextOut(180, 10, fname);

	/*下面读取注释文件的QRS*/
	WFDB_Anninfo a;
	WFDB_Annotation annot;
	int *QRS = (int *)malloc(3000 *sizeof(int));
	int rnum = 0;
	long atime = 0;
	
	a.name = "atr";		//
	a.stat = WFDB_READ;
	
	if(annopen(fname, &a, 1) < 0)
	{
		exit(2);
	}
	
	while(getann(0,&annot) == 0)
	{
		if (isqrs(annot.anntyp))
		{
			if (1)
			{
				QRS[rnum++] = annot.time - atime;
			}
			atime = annot.time;
		}
	}
	


	//基线
//	pDC -> MoveTo(0,500-1024/3); pDC -> LineTo(N_data,500 - 1024/3);
	LineDashLineR(pDC, 20, 500-1024/3, N_data,500-1024/3);
//	pDC -> MoveTo(0,700); pDC ->LineTo(N_data,700);



	/*	
	//低通滤波器显示在这里
	int j0, j1,j2,j3,j4;
	int Low_Pass_Filter_plot;
	j4 = j3 =j2 = j1 =j0 = v[0];
	isigsettime(0);
	(void)getvec(v);
	pDC->MoveTo(20,500);
	for (int l = 0; l < n-1; l++)
	{
		if(getvec(v) < 0) break;
		Low_Pass_Filter_plot = 0.2*(j0 + j1  + j2 + j3 + j4);
		pDC ->LineTo(l +21, 700-Low_Pass_Filter_plot/4);
		j4 = j3;
		j3 = j2;
		j2 = j1;
		j1 = j0;
		j0 = v[0];
	}

*/




	//绘图
	long n0 = strtim("0:00");
	int n = N_data;
/*

*/
	for (int j = 0; j < N_data; j++)
	{
		//if(getvec(v) < 0) break;
		pDC -> MoveTo(20 + j, 500 - origin[j]/3);
		pDC -> LineTo(j + 21, 500 - origin[j+1]/3);
	}

	//基线漂移修正图像

	long int *f5;
	f5 = (long int *)malloc(N_data * sizeof(long int));

	for ( j = 0; j < N_data; j++)
	{	
		if (j <= 56)
		{
		//	f3[j] = 0;
			f5[j] = 0;
		}
	//	else{
	//	pDC ->MoveTo(j + 20, 700 - f3[j]);
	//	pDC -> LineTo(j + 21, 700 - f3[j+1]);
	//	}
	}

	//
	//IIR
	//Butterworth	

	/*
	Wp = 20/180; Ws = 2/180;
	Rp = 3; Rs = 40;
	[n,Wn] = buttord(Wp,Ws,Rp,Rs);
	% Returns n =2; Wn =0.1100;
	[b,a] = butter(n,Wn);

  //	a	1	-1.51808406371133	0.613491928464162
  //	b	0.0238519661882088	0.0477039323764177	0.0238519661882088

*/
	float x0, x1, x2, y0, y1, y2;
	y2 = y1 = y0 = x2 = x1 = x0 = 0.0;
	
	for (int i = 57; i < N_data - 104; i++)
	{
		y0 = (int)(1.51808406371133*y1 - 0.613491928464162*y2 + 0.0238519661882088*x0 + 0.0477039323764177*x1 + 0.0238519661882088*x2);
		f5[i] = y0;
		
		y2 = y1;
		y1 = y0;
		
		x0 = x1;
		x1 = x2;
		x2 = f3[i];
		
	}


	for ( j = 0; j < N_data; j++)
	{	
			pDC ->MoveTo(j + 20, 500 - f5[j]);
			pDC -> LineTo(j + 21, 500 - f5[j+1]);
	}
/*
	FILE *fp3;
	if ((fp3 = fopen("H:\\baseline109.txt", "w+")) != NULL)
	{
		for (int g = 0;g < N_data -109; g++)
		{
			fprintf(fp3, "%d ", f3[g]);
		}
		fclose(fp3);
	}
	else{MessageBox("写入文件失败！","错误");
	}

*/
	int *R_Position,*Q_Position,*S_Position;
	R_Position = (int *)calloc(3000,sizeof(int));
	Q_Position = (int *)calloc(3000,sizeof(int));
	S_Position = (int *)calloc(3000,sizeof(int));
	int *R_NegativePosition;
	R_NegativePosition = (int *)calloc(1000,sizeof(int));

	R_detect(f5,R_Position,R_NegativePosition,N_data,R_num,R_negativePositionNum);

	Q_detect(f5,R_Position,Q_Position,R_num);

	S_detect(f5,R_Position,S_Position,R_num);

	//绿色虚线表标准
	int r = 0;
	long t = 0;
	long it = 0;
	int z = 0;
	int iz = 0;
	CString Precise_R_Position;
	CString number;
	
	for (int g = 0; g < rnum; g++)
	{
		
		z = QRS[g] + z;
		if (z < n + n0 + 10 && z > n0)
		{
			t = z + 20 -n0;
			pDC -> TextOut(t,80,"QRS");
			LineDashLine(pDC, t,60, t, 400/*500- v[0] / 4*/,RGB(0,255,0));
			Precise_R_Position.Format("%d",t -20);
			pDC -> TextOut(z-20,210,Precise_R_Position);
			number.Format("%d", g+1);
			pDC -> TextOut(z-18,195,number);
		}
	}

	//红色虚线表示检测到的R波
	z = 0;
	iz = 0;
	t = 0;
	it = 0;
	CString Detect_R_Position;
	for (i = 0; i < *R_num; i++)
	{
		z = R_Position[i];
		
		if (z < n + n0 && z > 0)
		{
			t = z + 20 -n0;
			LineDashLine(pDC, t,600, t, 240, RGB(0,0,0));
			pDC -> TextOut(t,300,"R");
			Detect_R_Position.Format("%d",t-20);
			pDC -> TextOut(z-20,300,Detect_R_Position);

			number.Format("%d", i+1);
			pDC -> TextOut(z-18,280,number);

		}

	}

	//Q
	z = 0;
	t = 0;
	for (i = 0; i < *R_num; i++)
	{
		z = Q_Position[i];
		if (z < N_data && z > 0)
		{
			t = z +20;
			LineDashLineR(pDC,t,550,t,400);
			pDC ->TextOut(t-10,500,"Q");
		}
	}
	
	//S
	z = 0;
	t = 0;
	for (i = 0; i < *R_num; i++)
	{
		z = S_Position[i];
		if (z < N_data && z > 0)
		{
			t = z +20;
			LineDashLineR(pDC,t,550,t,400);
			pDC ->TextOut(t+10,500,"S");
		}
	}




	CString Detect_R_Neg_Position;
	for(i = 0; i < *R_negativePositionNum; i++)
	{
		iz = R_NegativePosition[i];

		if (iz < n + n0 && iz > 0)
		{
			it  = iz + 20 -n0;
			LineDashLine(pDC, it, 900, it, 540, RGB(24,56,89));
			pDC -> TextOut(it, 700, "R");
			Detect_R_Neg_Position.Format("%d",it-20);
			pDC -> TextOut(iz-20,700,Detect_R_Neg_Position);
		}
	}



	//R波检出率
	float R_Percentage;
	R_Percentage = (((float)*R_num)/(float)rnum)*100;
	CString data_R_num,data_rnum,data_R_Percentage,data_R_Neg;
	
	data_R_num.Format("%d",*R_num);
	data_rnum.Format("%d",rnum);
	data_R_Percentage.Format("%5.3f",R_Percentage);
	data_R_Neg.Format("%d",*R_negativePositionNum);
	
	pDC ->TextOut(500,30,"R_num:");
	pDC ->TextOut(630,30,"Detect_R_num:");
	pDC ->TextOut(500,50,"Percentage%:");
	
	pDC ->TextOut(550,30,data_rnum);
	pDC ->TextOut(730,30,data_R_num);
	pDC ->TextOut(600,50,data_R_Percentage);
	pDC ->TextOut(600,70,data_R_Neg);

	ReleaseDC(pDC);
	free(f1);
	free(f2);
	free(f3);
	free(f5);
	free(origin);
	free(cof0k1);
	free(cof1k2);
	free(ocf0k1);
	free(ocf1k2);

	return;
}



void CECGView::LineDashLine(CDC *pDC, int xStart, int yStart, int xEnd, int yEnd, COLORREF color)
{
	CPen * pOldPen;
	CPen * pNewPen = new CPen (PS_DOT, 1, color);
	pOldPen = (CPen *)pDC->SelectObject(pNewPen);
	pDC->MoveTo(xStart, yStart);
	pDC->LineTo(xEnd, yEnd);
	pNewPen->DeleteObject();
	pDC->SelectObject(pOldPen);
	delete pNewPen;

}


//腐蚀运算
void CECGView::Erosion(long int *origin, int* structElement, long int *Eroded,int N_data, int se){

	int M = se;
	int N = N_data;
	int *minErosion;

	minErosion = (int *) malloc(M * sizeof(int));
	for(int n = 0; n < N - M; n++){
		int min_num = 0;
		for (int m = 0; m < M; m++)
		{
			minErosion[m] = origin[m+n] - structElement[m];
			min_num = minErosion[0];
			if (minErosion[m] <= min_num)
			{
				min_num = minErosion[m];
			}
		}

		Eroded[n] = min_num;
	}
	return;
}

//膨胀运算
void CECGView::Dilation(long int *origin, int* structElement, long int *Dilated,int N_data, int se)
{
	int M = se;
	int N = N_data;
	int *maxDilation;
	maxDilation = (int *) malloc(M * sizeof(int));

	for(int n = M - 1; n < N - 1; n++){
		int max_num = 0;
		for (int m = 0; m < M; m++)
		{
			maxDilation[m] = origin[n - m] - structElement[m];
			max_num = maxDilation[0];
			if (maxDilation[m] >= max_num)
			{
				max_num = maxDilation[m];
			}
		}

		Dilated[n] = max_num;
	}
	return;
}


void CECGView::OpeningClosing(long int *origin, int* structElement, long int *OC,int N_data,int se)
{
	long int *preOC0,*preOC1,*preOC2;
	long int N = N_data;

	preOC0 = (long int *)malloc(N * sizeof(long int));
	preOC1 = (long int *)malloc(N * sizeof(long int));
	preOC2 = (long int *)malloc(N * sizeof(long int));

	Erosion(origin,structElement,preOC0,N - se,se);
	Dilation(preOC0,structElement,preOC1,N - se,se);

	Dilation(preOC1,structElement,preOC2,N - se,se);
	Erosion(preOC2,structElement,OC,N - se,se);

	free(preOC0);
	free(preOC1);
	free(preOC2);

	return;
}


void CECGView::ClosingOpening(long int *origin, int* structElement, long int *CO,int N_data,int se)
{
	long int *preCO0,*preCO1,*preCO2;
	long int N = N_data;

	preCO0 = (long int *)malloc(N * sizeof(long int));
	preCO1 = (long int *)malloc(N * sizeof(long int));
	preCO2 = (long int *)malloc(N * sizeof(long int));

	Dilation(origin,structElement,preCO0,N - se,se);
	Erosion(preCO0,structElement,preCO1,N - se,se);
	
	Erosion(preCO1,structElement,preCO2,N - se,se);
	Dilation(preCO2,structElement,CO,N - se,se);

	free(preCO0);
	free(preCO1);
	free(preCO2);

	return;
}

void CECGView::R_detect(long int *origin, int *R_Position,int *R_NegativePosition, int N_data, int *R_num,int *R_negativePositionNum)
{
	//取开始的10段，每段3s
	int Num_segment = 10;
	int Len_segment = 1080;

	long int *diff = (long int *)malloc(N_data * sizeof(long int));

	//求全部差分
	for (int q = 0; q < N_data; q++)
	{
		diff[q] = origin[q + 1] - origin[q];
	}

	//分为10段，求各段内的最大差分值
	int Segment_data[10][1080];
	int *max_diff = (int *)malloc(Num_segment * sizeof(int));

	for (int n = 0; n < Num_segment; n++)
	{
		int Max_segment_diff = 0;

		for (int j = n*Len_segment, k = 0; j < (n + 1)*Len_segment; j++, k++)
		{
			Segment_data[n][k] = diff[j];
			
			if(Max_segment_diff < Segment_data[n][k])
			{
				Max_segment_diff = Segment_data[n][k];
			}
		}	
		max_diff[n] = Max_segment_diff;
	}

	//对max_diff[]进行排序，去掉最大最小，得到算数平均值alpha_0
	sort(max_diff, max_diff + Num_segment);

	double alpha_0 = 0;

	for (int r = 1; r < 9; r++) {
		alpha_0 += max_diff[r];
	}
	alpha_0 = alpha_0 / 8.0;


	//确定初始阈值系数delta
	double delta_0 = (2.0/5.0)*alpha_0;
	double delta_1 = (2.0/5.0)*alpha_0 + 2;
	double delta_2 = (2.0/9.0)*alpha_0;
	
	//确定初始常数c
	double c_1 = 0.1 * alpha_0;
	double c_2 = 0.1 * alpha_0 + 2;
	double c_3 = 0.1 * alpha_0;

	//开始检测R波
	int *max_diff_pre = (int *)malloc(3000 * sizeof(int));
	int i = 0;


	while (i < N_data - 1)
	{
		if ((diff[i] > delta_0) && (diff[i+1] > delta_1))//第一个判断
		{
			for (int j = i; j < i + 30; j++)
			{
				if ((diff[j] < 0) && (fabs((float)diff[j]) > delta_2))//第二个判断
				{
					R_Position[*R_num] = j;
					
					int max_diff_ecg = 0;
					if (*R_num >= 1)
					{
						for(int s = R_Position[*R_num - 1]; s <= R_Position[*R_num]; s++) {    //两个R波之间一定包含有一个差分最大值点
							if(max_diff_ecg < diff[s])
							{
								max_diff_ecg = diff[s];
							}
						}
						max_diff_pre[*R_num] = max_diff_ecg;
					}
					else
					{
						max_diff_ecg = 0;
						for(int s = 0; s <= R_Position[*R_num]; s++)
						{
							if(max_diff_ecg < diff[s])
							{
								max_diff_ecg = diff[s];
							}
						}
						max_diff_pre[*R_num] = max_diff_ecg;
					}
					
					*R_num += 1; 
					break;
				}
			}
			
			if (*R_num >= 8)
			{
				int update_alpha_0 = 0;//取得前8个最大差分值之后*R_num已经加1
				for (int t = *R_num - 8; t <= *R_num - 1; t++)
				{
					update_alpha_0 += max_diff_pre[t];
				}
				
				update_alpha_0 = update_alpha_0 / 8.0;
				delta_0 = 0.25  * update_alpha_0 + c_1;
				delta_1 = 0.25  * update_alpha_0 + c_2;
				delta_2 = 0.125 * update_alpha_0 + c_3;
			}
	
			if ( i < N_data - 1) {
				i += 60;
			}
			
		}
		else
		{
			i++;    //i索引要更新
		}
	}



	//倒置R波检测漏检检测

	int *RR_interval = (int *)malloc(3000 * sizeof(int));
	*R_negativePositionNum = 0;

	int aveRR;
	float aveHR;
	int sum = 0;
	int baseave = 0;

	aveRR = (R_Position[*R_num-1] - R_Position[0])/(*R_num);
	for(int k = 0; k < *R_num; k++)
	{
		RR_interval[k] = R_Position[k+1] - R_Position[k];
		
		sum = origin[R_Position[k]] + sum;

		aveHR = sum/(k+1);

		if (k > 3)
		{
			if (RR_interval[k] > 1.6 * aveRR)
			{
				int max_R = 0;
				int min_R = 0;
				int max_origin = 0;
				int min_origin = 0;
				for (int ir = R_Position[k] + 0.7*aveRR; ir <= R_Position[k] + 1.2*aveRR; ir ++)
				{
					if(max_R < origin[ir])
					{
						max_R = origin[ir];
						max_origin = ir;
					}
					
					if (min_R > origin[ir])
					{
						min_R = origin[ir];
						min_origin = ir;
					}
				}
				baseave = (max_R + min_R)/2;

				if ((max_R > 0.8*aveHR) && (max_R < 1.2*aveHR))
				{
					*R_num = *R_num + 1;
					R_NegativePosition[*R_negativePositionNum] = max_origin;
					*R_negativePositionNum +=1;
				}
			
				else if((baseave <= 30) && (baseave >= -30)) //1024和0代表着不同的信号
				{
					
					*R_num = *R_num + 1;
					R_NegativePosition[*R_negativePositionNum] = min_origin;
					*R_negativePositionNum +=1;
				}
			
				else
				{
					break;
				}
			}
		}

	}

	free(RR_interval);
	free(diff);
	free(max_diff_pre);
	free(max_diff);
	return;
}


void CECGView::LineDashLineR(CDC *pDC, int xStart, int yStart, int xEnd, int yEnd)
{
	CPen * pOldPen;
	CPen * pNewPen = new CPen (PS_DOT, 1, RGB(255,0,512));
	pOldPen = (CPen *)pDC->SelectObject(pNewPen);
	pDC->MoveTo(xStart, yStart);
	pDC->LineTo(xEnd, yEnd);
	pNewPen->DeleteObject();
	pDC->SelectObject(pOldPen);
	delete pNewPen;
	
}


void CECGView::Q_detect( int long *origin, int *R_Position, int *Q_Position, int *R_num)
{
	int i = 0;
	
	while(i < *R_num)
	{
		for (int j = 2; j < 20; j++)
		{
			if (origin[R_Position[i] - j - 1] <= origin[R_Position[i] - j - 2])
			{
				Q_Position[i] = R_Position[i] - j -2;
				break;
			}
		}
		i++;
	}
	
}

void CECGView::S_detect( int long *origin, int *R_Position, int *S_Position, int *R_num)
{
	int i = 0;
	
	while(i < *R_num)
	{
		for (int j = 2; j < 30; j++)
		{
			if (origin[R_Position[i] + j + 1] <= origin[R_Position[i] + j + 2])
			{
				S_Position[i] = R_Position[i] + j + 2;
				break;
			}
		}
		i++;
	}
}