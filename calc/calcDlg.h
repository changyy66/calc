
// calcDlg.h : ͷ�ļ�
//

#pragma once
#define VALUE 0
#define SPOTID 1
#define SUM 2
#define SUB 3
#define MUL 4
#define DIV 5

// CcalcDlg �Ի���
class CcalcDlg : public CDialogEx
{
// ����
public:
	CcalcDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CALC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString strInput;
	CString strValue;
	afx_msg void OnBnClickedOk();
	CString calc(CString formula);
	CString GetSpotValue(CString SpotID);
	CString Sum(CString formula);
	CString Sub(CString formula);
	CString Mul(CString formula);
	CString Div(CString formula);
	bool split(CStringList &factorList,CString formula);
	int GetType(CString factor);
	bool IsDigit(CString factor);
	bool IsSpotID(CString factor);
};
