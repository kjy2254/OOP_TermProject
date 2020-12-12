#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>

#define WRONGINPUT "�߸��� ����Դϴ�."

using namespace std;

static int startPageIndex = 0;
static int lastWordIndex = 0;

int printPage(int startIndex, vector<string>& vector) {
	//���� ��ġ ����
	startPageIndex = startIndex;
	//���ٿ� �ִ� 75����Ʈ�� 20�� ���
	int lineByte = 0;
	for (int lineNumber = 1; lineNumber <= 20; lineNumber++) {
		printf("%2d| ", lineNumber);
		while (vector[startIndex] != "\0" && lineByte <= 76) {
			lineByte += vector[startIndex].length();
			if (lineByte <= 76) {
				cout << vector[startIndex];
			}
			else
				startIndex--;
			startIndex++;
		}
		cout << endl;
		lineByte = 0;
	}
	//������������ ���� ��ġ ��ȯ
	return startIndex;
}
bool is_number(const string& s) {
	//string�� ���ڰ� �ƴ� ���ڰ� �����ϴ��� Ȯ��
	string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}
void printMenu() {
	for (int i = 1; i < 81; i++) {
		cout << "-";
	}
	cout << endl;
	cout << " n: ����������, p: ����������, i: ����, d: ����, c: ����, s:ã��, t: ����������" << endl;
	for (int i = 1; i < 81; i++) {
		cout << "-";
	}
	cout << endl;
}
void printConsolMessage(string e) {
	cout << " (�ܼ� �޽���) " << e << endl;
	for (int i = 1; i < 81; i++) {
		cout << "-";
	}
	cout << endl;
}
string inputUserMessage() {
	string input;
	cout << " �Է�: ";
	cin >> input;
	return input;
}
void inputFileToVector(fstream& file, vector<string>& vector) {
	//������ ������ ������ �������� vector�� ����
	string temp;
	while (file.peek() != EOF) {
		file >> temp;
		vector.push_back(temp + ' ');
	}
	vector.push_back("\0");
}
class Command {
public:
	virtual void excuteCommand(string s, vector<string>& vector) = 0;
};
class excuteCommandN : public Command {
public:
	void excuteCommand(string s, vector<string>& vector) {
		//�߸��� �Է� üũ
		if (s.length() >= 2) {
			lastWordIndex = printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		//��¿� ������ �ܾ ���ԵǴ��� üũ
		int startIndex = lastWordIndex;
		int lineByte = 0;
		bool printLastWord = true;
		for (int lineNumber = 1; lineNumber <= 20; lineNumber++) {
			while (vector[startIndex] != "\0" && lineByte <= 76) {
				lineByte += vector[startIndex].length();
				if (lineByte <= 76) {
					printLastWord = false;
					startIndex++;
				}
			}
			lineByte = 0;
		}
		//������ �ܾ ���ԵǴ� ���
		if (printLastWord) {
			int lineByte = 0;
			int lastIndex = startIndex;
			for (int i = 0; i < 20; i++) {
				while (lineByte < 76) {
					lastIndex--;
					if (lastIndex < 0) {
						lastIndex = 0;
						break;
					}
					lineByte += vector[lastIndex].length();
					if (lineByte > 76) {
						lineByte = 0;
						lastIndex++;
						break;
					}
					else if (lineByte == 76) {
						lineByte = 0;
						break;
					}

				}
				if (lastIndex < 0) {
					lastIndex = 0;
					break;
				}
			}
			lastWordIndex = printPage(lastIndex, vector);
			throw string("������ ������ �Դϴ�.");
		}
		//���Ե��� �ʴ� ���
		else {
			lastWordIndex = printPage(lastWordIndex, vector);
			throw string("���� �������� ��µǾ����ϴ�.");
		}
	}
};
class excuteCommandP : public Command {
public:
	void excuteCommand(string s, vector<string>& vector) {
		//�߸��� �Է� üũ
		if (s.length() >= 2) {
			lastWordIndex = printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		//ù ��° ������������ �Է����� üũ
		if (startPageIndex == 0) {
			lastWordIndex = printPage(startPageIndex, vector);
			throw string("ù ��° ������ �Դϴ�.");
		}
		//���� ������ ���
		int lineByte = 0;
		int lastIndex = startPageIndex;
		for (int i = 0; i < 20; i++) {
			while (lineByte < 76) {
				lastIndex--;
				if (lastIndex < 0) {
					lastIndex = 0;
					break;
				}
				lineByte += vector[lastIndex].length();
				if (lineByte > 76) {
					lineByte = 0;
					lastIndex++;
					break;
				}
				else if (lineByte == 76) {
					lineByte = 0;
					break;
				}

			}
			if (lastIndex < 0) {
				lastIndex = 0;
				break;
			}
		}
		lastWordIndex = printPage(lastIndex, vector);
		throw string("���� �������� �ͷµǾ����ϴ�.");
		printMenu();
		printConsolMessage("");
	}
};
class excuteCommandI : public Command {
public:
	void excuteCommand(string s, vector<string>& vector) {
		char lineNumberBuffer[4];
		char wordNumberBuffer[4];
		char wordBuffer[80];
		int commaIndex;
		int lineNumber;
		int wordNumber;
		string temp = s.substr(1);
		// '('�� ',' , ')'�� ���� ��� üũ 
		if (temp[0] != '(') {
			printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		if (temp[temp.length() - 1] != ')') {
			printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		temp = temp.substr(1);
		if ((commaIndex = temp.find(",")) == string::npos) {
			printPage(startPageIndex, vector);
			throw string("���ڰ� �����մϴ�.");
		}
		//ù ��° ������ ���� üũ
		if (commaIndex > 2) {
			printPage(startPageIndex, vector);
			throw string("�߸��� LineNumber �Դϴ�.");
		}
		//ù ��° ������ LineNumber�� ����
		int i = 0;
		while (i < commaIndex) {
			lineNumberBuffer[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		lineNumberBuffer[i] = '\0';
		i++;
		temp = temp.substr(1);
		stringstream s1(lineNumberBuffer);
		s1 >> lineNumber;
		//LineNumer�� 20�� �Ѱų� ���ڰ� �ƴѰ�� üũ
		if (!is_number(lineNumberBuffer) || lineNumber > 20 || lineNumber <= 0) {
			printPage(startPageIndex, vector);
			throw string("�߸��� LineNumber �Դϴ�.");
		}
		//�� ��° ',' üũ
		if ((commaIndex = temp.find(",")) == string::npos) {
			printPage(startPageIndex, vector);
			throw string("���ڰ� �����մϴ�.");
		}
		//�� ��° ������ ���� üũ
		if (commaIndex > 2) {
			printPage(startPageIndex, vector);
			throw string("�߸��� WordNumber �Դϴ�.");
		}
		//�� ��° ������ WordNumber�� ����
		i = 0;
		while (i < commaIndex) {
			wordNumberBuffer[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		wordNumberBuffer[i] = '\0';
		stringstream s2(wordNumberBuffer);
		s2 >> wordNumber;
		//WordNumer�� ���ڰ� �ƴϰų� 0������ ��� üũ
		if (!is_number(wordNumberBuffer) || wordNumber <= 0) {
			printPage(startPageIndex, vector);
			throw string("�߸��� WordNumber �Դϴ�.");
		}
		//�� ��° ������ �Է� ���ڸ� ����
		temp = temp.substr(1);
		if (temp.length() > 76 || temp.length() <= 1) {
			printPage(startPageIndex, vector);
			throw string("�Է��ϰ��� �ϴ� ���ڰ� ���ų� �ʹ� ��ϴ�.");
		}
		int j = 0;
		while (temp.length() != 1) {
			wordBuffer[j] = temp[0];
			temp = temp.substr(1);
			j++;
		}
		wordBuffer[j] = ' ';
		wordBuffer[j + 1] = '\0';
		string insertWord(wordBuffer);

		//Line�� ������ġ�� �̵�
		int findIndex = startPageIndex;
		int lineByte = 0;
		for (int line = 1; line < lineNumber; line++) {
			while (vector[findIndex] != "\0" && lineByte <= 76) {
				lineByte += vector[findIndex].length();
				if (lineByte > 76) {
					findIndex--;
				}
				findIndex++;
				if (vector[findIndex] == "\0") {
					printPage(startPageIndex, vector);
					throw string("�ش� ������ ���������ʽ��ϴ�.");
				}
			}
			lineByte = 0;
		}
		//Word�� ��ġ�� �̵�
		for (int word = 1; word < wordNumber; word++) {
			lineByte += vector[findIndex].length();
			findIndex++;
			if (lineByte > 76 || vector[findIndex] == "\0") {
				printPage(startPageIndex, vector);
				throw string("�ش� ��ġ�� Word�� �������� �ʽ��ϴ�.");
			}
		}
		//�ش� ��ġ�� ������ insertWord �Է�
		vector.insert(vector.begin() + findIndex + 1, insertWord);
		//���
		printPage(startPageIndex, vector);
		printMenu();
		printConsolMessage("�־��� �ܾ �Է��߽��ϴ�.");
	}
};
class excuteCommandD : public Command {
public:
	void excuteCommand(string s, vector<string>& vector) {
		char lineNumberBuffer[4];
		char wordNumberBuffer[4];
		int commaIndex;
		int lineNumber;
		int wordNumber;
		string temp = s.substr(1);
		//������ ���� üũ
		if (temp.length() > 7) {
			printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		// '('�� ',' , ')'�� ���� ��� üũ 
		if (temp[0] != '(') {
			printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		if (temp[temp.length() - 1] != ')') {
			printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		temp = temp.substr(1);
		if ((commaIndex = temp.find(",")) == string::npos) {
			printPage(startPageIndex, vector);
			throw string("���ڰ� �����մϴ�.");
		}
		//','�� �������� ���� LineNumber�� ����
		int i = 0;
		while (i < commaIndex) {
			lineNumberBuffer[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		lineNumberBuffer[i] = '\0';
		i++;
		temp = temp.substr(1);
		stringstream s1(lineNumberBuffer);
		s1 >> lineNumber;
		//LineNumer�� ���� ���̰ų� ���ڰ� �ƴѰ�� üũ
		if (!is_number(lineNumberBuffer) || lineNumber > 20 || lineNumber <= 0) {
			printPage(startPageIndex, vector);
			throw string("�߸��� LineNumber �Դϴ�.");
		}
		//','�� �������� ���� WordNumber�� ����
		int j = 0;
		while (temp.length() != 1) {
			wordNumberBuffer[j] = temp[0];
			temp = temp.substr(1);
			j++;
		}
		wordNumberBuffer[j] = '\0';
		stringstream s2(wordNumberBuffer);
		s2 >> wordNumber;
		//WordNumer�� ���ڰ� �ƴϰų� 0������ ��� üũ
		if (!is_number(wordNumberBuffer) || wordNumber <= 0) {
			printPage(startPageIndex, vector);
			throw string("�߸��� WordNumber �Դϴ�.");
		}
		//Line�� ������ġ�� �̵�
		int findIndex = startPageIndex;
		int lineByte = 0;
		for (int line = 1; line < lineNumber; line++) {
			while (vector[findIndex] != "\0" && lineByte <= 76) {
				lineByte += vector[findIndex].length();
				if (lineByte > 76) {
					findIndex--;
				}
				findIndex++;
				if (vector[findIndex] == "\0") {
					printPage(startPageIndex, vector);
					throw string("�ش� ������ ���������ʽ��ϴ�.");
				}
			}
			lineByte = 0;
		}
		//Word�� ��ġ�� �̵�
		for (int word = 1; word < wordNumber; word++) {
			lineByte += vector[findIndex].length();
			findIndex++;
			if (lineByte > 76 || vector[findIndex] == "\0") {
				printPage(startPageIndex, vector);
				throw string("�ش� ��ġ�� Word�� �������� �ʽ��ϴ�.");
			}
		}
		//�ش� ��ġ�� Word ����
		vector.erase(vector.begin() + findIndex);
		//���
		printPage(startPageIndex, vector);
		printMenu();
		printConsolMessage("�־��� �ܾ �����߽��ϴ�.");
	}
};
class excuteCommandC : public Command {
public:
	void excuteCommand(string s, vector<string>& vector) {
		char firstWordBuffer[80];
		char secondWordBuffer[80];
		unsigned int commaIndex;
		string temp = s.substr(1);

		// '('�� ',' , ')'�� ���� ��� üũ 
		if (temp[0] != '(') {
			printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		if (temp[temp.length() - 1] != ')') {
			printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		temp = temp.substr(1);
		if ((commaIndex = temp.find(",")) == string::npos) {
			printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		//ù ��° ���ڰ� 75����Ʈ �̻��� ��� üũ
		if (commaIndex > 75) {
			printPage(startPageIndex, vector);
			throw string("ù ��° ���ڰ� �ʹ� ��ϴ�.");
		}
		//','�� �������� ���� ���ڸ� ����
		unsigned int i = 0;
		while (i < commaIndex) {
			firstWordBuffer[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		firstWordBuffer[i] = ' ';
		firstWordBuffer[i + 1] = '\0';
		i++;
		temp = temp.substr(1);
		string firstWord(firstWordBuffer);
		//s1 >> firstWord;
		//','�� �������� ���� ���ڸ� ����
		int j = 0;
		while (i < s.length() - 3) {
			secondWordBuffer[j] = temp[0];
			temp = temp.substr(1);
			if (j >= 75) {
				printPage(startPageIndex, vector);
				throw string("�� ��° ���ڰ� �ʹ� ��ϴ�.");
			}
			i++;
			j++;
		}
		secondWordBuffer[j] = ' ';
		secondWordBuffer[j + 1] = '\0';
		string secondWord(secondWordBuffer);
		//ù ��° ���� �˻� �� �� ��° ���� ��ü
		string temp1(firstWord);
		j = 0;
		bool find = false;
		while (vector[j] != "\0") {
			if (vector[j].compare(firstWord) == 0) {
				find = true;
				vector.erase(vector.begin() + j);
				vector.insert(vector.begin() + j, secondWord);
			}
			j++;
		}
		if (find == false) {
			printPage(startPageIndex, vector);
			throw string("�ش� �ܾ ã�� �� �����ϴ�.");
			return;
		}
		//���
		printPage(startPageIndex, vector);
		printMenu();
		printConsolMessage("�־��� �� �ܾ ��ü�߽��ϴ�.");
	}
};
class excuteCommandS : public Command {
public:
	void excuteCommand(string s, vector<string>& vector) {
		char serchWord[80];
		string temp = s.substr(1);
		//��ȣ�� ���ų� ������ ���̰� �� ��� üũ
		if (temp[0] != '(' || temp.length() < 2) {
			printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		if (temp[temp.length() - 1] != ')') {
			printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		if (temp.length() > 77) {
			printPage(startPageIndex, vector);
			throw string("ã���� �ϴ� ���ڰ� 75Byte�� �ѽ��ϴ�.");
		}
		//�Էµ� string ����
		temp = temp.substr(1);
		unsigned int i = 0;
		while (i < s.length() - 3) {
			serchWord[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		serchWord[i] = ' ';
		serchWord[i + 1] = '\0';
		string temp2(serchWord);
		//�Էµ� string�� �����ϴ��� üũ
		int j = 0;
		while (vector[j].compare(temp2) != 0) {
			j++;
			if (vector[j] == "\0") {
				printPage(startPageIndex, vector);
				throw string("�ش� �ܾ ã�� �� �����ϴ�.");
				return;
			}
		}
		//ã�� ���ڸ� �������� ������ ���
		printPage(j, vector);
		printMenu();
		printConsolMessage("�־��� �ܾ ã�ҽ��ϴ�.");
	}
};
class excuteCommandT : public Command {
public:
	void excuteCommand(string s, vector<string>& vector) {
		//�߸��� �Է� üũ
		if (s.length() >= 2) {
			lastWordIndex = printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		ofstream writeFile;
		writeFile.open("test.txt");
		int i = 0;
		for (; vector[i + 1] != "\0"; i++) {
			writeFile << vector[i];
		}
		//������ ���ڵ��� ������ ������ ����
		vector[i].erase(remove_if(vector[i].begin(), vector[i].end(), isspace), vector[i].end());
		writeFile << vector[i];
		writeFile.close();
		printPage(startPageIndex, vector);
		printMenu();
		printConsolMessage("���� ������ ���Ͽ� �����߽��ϴ�.  ���α׷��� �����մϴ�.");
	}
};
class SelectCommand {
public:
	SelectCommand(Command* command) {
		this->command = command;
	}
	SelectCommand() {
		this->command = NULL;
	}
	virtual void excuteCommand(string s, vector<string>& vector) {
		this->command->excuteCommand(s, vector);
	}
	void setSelectCommand(Command* command) {
		this->command = command;
	}
private:
	Command* command;
};

int main() {
	//���� �Է�
	fstream inputfile;
	inputfile.open("test.txt");
	if (!inputfile.is_open()) {
		cout << "������ ã�� �� �����ϴ�." << endl;
		return 0;
	}
	//vector�� file�� string ����
	vector<string> vector;
	inputFileToVector(inputfile, vector);
	//�ʱ�ȭ�� ���
	lastWordIndex = printPage(lastWordIndex, vector);
	printMenu();
	printConsolMessage("");
	//�Է��� �޾� �۾� ����
	Command* n = new excuteCommandN;
	Command* p = new excuteCommandP;
	Command* i = new excuteCommandI;
	Command* d = new excuteCommandD;
	Command* c = new excuteCommandC;
	Command* s = new excuteCommandS;
	Command* t = new excuteCommandT;
	SelectCommand* selectCommand = new SelectCommand;
	while (true) {
		string userInput = inputUserMessage();
		char menu = userInput[0];
		system("cls");
		try {
			switch (menu) {
			case 'n':
				selectCommand->setSelectCommand(n);
				selectCommand->excuteCommand(userInput, vector);
				break;
			case 'p':
				selectCommand->setSelectCommand(p);
				selectCommand->excuteCommand(userInput, vector);
				break;
			case 'i':
				selectCommand->setSelectCommand(i);
				selectCommand->excuteCommand(userInput, vector);
				break;
			case 'd':
				selectCommand->setSelectCommand(d);
				selectCommand->excuteCommand(userInput, vector);
				break;
			case 'c':
				selectCommand->setSelectCommand(c);
				selectCommand->excuteCommand(userInput, vector);
				break;
			case 's':
				selectCommand->setSelectCommand(s);
				selectCommand->excuteCommand(userInput, vector);
				break;
			case 't':
				selectCommand->setSelectCommand(t);
				selectCommand->excuteCommand(userInput, vector);
				throw - 1;
				break;
			default:
				lastWordIndex = printPage(startPageIndex, vector);
				printMenu();
				printConsolMessage(WRONGINPUT);
				break;
			}
		}
		catch (string e) {
			printMenu();
			printConsolMessage(e);
		}
		catch (int) {
			inputfile.close();
			delete n;
			delete p;
			delete i;
			delete d;
			delete c;
			delete s;
			delete t;
			delete selectCommand;
			return 0;
		}
	}
}