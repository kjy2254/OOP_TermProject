#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>

#define WRONGINPUT "잘못된 명령입니다."

using namespace std;

static int startPageIndex = 0;
static int lastWordIndex = 0;

int printPage(int startIndex, vector<string>& vector) {
	//시작 위치 저장
	startPageIndex = startIndex;
	//한줄에 최대 75바이트씩 20줄 출력
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
	//다음페이지의 시작 위치 반환
	return startIndex;
}
bool is_number(const string& s) {
	//string에 숫자가 아닌 문자가 존재하는지 확인
	string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}
void printMenu() {
	for (int i = 1; i < 81; i++) {
		cout << "-";
	}
	cout << endl;
	cout << " n: 다음페이지, p: 이전페이지, i: 삽입, d: 삭제, c: 변경, s:찾기, t: 저장후종료" << endl;
	for (int i = 1; i < 81; i++) {
		cout << "-";
	}
	cout << endl;
}
void printConsolMessage(string e) {
	cout << " (콘솔 메시지) " << e << endl;
	for (int i = 1; i < 81; i++) {
		cout << "-";
	}
	cout << endl;
}
string inputUserMessage() {
	string input;
	cout << " 입력: ";
	cin >> input;
	return input;
}
void inputFileToVector(fstream& file, vector<string>& vector) {
	//파일의 끝까지 공백을 기준으로 vector에 저장
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
		//잘못된 입력 체크
		if (s.length() >= 2) {
			lastWordIndex = printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		//출력에 마지막 단어가 포함되는지 체크
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
		//마지막 단어가 포함되는 경우
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
			throw string("마지막 페이지 입니다.");
		}
		//포함되지 않는 경우
		else {
			lastWordIndex = printPage(lastWordIndex, vector);
			throw string("다음 페이지가 출력되었습니다.");
		}
	}
};
class excuteCommandP : public Command {
public:
	void excuteCommand(string s, vector<string>& vector) {
		//잘못된 입력 체크
		if (s.length() >= 2) {
			lastWordIndex = printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		//첫 번째 페이지에서의 입력인지 체크
		if (startPageIndex == 0) {
			lastWordIndex = printPage(startPageIndex, vector);
			throw string("첫 번째 페이지 입니다.");
		}
		//이전 페이지 출력
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
		throw string("이전 페이지가 촐력되었습니다.");
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
		// '('나 ',' , ')'가 없는 경우 체크 
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
			throw string("인자가 부족합니다.");
		}
		//첫 번째 인자의 길이 체크
		if (commaIndex > 2) {
			printPage(startPageIndex, vector);
			throw string("잘못된 LineNumber 입니다.");
		}
		//첫 번째 인자인 LineNumber를 저장
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
		//LineNumer가 20이 넘거나 숫자가 아닌경우 체크
		if (!is_number(lineNumberBuffer) || lineNumber > 20 || lineNumber <= 0) {
			printPage(startPageIndex, vector);
			throw string("잘못된 LineNumber 입니다.");
		}
		//두 번째 ',' 체크
		if ((commaIndex = temp.find(",")) == string::npos) {
			printPage(startPageIndex, vector);
			throw string("인자가 부족합니다.");
		}
		//두 번째 인자의 길이 체크
		if (commaIndex > 2) {
			printPage(startPageIndex, vector);
			throw string("잘못된 WordNumber 입니다.");
		}
		//두 번째 인자인 WordNumber를 저장
		i = 0;
		while (i < commaIndex) {
			wordNumberBuffer[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		wordNumberBuffer[i] = '\0';
		stringstream s2(wordNumberBuffer);
		s2 >> wordNumber;
		//WordNumer가 숫자가 아니거나 0이하인 경우 체크
		if (!is_number(wordNumberBuffer) || wordNumber <= 0) {
			printPage(startPageIndex, vector);
			throw string("잘못된 WordNumber 입니다.");
		}
		//세 번째 인자인 입력 문자를 저장
		temp = temp.substr(1);
		if (temp.length() > 76 || temp.length() <= 1) {
			printPage(startPageIndex, vector);
			throw string("입력하고자 하는 문자가 없거나 너무 깁니다.");
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

		//Line의 시작위치로 이동
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
					throw string("해당 라인이 존재하지않습니다.");
				}
			}
			lineByte = 0;
		}
		//Word의 위치로 이동
		for (int word = 1; word < wordNumber; word++) {
			lineByte += vector[findIndex].length();
			findIndex++;
			if (lineByte > 76 || vector[findIndex] == "\0") {
				printPage(startPageIndex, vector);
				throw string("해당 위치에 Word가 존재하지 않습니다.");
			}
		}
		//해당 위치의 다음에 insertWord 입력
		vector.insert(vector.begin() + findIndex + 1, insertWord);
		//출력
		printPage(startPageIndex, vector);
		printMenu();
		printConsolMessage("주어진 단어를 입력했습니다.");
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
		//비정상 길이 체크
		if (temp.length() > 7) {
			printPage(startPageIndex, vector);
			throw string(WRONGINPUT);
		}
		// '('나 ',' , ')'가 없는 경우 체크 
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
			throw string("인자가 부족합니다.");
		}
		//','를 기준으로 앞의 LineNumber를 저장
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
		//LineNumer가 범위 밖이거나 숫자가 아닌경우 체크
		if (!is_number(lineNumberBuffer) || lineNumber > 20 || lineNumber <= 0) {
			printPage(startPageIndex, vector);
			throw string("잘못된 LineNumber 입니다.");
		}
		//','를 기준으로 뒤의 WordNumber를 저장
		int j = 0;
		while (temp.length() != 1) {
			wordNumberBuffer[j] = temp[0];
			temp = temp.substr(1);
			j++;
		}
		wordNumberBuffer[j] = '\0';
		stringstream s2(wordNumberBuffer);
		s2 >> wordNumber;
		//WordNumer가 숫자가 아니거나 0이하인 경우 체크
		if (!is_number(wordNumberBuffer) || wordNumber <= 0) {
			printPage(startPageIndex, vector);
			throw string("잘못된 WordNumber 입니다.");
		}
		//Line의 시작위치로 이동
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
					throw string("해당 라인이 존재하지않습니다.");
				}
			}
			lineByte = 0;
		}
		//Word의 위치로 이동
		for (int word = 1; word < wordNumber; word++) {
			lineByte += vector[findIndex].length();
			findIndex++;
			if (lineByte > 76 || vector[findIndex] == "\0") {
				printPage(startPageIndex, vector);
				throw string("해당 위치에 Word가 존재하지 않습니다.");
			}
		}
		//해당 위치의 Word 삭제
		vector.erase(vector.begin() + findIndex);
		//출력
		printPage(startPageIndex, vector);
		printMenu();
		printConsolMessage("주어진 단어를 삭제했습니다.");
	}
};
class excuteCommandC : public Command {
public:
	void excuteCommand(string s, vector<string>& vector) {
		char firstWordBuffer[80];
		char secondWordBuffer[80];
		unsigned int commaIndex;
		string temp = s.substr(1);

		// '('나 ',' , ')'가 없는 경우 체크 
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
		//첫 번째 인자가 75바이트 이상인 경우 체크
		if (commaIndex > 75) {
			printPage(startPageIndex, vector);
			throw string("첫 번째 인자가 너무 깁니다.");
		}
		//','를 기준으로 앞의 인자를 저장
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
		//','를 기준으로 뒤의 인자를 저장
		int j = 0;
		while (i < s.length() - 3) {
			secondWordBuffer[j] = temp[0];
			temp = temp.substr(1);
			if (j >= 75) {
				printPage(startPageIndex, vector);
				throw string("두 번째 인자가 너무 깁니다.");
			}
			i++;
			j++;
		}
		secondWordBuffer[j] = ' ';
		secondWordBuffer[j + 1] = '\0';
		string secondWord(secondWordBuffer);
		//첫 번째 인자 검색 후 두 번째 인자 교체
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
			throw string("해당 단어를 찾을 수 없습니다.");
			return;
		}
		//출력
		printPage(startPageIndex, vector);
		printMenu();
		printConsolMessage("주어진 두 단어를 교체했습니다.");
	}
};
class excuteCommandS : public Command {
public:
	void excuteCommand(string s, vector<string>& vector) {
		char serchWord[80];
		string temp = s.substr(1);
		//괄호가 없거나 문자의 길이가 긴 경우 체크
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
			throw string("찾고자 하는 문자가 75Byte가 넘습니다.");
		}
		//입력된 string 저장
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
		//입력된 string이 존재하는지 체크
		int j = 0;
		while (vector[j].compare(temp2) != 0) {
			j++;
			if (vector[j] == "\0") {
				printPage(startPageIndex, vector);
				throw string("해당 단어를 찾을 수 없습니다.");
				return;
			}
		}
		//찾은 문자를 시작으로 페이지 출력
		printPage(j, vector);
		printMenu();
		printConsolMessage("주어진 단어를 찾았습니다.");
	}
};
class excuteCommandT : public Command {
public:
	void excuteCommand(string s, vector<string>& vector) {
		//잘못된 입력 체크
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
		//마지막 문자뒤의 공백을 지워서 저장
		vector[i].erase(remove_if(vector[i].begin(), vector[i].end(), isspace), vector[i].end());
		writeFile << vector[i];
		writeFile.close();
		printPage(startPageIndex, vector);
		printMenu();
		printConsolMessage("변경 내용을 파일에 저장했습니다.  프로그램을 종료합니다.");
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
	//파일 입력
	fstream inputfile;
	inputfile.open("test.txt");
	if (!inputfile.is_open()) {
		cout << "파일을 찾을 수 없습니다." << endl;
		return 0;
	}
	//vector에 file의 string 저장
	vector<string> vector;
	inputFileToVector(inputfile, vector);
	//초기화면 출력
	lastWordIndex = printPage(lastWordIndex, vector);
	printMenu();
	printConsolMessage("");
	//입력을 받아 작업 수행
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