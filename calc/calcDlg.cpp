
// calcDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "calc.h"
#include "calcDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CcalcDlg �Ի���




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


// CcalcDlg ��Ϣ�������

BOOL CcalcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CcalcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CcalcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CcalcDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//����S����S���������֣���ʾվ��
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
	//ȥ������
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
	//ȥ������
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
		//ȥ������
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
		//ȥ������
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

//��ȡ���� 0����ֵ 1�����  2�ӷ���ʽ 3������ʽ
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
	//����S����S���������֣���ʾվ��
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
//����ʽ�������ӷָ
//�ָ�ʧ�ܣ������ʽ�����⣬�򷵻�false
bool CcalcDlg::split(CStringList &factorList,CString formula)
{
	if (formula.GetLength()==0)return true;
	CString strFactor;
	int posDot = formula.Find(",");
	//��ʽǶ��
	int pos2 = formula.Find("(");
	//�����������Ż���������λ�ô��ڶ���
	//�����ڹ�ʽǶ��,���ߵ�һ��ǹ�ʽ
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
	//���������ţ���λ��С�ڶ���
	//""SUM(2,3) "SUM(2,3),1" "SUM(2,3),SUM(3,4),SUM(3,4)" "SUM(SUM(1,2),2),3"
	else
	{
		//û��ƥ���������
		int count = 1;
		int pos = pos2+1;
		while(count&&pos<formula.GetLength())
		{
			char c = formula.GetAt(pos);
			if (c=='(')count++;
			if(c==')')count--;
			pos++;
		}
		//������������ƥ��
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