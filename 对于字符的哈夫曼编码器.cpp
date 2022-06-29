#include<iostream>
#include<string>
#include<algorithm>
using namespace std;
struct HCode//����ṹ
{
	char data;//��������ַ�
	string code;//����
};
struct HNode//���������ṹ
{
	double weight;//Ȩ��
	int parent;
	int Lch;
	int Rch;
};
class Huffman
{
private:
	HNode* huffTree;//������
	HCode* HCodeTable;//���
	int num;//Ҷ�ӽ�������
public:
	void CreateHTree(double a[], int n);//������
	void CreateCodeTable(char b[]);//�����������ݴ�����
	void showHCodeTable();//չʾ������ɵı����
	void Encode(string s, string &d);//����
	void Decode(string s, string &d);//����
	~Huffman();
};
Huffman::~Huffman()
{
	delete[]this->HCodeTable;
	delete[]this->huffTree;
	this->HCodeTable = NULL;
	this->huffTree = NULL;
	this->num = 0;
}
void SelectMin(HNode* hTree, int n, int& i1, int& i2)//ѡ��Ȩ������С������
{
	int i=0;
	//��һ���Ƚ�ֵ����ʼֵ
	for (; i < n; i++)//��i1
	{
		if (hTree[i].parent == -1)
		{
			i1 = i;
			break;
		}
	}
	i++;
	for (; i < n; i++)//��i2
	{
		if (hTree[i].parent == -1)
		{
			i2 = i;
			break;
		}
	}
	if (hTree[i1].weight > hTree[i2].weight)
	{
		int j = i2;
		i2 = i1;
		i1 = j;
	}
	//ȷ����ʼֵ�󣬿�ʼѰ����С����������
	i++;
	for (; i < n; i++)
	{
		if (hTree[i].parent == -1)
		{
			if (hTree[i].weight < hTree[i1].weight)
			{
				i2 = i1;              
				i1 = i;
			}//����i1 ��֤i1λ��������С��
			else if (hTree[i].weight < hTree[i2].weight)
			{
				i2 = i;
			}//����i2 ��֤�ǵڶ�С��

		}
	}
}
void Huffman::CreateHTree(double a[], int n)
{
	huffTree = new HNode[2 * n - 1];
	this->num = n;
	for (int i = 0; i < n; i++)
	{
		huffTree[i].weight = a[i];
		huffTree[i].Lch = -1;
		huffTree[i].Rch = -1;
		huffTree[i].parent = -1;
	}
	int li, ri;//�ֱ𴢴�ĿǰȨ����ǰ��С��
	for (int i = num; i < 2 * n - 1; i++)//��ȡ������������С��
	{
		SelectMin(huffTree, i, li, ri);
		huffTree[li].parent = huffTree[ri].parent = i;
		huffTree[i].weight = huffTree[li].weight + huffTree[ri].weight;//����½��
		huffTree[i].Lch = li;//�µĽ��Ϊԭ����С���������ĸ��ڵ�
		huffTree[i].Rch = ri;
		huffTree[i].parent = -1;
	}
}
void Huffman::CreateCodeTable(char b[])
{
	HCodeTable = new HCode[num];//���ɱ����
	for (int i = 0; i < num; i++)
	{
		HCodeTable[i].data = b[i];
		int ic = i;
		int ip = huffTree[i].parent;
		int k = 0;//��¼HCodeTable������code�����ݱ���λ��
		while (ip != -1)
		{
			if (ic == huffTree[ip].Lch)//���ӱ�0
				HCodeTable[i].code +="0";
			else//�Һ��ӱ�1
				HCodeTable[i].code +="1";
			k++;
			ic = ip;
			ip = huffTree[ic].parent;
		}
		reverse(HCodeTable[i].code.begin(), HCodeTable[i].code.end());//���õ��ı��뵹��
	}
}
void Huffman::showHCodeTable()
{
	for (int i = 0; i < num; i++)
	{
		cout << HCodeTable[i].data << "�ı���Ϊ:" << HCodeTable[i].code << endl;
	}
}
void Huffman::Encode(string s, string &d)//sΪԭ�� dΪ���봮
{
	for (int i = 0; i < s.length(); i++)
	{
		for (int j = 0; j < num; j++)
		{
			if (s[i] == HCodeTable[j].data)
				d += HCodeTable[j].code;
		}
	}
	cout << "������Ϊ��" << d << endl;
	cout << "����ǰ�Ĵ�С" << s.length()*8 << endl;//�ַ�����ռ���ܱ�����
	cout << "�����Ĵ�С" << d.length() << endl;//���봮��ռ�ܱ�����
	cout << "ѹ����" << (double)(s.length() * 8-d.length())/ s.length()*8 << endl; 

}//����
void Huffman::Decode(string s, string& d)//sΪ���봮 dΪ���봮
{
	for (int i = 0; i < s.length();)
	{
		int k = 2 * num - 1 - 1;//���ڵ��ڹ��������е��±�
		while (huffTree[k].Lch != -1)//���������õ�����
		{
			if ( s[i] == '0')
				k = huffTree[k].Lch;
			else
				k = huffTree[k].Rch;
			i++;
		}	
		d += HCodeTable[k].data;
	}

	cout << "������Ϊ��" << d << endl;
}//����
void charnumber(string s, int& n)//ͳ���ַ����г����ַ��ĸ���
{
	string s1 = s;
	int num2 = 0;//��¼ͳ�Ƶ��ַ�����
	char temp;//��¼ͷ�ַ�
	while (!s1.empty())
	{	temp = s1[0];
		for (int i = 0; i < s1.length();)//���ַ������Ѿ�ͳ�ƹ����ַ�ȫ��ɾ��
		{
			if (s1[i] == temp)
			{
				s1.erase(i,1);
				continue;
			}
			i++;
		}
		num2++;
	}
	n = num2;
}
void deal(string s, double value[], char b[])//���ַ������д����õ��ַ������Ӧ��Ȩ��
{
	string s1 = s;
	int num1= 0;//��¼ÿ���ַ�������
	int num2 = 0;//��¼ͳ�Ƶ��ַ�����
	char temp;
	int sum = 0;//����Ȩ���ܺ�
	while (!s1.empty())
	{
		num1 = count(s1.begin(), s1.end(), s1[0]);
		temp = s1[0];
		value[num2] = num1;
		b[num2] = temp;
		sum += value[num2];
		for (int i = 0; i < s1.length();)//���ַ������Ѿ�ͳ�ƹ����ַ�ȫ��ɾ��
		{
			if (s1[i] == temp)
			{
				s1.erase(i, 1);
				continue;
			}
			i++;
		}
		num2++;
	}
	for (int i = 0; i < num2; i++)//�õ�Ȩ��
	{
		value[i] = (double)value[i] / sum;
	}
}
void showMeau()
{
	cout << "**********************" << endl;
	cout << "*****  1������� *****" << endl;
	cout << "*****  2������   *****" << endl;
	cout << "*****  3������   *****" << endl;
	cout << "*****  0���˳�   *****" << endl;
	cout << "**********************" << endl;
}
int main()
{
	int select = -1;
	int b = -1;
	cout << "����������Ҫ��ȡ�Ĵ������������ķ�ʽ��"<<endl;
	cout << "********1�������ַ����Զ�����*********"<<endl;
	cout << "********0���ֶ������ַ���Ȩ��*********"<<endl;
	cin >> b;
	Huffman h;
	int n = 0;
	if (b == -1)
	{
		cout << "��զ��������,����������" << endl;
		return 0;
	}
	else if (b==1)
	{
		string s;
		cout << "����������Ҫ���������������ַ�����";
		getline(cin, s);
		getline(cin, s);
		charnumber(s, n);
		double* value = new double[n];
		char* b = new char[n];
		deal(s, value, b);	
		h.CreateHTree(value, n);
		h.CreateCodeTable(b);
		delete[]value, b;
	}
	else if(b==0)
	{
		cout << "�����봴���ַ��ĸ�����";
		cin >> n;
		double* value = new double[n];
		char* b = new char[n];
		for (int i = 0; i < n; i++)
		{
			cout << "�������" << i + 1 << "���ַ�" << endl;
			cin >> b[i];
			cout << "�������" << i + 1 << "���ַ���Ȩ��" << endl;
			cin >> value[i];
			
		}
		h.CreateHTree(value, n);
		h.CreateCodeTable(b);
		delete[]value, b;
	}
	else
	{
		cout << "����ָʾ��С�ı�����" << endl;
		return 0;
	}
	while (select)
	{
		showMeau();
		cin >> select;
		switch (select)
		{
			case 1:
			{
				h.showHCodeTable();
				break;
			}
			case 2:
			{
				string c, d;
				cout << "����������Ҫ������ַ����������ֵ��ַ�Ҫ�ڱ�����г���):" << endl;
				getline(cin, c);//��һ�����ȡ���س�
				getline(cin, c);
				h.Encode(c, d);
				break;
			}
			case 3:
			{
				string c, d;
				cout << "����������Ҫ��������봮��" << endl;
				getline(cin, c);//��һ�����ȡ���س�
				getline(cin, c);
				h.Decode(c, d);
				break;
			}
			case 0:
			{
				cout << "��ӭ�´�ʹ��" << endl;
				break;
			}
			default:
				break;
		}
	}
	return 0;
}