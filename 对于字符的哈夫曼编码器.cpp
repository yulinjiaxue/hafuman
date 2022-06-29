#include<iostream>
#include<string>
#include<algorithm>
using namespace std;
struct HCode//编码结构
{
	char data;//储存编码字符
	string code;//编码
};
struct HNode//哈夫曼树结构
{
	double weight;//权重
	int parent;
	int Lch;
	int Rch;
};
class Huffman
{
private:
	HNode* huffTree;//树本体
	HCode* HCodeTable;//码表
	int num;//叶子结点的数量
public:
	void CreateHTree(double a[], int n);//创建树
	void CreateCodeTable(char b[]);//根据树的内容创建表
	void showHCodeTable();//展示创建完成的编码表
	void Encode(string s, string &d);//编码
	void Decode(string s, string &d);//解码
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
void SelectMin(HNode* hTree, int n, int& i1, int& i2)//选择权重中最小的两个
{
	int i=0;
	//找一个比较值的起始值
	for (; i < n; i++)//找i1
	{
		if (hTree[i].parent == -1)
		{
			i1 = i;
			break;
		}
	}
	i++;
	for (; i < n; i++)//找i2
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
	//确定初始值后，开始寻找最小的两个数据
	i++;
	for (; i < n; i++)
	{
		if (hTree[i].parent == -1)
		{
			if (hTree[i].weight < hTree[i1].weight)
			{
				i2 = i1;              
				i1 = i;
			}//更新i1 保证i1位置上是最小的
			else if (hTree[i].weight < hTree[i2].weight)
			{
				i2 = i;
			}//更新i2 保证是第二小的

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
	int li, ri;//分别储存目前权重中前两小的
	for (int i = num; i < 2 * n - 1; i++)//获取数据中两个最小的
	{
		SelectMin(huffTree, i, li, ri);
		huffTree[li].parent = huffTree[ri].parent = i;
		huffTree[i].weight = huffTree[li].weight + huffTree[ri].weight;//获得新结点
		huffTree[i].Lch = li;//新的结点为原先最小的两个结点的父节点
		huffTree[i].Rch = ri;
		huffTree[i].parent = -1;
	}
}
void Huffman::CreateCodeTable(char b[])
{
	HCodeTable = new HCode[num];//生成编码表
	for (int i = 0; i < num; i++)
	{
		HCodeTable[i].data = b[i];
		int ic = i;
		int ip = huffTree[i].parent;
		int k = 0;//记录HCodeTable数组中code中数据编码位数
		while (ip != -1)
		{
			if (ic == huffTree[ip].Lch)//左孩子标0
				HCodeTable[i].code +="0";
			else//右孩子标1
				HCodeTable[i].code +="1";
			k++;
			ic = ip;
			ip = huffTree[ic].parent;
		}
		reverse(HCodeTable[i].code.begin(), HCodeTable[i].code.end());//将得到的编码倒置
	}
}
void Huffman::showHCodeTable()
{
	for (int i = 0; i < num; i++)
	{
		cout << HCodeTable[i].data << "的编码为:" << HCodeTable[i].code << endl;
	}
}
void Huffman::Encode(string s, string &d)//s为原串 d为编码串
{
	for (int i = 0; i < s.length(); i++)
	{
		for (int j = 0; j < num; j++)
		{
			if (s[i] == HCodeTable[j].data)
				d += HCodeTable[j].code;
		}
	}
	cout << "编码结果为：" << d << endl;
	cout << "编码前的大小" << s.length()*8 << endl;//字符串所占的总比特数
	cout << "编码后的大小" << d.length() << endl;//编码串所占总比特数
	cout << "压缩率" << (double)(s.length() * 8-d.length())/ s.length()*8 << endl; 

}//编码
void Huffman::Decode(string s, string& d)//s为密码串 d为解码串
{
	for (int i = 0; i < s.length();)
	{
		int k = 2 * num - 1 - 1;//根节点在哈夫曼树中的下标
		while (huffTree[k].Lch != -1)//遍历树，得到解码
		{
			if ( s[i] == '0')
				k = huffTree[k].Lch;
			else
				k = huffTree[k].Rch;
			i++;
		}	
		d += HCodeTable[k].data;
	}

	cout << "解码结果为：" << d << endl;
}//解码
void charnumber(string s, int& n)//统计字符串中出现字符的个数
{
	string s1 = s;
	int num2 = 0;//记录统计的字符个数
	char temp;//记录头字符
	while (!s1.empty())
	{	temp = s1[0];
		for (int i = 0; i < s1.length();)//将字符串中已经统计过的字符全部删除
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
void deal(string s, double value[], char b[])//对字符串进行处理，得到字符与其对应的权重
{
	string s1 = s;
	int num1= 0;//记录每个字符的数量
	int num2 = 0;//记录统计的字符个数
	char temp;
	int sum = 0;//计算权重总和
	while (!s1.empty())
	{
		num1 = count(s1.begin(), s1.end(), s1[0]);
		temp = s1[0];
		value[num2] = num1;
		b[num2] = temp;
		sum += value[num2];
		for (int i = 0; i < s1.length();)//将字符串中已经统计过的字符全部删除
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
	for (int i = 0; i < num2; i++)//得到权重
	{
		value[i] = (double)value[i] / sum;
	}
}
void showMeau()
{
	cout << "**********************" << endl;
	cout << "*****  1、编码表 *****" << endl;
	cout << "*****  2、编码   *****" << endl;
	cout << "*****  3、译码   *****" << endl;
	cout << "*****  0、退出   *****" << endl;
	cout << "**********************" << endl;
}
int main()
{
	int select = -1;
	int b = -1;
	cout << "请输入你想要采取的创建哈夫曼树的方式："<<endl;
	cout << "********1、输入字符串自动生成*********"<<endl;
	cout << "********0、手动输入字符与权重*********"<<endl;
	cin >> b;
	Huffman h;
	int n = 0;
	if (b == -1)
	{
		cout << "你咋不输入捏,不和你玩了" << endl;
		return 0;
	}
	else if (b==1)
	{
		string s;
		cout << "请输入你想要创建哈夫曼树的字符串：";
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
		cout << "请输入创建字符的个数：";
		cin >> n;
		double* value = new double[n];
		char* b = new char[n];
		for (int i = 0; i < n; i++)
		{
			cout << "请输入第" << i + 1 << "个字符" << endl;
			cin >> b[i];
			cout << "请输入第" << i + 1 << "个字符的权重" << endl;
			cin >> value[i];
			
		}
		h.CreateHTree(value, n);
		h.CreateCodeTable(b);
		delete[]value, b;
	}
	else
	{
		cout << "不按指示，小心崩坏啊" << endl;
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
				cout << "请输入你想要编码的字符串（所出现的字符要在编码表中出现):" << endl;
				getline(cin, c);//第一个会读取到回车
				getline(cin, c);
				h.Encode(c, d);
				break;
			}
			case 3:
			{
				string c, d;
				cout << "请输入你想要解码的密码串：" << endl;
				getline(cin, c);//第一个会读取到回车
				getline(cin, c);
				h.Decode(c, d);
				break;
			}
			case 0:
			{
				cout << "欢迎下次使用" << endl;
				break;
			}
			default:
				break;
		}
	}
	return 0;
}