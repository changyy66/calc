
// calcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "calc.h"
#include "calcDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcalcDlg 对话框




CcalcDlg::CcalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcalcDlg::IDD, pParent)
	, strInput(_T(""))
	, strValue(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, strInput);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, strValue);
}

BEGIN_MESSAGE_MAP(CcalcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CcalcDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CcalcDlg 消息处理程序

BOOL CcalcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CcalcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcalcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcalcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CcalcDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	strValue = calc(strInput);

	UpdateData(false);
	//CDialogEx::OnOK();
}

CString CcalcDlg::calc(CString formula)
{
	int type = GetType(formula);
	switch(type)
	{	
	case VALUE:
		return formula;
	case SPOTID:
		return GetSpotValue(formula);
	case SUM:
		return Sum(formula);
	case SUB:
		return Sub(formula);
	case MUL:
		return Mul(formula);
	case DIV:
		return Div(formula);
	default:
		return "";
	}
}

CString CcalcDlg::GetSpotValue(CString SpotID)
{
		int posS = SpotID.Find("S");
	int posE = SpotID.Find("E");
	//存在S，且S后面是数字，表示站点
	if(posS>-1)
	{
		CString stationNo;
		if(posE>-1)
		{
			stationNo = SpotID.Mid(posS+1,posE-posS-1);
		}
		else
		{
			stationNo = SpotID.Mid(posS+1);
		}
		return stationNo;		
	}
	return "";
}

CString CcalcDlg::Sum(CString formula)
{
	//去掉外层壳
	formula.TrimLeft(" ");
	formula.TrimRight(" ");
	formula = formula.Mid(4);
	formula = formula.Left(formula.GetLength()-1);

	double value=0;
	CStringList factorList;
	if(split(factorList,formula))
	{
		POSITION pos = factorList.GetHeadPosition();
		while(pos)
		{
			CString tmp = factorList.GetNext(pos);
			CString strValue = calc(tmp);
			value += atof(strValue);
		}
	}
	CString retValue;
	retValue.Format("%.4f",value);
	return retValue;
}

CString CcalcDlg::Sub(CString formula)
{
	//去掉外层壳
	formula.TrimLeft(" ");
	formula.TrimRight(" ");
	formula = formula.Mid(4);
	formula = formula.Left(formula.GetLength()-1);

	double value=0;
	CString strValue;
	CStringList factorList;
	if(split(factorList,formula))
	{
		POSITION pos = factorList.GetHeadPosition();
		if(pos)
		{
			CString tmp = factorList.GetNext(pos);
			strValue = calc(tmp);
			value = atof(strValue);
		}
		while(pos)
		{
			CString tmp = factorList.GetNext(pos);
			strValue = calc(tmp);
			value -= atof(strValue);
		}
	}
	CString retValue;
	retValue.Format("%.4f",value);
	return retValue;
}

CString CcalcDlg::Mul(CString formula)
{
		//去掉外层壳
	formula.TrimLeft(" ");
	formula.TrimRight(" ");
	formula = formula.Mid(4);
	formula = formula.Left(formula.GetLength()-1);

	double value=1;
	CString strValue;
	CStringList factorList;
	if(split(factorList,formula))
	{
		POSITION pos = factorList.GetHeadPosition();
		while(pos)
		{
			CString tmp = factorList.GetNext(pos);
			strValue = calc(tmp);
			value *= atof(strValue);
		}
	}
	CString retValue;
	retValue.Format("%.4f",value);
	return retValue;
}

CString CcalcDlg::Div(CString formula)
{
		//去掉外层壳
	formula.TrimLeft(" ");
	formula.TrimRight(" ");
	formula = formula.Mid(4);
	formula = formula.Left(formula.GetLength()-1);

	double value=0;
	CString strValue;
	CStringList factorList;
	if(split(factorList,formula))
	{
		POSITION pos = factorList.GetHeadPosition();
		if(pos)
		{
			CString tmp = factorList.GetNext(pos);
			strValue = calc(tmp);
			value = atof(strValue);
		}
		while(pos)
		{
			CString tmp = factorList.GetNext(pos);
			strValue = calc(tmp);
			double divValue = atof(strValue);
			if(divValue<0.0001&&divValue>-0.0001)return "NAM";
			value /= divValue;
		}
	}
	CString retValue;
	retValue.Format("%.4f",value);
	return retValue;
}

//获取类型 0：数值 1测点编号  2加法公式 3减法公式
int CcalcDlg::GetType(CString factor)
{
	if(IsDigit(factor)) return VALUE;
	else if(IsSpotID(factor))return SPOTID;
	else if(factor.Left(3)=="SUM")return SUM;
	else if(factor.Left(3)=="SUB")return SUB;
	else if(factor.Left(3)=="MUL")return MUL;
	else if(factor.Left(3)=="DIV")return DIV;
	else return -1;
}

bool CcalcDlg::IsDigit(CString factor)
{
	if(factor.GetLength()==0)return false;

	int i=0;
	if(factor.GetAt(0)=='-'||factor.GetAt(0)=='+')i++;
	for(;i<factor.GetLength();i++)
	{
		if((!isdigit(factor.GetAt(i))) && factor.GetAt(i)!='.')return false;
	}
	return true;
}

bool CcalcDlg::IsSpotID(CString factor)
{
	int posS = factor.Find("S");
	int posE = factor.Find("E");
	//存在S，且S后面是数字，表示站点
	if(posS>-1)
	{
		CString stationNo;
		if(posE>-1)
		{
			stationNo = factor.Mid(posS+1,posE-posS-1);
		}
		else
		{
			stationNo = factor.Mid(posS+1);
		}
		return IsDigit(stationNo);		
	}
	return false;
}
//将公式按照因子分割开
//分割失败，如果公式有问题，则返回false
bool CcalcDlg::split(CStringList &factorList,CString formula)
{
	if (formula.GetLength()==0)return true;
	CString strFactor;
	int posDot = formula.Find(",");
	//公式嵌套
	int pos2 = formula.Find("(");
	//不存在左括号或者左括号位置大于逗号
	//不存在公式嵌套,或者第一项不是公式
	//"1,2"  "1,SUM(2,3)"
	if (pos2<0 || pos2>posDot)
	{

		//"1" 
		if (posDot<0)
		{
			strFactor = formula;
			formula = "";
		}
		else
		{
			strFactor = formula.Left(posDot);
			formula = formula.Mid(posDot+1);
		}
		factorList.AddTail(strFactor);		
	}
	//存在左括号，且位置小于逗号
	//""SUM(2,3) "SUM(2,3),1" "SUM(2,3),SUM(3,4),SUM(3,4)" "SUM(SUM(1,2),2),3"
	else
	{
		//没有匹配的括号数
		int count = 1;
		int pos = pos2+1;
		while(count&&pos<formula.GetLength())
		{
			char c = formula.GetAt(pos);
			if (c=='(')count++;
			if(c==')')count--;
			pos++;
		}
		//左括号右括号匹配
		if(count==0)
		{
			strFactor = formula.Left(pos);
			factorList.AddTail(strFactor);
			if (pos==formula.GetLength())
			{
				formula = "";
			}
			else
			{
				formula = formula.Mid(pos+1);
			}
		}
		else
		{
			return false;
		}
	}
	return split(factorList,formula);

}